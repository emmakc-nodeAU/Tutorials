#include "RenderShader.h"

int main() {
	
	auto app = new RenderShader();
	app->run("AIE", 1280, 720, false);
	delete app;

	return 0;
}