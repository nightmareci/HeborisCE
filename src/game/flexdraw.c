// 描画倍率を可変できるようになった関数群。
// 通常の描画関数の頭に Ex をつけます。

#include "flexdraw.h"

int32_t lastZoomRate = 1;
int32_t zoomRate = 1;			/* 描画倍率 */

void ExBlt(int32_t pno, int32_t dx, int32_t dy)
{
	APP_DrawPlane(pno, dx * zoomRate, dy * zoomRate);
}

void ExBltRect(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy)
{
	APP_DrawPlaneRect(pno, dx * zoomRate, dy * zoomRate, sx * zoomRate, sy * zoomRate, hx * zoomRate, hy * zoomRate);
}

void ExBltRectScaled(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy, int32_t scx, int32_t scy)
{
	APP_DrawPlaneRectScaled(pno, dx * zoomRate, dy * zoomRate, sx * zoomRate, sy * zoomRate, hx * zoomRate, hy * zoomRate, scx, scy);
}

void ExBlendBlt(int32_t pno, int32_t dx, int32_t dy, int32_t a)
{
	APP_DrawPlaneTransparent(pno, dx * zoomRate, dy * zoomRate, a);
}

void ExBlendBltRect(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy, int32_t a)
{
	APP_DrawPlaneRectTransparent(pno, dx * zoomRate, dy * zoomRate, sx * zoomRate, sy * zoomRate, hx * zoomRate, hy * zoomRate, a);
}

void ExBlendBltRectScaled(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy, int32_t a, int32_t scx, int32_t scy)
{
	APP_DrawPlaneRectTransparentScaled(pno, dx * zoomRate, dy * zoomRate, sx * zoomRate, sy * zoomRate, hx * zoomRate, hy * zoomRate, a, scx, scy);
}

void ExBltText(int32_t pno, const char* str, int32_t cf, int32_t cw, int32_t ch, int32_t dx, int32_t dy, int32_t shx, int32_t shy, int32_t shw)
{
	APP_DrawPlaneText(pno, str, cf, cw * zoomRate, ch * zoomRate, dx * zoomRate, dy * zoomRate, shx * zoomRate, shy * zoomRate, shw * zoomRate);
}

void ExTextLayerSize(int32_t n, int32_t size)
{
	 APP_SetTextLayerSize(n, size * zoomRate);
}

void ExTextLayerMove(int32_t n, int32_t x, int32_t y)
{
	 APP_SetTextLayerDrawPosition(n, x * zoomRate, y * zoomRate);
}

void ExTextLayerOn(int32_t n, int32_t x, int32_t y)
{
	 APP_EnableTextLayer(n, x * zoomRate, y * zoomRate);
}

int32_t getLastDrawRate() {
	return lastZoomRate;
}

int32_t getDrawRate() {
	return zoomRate;
}

void setDrawRate(int32_t size) {
	lastZoomRate = zoomRate;
	zoomRate = size;
}
