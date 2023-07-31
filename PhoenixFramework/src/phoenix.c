#include <phoenix/phoenix.h>
#include <phoenix/window.h>

#include <phoenix/platform/vulkan/context.h>

void pxInit(I32 width, I32 height, const cstr_t title) {
	PHNX_LOG("Starting Phoenix v%d.%d", PHNX_VMAJOR, PHNX_VMINOR);

	pxCreateWindow(width, height, title, false);

	PHNX_LOG("Created Window");
	pxInitializeVulkan(title);


	pxShowWindow();
}

bool pxIsRunning() {
	return !glfwWindowShouldClose(pxGetWindow());
}

void pxPollEvents() {
	glfwPollEvents();
}

void pxSwapFrame() {
	// TODO
}

void pxDestroy() {
	pxDestroyContext();

	glfwDestroyWindow(pxGetWindow());
	glfwTerminate();
}

void pxBeginFrame() { pxPollEvents(); }
void pxEndFrame() { pxSwapFrame(); }

void* pxCalloc(size_t count, size_t size) {
	void* ptr = malloc(size * count);
	if (ptr == NULL) { PHNX_FATAL("Failed To Allocate %d Bytes", size); }
	return ptr;
}

void* pxMalloc(size_t size) {
	void* ptr = malloc(size);
	if (ptr == NULL) { PHNX_FATAL("Failed To Allocate %d Bytes", size); }
	return ptr;
}

void  pxFree(void* ptr) {
	free(ptr);
}