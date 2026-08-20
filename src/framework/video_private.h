#ifndef video_private_h_
#define video_private_h_

#include <framework/stdinc.h>

bool VIDEO_Init(int planeCount, int textLayerCount);
void VIDEO_Quit(void);
float VIDEO_GetScreenSubpixelOffset(void);

#endif
