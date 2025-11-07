#ifndef flexdraw_h_
#define flexdraw_h_

#include "app/APP.h"

extern int32_t lastZoomRate;
extern int32_t zoomRate;

void ExTextSize(int32_t n,int32_t size);
void ExTextMove(int32_t n,int32_t x,int32_t y);
void ExTextLayerOn(int32_t n,int32_t x,int32_t y);
void BlendExBltRectR(int32_t pno,int32_t dx,int32_t dy,int32_t sx,int32_t sy,int32_t hx,int32_t hy,int32_t a,int32_t scx,int32_t scy);
void BlendExBltR(int32_t pno,int32_t dx,int32_t dy,int32_t a,int32_t scx,int32_t scy);
void BlendExBltRect(int32_t pno,int32_t dx,int32_t dy,int32_t sx,int32_t sy,int32_t hx,int32_t hy,int32_t a);
void BlendExBlt(int32_t pno,int32_t dx,int32_t dy,int32_t a);
void ExBltFastRectR(int32_t pno,int32_t dx,int32_t dy,int32_t sx,int32_t sy,int32_t hx,int32_t hy,int32_t scx,int32_t scy);
void ExBltFastR(int32_t pno,int32_t dx,int32_t dy,int32_t scx,int32_t scy);
void ExBltRectR(int32_t pno,int32_t dx,int32_t dy,int32_t sx,int32_t sy,int32_t hx,int32_t hy,int32_t scx,int32_t scy);
void ExBltR(int32_t pno,int32_t dx,int32_t dy,int32_t scx,int32_t scy);
void ExBlendBltRect(int32_t pno,int32_t dx,int32_t dy,int32_t sx,int32_t sy,int32_t hx,int32_t hy,int32_t a);
void ExBltRect(int32_t pno,int32_t dx,int32_t dy,int32_t sx,int32_t sy,int32_t hx,int32_t hy);
void ExBlt(int32_t pno,int32_t dx,int32_t dy);

int32_t getLastDrawRate();
int32_t getDrawRate();
void setDrawRate(int32_t size);

#endif
