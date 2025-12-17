#include "APP_worker.h"
#include "APP_hashtable.h"
#include "APP_error.h"
#include <SDL3/SDL_cpuinfo.h>
#include <SDL3/SDL_thread.h>

typedef struct APP_WorkerJobData
{
	SDL_Semaphore* complete;
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
	SDL_Mutex* lock;
	APP_HashTable* jobsTable;
	APP_QueuedWorkerJob* jobsQueueHead;
	APP_QueuedWorkerJob* jobsQueueTail;
	SDL_Thread** threads;
	SDL_Semaphore* newJob;
	size_t threadsCount;
	SDL_AtomicInt shutdown;
	SDL_AtomicInt nextJobID; // TODO: Use SDL_AtomicU32 once we're on SDL 3.4
	bool failure;
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

		SDL_LockMutex(worker->lock);
		if (!worker->jobsQueueTail) {
			SDL_UnlockMutex(worker->lock);
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
		SDL_UnlockMutex(worker->lock);

		if (!queuedJob->callback(queuedJob->userdata)) {
			SDL_LockMutex(worker->lock);
			worker->failure = true;
			queuedJob->jobData->status = APP_WORKER_JOB_FAILURE;
			SDL_SignalSemaphore(queuedJob->jobData->complete);
			SDL_UnlockMutex(worker->lock);
			continue;
		}
		SDL_LockMutex(worker->lock);
		queuedJob->jobData->status = APP_WORKER_JOB_COMPLETE;
		SDL_SignalSemaphore(queuedJob->jobData->complete);
		SDL_UnlockMutex(worker->lock);
	}
	return 1;
}

APP_Worker* APP_CreateWorker(size_t threadsCount)
{
	APP_Worker* worker = SDL_calloc(1, sizeof(APP_Worker));
	if (!worker) {
		APP_SetError("Failed to create worker");
		return NULL;
	}

	worker->threadID = SDL_GetCurrentThreadID();

	worker->lock = SDL_CreateMutex();
	if (!worker->lock) {
		APP_SetError("Failed to create worker: %s", SDL_GetError());
		SDL_free(worker);
		return NULL;
	}

	worker->jobsTable = APP_CreateHashTable(16);
	if (!worker->jobsTable) {
		APP_SetError("Failed to create worker: %s", SDL_GetError());
		SDL_DestroyMutex(worker->lock);
		SDL_free(worker);
		return NULL;
	}

	if (threadsCount == 0) {
		threadsCount = SDL_GetNumLogicalCPUCores();
	}
	worker->threadsCount = threadsCount;

	worker->newJob = SDL_CreateSemaphore(0);
	if (!worker->newJob) {
		APP_SetError("Failed to create worker: %s", SDL_GetError());
		SDL_free(worker->threads);
		APP_DestroyHashTable(worker->jobsTable);
		SDL_DestroyMutex(worker->lock);
		SDL_free(worker);
		return NULL;
	}

	worker->threads = SDL_malloc(threadsCount * sizeof(SDL_Thread*));
	if (!worker->threads) {
		APP_SetError("Failed to create worker");
		APP_DestroyHashTable(worker->jobsTable);
		SDL_DestroyMutex(worker->lock);
		SDL_free(worker);
		return NULL;
	}

	for (size_t i = 0; i < threadsCount; i++) {
		worker->threads[i] = SDL_CreateThread(APP_WorkerThreadFunction, "APP_WorkerThreadFunction", worker);
		if (!worker->threads[i]) {
			APP_SetError("Failed to create worker: %s", SDL_GetError());
			SDL_MemoryBarrierRelease();
			SDL_SetAtomicInt(&worker->shutdown, 1);
			for (size_t j = 0; j < i; j++) {
				SDL_SignalSemaphore(worker->newJob);
			}
			for (size_t j = 0; j < i; j++) {
				SDL_WaitThread(worker->threads[j], NULL);
			}
			SDL_free(worker->threads);
			SDL_DestroySemaphore(worker->newJob);
			APP_DestroyHashTable(worker->jobsTable);
			SDL_DestroyMutex(worker->lock);
			SDL_free(worker);
			return NULL;
		}
	}

	return worker;
}

static void APP_DestroyWorkerJobData(uint64_t key, void* value, void* userdata)
{
	APP_WorkerJobData* const jobData = value;
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

	SDL_LockMutex(worker->lock);
	if (worker->failure) {
		SDL_UnlockMutex(worker->lock);
		return APP_SetError("Worker's previously submitted in-progress job failed while trying to submit a new job");
	}
	const bool noInProgressJobs = APP_GetHashTableSize(worker->jobsTable) == 0;
	SDL_UnlockMutex(worker->lock);

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
	queuedJob->callback = callback;
	queuedJob->userdata = userdata;
	if (noInProgressJobs) {
		SDL_SetAtomicInt(&worker->nextJobID, 0);
	}
	const APP_WorkerJobID job = (APP_WorkerJobID)SDL_AtomicIncRef(&worker->nextJobID) + 1;
	if (job == 0) {
		SDL_DestroySemaphore(jobData->complete);
		SDL_free(jobData);
		SDL_free(queuedJob);
		return APP_SetError("Out of worker job IDs");
	}
	queuedJob->jobData = jobData;
	SDL_LockMutex(worker->lock);
	if (worker->failure) {
		SDL_UnlockMutex(worker->lock);
		SDL_free(jobData);
		SDL_free(queuedJob);
		return APP_SetError("Worker's in-progress job failed while trying to submit a new job");
	}
	if (!APP_SetHashTableValue(worker->jobsTable, job, jobData, APP_DestroyWorkerJobData, NULL)) {
		worker->failure = true;
		SDL_UnlockMutex(worker->lock);
		SDL_free(queuedJob);
		APP_SetError("Failed submitting a worker job: %s", SDL_GetError());
		return 0;
	}
	if (worker->jobsQueueHead) {
		queuedJob->next = worker->jobsQueueHead;
		worker->jobsQueueHead = queuedJob;
	}
	else {
		worker->jobsQueueHead = queuedJob;
		worker->jobsQueueTail = queuedJob;
	}
	SDL_UnlockMutex(worker->lock);
	SDL_SignalSemaphore(worker->newJob);
	return job;
}

static SDL_EnumerationResult APP_EnumerateWorkerJob(uint64_t key, void* value, void* userdata)
{
	const APP_WorkerJobID job = (APP_WorkerJobID)key;
	APP_WorkerJobData* const jobData = value;
	APP_WorkerJobStatus* const statusOut = userdata;

	switch (jobData->status) {
	case APP_WORKER_JOB_COMPLETE:
		return SDL_ENUM_CONTINUE;

	case APP_WORKER_JOB_IN_PROGRESS:
		*statusOut = APP_WORKER_JOB_IN_PROGRESS;
		return SDL_ENUM_CONTINUE;

	default:
	case APP_WORKER_JOB_FAILURE:
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
		SDL_LockMutex(worker->lock);
		APP_WorkerJobStatus status;
		APP_SetHashTableUserdata(worker->jobsTable, &status);
		if (!APP_EnumerateHashTable(worker->jobsTable, APP_EnumerateWorkerJob)) {
			SDL_UnlockMutex(worker->lock);
			APP_SetError("Failed getting worker job status: %s", SDL_GetError());
			return APP_WORKER_JOB_FAILURE;
		}
		SDL_UnlockMutex(worker->lock);
	}
	else {
		SDL_LockMutex(worker->lock);
		void* value;
		if (!APP_GetHashTableValue(worker->jobsTable, job, &value)) {
			status = APP_WORKER_JOB_COMPLETE;
		}
		else {
			status = (APP_WorkerJobStatus)(uintptr_t)value;
		}
		SDL_UnlockMutex(worker->lock);
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

	SDL_LockMutex(worker->lock);
	const APP_WorkerJobStatus status = jobData->status;
	SDL_UnlockMutex(worker->lock);
	if (status == APP_WORKER_JOB_FAILURE) {
		return SDL_ENUM_FAILURE;
	}
	SDL_WaitSemaphore(jobData->complete);
	if (jobData->status == APP_WORKER_JOB_FAILURE) {
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
		APP_DestroyHashTable(worker->jobsTable);
		worker->jobsTable = APP_CreateHashTable(16);
		if (!worker->jobsTable) {
			return APP_SetError("Failed waiting for worker jobs");
		}
	}
	else {
		SDL_LockMutex(worker->lock);
		APP_WorkerJobData* const jobData = NULL;
		if (APP_GetHashTableValue(worker->jobsTable, job, (void**)&jobData)) {
			const APP_WorkerJobStatus status = jobData->status;
			SDL_UnlockMutex(worker->lock);
			if (status == APP_WORKER_JOB_FAILURE) {
				return APP_SetError("Worker job failed");
			}
			SDL_WaitSemaphore(jobData->complete);
			if (jobData->status == APP_WORKER_JOB_FAILURE) {
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
	for (size_t i = 0; i < worker->threadsCount; i++) {
		SDL_SignalSemaphore(worker->newJob);
	}
	for (size_t i = 0; i < worker->threadsCount; i++) {
		SDL_WaitThread(worker->threads[i], NULL);
	}
	APP_DestroyHashTable(worker->jobsTable);
	SDL_DestroySemaphore(worker->newJob);
	SDL_DestroyMutex(worker->lock);
	SDL_free(worker);
	return true;
}
