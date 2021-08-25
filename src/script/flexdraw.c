// 描画倍率を可変できるようになった関数群。
// 通常の描画関数の頭に Ex をつけます。

int zoomRate = 1;			/* 描画倍率 */

void setDrawRate(int size) {
	zoomRate = size;
}

int getDrawRate() {
	return zoomRate;
}

void ExBlt(int pno, int dx, int dy)
{
	Blt(pno, dx * zoomRate, dy * zoomRate);
}

void ExBltRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy)
{
	BltRect(pno, dx * zoomRate, dy * zoomRate, sx * zoomRate, sy * zoomRate, hx * zoomRate, hy * zoomRate);
}

void ExBltFast(int pno, int dx, int dy)
{
	BltFast(pno, dx * zoomRate, dy * zoomRate);
}

void ExBltFastRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy)
{
	BltFastRect(pno, dx * zoomRate, dy * zoomRate, sx * zoomRate, sy * zoomRate, hx * zoomRate, hy * zoomRate);
}


void ExBlendBlt(int pno, int dx, int dy, int ar, int ag, int ab, int br, int bg, int bb)
{
	BlendBlt(pno, dx * zoomRate, dy * zoomRate, ar, ag, ab, br, bg, bb);
}

void ExBlendBltRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int ar, int ag, int ab, int br, int bg, int bb)
{
	BlendBltRect(pno, dx * zoomRate, dy * zoomRate, sx * zoomRate, sy * zoomRate, hx * zoomRate, hy * zoomRate, ar, ag, ab, br, bg, bb);
}

void ExBltR(int pno, int dx, int dy, int scx, int scy)
{
	BltR(pno, dx * zoomRate, dy * zoomRate, scx, scy);
}

void ExBltRectR(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int scx, int scy)
{
	BltRectR(pno, dx * zoomRate, dy * zoomRate, sx * zoomRate, sy * zoomRate, hx * zoomRate, hy * zoomRate, scx, scy);
}

void ExBltFastR(int pno, int dx, int dy, int scx, int scy)
{
	BltFastR(pno, dx * zoomRate, dy * zoomRate, scx, scy);
}

void ExBltFastRectR(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int scx, int scy)
{
	BltFastRectR(pno, dx * zoomRate, dy * zoomRate, sx * zoomRate, sy * zoomRate, hx * zoomRate, hy * zoomRate, scx, scy);
}

/*
void ExBltTrans(int pno, int dx, int dy)
{
	BltTrans(pno, dx * zoomRate, dy * zoomRate);
}
*/

void BlendExBlt(int pno, int dx, int dy, int ar, int ag, int ab, int br, int bg, int bb)
{
	BlendBlt(pno, dx * zoomRate, dy * zoomRate, ar, ag, ab, br, bg, bb);
}

void BlendExBltRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int ar, int ag, int ab, int br, int bg, int bb)
{
	BlendBltRect(pno, dx * zoomRate, dy * zoomRate, sx * zoomRate, sy * zoomRate, hx * zoomRate, hy * zoomRate, ar, ag, ab, br, bg, bb);
}

void BlendExBltR(int pno, int dx, int dy, int ar, int ag, int ab, int br, int bg, int bb, int scx, int scy)
{
	BlendBltR(pno, dx * zoomRate, dy * zoomRate, ar, ag, ab, br, bg, bb, scx, scy);
}

void BlendExBltRectR(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int ar, int ag, int ab, int br, int bg, int bb, int scx, int scy)
{
	BlendBltRectR(pno, dx * zoomRate, dy * zoomRate, sx * zoomRate, sy * zoomRate, hx * zoomRate, hy * zoomRate, ar, ag, ab, br, bg, bb, scx, scy);
}

void ExTextLayerOn(int n, int x, int y)
{
	 TextLayerOn(n, x * zoomRate, y * zoomRate);
}

void ExTextMove(int n, int x, int y)
{
	 TextMove(n, x * zoomRate, y * zoomRate);
}

void ExTextSize(int n, int size)
{
	 TextSize(n, size * zoomRate);
}

void ExTextHeight(int n, int size)
{
	 TextHeight(n, size * zoomRate);
}

void ExCreateSurface(int pno, int x, int y)
{
	 CreateSurface(pno, x * zoomRate, y * zoomRate);
}
