#ifndef APP_update_h_
#define APP_update_h_

#include "APP_stdinc.h"

void APP_ResetFrameStep(void);
void APP_Start(void);
bool APP_Update(void);
void APP_SetFPS(unsigned fps);
int APP_GetFPS(void);
int APP_GetRealFPS(void);
void APP_SetRenderWhileSkippingFrames(bool draw);

#endif
