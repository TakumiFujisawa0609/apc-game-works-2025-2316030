#include <thread>
#include<DxLib.h>
#include <EffekseerForDXLib.h>
#include"Input.h"
#include"Manager/Camera.h"
#include"Manager/Config.h"
#include"Manager/InputManager.h"
#include"Manager/ResourceManager.h"
#include"Manager/SceneController.h"
#include"Scene/TitleScene.h"
#include"Scene/GameScene.h"

#include "Application.h"

Application* Application::instance_ = nullptr;

const std::wstring Application::PATH_IMAGE = L"Data/Img/";
const std::wstring Application::PATH_MODEL = L"Data/Model/";
const std::wstring Application::PATH_EFFECT = L"Data/Effect/";
const std::wstring Application::PATH_SHADER = L"Data/Shader/";
const std::wstring Application::PATH_SOUND = L"Data/Sound/";

Application::Application() 
    :
    isInitFailed_(false),
    isReleaseFailed_(false),
    isGemeEnd_(false),
    frameCount_(0),
    fps_(0),
    lastTime_(-1),
    deltaTime_(0.0f),
    startTime_(-1),
    fontHandle_(-1)
{
}

Application::~Application()
{

}

void Application::CreateInstance(void)
{
	if (instance_ == nullptr) {
		instance_ = new Application();
	}
	instance_->Init(640,480);

}

Application&
Application::GetInstance()
{
    return *instance_;
}

const Size& Application::GetWindowSize() const
{
    return windowSize_;
}

void Application::Init(int w, int h)
{
    // アプリケーションの初期設定
    SetWindowText(L"LURK");

    // ウィンドウサイズ
    windowSize_ = { w,h };
    SetGraphMode(w, h, 32);
    ChangeWindowMode(true);

    // 使用する Direct3D のバージョンの設定
	SetUseDirect3DVersion(DX_DIRECT3D_11);
    
    // DXライブラリの初期化
    isInitFailed_ = false;
    if (DxLib_Init() == -1) {
        isInitFailed_ = true;
        return;
    }

    // 設定の初期化
    config_ = std::make_shared<Config>();
    config_->Init();

    //カメラ初期化
	camera_ = std::make_shared<Camera>();
	camera_->Init();

	// Effekseerの初期化
	InitEffekseer();
    
	// 3D描画初期化 
	Init3D();

     // フォントの初期化
    SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF16LE);

    // フォントの作成
    fontHandle_ = CreateFontToHandle(L"メイリオ", 24, 3, DX_FONTTYPE_NORMAL);

    // キー制御初期化
	SetUseDirectInputFlag(TRUE);
	input_ = std::make_shared<Input>();
	InputManager::CreateInstance();

    // リソース管理初期化
	ResourceManager::CreateInstance();

    // シーン管理初期化
	controller_ = std::make_shared<SceneController>();

	// 最初のシーンを設定
    controller_->ChangeScene(std::make_shared<TitleScene>(*controller_), *input_);
    //controller_->ChangeScene(std::make_shared<GameScene>(*controller_), *input_);

}

void Application::Run()
{
    auto& inputManager = InputManager::GetInstance();

    // FPS計測用
    startTime_ = GetNowCount();

    while (ProcessMessage() == 0 && !isGemeEnd_) {
        ClearDrawScreen();
        ClearDrawScreenZBuffer();

        UpdateDeltaTime();

        input_->Update();
        inputManager.Update();

        config_->Update();

        controller_->Update(*input_);
        camera_->Update();

        // カメラ設定
        camera_->SetBeforeDraw(*input_);

        // Effekseerにより再生中のエフェクトを描画する
        DrawEffekseer3D();

        // シーンごとの描画
        controller_->Draw();

        // 主にポストエフェクト用
        camera_->Draw();

        controller_->DrawUI();

        // FPSカウント更新
        frameCount_++;
        CalcFrameRate();

#ifdef _DEBUG

        DrawFrameRate();
        DrawDrawCall();

#endif // DEBUG

        WaitVSync(1);
        ScreenFlip();
    }

   
}

void Application::Terminate()
{
    InputManager::GetInstance().Destroy();
	ResourceManager::GetInstance().Destroy();

    // Effekseerを終了する。
    Effkseer_End();

    // DxLib終了
    if (DxLib_End() == -1)
    {
        isReleaseFailed_ = true;
    }

    delete instance_;
}

bool Application::IsInitFail(void) const
{
    return isInitFailed_;
}

bool Application::IsReleaseFail(void) const
{
    return isReleaseFailed_;
}

bool Application::IsGemeEnd(void)
{
    return isGemeEnd_;
}

void Application::SetGemeEnd(bool frag)
{
    isGemeEnd_ = frag;
}

void Application::UpdateDeltaTime(void)
{
    int now = GetNowCount();
    deltaTime_ = (now - lastTime_) / 1000.0f;
    lastTime_ = now;
}

float Application::GetDeltaTime(void)
{
    return deltaTime_;
}

void Application::ResetDeltaTime(void)
{
    lastTime_ = GetNowCount();
    deltaTime_ = 0.0f;
}

std::shared_ptr<Camera> Application::GetCamera(void)
{
    return camera_;
}

int Application::GetFontHandle(void) const
{
    return fontHandle_;
}

std::shared_ptr<SceneController> Application::GetSceneController(void) const
{
    return controller_;
}

std::shared_ptr<Input> Application::GetInput(void) const
{
    return input_;
}

std::shared_ptr<Config> Application::GetConfig(void) const
{
    return config_;
}

void Application::Init3D(void)
{
    // 背景色設定
    SetBackgroundColor(0, 139, 139);

    // Zバッファを有効にする
    SetUseZBuffer3D(true);

    // Zバッファへの書き込みを有効にする
    SetWriteZBuffer3D(true);

    // バックカリングを有効にする
    SetUseBackCulling(true);

    // ライトの設定
    SetUseLighting(true);

    SetLightEnable(true);

    // ライトの設定
    //ChangeLightTypeDir({ -0.4f, -0.7f, 0.5f });

    // フォグ設定
    SetFogEnable(false);
    //SetFogColor(255, 255, 255);
    SetFogColor(0, 0, 0);
    SetFogStartEnd(100.0f, 20000.0f);
}

void Application::InitEffekseer(void)
{
    if (Effekseer_Init(8000) == -1)
    {
        DxLib_End();
    }

    SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

    Effekseer_SetGraphicsDeviceLostCallbackFunctions();
}

void Application::CalcFrameRate(void)
{
    int now = GetNowCount();
    if (now - startTime_ >= 1000)
    {
        fps_ = frameCount_;
        frameCount_ = 0;
        startTime_ = now;
    }
}

void Application::DrawFrameRate(void)
{
    int color = GetColor(0, 255, 0);
    if (fps_ < 30)color = GetColor(255, 0, 0);
    else if (fps_ < 55)color = GetColor(255, 255, 0);

    DrawFormatString(windowSize_.width - 150, 0, color, L"FPS : %d", fps_);
    DrawFormatString(windowSize_.width - 150, 16, color, L"deltaTime : %2f", deltaTime_);
}

void Application::DrawDrawCall(void)
{
    DrawFormatString(windowSize_.width - 150, 32, GetColor(255, 255, 255), L"DrawCall = %d", GetDrawCallCount());

}
