#ifndef flexdraw_h_
#define flexdraw_h_

#include <framework/framework.h>

void DrawPlane(int32_t pno, int32_t dx, int32_t dy);
void DrawPlaneRect(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy);
void DrawPlaneRectScaled(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy, float scx, float scy);

void DrawPlaneTransparent(int32_t pno, int32_t dx, int32_t dy, int32_t a);
void DrawPlaneRectTransparent(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy, int32_t a);
void DrawPlaneRectTransparentScaled(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy, int32_t a, float scx, float scy);

void DrawPlaneText(int32_t pno, const char* str, int32_t cf, int32_t cw, int32_t ch, int32_t dx, int32_t dy, int32_t shx, int32_t shy, int32_t shw);

void SetTextLayerSize(int32_t n, int32_t size);
void SetTextLayerDrawPosition(int32_t n, int32_t x, int32_t y);
void EnableTextLayer(int32_t n, int32_t x, int32_t y);

int32_t GetLastDrawScale(void);
int32_t GetDrawScale(void);
void SetDrawScale(int32_t size);

#endif
