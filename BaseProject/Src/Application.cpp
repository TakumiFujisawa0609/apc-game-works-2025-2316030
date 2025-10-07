#include <thread>
#include<DxLib.h>
#include <EffekseerForDXLib.h>
#include"Input.h"
#include"Manager/Camera.h"
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

//unsigned int lastTime = GetNowCount(); // ミリ秒単位の現在時間

Application::Application() {
    isInitFailed_ = false;
	isReleaseFailed_ = false;
	frameCount_ = 0;
	fps_ = 0;
    lastTime_ = GetNowCount();
	deltaTime_ = 0.0f;
    fontHandle_ = -1;
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

	// 描画先を裏画面に設定
    SetDrawScreen(DX_SCREEN_BACK);
}

void Application::Run()
{
    auto& inputManager = InputManager::GetInstance();

    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
        ClearDrawScreen();

        UpdateDeltaTime();

        input_->Update();
        inputManager.Update();

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

    // ライトの設定
    ChangeLightTypeDir({ 0.3f, -0.7f, 0.8f });


    // フォグ設定
    SetFogEnable(true);
    SetFogColor(5, 5, 5);
    SetFogStartEnd(10000.0f, 20000.0f);
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
