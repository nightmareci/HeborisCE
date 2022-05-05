#include "delay.h"

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <inttypes.h>
#include <stdlib.h>

void Delay(uint32_t milliseconds) {
	static HANDLE timer = NULL;
	if (timer == NULL) {
		if ((timer = CreateWaitableTimer(NULL, TRUE, NULL)) == NULL) {
			abort();
		}
	}

	LARGE_INTEGER duration = { 0 };
	duration.QuadPart = milliseconds * INT64_C(-10000);
	SetWaitableTimer(timer, &duration, 0, NULL, NULL, 0);
	WaitForSingleObject(timer, INFINITE);
}

#endif
