#include <WORKER.h>
#include <ERROR.h>
#include "HASHTABLE.h"

typedef struct WORKER_JobData
{
	WORKER_JobCallback callback;
	void* userdata;
	WORKER_JobStatus status;
} WORKER_JobData;

struct WORKER_Object
{
	HASHTABLE_Object* jobs;
	WORKER_JobID nextJobID;
};

WORKER_Object* WORKER_Create(void)
{
	WORKER_Object* const worker = SDL_calloc(1, sizeof(WORKER_Object));
	if (!worker) {
		ERROR_Set("Failed to create worker");
		return NULL;
	}
	worker->jobs = HASHTABLE_Create(0);
	if (!worker->jobs) {
		SDL_free(worker);
		ERROR_Set("Failed to create hash table for worker jobs");
		return NULL;
	}
	return worker;
}

static void WORKER_DestroyJobData(uint64_t key, void* value, void* userdata)
{
	SDL_free(value);
}

WORKER_JobID WORKER_SubmitJob(WORKER_Object* worker, WORKER_JobCallback callback, void* userdata)
{
	if (!worker) {
		return ERROR_Set("Invalid parameter \"worker\"");
	}
	else if (!callback) {
		return ERROR_Set("Invalid parameter \"callback\"");
	}

	WORKER_JobData* const jobData = SDL_calloc(1, sizeof(WORKER_JobData));
	if (!jobData) {
		return ERROR_Set("Failed submitting worker job");
	}
	jobData->callback = callback;
	jobData->userdata = userdata;

	worker->nextJobID++;
	if (!HASHTABLE_SetValue(worker->jobs, worker->nextJobID, jobData, WORKER_DestroyJobData, NULL)) {
		SDL_free(jobData);
		return ERROR_Set("Failed to submit worker job: %s", SDL_GetError());
	}
	return worker->nextJobID;
}

static SDL_EnumerationResult WORKER_EnumerateJobStatus(uint64_t key, void* value, void* userdata)
{
	const WORKER_JobID job = (WORKER_JobID)key;
	WORKER_JobData* const jobData = value;
	WORKER_JobStatus* const statusOut = userdata;

	switch (jobData->status) {
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

	WORKER_JobStatus status;
	if (job == 0) {
		status = WORKER_JOB_COMPLETE;
		HASHTABLE_SetUserdata(worker->jobs, &status);
		if (!HASHTABLE_Enumerate(worker->jobs, WORKER_EnumerateJobStatus)) {
			ERROR_Set("Failed getting job status: %s", SDL_GetError());
			return WORKER_JOB_FAILURE;
		}
	}
	else {
		WORKER_JobData* jobData;
		if (!HASHTABLE_GetValue(worker->jobs, job, (void**)&jobData)) {
			status = WORKER_JOB_COMPLETE;
		}
		else {
			status = jobData->status;
		}
	}
	return status;
}

typedef struct WORKER_WaitForJobCompletionData
{
	WORKER_Object* worker;
	bool success;
} WORKER_WaitForJobCompletionData;

static SDL_EnumerationResult WORKER_EnumerateWaitForJobCompletion(uint64_t key, void* value, void* userdata)
{
	WORKER_JobData* const jobData = value;
	WORKER_WaitForJobCompletionData* const enumerateData = userdata;

	if (jobData->status == WORKER_JOB_IN_PROGRESS && !jobData->callback(jobData->userdata)) {
		jobData->status = WORKER_JOB_FAILURE;
		enumerateData->success = false;
	}
	return SDL_ENUM_CONTINUE;
}

bool WORKER_WaitForJobCompletion(WORKER_Object* worker, WORKER_JobID job)
{
	if (!worker) {
		return ERROR_Set("Invalid parameter \"worker\"");
	}

	if (job == 0) {
		WORKER_WaitForJobCompletionData enumerateData = { worker, true };
		HASHTABLE_SetUserdata(worker->jobs, worker);
		if (!HASHTABLE_Enumerate(worker->jobs, WORKER_EnumerateWaitForJobCompletion) || !enumerateData.success) {
			HASHTABLE_Empty(worker->jobs);
			return ERROR_Set("Worker job failed");
		}
		HASHTABLE_Empty(worker->jobs);
	}
	else {
		WORKER_JobData* jobData = NULL;
		if (!HASHTABLE_GetValue(worker->jobs, job, (void**)&jobData)) {
			return true;
		}
		else if (jobData->status == WORKER_JOB_FAILURE) {
			return ERROR_Set("Worker job failed");
		}
		else if (!jobData->callback(jobData->userdata)) {
			jobData->status = WORKER_JOB_FAILURE;
			return ERROR_Set("Worker job failed");
		}
		HASHTABLE_RemoveValue(worker->jobs, job, NULL);
	}

	return true;
}

bool WORKER_Destroy(WORKER_Object* worker)
{
	if (!worker) {
		return ERROR_Set("Invalid parameter \"worker\"");
	}
	else if (!WORKER_WaitForJobCompletion(worker, 0)) {
		return false;
	}

	HASHTABLE_Destroy(worker->jobs);
	SDL_free(worker);
	return true;
}
