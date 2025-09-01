#include "GameScene.h"
#include<DxLib.h>
#include<cassert>

#include"../Input.h"//入力用
#include"SceneController.h"//シーンの切り替えに使う
#include"OverScene.h"//次のシーン
#include"PauseScene.h"//ポーズシーン
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
	if (input.IsPressed("up")) {
		cameraPos_.y += 0.1f;
	}
	if (input.IsPressed("down")) {
		cameraPos_.y -= 0.1f;
	}
	if (input.IsPressed("right")) {
		cameraPos_.x += 0.1f;
	}
	if (input.IsPressed("left")) {
		cameraPos_.x -= 0.1f;
	}
	
}

void GameScene::FadeOutUpdate(Input& input)
{
	if (++frame_ >= fade_interval) {
		controller_.ChangeScene(std::make_shared<OverScene>(controller_));
		return;
	}
}

void GameScene::NormalDraw()
{
	DrawString(10, 10, L"Game Scene", 0xffffff);


}

void GameScene::FadeDraw()
{
	//DrawRotaGraph(320, 240, 1.0f, 0.0f, imgH_, true);
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
	LoadLocationData();
	//プロトタイプパターン
	int sphereProtoH = MV1LoadModel(L"model/model.mv1");
	int cubeProtoH = MV1LoadModel(L"model/bodyeater.mv1");
	int capsuleProtoH = MV1LoadModel(L"model/bat.mv1");
	models_.resize(locationData_.size());
	for (int i = 0; i < locationData_.size();++i) {
		constexpr float model_scale = 0.004f;
		if (locationData_[i].name == "Cube") {
			models_[i] = MV1DuplicateModel(cubeProtoH);
			MV1SetScale(models_[i], VGet(0.01, 0.01, 0.01));
		}
		else if(locationData_[i].name == "Sphere") {
			models_[i] = MV1DuplicateModel(sphereProtoH);
			MV1SetScale(models_[i], VGet(model_scale, model_scale, model_scale));
		}
		else if (locationData_[i].name == "Capsule") {
			models_[i] = MV1DuplicateModel(capsuleProtoH);
			MV1SetScale(models_[i], VGet(model_scale, model_scale, model_scale));
		}

		const auto& pos = locationData_[i].pos;
		const auto& rot = locationData_[i].angle;
		
		MV1SetRotationXYZ(models_[i],
			VGet(DegreeToRadian(rot.x), 
				DegreeToRadian(rot.y), 
				DegreeToRadian(rot.z)));
		MV1SetPosition(models_[i], VGet(pos.x, pos.y, pos.z));
	}
	MV1DeleteModel(capsuleProtoH);
	MV1DeleteModel(sphereProtoH);
	MV1DeleteModel(cubeProtoH);

	cameraPos_ = { 0.0f,1.0f,-10.0f };

	int sw, sh, depth;
	GetScreenState(&sw, &sh, &depth);


}

GameScene::~GameScene()
{
	for (auto model : models_) {
		MV1DeleteModel(model);
	}
}

void GameScene::Update(Input& input)
{
	(this->*update_)(input);
}

void GameScene::Draw()
{
	(this->*draw_)();
}
