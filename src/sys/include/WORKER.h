#ifndef WORKER_h_
#define WORKER_h_

// Worker functions may only be called for a worker on the thread that created
// the worker.

#include <STD.h>

typedef struct WORKER_Object WORKER_Object;

typedef uint32_t WORKER_JobID;

typedef enum WORKER_JobStatus
{
	WORKER_JOB_IN_PROGRESS = 0,
	WORKER_JOB_COMPLETE = 1,
	WORKER_JOB_FAILURE = 2
} WORKER_JobStatus;

typedef bool (*WORKER_JobCallback)(void* userdata);

// Create a worker.
WORKER_Object* WORKER_Create(void);

// Submit a job for a worker.
WORKER_JobID WORKER_SubmitJob(WORKER_Object* worker, WORKER_JobCallback callback, void* userdata);

// Get the status of job(s) previously submitted.
//
// If job == 0, returns the status of all submitted jobs:
// WORKER_JOB_COMPLETE means all jobs successfully completed.
// WORKER_JOB_IN_PROGRESS means at least one job is still in progress and no failure has occurred.
// WORKER_JOB_FAILURE means at least one job failed to complete.
WORKER_JobStatus WORKER_GetJobStatus(WORKER_Object* worker, WORKER_JobID job);

// Wait for job(s) to complete.
//
// If job == 0, waits for all jobs to complete.
//
// Returns true if waiting for worker(s) was successful, returns false if there
// was an error while waiting for worker(s).
bool WORKER_WaitForJobCompletion(WORKER_Object* worker, WORKER_JobID job);

// Destroy a worker. Also waits for all jobs to complete.
//
// Returns true if destroying the worker was successful, returns false if there
// was an error while destroying the worker.
bool WORKER_Destroy(WORKER_Object* worker);

#endif
