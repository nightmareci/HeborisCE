// 描画倍率を可変できるようになった関数群。
// 通常の描画関数の頭に Ex をつけます。

#include "include.h"

int32_t zoomRate = 1;			/* 描画倍率 */

void setDrawRate(int32_t size) {
	zoomRate = size;
}

int32_t getDrawRate() {
	return zoomRate;
}

void ExBlt(int32_t pno, int32_t dx, int32_t dy)
{
	Blt(pno, dx * zoomRate, dy * zoomRate);
}

void ExBltRect(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy)
{
	BltRect(pno, dx * zoomRate, dy * zoomRate, sx * zoomRate, sy * zoomRate, hx * zoomRate, hy * zoomRate);
}

void ExBltFast(int32_t pno, int32_t dx, int32_t dy)
{
	BltFast(pno, dx * zoomRate, dy * zoomRate);
}

void ExBltFastRect(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy)
{
	BltFastRect(pno, dx * zoomRate, dy * zoomRate, sx * zoomRate, sy * zoomRate, hx * zoomRate, hy * zoomRate);
}


void ExBlendBlt(int32_t pno, int32_t dx, int32_t dy, int32_t ar, int32_t ag, int32_t ab, int32_t br, int32_t bg, int32_t bb)
{
	BlendBlt(pno, dx * zoomRate, dy * zoomRate, ar, ag, ab, br, bg, bb);
}

void ExBlendBltRect(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy, int32_t ar, int32_t ag, int32_t ab, int32_t br, int32_t bg, int32_t bb)
{
	BlendBltRect(pno, dx * zoomRate, dy * zoomRate, sx * zoomRate, sy * zoomRate, hx * zoomRate, hy * zoomRate, ar, ag, ab, br, bg, bb);
}

void ExBltR(int32_t pno, int32_t dx, int32_t dy, int32_t scx, int32_t scy)
{
	BltR(pno, dx * zoomRate, dy * zoomRate, scx, scy);
}

void ExBltRectR(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy, int32_t scx, int32_t scy)
{
	BltRectR(pno, dx * zoomRate, dy * zoomRate, sx * zoomRate, sy * zoomRate, hx * zoomRate, hy * zoomRate, scx, scy);
}

void ExBltFastR(int32_t pno, int32_t dx, int32_t dy, int32_t scx, int32_t scy)
{
	BltFastR(pno, dx * zoomRate, dy * zoomRate, scx, scy);
}

void ExBltFastRectR(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy, int32_t scx, int32_t scy)
{
	BltFastRectR(pno, dx * zoomRate, dy * zoomRate, sx * zoomRate, sy * zoomRate, hx * zoomRate, hy * zoomRate, scx, scy);
}

/*
void ExBltTrans(int32_t pno, int32_t dx, int32_t dy)
{
	BltTrans(pno, dx * zoomRate, dy * zoomRate);
}
*/

void BlendExBlt(int32_t pno, int32_t dx, int32_t dy, int32_t ar, int32_t ag, int32_t ab, int32_t br, int32_t bg, int32_t bb)
{
	BlendBlt(pno, dx * zoomRate, dy * zoomRate, ar, ag, ab, br, bg, bb);
}

void BlendExBltRect(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy, int32_t ar, int32_t ag, int32_t ab, int32_t br, int32_t bg, int32_t bb)
{
	BlendBltRect(pno, dx * zoomRate, dy * zoomRate, sx * zoomRate, sy * zoomRate, hx * zoomRate, hy * zoomRate, ar, ag, ab, br, bg, bb);
}

void BlendExBltR(int32_t pno, int32_t dx, int32_t dy, int32_t ar, int32_t ag, int32_t ab, int32_t br, int32_t bg, int32_t bb, int32_t scx, int32_t scy)
{
	BlendBltR(pno, dx * zoomRate, dy * zoomRate, ar, ag, ab, br, bg, bb, scx, scy);
}

void BlendExBltRectR(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy, int32_t ar, int32_t ag, int32_t ab, int32_t br, int32_t bg, int32_t bb, int32_t scx, int32_t scy)
{
	BlendBltRectR(pno, dx * zoomRate, dy * zoomRate, sx * zoomRate, sy * zoomRate, hx * zoomRate, hy * zoomRate, ar, ag, ab, br, bg, bb, scx, scy);
}

void ExTextLayerOn(int32_t n, int32_t x, int32_t y)
{
	 TextLayerOn(n, x * zoomRate, y * zoomRate);
}

void ExTextMove(int32_t n, int32_t x, int32_t y)
{
	 TextMove(n, x * zoomRate, y * zoomRate);
}

void ExTextSize(int32_t n, int32_t size)
{
	 TextSize(n, size * zoomRate);
}

void ExTextHeight(int32_t n, int32_t size)
{
	 TextHeight(n, size * zoomRate);
}

void ExCreateSurface(int32_t pno, int32_t x, int32_t y)
{
	 CreateSurface(pno, x * zoomRate, y * zoomRate);
}
