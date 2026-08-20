#ifndef main_h_
#define main_h_

#include <framework/stdinc.h>

void MAIN_ResetFrameStep(void);

void MAIN_SetFPS(uint32_t fps);

int MAIN_GetFPS(void);

int MAIN_GetRealFPS(void);

void MAIN_SetRenderWhileSkippingFrames(bool render);

void MAIN_SetResourceSettings(
	int waveCount,
	const char* restrict const* restrict writeDirectories,
	size_t writeDirectoryCount,
	int planeCount,
	int textLayerCount
);

void MAIN_WaitToFinishLoading(void);

#endif
