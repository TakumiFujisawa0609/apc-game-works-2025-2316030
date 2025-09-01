#include "Application.h"
#include<DxLib.h>
#include"Input.h"
#include"Scene/SceneController.h"
#include"Scene/TitleScene.h"//àÍî‘ç≈èâÇÃÉVÅ[ÉìÇæÇØÇÕéùÇ¡ÇƒÇ®Ç≠

Application::Application() {

}

Application::~Application()
{

}

Application&
Application::GetInstance()
{
    static Application instance;
    return instance;
}

const Size& Application::GetWindowSize() const
{
    return windowSize_;
}

bool Application::Init(int w, int h)
{
    windowSize_ = { w,h };
    SetGraphMode(w, h, 32);
    ChangeWindowMode(true);
    if (DxLib_Init() == -1) {
        return false;
    }
    SetDrawScreen(DX_SCREEN_BACK);

    return true;
}

void Application::Run()
{
    SceneController controller = {};
    Input input = {};
    controller.ChangeScene(std::make_shared<TitleScene>(controller));
    while (ProcessMessage() != -1) {
        ClearDrawScreen();
        input.Update();
        controller.Update(input);
        controller.Draw();

        ScreenFlip();
    }
}

void Application::Terminate()
{
    DxLib_End();
}
