#include "delay.h"

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>

extern bool loopFlag;
extern int32_t restart;

void Delay(uint32_t milliseconds) {
	if (milliseconds == 0u) {
		SleepEx(0u, FALSE);
	}
	else {
		static HANDLE timer = NULL;
		if (timer == NULL) {
			if ((timer = CreateWaitableTimer(NULL, TRUE, NULL)) == NULL) {
				loopFlag = false;
				restart = 0;
				return;
			}
		}

		LARGE_INTEGER duration = { 0 };
		duration.QuadPart = milliseconds * INT64_C(-10000) + 1000;
		SetWaitableTimer(timer, &duration, 0, NULL, NULL, 0);
		WaitForSingleObject(timer, INFINITE);
	}

}

#endif
