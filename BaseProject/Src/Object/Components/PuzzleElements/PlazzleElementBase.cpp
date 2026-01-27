#include <DxLib.h>
#include "../Application.h"
#include "../Manager/Config.h"
#include "../Utility/AsoUtility.h"
#include "../../../Manager/Camera.h"
#include "PlazzleElementBase.h"

PlazzleElementBase::PlazzleElementBase(void)
	:
	imgH_(-1),
	posX_(0.0f),
	posY_(0.0f),
	scale_(0.0f),
	angle_(0.0f)
{
}

PlazzleElementBase::~PlazzleElementBase(void)
{
}

void PlazzleElementBase::Init(void)
{
}

void PlazzleElementBase::Update(float deltaTime)
{
	OnUpdate(deltaTime);

	transform_.Update();
}

void PlazzleElementBase::OnUpdate(float deltaTime)
{
}

void PlazzleElementBase::Draw(void)
{
	// アルファブレンドモードに設定
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	//　角度、拡大率を含む描画
	DrawRotaGraph(static_cast<int>(posX_), static_cast<int>(posY_), scale_, angle_, imgH_, true, false);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);

}

void PlazzleElementBase::DrawModelInScreenSpace(int ModelHandle, float screenPosX, float screenPosY, float screenAngleRad, float scale)
{
	// 現状、３Dモデルをスクリーン座標系で描画することは可能であるが
	// 時間がかかりすぎるため2D画像で行うことにする
}

void PlazzleElementBase::InitModel(float distance, VECTOR size, Quaternion quaRot, VECTOR quaLocal)
{
	auto camera = Application::GetInstance().GetCamera();

	// カメラの取得
	VECTOR camPos = camera->GetPos();

	// カメラの前ベクトル
	VECTOR forward = camera->GetForward();

	// この位置が、カメラの向きにかかわらず画面中央になります
	VECTOR modelPos = VAdd(camPos, VScale(forward, distance));

	// ワールド座標での座標
	transform_.pos = modelPos;

	auto& sizeW = Config::GetInstance().GetWindowSize();

	// スクリーン座標系に変換するためz座標の情報は不要
	VECTOR worldPos = { static_cast<float>(sizeW.width_ / 2), static_cast<float>(sizeW.height_), 0.0f };

	// 座標をスクリーン座標系に変換
	VECTOR screenPos = ConvWorldPosToScreenPos(worldPos);

	// モデル拡大率
	transform_.scl = size;

	// モデルのクォータニオン回転
	transform_.quaRot = quaRot;

	// モデルのローカルクォータニオン回転
	transform_.quaRotLocal = Quaternion::Euler({ AsoUtility::Deg2RadF(quaLocal.x),
	AsoUtility::Deg2RadF(quaLocal.y),AsoUtility::Deg2RadF(quaLocal.z) });
}

void PlazzleElementBase::InitImg(float posX, float posY, float scale, float angle)
{
	auto& sizeW_ = Config::GetInstance().GetWindowSize();

	// スクリーン座標
	posX_ = sizeW_.width_ / 2 + posX;
	posY_ = sizeW_.height_ / 2 + posY;

	// 拡大率
	scale_ = scale;

	// 角度
	angle_ = angle;
}