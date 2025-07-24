#ifndef _include_guard_nanotime_
#define _include_guard_nanotime_

/*
 * You can choose this license, if possible in your jurisdiction:
 *
 * Unlicense
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or distribute
 * this software, either in source code form or as a compiled binary, for any
 * purpose, commercial or non-commercial, and by any means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors of
 * this software dedicate any and all copyright interest in the software to the
 * public domain. We make this dedication for the benefit of the public at
 * large and to the detriment of our heirs and successors. We intend this
 * dedication to be an overt act of relinquishment in perpetuity of all present
 * and future rights to this software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
 *
 * Alternative license choice, if works can't be directly submitted to the
 * public domain in your jurisdiction:
 *
 * The MIT License (MIT)
 *
 * Copyright (C) 2022 Brandon McGriff <nightmareci@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#if defined(_MSC_VER)
	#if (_MSC_VER < 1600)
		#error "Current Visual Studio version is not at least Visual Studio 2010, the nanotime library requires at least 2010."
	#endif
#elif defined(__cplusplus)
	#if (__cplusplus < 201103L)
		#error "Current C++ standard is not at least C++11, the nanotime library requires at least C++11."
	#endif
#elif defined(__STDC_VERSION__)
	#if (__STDC_VERSION__ < 199901L)
		#error "Current C standard is not at least C99, the nanotime library requires at least C99."
	#endif
#else
	#error "Current C or C++ standard is unknown, the nanotime library requires stdint.h and stdbool.h to be available (C99 or higher, C++11 or higher, Visual Studio 2010 or higher)."
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Implementor's note: This library directly uses Win32 APIs both for MSVC and
 * MinGW GCC, as they work for both, and produce better behavior in MinGW
 * builds. Detection of them is accomplished via checking if _WIN32 is defined,
 * as it's defined in both MSVC and MinGW GCC. Though it's convenient to have
 * UNIX-like APIs on Windows provided by MinGW, they just aren't as good as
 * directly using Win32 APIs on Windows.
 */

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#define NANOTIME_NSEC_PER_SEC UINT64_C(1000000000)

#ifndef NANOTIME_ONLY_STEP

/*
 * Returns the current time since some unspecified epoch. With the exception of
 * the standard C11 implementation and non-Apple/Mach kernel POSIX
 * implementation when neither CLOCK_MONOTONIC_RAW nor CLOCK_MONOTONIC are
 * available, the time values monotonically increase, so they're not equivalent
 * to calendar time (i.e., no leap seconds are accounted for, etc.). Calendar
 * time has to be used as a last resort sometimes, as monotonic time isn't
 * always available.
 */
uint64_t nanotime_now();

/*
 * Returns the maximum possible timestamp value. Use of this value is required
 * to properly handle overflow of timestamp values, such as when calculating the
 * interval between a time value before overflow and the next time value after
 * overflow.
 */
uint64_t nanotime_now_max();

/*
 * Sleeps the current thread for the requested count of nanoseconds. The slept
 * duration may be less than, equal to, or greater than the time requested.
 */
void nanotime_sleep(uint64_t nsec_count);

/*
 * Yield the CPU/core that called nanotime_yield to the operating system for a
 * small time slice.
 */
void nanotime_yield();

#endif

/*
 * Calculates the time interval between two nanosecond time values, correctly
 * handling the case when the end time value overflows past max. You should
 * probably use this function when calculating time intervals, as not all
 * platforms' maximum timestamp value is UINT64_MAX, which is required for the
 * trivial "end - start" formula for interval calculation to work as expected.
 */
uint64_t nanotime_interval(const uint64_t start, const uint64_t end, const uint64_t max);

typedef struct nanotime_step_data {
	uint64_t sleep_duration;
	uint64_t now_max;
	uint64_t (* now)();
	void (* sleep)(uint64_t nsec_count);

	uint64_t zero_sleep_duration;
	uint64_t accumulator;
	uint64_t sleep_point;
} nanotime_step_data;

/*
 * Initializes the nanotime precise fixed timestep object. Call immediately
 * before entering the loop using the stepper object.
 */
void nanotime_step_init(
	nanotime_step_data* const stepper,
	const uint64_t sleep_duration,
	const uint64_t now_max,
	uint64_t (* const now)(),
	void (* const sleep)(uint64_t nsec_count)
);

/*
 * Does one step of sleeping for a fixed timestep logic update cycle. It makes
 * a best-attempt at a precise delay per iteration, but might skip a cycle of
 * sleeping if skipping sleeps is required to catch up to the correct
 * wall-clock time. Returns true if a sleep up to the latest target sleep end
 * time occurred, otherwise returns false in the case of a sleep step skip.
 */
bool nanotime_step(nanotime_step_data* const stepper);

#if !defined(NANOTIME_ONLY_STEP) && defined(NANOTIME_IMPLEMENTATION)

/*
 * Non-portable, platform-specific implementations are first. If none of them
 * match the current platform, the standard C/C++ versions are used as a last
 * resort.
 */

/*
 * Checking _WIN32 must be above the UNIX-like implementations, so MinGW is
 * guaranteed to use it.
 */
#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

#ifndef NANOTIME_NOW_IMPLEMENTED
uint64_t nanotime_now() {
	static uint64_t scale = UINT64_C(0);
	static bool multiply;
	if (scale == 0u) {
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		if (frequency.QuadPart < NANOTIME_NSEC_PER_SEC) {
			scale = NANOTIME_NSEC_PER_SEC / frequency.QuadPart;
			multiply = true;
		}
		else {
			scale = frequency.QuadPart / NANOTIME_NSEC_PER_SEC;
			multiply = false;
		}
	}
	LARGE_INTEGER performanceCount;
	QueryPerformanceCounter(&performanceCount);
	if (multiply) {
		return performanceCount.QuadPart * scale;
	}
	else {
		return performanceCount.QuadPart / scale;
	}
}
#define NANOTIME_NOW_IMPLEMENTED
#endif

#ifndef NANOTIME_NOW_MAX_IMPLEMENTED
uint64_t nanotime_now_max() {
	static uint64_t now_max;
	if (now_max == UINT64_C(0)) {
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		if (frequency.QuadPart < NANOTIME_NSEC_PER_SEC) {
			now_max = UINT64_MAX * (NANOTIME_NSEC_PER_SEC / frequency.QuadPart);
		}
		else {
			now_max = UINT64_MAX / (frequency.QuadPart / NANOTIME_NSEC_PER_SEC);
		}
	}
	return now_max;
}
#define NANOTIME_NOW_MAX_IMPLEMENTED
#endif

#ifndef NANOTIME_SLEEP_IMPLEMENTED
void nanotime_sleep(uint64_t nsec_count) {
	LARGE_INTEGER dueTime;

	if (nsec_count < UINT64_C(100)) {
		/*
		 * Allows the OS to schedule another process for a single time
		 * slice. Better than a delay of 0, which immediately returns
		 * with no actual non-CPU-hogging delay. The time-slice-yield
		 * behavior is specified in Microsoft's Windows documentation.
		 */
		SleepEx(0UL, FALSE);
	}
	else {
		HANDLE timer = NULL;
		if (
			#ifdef CREATE_WAITABLE_TIMER_HIGH_RESOLUTION
			/*
			 * Requesting a high resolution timer can make quite the
			 * difference, so always request high resolution if available. It's
			 * available in Windows 10 1803 and above. This arrangement of
			 * building it if the build system supports it will allow the
			 * executable to use high resolution if available on a user's
			 * system, but revert to low resolution if the user's system
			 * doesn't support high resolution.
			 */
			(timer = CreateWaitableTimerEx(NULL, NULL, CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_ALL_ACCESS)) == NULL &&
			#endif
			(timer = CreateWaitableTimer(NULL, TRUE, NULL)) == NULL
		) {
			return;
		}

		dueTime.QuadPart = -(LONGLONG)(nsec_count / UINT64_C(100));

		SetWaitableTimer(timer, &dueTime, 0L, NULL, NULL, FALSE);
		WaitForSingleObject(timer, INFINITE);

		CloseHandle(timer);
	}
}
#define NANOTIME_SLEEP_IMPLEMENTED
#endif

#ifndef NANOTIME_YIELD_IMPLEMENTED
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
void nanotime_yield() {
	YieldProcessor();
}
#define NANOTIME_YIELD_IMPLEMENTED
#endif

#endif

/*
 * To avoid using standard UNIX APIs on UNIX-like platforms, the
 * platform-specific implementations must be first. That way, the
 * lower-overhead kernel APIs can be used, that aren't UNIX-like.
 */

#ifndef NANOTIME_NOW_IMPLEMENTED
#if defined(__APPLE__) || defined(__MACH__)
/*
 * The current platform is some Apple operating system, or at least uses some
 * Mach kernel. The POSIX implementation below using clock_gettime works on at
 * least Apple platforms, though this version using Mach functions has lower
 * overhead.
 */
#include <mach/mach_time.h>
uint64_t nanotime_now() {
	static mach_timebase_info_data_t info = { 0 };
	if (info.denom == UINT32_C(0)) {
		const kern_return_t status = mach_timebase_info(&info);
		assert(status == KERN_SUCCESS);
		if (status != KERN_SUCCESS) {
			return UINT64_C(0);
		}
	}
	return (mach_absolute_time() * info.numer) / info.denom;
}
#define NANOTIME_NOW_IMPLEMENTED
#endif
#endif

#ifndef NANOTIME_NOW_MAX_IMPLEMENTED
#if defined(__APPLE__) || defined(__MACH__)
#include <mach/mach_time.h>
uint64_t nanotime_now_max() {
	static uint64_t now_max = UINT64_C(0);
	if (now_max == UINT64_C(0)) {
		mach_timebase_info_data_t info;
		const kern_return_t status = mach_timebase_info(&info);
		assert(status == KERN_SUCCESS);
		if (status != KERN_SUCCESS) {
			return UINT64_C(0);
		}
		else {
			now_max = UINT64_MAX / info.denom;
		}
	}
	return now_max;
}
#define NANOTIME_NOW_MAX_IMPLEMENTED
#endif
#endif

#ifndef NANOTIME_NOW_IMPLEMENTED
#if defined(__unix__) && defined(_POSIX_VERSION) && (_POSIX_VERSION >= 199309L) && !defined(NANOTIME_NOW_IMPLEMENTED)
/*
 * Current platform is some version of POSIX, that might have clock_gettime.
 */
#include <unistd.h>
#include <time.h>
#include <errno.h>
uint64_t nanotime_now() {
	struct timespec now;
	const int status = clock_gettime(
		#if defined(CLOCK_MONOTONIC_RAW)
		/*
		 * Monotonic raw is more precise, but not always available. For
		 * the sorts of applications this code is intended for, mainly
		 * soft real time applications such as game programming, the
		 * subtle inconsistencies of it vs. monotonic aren't an issue.
		 */
		CLOCK_MONOTONIC_RAW
		#elif defined(CLOCK_MONOTONIC)
		/*
		 * Monotonic is quite good, and widely available, but not as
		 * precise as monotonic raw, so it's only used if required.
		 */
		CLOCK_MONOTONIC
		#else
		/*
		 * Realtime isn't fully correct, as it's calendar time, but is
		 * even more widely available than monotonic. Monotonic is only
		 * unavailable on very old platforms though, so old they're
		 * likely unused now (as of last editing this, 2023).
		 */
		CLOCK_REALTIME
		#endif
	, &now);
	assert(status == 0 || (status == -1 && errno != EOVERFLOW));
	if (status == 0 || (status == -1 && errno != EOVERFLOW)) {
		return (uint64_t)now.tv_sec * NANOTIME_NSEC_PER_SEC + (uint64_t)now.tv_nsec;
	}
	else {
		return UINT64_C(0);
	}
}
#define NANOTIME_NOW_IMPLEMENTED
#endif
#endif

#ifndef NANOTIME_SLEEP_IMPLEMENTED
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__)) || defined(__MINGW32__) || defined(__MINGW64__)
#include <unistd.h>
#include <time.h>
#include <errno.h>
void nanotime_sleep(uint64_t nsec_count) {
	const struct timespec req = {
		.tv_sec = (time_t)(nsec_count / NANOTIME_NSEC_PER_SEC),
		.tv_nsec = (long)(nsec_count % NANOTIME_NSEC_PER_SEC)
	};
	#ifndef NDEBUG
	const int status =
	#endif
	nanosleep(&req, NULL);
	assert(status == 0 || (status == -1 && errno != EINVAL));
}
#define NANOTIME_SLEEP_IMPLEMENTED
#endif
#endif

#ifndef NANOTIME_YIELD_IMPLEMENTED
#if (defined(__unix__) || defined(__APPLE__)) && defined(_POSIX_VERSION) && (_POSIX_VERSION >= 200112L)
#include <sched.h>
void nanotime_yield() {
	(void)sched_yield();
}
#define NANOTIME_YIELD_IMPLEMENTED
#endif
#endif


#ifndef NANOTIME_NOW_IMPLEMENTED
#if defined(__vita__)
#include <psp2/kernel/processmgr.h>
uint64_t nanotime_now() {
	return sceKernelGetProcessTimeWide() * UINT64_C(1000);
}
#define NANOTIME_NOW_IMPLEMENTED
#endif
#endif

#ifndef NANOTIME_SLEEP_IMPLEMENTED
#if defined(__vita__)
#include <psp2/kernel/processmgr.h>
void nanotime_sleep(uint64_t nsec_count) {
	sceKernelDelayThreadCB(nsec_count / UINT64_C(1000));
}
#define NANOTIME_SLEEP_IMPLEMENTED
#endif
#endif


#ifndef NANOTIME_SLEEP_IMPLEMENTED
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
/*
 * NOTE: You *must* have asyncify enabled in the Emscripten build (pass
 * -sASYNCIFY to the compiler/linker) or sleeping won't work.
 */
void nanotime_sleep(uint64_t nsec_count) {
	emscripten_sleep(nsec_count / UINT64_C(1000000));
}
#define NANOTIME_SLEEP_IMPLEMENTED
#endif
#endif

#ifndef NANOTIME_NOW_IMPLEMENTED
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
uint64_t nanotime_now() {
	const double now = emscripten_get_now();
	return (uint64_t)now * UINT64_C(1000000);
}
#define NANOTIME_NOW_IMPLEMENTED
#endif
#endif


#ifndef NANOTIME_SLEEP_IMPLEMENTED
#ifdef __SWITCH__
#include <switch.h>
void nanotime_sleep(uint64_t nsec_count) {
	if (nsec_count > INT64_MAX) {
		svcSleepThread(INT64_MAX);
	}
	else {
		svcSleepThread((s64)nsec_count);
	}
}
#define NANOTIME_SLEEP_IMPLEMENTED
#endif
#endif

#ifndef NANOTIME_NOW_IMPLEMENTED
#ifdef __SWITCH__
#include <switch.h>
uint64_t nanotime_now() {
	return armTicksToNs(armGetSystemTick());
}
#define NANOTIME_NOW_IMPLEMENTED
#endif
#endif

#ifndef NANOTIME_YIELD_IMPLEMENTED
#if defined(__SWITCH__)
#include <switch.h>
void nanotime_yield() {
	svcSleepThread(YieldType_ToAnyThread);
}
#define NANOTIME_YIELD_IMPLEMENTED
#endif
#endif


#ifndef NANOTIME_NOW_IMPLEMENTED
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
#include <time.h>
uint64_t nanotime_now() {
	struct timespec now;
	const int status = timespec_get(&now, TIME_UTC);
	assert(status == TIME_UTC);
	if (status == TIME_UTC) {
		return (uint64_t)now.tv_sec * NANOTIME_NSEC_PER_SEC + (uint64_t)now.tv_nsec;
	}
	else {
		return UINT64_C(0);
	}
}
#define NANOTIME_NOW_IMPLEMENTED
#endif
#endif

#ifndef NANOTIME_SLEEP_IMPLEMENTED
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L) && !defined(__STDC_NO_THREADS__)
#include <threads.h>
void nanotime_sleep(uint64_t nsec_count) {
	const struct timespec req = {
		.tv_sec = (time_t)(nsec_count / NANOTIME_NSEC_PER_SEC),
		.tv_nsec = (long)(nsec_count % NANOTIME_NSEC_PER_SEC)
	};
	const int status = thrd_sleep(&req, NULL);
	assert(status == 0 || status == -1);
}
#define NANOTIME_SLEEP_IMPLEMENTED
#endif
#endif

#ifndef NANOTIME_YIELD_IMPLEMENTED
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L) && !defined(__STDC_NO_THREADS__)
#include <threads.h>
void nanotime_yield() {
	thrd_yield();
}
#define NANOTIME_YIELD_IMPLEMENTED
#endif
#endif


#ifdef __cplusplus
}
#endif

/*
 * C++ implementations follow here, but defined with C linkage.
 */

#ifndef NANOTIME_NOW_IMPLEMENTED
#ifdef __cplusplus
#include <cstdint>
#include <chrono>
extern "C" uint64_t nanotime_now() {
	return static_cast<uint64_t>(
		std::chrono::time_point_cast<std::chrono::nanoseconds>(
			std::chrono::steady_clock::now()
		).time_since_epoch().count()
	);
}
#define NANOTIME_NOW_IMPLEMENTED
#endif
#endif

#ifndef NANOTIME_SLEEP_IMPLEMENTED
#ifdef __cplusplus
#include <cstdint>
#include <thread>
#include <exception>
extern "C" void nanotime_sleep(uint64_t nsec_count) {
	try {
		std::this_thread::sleep_for(std::chrono::nanoseconds(nsec_count));
	}
	catch (std::exception e) {
	}
}
#define NANOTIME_SLEEP_IMPLEMENTED
#endif
#endif

#ifndef NANOTIME_YIELD_IMPLEMENTED
#ifdef __cplusplus
#include <thread>
extern "C" void nanotime_yield() {
	std::this_thread::yield();
}
#define NANOTIME_YIELD_IMPLEMENTED
#endif
#endif


#ifndef NANOTIME_NOW_IMPLEMENTED
#error "Failed to implement nanotime_now (try using C11 with C11 threads support or C++11)."
#endif

#ifndef NANOTIME_SLEEP_IMPLEMENTED
#error "Failed to implement nanotime_sleep (try using C11 with C11 threads support or C++11)."
#endif

#ifndef NANOTIME_YIELD_IMPLEMENTED
#ifdef __cplusplus
extern "C" {
#endif
/*
 * As a last resort, make a zero-duration sleep request to implement yield.
 * Such sleep requests often have the desired yielding behavior on many
 * platforms.
 */
void nanotime_yield() {
	nanotime_sleep(0u);
}
#define NANOTIME_YIELD_IMPLEMENTED
#ifdef __cplusplus
}
#endif
#endif


#ifdef __cplusplus
extern "C" {
#endif

#ifndef NANOTIME_NOW_MAX_IMPLEMENTED
/*
 * Might not be correct on some platforms, but it's the best we can do as a last
 * resort.
 */
uint64_t nanotime_now_max() {
	return UINT64_MAX;
}
#define NANOTIME_NOW_MAX_IMPLEMENTED
#endif

#endif


#ifdef NANOTIME_IMPLEMENTATION

uint64_t nanotime_interval(const uint64_t start, const uint64_t end, const uint64_t max) {
	assert(max > UINT64_C(0));
	assert(start <= max);
	assert(end <= max);

	if (end >= start) {
		return end - start;
	}
	else {
		return end + (max - start) + UINT64_C(1);
	}
}

void nanotime_step_init(
	nanotime_step_data* const stepper,
	const uint64_t sleep_duration,
	const uint64_t now_max,
	uint64_t (* const now)(),
	void (* const sleep)(uint64_t nsec_count)
) {
	assert(stepper != NULL);
	assert(sleep_duration > UINT64_C(0));
	assert(now_max > UINT64_C(0));
	assert(now != NULL);
	assert(sleep != NULL);

	stepper->sleep_duration = sleep_duration;
	stepper->now_max = now_max;
	stepper->now = now;
	stepper->sleep = sleep;

	const uint64_t start = now();
	sleep(UINT64_C(0));
	stepper->zero_sleep_duration = nanotime_interval(start, now(), now_max);
	stepper->accumulator = UINT64_C(0);

	/*
	 * This should be last here, so the sleep point is close to what it
	 * should be.
	 */
	stepper->sleep_point = now();
}

bool nanotime_step(nanotime_step_data* const stepper) {
	assert(stepper != NULL);

	const uint64_t start_point = stepper->now();

	if (nanotime_interval(stepper->sleep_point, start_point, stepper->now_max) >= stepper->sleep_duration + NANOTIME_NSEC_PER_SEC / UINT64_C(10)) {
		stepper->sleep_point = start_point;
		stepper->accumulator = UINT64_C(0);
	}

	bool slept;
	if (stepper->accumulator < stepper->sleep_duration) {
		const uint64_t total_sleep_duration = stepper->sleep_duration - stepper->accumulator;
		uint64_t current_sleep_duration = total_sleep_duration;
		const uint64_t shift = UINT64_C(4);

		/*
		 * The algorithm implemented here takes the assumption that a
		 * sequence of repeated sleep requests of the same requested
		 * duration end up being approximately of equal actual sleep
		 * duration, even if they're all well above the requested
		 * duration. In practice, such an assumption proves out to be
		 * true on various platforms.
		 */

		/*
		 * A big initial sleep lowers power usage on any platform, as
		 * more small sleep requests use more power than fewer bigger,
		 * equivalent sleep requests. In practice, operating systems
		 * "actually sleep" when 1ms or more is requested, and 1ms is
		 * the minimum request duration you can make on some platforms
		 * (like older versions of Windows). Additionally, power usage
		 * is nice and low when doing the number of 1ms sleeps that's
		 * (hopefully) short of the target duration.
		 *
		 * But, the loop here maintains a maximum of the actual slept
		 * durations, breaking out when the time remaining is greater
		 * than or equal to the maximum found. By breaking out on the
		 * maximum found rather than just 1ms-or-less remaining,
		 * sleeping beyond the target deadline is reduced.
		 */
		{
			uint64_t max = NANOTIME_NSEC_PER_SEC / UINT64_C(1000);
			uint64_t start = stepper->now();
			while (nanotime_interval(stepper->sleep_point, start, stepper->now_max) + max < total_sleep_duration) {
				stepper->sleep(NANOTIME_NSEC_PER_SEC / UINT64_C(1000));
				const uint64_t next = stepper->now();
				const uint64_t current_interval = nanotime_interval(start, next, stepper->now_max);
				if (current_interval > max) {
					max = current_interval;
				}
				start = next;
			}
			const uint64_t initial_duration = nanotime_interval(start_point, stepper->now(), stepper->now_max);
			if (initial_duration < current_sleep_duration) {
				current_sleep_duration -= initial_duration;
			}
			else {
				goto step_end;
			}
		}

		/*
		 * This has the flavor of Zeno's dichotomous paradox of motion,
		 * as it successively divides the time remaining to sleep, but
		 * attempts to stop short of the deadline to hopefully be able
		 * to precisely sleep up to the deadline below this loop. The
		 * divisor is larger than two though, as it produces better
		 * behavior, and seems to work fine in testing on real
		 * hardware. The same method of keeping track of the max
		 * duration per loop of same sleep request durations above is
		 * used here. The overshoot possible in the loop below this one
		 * won't overshoot much, or in the best case won't overshoot,
		 * so the busyloop can finish up the sleep precisely.
		 */
		current_sleep_duration >>= shift;
		for (
			uint64_t max = stepper->zero_sleep_duration;
			nanotime_interval(stepper->sleep_point, stepper->now(), stepper->now_max) + max < total_sleep_duration && current_sleep_duration > UINT64_C(0);
			current_sleep_duration >>= shift
		) {
			max = stepper->zero_sleep_duration;
			uint64_t start;
			while (max < stepper->sleep_duration && nanotime_interval(stepper->sleep_point, start = stepper->now(), stepper->now_max) + max < total_sleep_duration) {
				stepper->sleep(current_sleep_duration);
				uint64_t slept_duration;
				if ((slept_duration = nanotime_interval(start, stepper->now(), stepper->now_max)) > max) {
					max = slept_duration;
				}
			}
		}
		if (nanotime_interval(stepper->sleep_point, stepper->now(), stepper->now_max) >= total_sleep_duration) {
			goto step_end;
		}

		{
			/*
			 * After (hopefully) stopping short of the deadline by
			 * a small amount, do small sleeps here to get closer
			 * to the deadline, but again attempting to stop short
			 * by an even smaller amount. It's best to do larger
			 * sleeps as done in the above loops, to reduce
			 * CPU/power usage, as each sleep iteration has a
			 * more-or-less fixed overhead of CPU/power usage.
			 *
			 * In testing on an M1 Mac mini running macOS, power
			 * usage is lower using zero-duration sleeps vs.
			 * nanotime_yield(), with no loss of timing precision.
			 * The same might be true for other hardwares/operating
			 * systems.
			 */
			uint64_t max = stepper->zero_sleep_duration;
			uint64_t start;
			while (nanotime_interval(stepper->sleep_point, start = stepper->now(), stepper->now_max) + max < total_sleep_duration) {
				stepper->sleep(UINT64_C(0));
				if ((stepper->zero_sleep_duration = nanotime_interval(start, stepper->now(), stepper->now_max)) > max) {
					max = stepper->zero_sleep_duration;
				}
			}
		}

		step_end:
		{
			/*
			 * Finally, do a busyloop to precisely sleep up to the
			 * deadline. The code above this loop attempts to
			 * reduce the remaining time to sleep to a minimum via
			 * process-yielding sleeps, so the amount of time spent
			 * spinning here is hopefully quite low.
			 *
			 * In testing on an M1 Mac mini running macOS,
			 * busylooping here produces the absolute greatest
			 * precision possible on the hardware, down to the
			 * sub-10ns-off-per-update range for longish stretches
			 * during 60 Hz updates, but in the
			 * hundreds-to-thousands of nanoseconds off when using
			 * nanotime_yield() or zero-duration sleeps. And,
			 * because the sleeping algorithm above does such a
			 * good job of stopping very close to the deadline,
			 * busylooping here has basically negligible difference
			 * in power usage vs. yields/zero-duration sleeps.
			 */
			uint64_t current_time;
			uint64_t accumulated;
			while ((accumulated = nanotime_interval(stepper->sleep_point, current_time = stepper->now(), stepper->now_max)) < total_sleep_duration);

			stepper->accumulator += accumulated;
			stepper->sleep_point = current_time;
			slept = true;
		}
	}
	else {
		slept = false;
	}
	stepper->accumulator -= stepper->sleep_duration;
	return slept;
}

#endif

#ifdef __cplusplus
}
#endif

#endif /* _include_guard_nanotime_ */
