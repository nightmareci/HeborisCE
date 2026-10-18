#include "flexdraw.h"

static int32_t lastDrawScale = 1;
static int32_t drawScale = 1;

void DrawPlane(int32_t pno, int32_t dx, int32_t dy)
{
	VIDEO_DrawPlane(pno, dx * drawScale, dy * drawScale, 0, 0, 0, 0, 0xFF, 1.0f, 1.0f);
}

void DrawPlaneRect(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy)
{
	VIDEO_DrawPlane(pno, dx * drawScale, dy * drawScale, sx * drawScale, sy * drawScale, hx * drawScale, hy * drawScale, 0xFF, 1.0f, 1.0f);
}

void DrawPlaneRectScaled(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy, float scx, float scy)
{
	VIDEO_DrawPlane(pno, dx * drawScale, dy * drawScale, sx * drawScale, sy * drawScale, hx * drawScale, hy * drawScale, 0xFF, scx, scy);
}

void DrawPlaneTransparent(int32_t pno, int32_t dx, int32_t dy, int32_t a)
{
	VIDEO_DrawPlane(pno, dx * drawScale, dy * drawScale, 0, 0, 0, 0, a, 1.0f, 1.0f);
}

void DrawPlaneRectTransparent(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy, int32_t a)
{
	VIDEO_DrawPlane(pno, dx * drawScale, dy * drawScale, sx * drawScale, sy * drawScale, hx * drawScale, hy * drawScale, a, 1.0f, 1.0f);
}

void DrawPlaneRectTransparentScaled(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy, int32_t a, float scx, float scy)
{
	VIDEO_DrawPlane(pno, dx * drawScale, dy * drawScale, sx * drawScale, sy * drawScale, hx * drawScale, hy * drawScale, a, scx, scy);
}

void DrawPlaneText(int32_t pno, const char* str, int32_t cf, int32_t cw, int32_t ch, int32_t dx, int32_t dy, int32_t shx, int32_t shy, int32_t shw)
{
	VIDEO_DrawPlaneText(pno, str, cf, cw * drawScale, ch * drawScale, dx * drawScale, dy * drawScale, shx * drawScale, shy * drawScale, shw * drawScale);
}

void SetTextLayerSize(int32_t n, int32_t size)
{
	 VIDEO_SetTextLayerSize(n, size * drawScale);
}

void SetTextLayerDrawPosition(int32_t n, int32_t x, int32_t y)
{
	 VIDEO_SetTextLayerDrawPosition(n, x * drawScale, y * drawScale);
}

void EnableTextLayer(int32_t n, int32_t x, int32_t y)
{
	 VIDEO_EnableTextLayer(n, x * drawScale, y * drawScale);
}

int32_t GetLastDrawScale(void) {
	return lastDrawScale;
}

int32_t GetDrawScale(void) {
	return drawScale;
}

void SetDrawScale(int32_t scale) {
	lastDrawScale = drawScale;
	drawScale = scale;
}
