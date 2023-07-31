#include <phoenix/phoenix.h>
#include <phoenix/window.h>

int main(void) {
	pxInit(1080, 720, "Phoenix App");

	while (pxIsRunning()) {
		pxBeginFrame();
		pxEndFrame();
	}

	pxDestroy();

	return 0;
}