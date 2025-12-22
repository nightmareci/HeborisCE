#include "APP_worker.h"
#include "APP_hashtable.h"
#include "APP_error.h"

typedef struct APP_WorkerJobData
{
	APP_WorkerJobCallback callback;
	void* userdata;
	APP_WorkerJobStatus status;
} APP_WorkerJobData;

struct APP_Worker
{
	APP_HashTable* jobs;
	APP_WorkerJobID nextJobID;
};

APP_Worker* APP_CreateWorker(void)
{
	APP_Worker* const worker = SDL_calloc(1, sizeof(APP_Worker));
	if (!worker) {
		APP_SetError("Failed to create worker");
		return NULL;
	}
	worker->jobs = APP_CreateHashTable(0);
	if (!worker->jobs) {
		SDL_free(worker);
		APP_SetError("Failed to create hash table for worker jobs");
		return NULL;
	}
	return worker;
}

static void APP_DestroyJobData(uint64_t key, void* value, void* userdata)
{
	SDL_free(value);
}

APP_WorkerJobID APP_SubmitWorkerJob(APP_Worker* worker, APP_WorkerJobCallback callback, void* userdata)
{
	if (!worker) {
		return APP_SetError("Invalid parameter \"worker\"");
	}
	else if (!callback) {
		return APP_SetError("Invalid parameter \"callback\"");
	}

	APP_WorkerJobData* const jobData = SDL_calloc(1, sizeof(APP_WorkerJobData));
	if (!jobData) {
		return APP_SetError("Failed submitting worker job");
	}
	jobData->callback = callback;
	jobData->userdata = userdata;

	worker->nextJobID++;
	if (!APP_SetHashTableValue(worker->jobs, worker->nextJobID, jobData, APP_DestroyJobData, NULL)) {
		SDL_free(jobData);
		return APP_SetError("Failed to submit worker job: %s", SDL_GetError());
	}
	return worker->nextJobID;
}

static SDL_EnumerationResult APP_EnumerateWorkerJobStatus(uint64_t key, void* value, void* userdata)
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

	APP_WorkerJobStatus status;
	if (job == 0) {
		status = APP_WORKER_JOB_COMPLETE;
		APP_SetHashTableUserdata(worker->jobs, &status);
		if (!APP_EnumerateHashTable(worker->jobs, APP_EnumerateWorkerJobStatus)) {
			APP_SetError("Failed getting job status: %s", SDL_GetError());
			return APP_WORKER_JOB_FAILURE;
		}
	}
	else {
		APP_WorkerJobData* jobData;
		if (!APP_GetHashTableValue(worker->jobs, job, (void**)&jobData)) {
			status = APP_WORKER_JOB_COMPLETE;
		}
		else {
			status = jobData->status;
		}
	}
	return status;
}

typedef struct APP_EnumerateWaitWorkerJobData
{
	APP_Worker* worker;
	bool success;
} APP_EnumerateWaitWorkerJobData;

static SDL_EnumerationResult APP_EnumerateWaitWorkerJob(uint64_t key, void* value, void* userdata)
{
	APP_WorkerJobData* const jobData = value;
	APP_EnumerateWaitWorkerJobData* const enumerateData = userdata;

	if (jobData->status == APP_WORKER_JOB_IN_PROGRESS && !jobData->callback(jobData->userdata)) {
		jobData->status = APP_WORKER_JOB_FAILURE;
		enumerateData->success = false;
	}
	return SDL_ENUM_CONTINUE;
}

bool APP_WaitWorkerJob(APP_Worker* worker, APP_WorkerJobID job)
{
	if (!worker) {
		return APP_SetError("Invalid parameter \"worker\"");
	}

	if (job == 0) {
		APP_EnumerateWaitWorkerJobData enumerateData = { worker, true };
		APP_SetHashTableUserdata(worker->jobs, worker);
		if (!APP_EnumerateHashTable(worker->jobs, APP_EnumerateWaitWorkerJob) || !enumerateData.success) {
			APP_EmptyHashTable(worker->jobs);
			return APP_SetError("Worker job failed");
		}
		APP_EmptyHashTable(worker->jobs);
	}
	else {
		APP_WorkerJobData* jobData = NULL;
		if (!APP_GetHashTableValue(worker->jobs, job, (void**)&jobData)) {
			return true;
		}
		else if (jobData->status == APP_WORKER_JOB_FAILURE) {
			return APP_SetError("Worker job failed");
		}
		else if (!jobData->callback(jobData->userdata)) {
			jobData->status = APP_WORKER_JOB_FAILURE;
			return APP_SetError("Worker job failed");
		}
		APP_RemoveHashTableValue(worker->jobs, job, NULL);
	}

	return true;
}

bool APP_DestroyWorker(APP_Worker* worker)
{
	if (!worker) {
		return APP_SetError("Invalid parameter \"worker\"");
	}
	else if (!APP_WaitWorkerJob(worker, 0)) {
		return false;
	}

	APP_DestroyHashTable(worker->jobs);
	SDL_free(worker);
	return true;
}
