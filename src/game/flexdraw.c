// 描画倍率を可変できるようになった関数群。
// 通常の描画関数の頭に Ex をつけます。

#include "common.h"

int32_t lastZoomRate = 1;
int32_t zoomRate = 1;			/* 描画倍率 */

void setDrawRate(int32_t size) {
	lastZoomRate = zoomRate;
	zoomRate = size;
}

int32_t getDrawRate() {
	return zoomRate;
}

int32_t getLastDrawRate() {
	return lastZoomRate;
}

void ExBlt(int32_t pno, int32_t dx, int32_t dy)
{
	APP_DrawPlane(pno, dx * zoomRate, dy * zoomRate);
}

void ExBltRect(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy)
{
	APP_DrawPlaneRect(pno, dx * zoomRate, dy * zoomRate, sx * zoomRate, sy * zoomRate, hx * zoomRate, hy * zoomRate);
}

void ExBlendBltRect(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy, int32_t a)
{
	APP_DrawPlaneRectTransparent(pno, dx * zoomRate, dy * zoomRate, sx * zoomRate, sy * zoomRate, hx * zoomRate, hy * zoomRate, a);
}

void ExBltR(int32_t pno, int32_t dx, int32_t dy, int32_t scx, int32_t scy)
{
	APP_DrawPlaneScaled(pno, dx * zoomRate, dy * zoomRate, scx, scy);
}

void ExBltRectR(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy, int32_t scx, int32_t scy)
{
	APP_DrawPlaneRectScaled(pno, dx * zoomRate, dy * zoomRate, sx * zoomRate, sy * zoomRate, hx * zoomRate, hy * zoomRate, scx, scy);
}

void ExBltFastR(int32_t pno, int32_t dx, int32_t dy, int32_t scx, int32_t scy)
{
	APP_DrawPlaneScaled(pno, dx * zoomRate, dy * zoomRate, scx, scy);
}

void ExBltFastRectR(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy, int32_t scx, int32_t scy)
{
	APP_DrawPlaneRectScaled(pno, dx * zoomRate, dy * zoomRate, sx * zoomRate, sy * zoomRate, hx * zoomRate, hy * zoomRate, scx, scy);
}

void BlendExBlt(int32_t pno, int32_t dx, int32_t dy, int32_t a)
{
	APP_DrawPlaneTransparent(pno, dx * zoomRate, dy * zoomRate, a);
}

void BlendExBltRect(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy, int32_t a)
{
	APP_DrawPlaneRectTransparent(pno, dx * zoomRate, dy * zoomRate, sx * zoomRate, sy * zoomRate, hx * zoomRate, hy * zoomRate, a);
}

void BlendExBltR(int32_t pno, int32_t dx, int32_t dy, int32_t a, int32_t scx, int32_t scy)
{
	APP_DrawPlaneTransparentScaled(pno, dx * zoomRate, dy * zoomRate, a, scx, scy);
}

void BlendExBltRectR(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy, int32_t a, int32_t scx, int32_t scy)
{
	APP_DrawPlaneRectTransparentScaled(pno, dx * zoomRate, dy * zoomRate, sx * zoomRate, sy * zoomRate, hx * zoomRate, hy * zoomRate, a, scx, scy);
}

void ExBltSpriteText(int32_t pno, const char* str, int32_t cf, int32_t cw, int32_t ch, int32_t dx, int32_t dy, int32_t shx, int32_t shy, int32_t shw)
{
	APP_DrawPlaneText(pno, str, cf, cw * zoomRate, ch * zoomRate, dx * zoomRate, dy * zoomRate, shx * zoomRate, shy * zoomRate, shw * zoomRate);
}

void ExTextLayerOn(int32_t n, int32_t x, int32_t y)
{
	 APP_EnableTextLayer(n, x * zoomRate, y * zoomRate);
}

void ExTextLayerMove(int32_t n, int32_t x, int32_t y)
{
	 APP_SetTextLayerDrawPosition(n, x * zoomRate, y * zoomRate);
}

void ExTextLayerSize(int32_t n, int32_t size)
{
	 APP_SetTextLayerSize(n, size * zoomRate);
}
