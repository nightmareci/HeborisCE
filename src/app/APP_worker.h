#ifndef APP_worker_h_
#define APP_worker_h_

// Worker functions may only be called for a worker on the thread that created
// the worker.

#include "APP_stdinc.h"

typedef struct APP_Worker APP_Worker;

typedef uint32_t APP_WorkerJobID;

typedef enum APP_WorkerJobStatus
{
	APP_WORKER_JOB_IN_PROGRESS = 0,
	APP_WORKER_JOB_COMPLETE = 1,
	APP_WORKER_JOB_FAILURE = 2
} APP_WorkerJobStatus;

typedef bool (*APP_WorkerJobCallback)(void* userdata);

// Create a worker.
APP_Worker* APP_CreateWorker(int threadsCount);

// Submit a job for a worker.
APP_WorkerJobID APP_SubmitWorkerJob(APP_Worker* worker, APP_WorkerJobCallback callback, void* userdata);

// Get the status of job(s) previously submitted.
//
// If job == 0, returns the status of all submitted jobs:
// APP_WORKER_JOB_COMPLETE means all jobs successfully completed.
// APP_WORKER_JOB_IN_PROGRESS means at least one job is still in progress and no failure has occurred.
// APP_WORKER_JOB_FAILURE means at least one job failed to complete.
APP_WorkerJobStatus APP_GetWorkerJobStatus(APP_Worker* worker, APP_WorkerJobID job);

// Wait for job(s) to complete.
//
// If job == 0, waits for all jobs to complete.
//
// Returns true if waiting for worker(s) was successful, returns false if there
// was an error while waiting for worker(s).
bool APP_WaitWorkerJob(APP_Worker* worker, APP_WorkerJobID job);

// Destroy a worker. Also waits for all jobs to complete.
//
// Returns true if destroying the worker was successful, returns false if there
// was an error while destroying the worker.
bool APP_DestroyWorker(APP_Worker* worker);

#endif
