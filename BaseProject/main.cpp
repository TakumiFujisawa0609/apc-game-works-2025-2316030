#include"Application.h"
#include<DxLib.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Application& app = Application::GetInstance();
	if (!app.Init()) {
		return -1;
	}
	app.Run();
	app.Terminate();
	return 0;
}