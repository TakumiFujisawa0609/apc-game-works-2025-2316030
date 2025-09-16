#include "GameScene.h"
#include<DxLib.h>
#include<cassert>

#include"../Application.h"
#include"../Input.h"//入力用
#include"../Manager/Camera.h"
#include"../Manager/ResourceManager.h"//リソース管理用
#include"../Manager/SceneController.h"//シーンの切り替えに使う
#include"OverScene.h"//次のシーン
#include"PauseScene.h"//ポーズシーン
#include"../Object/Submarine.h"
#include"../Object/Player/Player.h"
#include"../Object/Stage/Stage.h"

#include"../DrawUtil.h"

namespace {
	constexpr int fade_interval = 60;
	float DegreeToRadian(float degree) {
		//0～360→0～2π
		//0～180→0～π
		//÷180 x π
		return (degree * DX_PI_F) / 180.0f;
	}
}

void GameScene::FadeInUpdate(Input& input)
{
	if (--frame_ <= 0) {
		update_ = &GameScene::NormalUpdate;
		draw_ = &GameScene::NormalDraw;
	}
}

void GameScene::NormalUpdate(Input& input)
{
	++frame_;
	if (input.IsTriggered("ok")) {
		update_ = &GameScene::FadeOutUpdate;
		draw_ = &GameScene::FadeDraw;
		frame_ = 0;
		return;
	}
	if (input.IsTriggered("pause")) {
		controller_.PushScene(std::make_shared<PauseScene>(controller_));
		return;
	}


	
}

void GameScene::FadeOutUpdate(Input& input)
{
	if (++frame_ >= fade_interval) {
		controller_.ChangeScene(std::make_shared<OverScene>(controller_),input);
		return;
	}
}

void GameScene::NormalDraw()
{
	DrawString(10, 10, L"Game Scene", 0xffffff);


	stage_->Draw();
	submarine_->Draw();
	player_->Draw();
}

void GameScene::FadeDraw()
{
	float rate = static_cast<float>(frame_) /
					static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, rate * 255);
	DrawBox(0, 0, 640, 480, 0x000000,true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);



}

void GameScene::LoadLocationData()
{
	//ヘッダのサイズは固定
	struct Header {
		char signature[4];
		float version;
		int count;
	};
	Header header = {};
	auto fHandle=FileRead_open(L"./location.dat");
	FileRead_read(&header, sizeof(header), fHandle);
	locationData_.resize(header.count);
	for (auto& location : locationData_) {
		uint8_t nameSize = 0;
		FileRead_read(&nameSize, sizeof(nameSize), fHandle);
		location.name.resize(nameSize);
		FileRead_read(location.name.data(), location.name.size(), fHandle);
		FileRead_read(&location.pos, sizeof(location.pos), fHandle);
		FileRead_read(&location.angle, sizeof(location.angle), fHandle);
	}
	
	FileRead_close(fHandle);
}

GameScene::GameScene(SceneController& controller) :Scene(controller)
{
	//imgH_=LoadGraph(L"img/brook.png");
	//assert(imgH_ >= 0);
	update_ = &GameScene::FadeInUpdate;
	draw_ = &GameScene::FadeDraw;
	frame_ = fade_interval;
	
	int sw, sh, depth;
	GetScreenState(&sw, &sh, &depth);


}

GameScene::~GameScene()
{
	
}

void GameScene::Init(Input& input)
{
	// 潜水艦
	submarine_ = std::make_shared<Submarine>();
	submarine_->Init();

	// プレイヤー
	player_ = std::make_shared<Player>();
	player_->Init();

	// ステージ
	stage_ = std::make_shared<Stage>();
	stage_->Init();


	Application::GetInstance().GetCamera()->SetFollow(&player_->GetTransform());
	Application::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FPS_MOUSE);
}

void GameScene::Update(Input& input)
{
	float time = Application::GetInstance().GetDeltaTime();

	submarine_->Update(time);
	stage_->Update(time);
	player_->Update(time);
	

	(this->*update_)(input);

}

void GameScene::Draw()
{
	(this->*draw_)();

}
