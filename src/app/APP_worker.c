#include "APP_worker.h"
#include "APP_hashtable.h"
#include "APP_error.h"
#include <SDL3/SDL_cpuinfo.h>
#include <SDL3/SDL_thread.h>

typedef struct APP_WorkerJobData
{
	SDL_Semaphore* complete;
	SDL_Mutex* lock;
	APP_WorkerJobStatus status;
} APP_WorkerJobData;

typedef struct APP_QueuedWorkerJob APP_QueuedWorkerJob;
struct APP_QueuedWorkerJob
{
	APP_QueuedWorkerJob* prev;
	APP_QueuedWorkerJob* next;
	APP_WorkerJobCallback callback;
	void* userdata;
	APP_WorkerJobData* jobData;
};

struct APP_Worker
{
	SDL_ThreadID threadID;
	APP_HashTable* jobsTable;
	SDL_Mutex* jobsQueueLock;
	APP_QueuedWorkerJob* jobsQueueHead;
	APP_QueuedWorkerJob* jobsQueueTail;
	SDL_Thread** threads;
	SDL_Semaphore* newJob;
	int threadsCount;
	SDL_AtomicInt shutdown;
	SDL_AtomicInt failure;
	APP_WorkerJobID nextJobID;
};

static int SDLCALL APP_WorkerThreadFunction(void* data)
{
	APP_Worker* const worker = data;
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
		APP_QueuedWorkerJob* const queuedJob = worker->jobsQueueTail;
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
			SDL_LockMutex(queuedJob->jobData->lock);
			queuedJob->jobData->status = APP_WORKER_JOB_FAILURE;
			SDL_SignalSemaphore(queuedJob->jobData->complete);
			SDL_UnlockMutex(queuedJob->jobData->lock);
			SDL_free(queuedJob);
			continue;
		}
		SDL_LockMutex(queuedJob->jobData->lock);
		queuedJob->jobData->status = APP_WORKER_JOB_COMPLETE;
		SDL_SignalSemaphore(queuedJob->jobData->complete);
		SDL_UnlockMutex(queuedJob->jobData->lock);
		SDL_free(queuedJob);
	}
	return 1;
}

APP_Worker* APP_CreateWorker(int threadsCount)
{
	APP_Worker* worker = SDL_calloc(1, sizeof(APP_Worker));
	if (!worker) {
		APP_SetError("Failed to create worker");
		return NULL;
	}

	worker->threadID = SDL_GetCurrentThreadID();

	worker->jobsQueueLock = SDL_CreateMutex();
	if (!worker->jobsQueueLock) {
		APP_SetError("Failed to create worker: %s", SDL_GetError());
		SDL_free(worker);
		return NULL;
	}

	worker->jobsTable = APP_CreateHashTable(16);
	if (!worker->jobsTable) {
		APP_SetError("Failed to create worker: %s", SDL_GetError());
		SDL_DestroyMutex(worker->jobsQueueLock);
		SDL_free(worker);
		return NULL;
	}

	if (threadsCount <= 0) {
		threadsCount = SDL_GetNumLogicalCPUCores();
	}
	worker->threadsCount = threadsCount;

	worker->newJob = SDL_CreateSemaphore(0);
	if (!worker->newJob) {
		APP_SetError("Failed to create worker: %s", SDL_GetError());
		SDL_free(worker->threads);
		APP_DestroyHashTable(worker->jobsTable);
		SDL_DestroyMutex(worker->jobsQueueLock);
		SDL_free(worker);
		return NULL;
	}

	worker->threads = SDL_malloc(threadsCount * sizeof(SDL_Thread*));
	if (!worker->threads) {
		APP_SetError("Failed to create worker");
		SDL_DestroySemaphore(worker->newJob);
		APP_DestroyHashTable(worker->jobsTable);
		SDL_DestroyMutex(worker->jobsQueueLock);
		SDL_free(worker);
		return NULL;
	}

	for (int i = 0; i < threadsCount; i++) {
		worker->threads[i] = SDL_CreateThread(APP_WorkerThreadFunction, "APP_WorkerThreadFunction", worker);
		if (!worker->threads[i]) {
			APP_SetError("Failed to create worker: %s", SDL_GetError());
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
			APP_DestroyHashTable(worker->jobsTable);
			SDL_DestroyMutex(worker->jobsQueueLock);
			SDL_free(worker);
			return NULL;
		}
	}

	return worker;
}

static void APP_DestroyWorkerJobData(uint64_t key, void* value, void* userdata)
{
	APP_WorkerJobData* const jobData = value;
	SDL_DestroyMutex(jobData->lock);
	SDL_DestroySemaphore(jobData->complete);
	SDL_free(jobData);
}

APP_WorkerJobID APP_SubmitWorkerJob(APP_Worker* worker, APP_WorkerJobCallback callback, void* userdata)
{
	if (!worker) {
		return APP_SetError("Invalid parameter \"worker\"");
	}
	else if (!callback) {
		return APP_SetError("Invalid parameter \"callback\"");
	}
	else if (worker->threadID != SDL_GetCurrentThreadID()) {
		return APP_SetError("Cannot do worker operations on threads that didn't create the worker");
	}

	int failure = SDL_GetAtomicInt(&worker->failure);
	SDL_MemoryBarrierAcquire();
	if (failure) {
		return APP_SetError("Worker's previously submitted in-progress job failed while trying to submit a new job");
	}

	const bool noInProgressJobs = APP_GetHashTableSize(worker->jobsTable) == 0;

	APP_QueuedWorkerJob* const queuedJob = SDL_calloc(1, sizeof(APP_QueuedWorkerJob));
	if (!queuedJob) {
		return APP_SetError("Failed submitting a worker job");
	}
	APP_WorkerJobData* const jobData = SDL_calloc(1, sizeof(APP_WorkerJobData));
	if (!jobData) {
		SDL_free(queuedJob);
		return APP_SetError("Failed submitting a worker job");
	}
	jobData->complete = SDL_CreateSemaphore(0);
	if (!jobData->complete) {
		SDL_free(jobData);
		SDL_free(queuedJob);
		return APP_SetError("Failed submitting a worker job: %s", SDL_GetError());
	}
	jobData->lock = SDL_CreateMutex();
	if (!jobData->lock) {
		SDL_DestroySemaphore(jobData->complete);
		SDL_free(jobData);
		SDL_free(queuedJob);
		return APP_SetError("Failed submitting a worker job: %s", SDL_GetError());
	}
	queuedJob->callback = callback;
	queuedJob->userdata = userdata;
	#if 0
	if (noInProgressJobs) {
		worker->nextJobID = 0;
	}
	#endif
	const APP_WorkerJobID job = ++worker->nextJobID;
	if (job == 0) {
		SDL_DestroyMutex(jobData->lock);
		SDL_DestroySemaphore(jobData->complete);
		SDL_free(jobData);
		SDL_free(queuedJob);
		return APP_SetError("Out of worker job IDs");
	}
	queuedJob->jobData = jobData;
	if (!APP_SetHashTableValue(worker->jobsTable, job, jobData, APP_DestroyWorkerJobData, NULL)) {
		SDL_MemoryBarrierRelease();
		SDL_SetAtomicInt(&worker->failure, 1);
		SDL_DestroyMutex(jobData->lock);
		SDL_DestroySemaphore(jobData->complete);
		SDL_free(jobData);
		SDL_free(queuedJob);
		APP_SetError("Failed submitting a worker job: %s", SDL_GetError());
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

static SDL_EnumerationResult APP_EnumerateWorkerJob(uint64_t key, void* value, void* userdata)
{
	const APP_WorkerJobID job = (APP_WorkerJobID)key;
	APP_WorkerJobData* const jobData = value;
	APP_WorkerJobStatus* const statusOut = userdata;

	SDL_LockMutex(jobData->lock);
	switch (jobData->status) {
	case APP_WORKER_JOB_COMPLETE:
		SDL_UnlockMutex(jobData->lock);
		return SDL_ENUM_CONTINUE;

	case APP_WORKER_JOB_IN_PROGRESS:
		SDL_UnlockMutex(jobData->lock);
		*statusOut = APP_WORKER_JOB_IN_PROGRESS;
		return SDL_ENUM_CONTINUE;

	default:
	case APP_WORKER_JOB_FAILURE:
		SDL_UnlockMutex(jobData->lock);
		*statusOut = APP_WORKER_JOB_FAILURE;
		return SDL_ENUM_SUCCESS;
	}
}

APP_WorkerJobStatus APP_GetWorkerJobStatus(APP_Worker* worker, APP_WorkerJobID job)
{
	if (!worker) {
		APP_SetError("Invalid parameter \"worker\"");
		return APP_WORKER_JOB_FAILURE;
	}
	else if (worker->threadID != SDL_GetCurrentThreadID()) {
		APP_SetError("Cannot do worker operations on threads that didn't create the worker");
		return APP_WORKER_JOB_FAILURE;
	}

	APP_WorkerJobStatus status;
	if (job == 0) {
		status = APP_WORKER_JOB_COMPLETE;
		APP_WorkerJobStatus status;
		APP_SetHashTableUserdata(worker->jobsTable, &status);
		if (!APP_EnumerateHashTable(worker->jobsTable, APP_EnumerateWorkerJob)) {
			APP_SetError("Failed getting worker job status: %s", SDL_GetError());
			return APP_WORKER_JOB_FAILURE;
		}
	}
	else {
		void* value;
		if (!APP_GetHashTableValue(worker->jobsTable, job, &value)) {
			status = APP_WORKER_JOB_COMPLETE;
		}
		else {
			status = (APP_WorkerJobStatus)(uintptr_t)value;
		}
	}
	if (status == APP_WORKER_JOB_FAILURE) {
		APP_SetError("Worker job failed");
	}
	return status;
}

static SDL_EnumerationResult APP_EnumerateWaitWorkerJob(uint64_t key, void* value, void* userdata)
{
	APP_WorkerJobData* const jobData = value;
	APP_Worker* const worker = userdata;
	APP_WorkerJobStatus status;

	SDL_LockMutex(jobData->lock);
	status = jobData->status;
	SDL_UnlockMutex(jobData->lock);
	if (status == APP_WORKER_JOB_FAILURE) {
		return SDL_ENUM_FAILURE;
	}

	SDL_WaitSemaphore(jobData->complete);

	SDL_LockMutex(jobData->lock);
	status = jobData->status;
	SDL_UnlockMutex(jobData->lock);
	if (status == APP_WORKER_JOB_FAILURE) {
		return SDL_ENUM_FAILURE;
	}
	return SDL_ENUM_CONTINUE;
}

bool APP_WaitWorkerJob(APP_Worker* worker, APP_WorkerJobID job)
{
	if (!worker) {
		return APP_SetError("Invalid parameter \"worker\"");
	}
	else if (worker->threadID != SDL_GetCurrentThreadID()) {
		APP_SetError("Cannot do worker operations on threads that didn't create the worker");
		return APP_WORKER_JOB_FAILURE;
	}

	if (job == 0) {
		APP_SetHashTableUserdata(worker->jobsTable, worker);
		if (!APP_EnumerateHashTable(worker->jobsTable, APP_EnumerateWaitWorkerJob)) {
			return APP_SetError("Worker job failed");
		}
		APP_EmptyHashTable(worker->jobsTable);
	}
	else {
		APP_WorkerJobData* const jobData = NULL;
		if (APP_GetHashTableValue(worker->jobsTable, job, (void**)&jobData)) {
			APP_WorkerJobStatus status;

			SDL_LockMutex(jobData->lock);
			status = jobData->status;
			SDL_UnlockMutex(jobData->lock);
			if (status == APP_WORKER_JOB_FAILURE) {
				return APP_SetError("Worker job failed");
			}

			SDL_WaitSemaphore(jobData->complete);

			SDL_LockMutex(jobData->lock);
			status = jobData->status;
			SDL_UnlockMutex(jobData->lock);
			if (status == APP_WORKER_JOB_FAILURE) {
				return APP_SetError("Worker job failed");
			}

			APP_RemoveHashTableValue(worker->jobsTable, job, NULL);
		}
	}

	return true;
}

bool APP_DestroyWorker(APP_Worker* worker)
{
	if (!worker) {
		return APP_SetError("Invalid parameter \"worker\"");
	}
	else if (worker->threadID != SDL_GetCurrentThreadID()) {
		return APP_SetError("Cannot do worker operations on threads that didn't create the worker");
	}
	else if (!APP_WaitWorkerJob(worker, 0)) {
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
	APP_DestroyHashTable(worker->jobsTable);
	SDL_DestroySemaphore(worker->newJob);
	SDL_DestroyMutex(worker->jobsQueueLock);
	SDL_free(worker);
	return true;
}
