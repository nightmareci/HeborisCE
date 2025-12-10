#ifndef flexdraw_h_
#define flexdraw_h_

#include "app/APP.h"

extern int32_t lastZoomRate;
extern int32_t zoomRate;

void ExBlt(int32_t pno,int32_t dx,int32_t dy);
void ExBltRect(int32_t pno,int32_t dx,int32_t dy,int32_t sx,int32_t sy,int32_t hx,int32_t hy);
void ExBltRectScaled(int32_t pno,int32_t dx,int32_t dy,int32_t sx,int32_t sy,int32_t hx,int32_t hy,int32_t scx,int32_t scy);

void ExBlendBlt(int32_t pno,int32_t dx,int32_t dy,int32_t a);
void ExBlendBltRect(int32_t pno,int32_t dx,int32_t dy,int32_t sx,int32_t sy,int32_t hx,int32_t hy,int32_t a);
void ExBlendBltRectScaled(int32_t pno,int32_t dx,int32_t dy,int32_t sx,int32_t sy,int32_t hx,int32_t hy,int32_t a,int32_t scx,int32_t scy);

void ExBltText(int32_t pno, const char* str, int32_t cf, int32_t cw, int32_t ch, int32_t dx, int32_t dy, int32_t shx, int32_t shy, int32_t shw);

void ExTextLayerSize(int32_t n,int32_t size);
void ExTextLayerMove(int32_t n,int32_t x,int32_t y);
void ExTextLayerOn(int32_t n,int32_t x,int32_t y);

int32_t getLastDrawRate();
int32_t getDrawRate();
void setDrawRate(int32_t size);

#endif
