#include <WORKER.h>
#include <ERROR.h>
#include "HASHTABLE.h"

typedef struct WORKER_JobData
{
	SDL_Semaphore* complete;
	SDL_AtomicInt status;
} WORKER_JobData;

typedef struct WORKER_QueuedJob WORKER_QueuedJob;
struct WORKER_QueuedJob
{
	WORKER_QueuedJob* prev;
	WORKER_QueuedJob* next;
	WORKER_JobCallback callback;
	void* userdata;
	WORKER_JobData* jobData;
};

struct WORKER_Object
{
	SDL_ThreadID threadID;
	HASHTABLE_Object* jobsTable;
	SDL_Mutex* jobsQueueLock;
	WORKER_QueuedJob* jobsQueueHead;
	WORKER_QueuedJob* jobsQueueTail;
	SDL_Thread** threads;
	SDL_Semaphore* newJob;
	int threadsCount;
	SDL_AtomicInt shutdown;
	SDL_AtomicInt failure;
	WORKER_JobID nextJobID;
};

static int SDLCALL WORKER_ThreadFunction(void* data)
{
	WORKER_Object* const worker = data;
	while (true) {
		SDL_WaitSemaphore(worker->newJob);

		const int shutdown = SDL_GetAtomicInt(&worker->shutdown);
		SDL_MemoryBarrierAcquire();
		if (shutdown) {
			break;
		}

		SDL_LockMutex(worker->jobsQueueLock);
		if (!worker->jobsQueueTail) {
			SDL_UnlockMutex(worker->jobsQueueLock);
			continue;
		}
		WORKER_QueuedJob* const queuedJob = worker->jobsQueueTail;
		if (queuedJob->prev) {
			queuedJob->prev->next = NULL;
			worker->jobsQueueTail = queuedJob->prev;
		}
		else {
			worker->jobsQueueHead = NULL;
			worker->jobsQueueTail = NULL;
		}
		SDL_UnlockMutex(worker->jobsQueueLock);

		if (!queuedJob->callback(queuedJob->userdata)) {
			SDL_MemoryBarrierRelease();
			SDL_SetAtomicInt(&worker->failure, 1);
			SDL_MemoryBarrierRelease();
			SDL_SetAtomicInt(&queuedJob->jobData->status, WORKER_JOB_FAILURE);
			SDL_SignalSemaphore(queuedJob->jobData->complete);
			SDL_free(queuedJob);
			continue;
		}
		SDL_MemoryBarrierRelease();
		SDL_SetAtomicInt(&queuedJob->jobData->status, WORKER_JOB_COMPLETE);
		SDL_SignalSemaphore(queuedJob->jobData->complete);
		SDL_free(queuedJob);
	}
	return 1;
}

WORKER_Object* WORKER_Create(void)
{
	WORKER_Object* worker = SDL_calloc(1, sizeof(WORKER_Object));
	if (!worker) {
		ERROR_Set("Failed to create worker");
		return NULL;
	}

	worker->threadID = SDL_GetCurrentThreadID();

	worker->jobsQueueLock = SDL_CreateMutex();
	if (!worker->jobsQueueLock) {
		ERROR_Set("Failed to create worker: %s", SDL_GetError());
		SDL_free(worker);
		return NULL;
	}

	worker->jobsTable = HASHTABLE_Create(0);
	if (!worker->jobsTable) {
		ERROR_Set("Failed to create worker: %s", SDL_GetError());
		SDL_DestroyMutex(worker->jobsQueueLock);
		SDL_free(worker);
		return NULL;
	}

	worker->threadsCount = SDL_GetNumLogicalCPUCores();

	worker->newJob = SDL_CreateSemaphore(0);
	if (!worker->newJob) {
		ERROR_Set("Failed to create worker: %s", SDL_GetError());
		SDL_free(worker->threads);
		HASHTABLE_Destroy(worker->jobsTable);
		SDL_DestroyMutex(worker->jobsQueueLock);
		SDL_free(worker);
		return NULL;
	}

	worker->threads = SDL_malloc(worker->threadsCount * sizeof(SDL_Thread*));
	if (!worker->threads) {
		ERROR_Set("Failed to create worker");
		SDL_DestroySemaphore(worker->newJob);
		HASHTABLE_Destroy(worker->jobsTable);
		SDL_DestroyMutex(worker->jobsQueueLock);
		SDL_free(worker);
		return NULL;
	}

	for (int i = 0; i < worker->threadsCount; i++) {
		worker->threads[i] = SDL_CreateThread(WORKER_ThreadFunction, "WORKER_ThreadFunction", worker);
		if (!worker->threads[i]) {
			ERROR_Set("Failed to create worker: %s", SDL_GetError());
			SDL_MemoryBarrierRelease();
			SDL_SetAtomicInt(&worker->shutdown, 1);
			for (int j = 0; j < i; j++) {
				SDL_SignalSemaphore(worker->newJob);
			}
			for (int j = 0; j < i; j++) {
				SDL_WaitThread(worker->threads[j], NULL);
			}
			SDL_free(worker->threads);
			SDL_DestroySemaphore(worker->newJob);
			HASHTABLE_Destroy(worker->jobsTable);
			SDL_DestroyMutex(worker->jobsQueueLock);
			SDL_free(worker);
			return NULL;
		}
	}

	return worker;
}

static void WORKER_DestroyJobData(uint64_t key, void* value, void* userdata)
{
	WORKER_JobData* const jobData = value;
	SDL_DestroySemaphore(jobData->complete);
	SDL_free(jobData);
}

WORKER_JobID WORKER_SubmitJob(WORKER_Object* worker, WORKER_JobCallback callback, void* userdata)
{
	if (!worker) {
		return ERROR_Set("Invalid parameter \"worker\"");
	}
	else if (!callback) {
		return ERROR_Set("Invalid parameter \"callback\"");
	}
	else if (worker->threadID != SDL_GetCurrentThreadID()) {
		return ERROR_Set("Cannot do worker operations on threads that didn't create the worker");
	}

	int failure = SDL_GetAtomicInt(&worker->failure);
	SDL_MemoryBarrierAcquire();
	if (failure) {
		return ERROR_Set("Worker's previously submitted in-progress job failed while trying to submit a new job");
	}

	const bool noInProgressJobs = HASHTABLE_GetSize(worker->jobsTable) == 0;

	WORKER_QueuedJob* const queuedJob = SDL_calloc(1, sizeof(WORKER_QueuedJob));
	if (!queuedJob) {
		return ERROR_Set("Failed submitting a worker job");
	}
	WORKER_JobData* const jobData = SDL_calloc(1, sizeof(WORKER_JobData));
	if (!jobData) {
		SDL_free(queuedJob);
		return ERROR_Set("Failed submitting a worker job");
	}
	jobData->complete = SDL_CreateSemaphore(0);
	if (!jobData->complete) {
		SDL_free(jobData);
		SDL_free(queuedJob);
		return ERROR_Set("Failed submitting a worker job: %s", SDL_GetError());
	}
	queuedJob->callback = callback;
	queuedJob->userdata = userdata;
	#if 0
	if (noInProgressJobs) {
		worker->nextJobID = 0;
	}
	#endif
	const WORKER_JobID job = ++worker->nextJobID;
	if (job == 0) {
		SDL_DestroySemaphore(jobData->complete);
		SDL_free(jobData);
		SDL_free(queuedJob);
		return ERROR_Set("Out of worker job IDs");
	}
	queuedJob->jobData = jobData;
	if (!HASHTABLE_SetValue(worker->jobsTable, job, jobData, WORKER_DestroyJobData, NULL)) {
		SDL_MemoryBarrierRelease();
		SDL_SetAtomicInt(&worker->failure, 1);
		SDL_DestroySemaphore(jobData->complete);
		SDL_free(jobData);
		SDL_free(queuedJob);
		ERROR_Set("Failed submitting a worker job: %s", SDL_GetError());
		return 0;
	}
	SDL_LockMutex(worker->jobsQueueLock);
	if (worker->jobsQueueHead) {
		queuedJob->next = worker->jobsQueueHead;
		worker->jobsQueueHead->prev = queuedJob;
		worker->jobsQueueHead = queuedJob;
	}
	else {
		worker->jobsQueueHead = queuedJob;
		worker->jobsQueueTail = queuedJob;
	}
	SDL_UnlockMutex(worker->jobsQueueLock);
	SDL_SignalSemaphore(worker->newJob);
	return job;
}

static SDL_EnumerationResult WORKER_EnumerateJobStatus(uint64_t key, void* value, void* userdata)
{
	const WORKER_JobID job = (WORKER_JobID)key;
	WORKER_JobData* const jobData = value;
	WORKER_JobStatus* const statusOut = userdata;

	const int status = SDL_GetAtomicInt(&jobData->status);
	SDL_MemoryBarrierAcquire();
	switch (status) {
	case WORKER_JOB_COMPLETE:
		return SDL_ENUM_CONTINUE;

	case WORKER_JOB_IN_PROGRESS:
		*statusOut = WORKER_JOB_IN_PROGRESS;
		return SDL_ENUM_CONTINUE;

	default:
	case WORKER_JOB_FAILURE:
		*statusOut = WORKER_JOB_FAILURE;
		return SDL_ENUM_SUCCESS;
	}
}

WORKER_JobStatus WORKER_GetJobStatus(WORKER_Object* worker, WORKER_JobID job)
{
	if (!worker) {
		ERROR_Set("Invalid parameter \"worker\"");
		return WORKER_JOB_FAILURE;
	}
	else if (worker->threadID != SDL_GetCurrentThreadID()) {
		ERROR_Set("Cannot do worker operations on threads that didn't create the worker");
		return WORKER_JOB_FAILURE;
	}

	WORKER_JobStatus status;
	if (job == 0) {
		status = WORKER_JOB_COMPLETE;
		WORKER_JobStatus status;
		HASHTABLE_SetUserdata(worker->jobsTable, &status);
		if (!HASHTABLE_Enumerate(worker->jobsTable, WORKER_EnumerateJobStatus)) {
			ERROR_Set("Failed getting worker job status: %s", SDL_GetError());
			return WORKER_JOB_FAILURE;
		}
	}
	else {
		WORKER_JobData* jobData;
		if (!HASHTABLE_GetValue(worker->jobsTable, job, (void**)&jobData)) {
			status = WORKER_JOB_COMPLETE;
		}
		else {
			status = (WORKER_JobStatus)SDL_GetAtomicInt(&jobData->status);
			SDL_MemoryBarrierAcquire();
		}
	}
	if (status == WORKER_JOB_FAILURE) {
		ERROR_Set("Worker job failed");
	}
	return status;
}

typedef struct WORKER_EnumerateWaitForJobCompletionData
{
	WORKER_Object* worker;
	bool success;
} WORKER_EnumerateWaitForJobCompletionData;

static SDL_EnumerationResult WORKER_EnumerateWaitForJobCompletion(uint64_t key, void* value, void* userdata)
{
	WORKER_JobData* const jobData = value;
	WORKER_EnumerateWaitForJobCompletionData* const enumerateData = userdata;

	int status = SDL_GetAtomicInt(&jobData->status);
	SDL_MemoryBarrierAcquire();
	if (status == WORKER_JOB_FAILURE) {
		enumerateData->success = false;
		return SDL_ENUM_CONTINUE;
	}

	SDL_WaitSemaphore(jobData->complete);

	status = SDL_GetAtomicInt(&jobData->status);
	SDL_MemoryBarrierAcquire();
	if (status == WORKER_JOB_FAILURE) {
		enumerateData->success = false;
	}
	return SDL_ENUM_CONTINUE;
}

bool WORKER_WaitForJobCompletion(WORKER_Object* worker, WORKER_JobID job)
{
	if (!worker) {
		return ERROR_Set("Invalid parameter \"worker\"");
	}
	else if (worker->threadID != SDL_GetCurrentThreadID()) {
		ERROR_Set("Cannot do worker operations on threads that didn't create the worker");
		return WORKER_JOB_FAILURE;
	}

	if (job == 0) {
		WORKER_EnumerateWaitForJobCompletionData enumerateData = { worker, true };
		HASHTABLE_SetUserdata(worker->jobsTable, &enumerateData);
		if (!HASHTABLE_Enumerate(worker->jobsTable, WORKER_EnumerateWaitForJobCompletion) || !enumerateData.success) {
			HASHTABLE_Empty(worker->jobsTable);
			return ERROR_Set("Worker job failed");
		}
		HASHTABLE_Empty(worker->jobsTable);
	}
	else {
		WORKER_JobData* jobData = NULL;
		if (HASHTABLE_GetValue(worker->jobsTable, job, (void**)&jobData)) {
			int status = SDL_GetAtomicInt(&jobData->status);
			SDL_MemoryBarrierAcquire();
			if (status == WORKER_JOB_FAILURE) {
				return ERROR_Set("Worker job failed");
			}

			SDL_WaitSemaphore(jobData->complete);

			status = SDL_GetAtomicInt(&jobData->status);
			SDL_MemoryBarrierAcquire();
			if (status == WORKER_JOB_FAILURE) {
				return ERROR_Set("Worker job failed");
			}

			HASHTABLE_RemoveValue(worker->jobsTable, job, NULL);
		}
	}

	return true;
}

bool WORKER_Destroy(WORKER_Object* worker)
{
	if (!worker) {
		return ERROR_Set("Invalid parameter \"worker\"");
	}
	else if (worker->threadID != SDL_GetCurrentThreadID()) {
		return ERROR_Set("Cannot do worker operations on threads that didn't create the worker");
	}
	else if (!WORKER_WaitForJobCompletion(worker, 0)) {
		return false;
	}

	SDL_MemoryBarrierRelease();
	SDL_SetAtomicInt(&worker->shutdown, 1);
	for (int i = 0; i < worker->threadsCount; i++) {
		SDL_SignalSemaphore(worker->newJob);
	}
	for (int i = 0; i < worker->threadsCount; i++) {
		SDL_WaitThread(worker->threads[i], NULL);
	}
	HASHTABLE_Destroy(worker->jobsTable);
	SDL_DestroySemaphore(worker->newJob);
	SDL_DestroyMutex(worker->jobsQueueLock);
	SDL_free(worker);
	return true;
}
