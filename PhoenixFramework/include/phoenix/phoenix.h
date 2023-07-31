#ifndef PHNX_H
#define PHNX_H
#include <phoenix/common.h>



void pxInit(I32 width, I32 height, const cstr_t title);
bool pxIsRunning();
void pxPollEvents();
void pxSwapFrame();

void pxDestroy();

void pxBeginFrame();
void pxEndFrame();

void* pxMalloc(size_t size);

void* pxCalloc(size_t count, size_t size);
void  pxFree(void* ptr);



#endif // !PHNX_H
