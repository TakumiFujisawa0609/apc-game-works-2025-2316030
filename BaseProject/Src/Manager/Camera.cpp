#include <math.h>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include <algorithm>
#include "../Utility/AsoUtility.h"
#include "../Application.h"
#include "../Input.h"
#include "../Manager/InputManager.h"
#include "../Manager/ResourceManager.h"
#include "../Object/Common/Transform.h"
#include "Camera.h"

Camera::Camera(void) : resMng_(ResourceManager::GetInstance())
{
	angles_ = VECTOR();
	cameraUp_ = VECTOR();
	mode_ = MODE::NONE;
	gameCamera_ = GAME_CAMERA::NONE;
	pos_ = AsoUtility::VECTOR_ZERO;
	targetPos_ = AsoUtility::VECTOR_ZERO;
	followTransform_ = nullptr;
	lockonSe_ = -1;

}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{

	ChangeMode(MODE::FIXED_POINT);
	ChangeGameCamera(GAME_CAMERA::MOUSE);
}

void Camera::Update(void)
{
}

void Camera::SetBeforeDraw(Input& input)
{
	auto& ins = InputManager::GetInstance();

	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow(input);
		break;
	case Camera::MODE::FOLLOW_MOUSE:
		if (gameCamera_ == GAME_CAMERA::MOUSE)
		{
			DINPUT_JOYSTATE input;

			// 入力状態を取得
			GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

			if (input.Rx > PAT_CAMERA_MAX_RX)
			{
				angles_.y += 1.5f;
			}
			if (input.Rx < PAT_CAMERA_MIN_RX)
			{
				angles_.y -= 1.5f;
			}
			if (input.Ry > PAT_CAMERA_MAX_RY && angles_.x < CAMERA_MAX_ANGLE_X)
			{
				angles_.x += 1.5f;
			}
			if (input.Ry < PAT_CAMERA_MIN_RY && angles_.x > CAMERA_MIN_ANGLE_X)
			{
				angles_.x -= 1.5f;
			}
			MouseMove(&angles_.y, &angles_.x, 1.f);
		}
		SetMousePoint(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2);
		SetBeforeDrawFollowMouse(input);
		break;

	case Camera::MODE::FPS_MOUSE:
		SetBeforeDrawFPSMouse();
		break;
	}

	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();

}

void Camera::Draw(void)
{
	//DrawFormatString(10, 100, 0x000000, "%2f", angles_.x);
}

void Camera::SetFollow(const Transform* follow)
{
	followTransform_ = follow;
}

void Camera::MouseMove(float* x_m, float* y_m, const float fov_per)
{
	int x_t, y_t;
	GetMousePoint(&x_t, &y_t);
	*x_m += float(std::clamp(x_t - Application::SCREEN_SIZE_X / 2, -120, 120)) * fov_per / GetFPS() * 5;
	*y_m += float(std::clamp(y_t - Application::SCREEN_SIZE_Y / 2, -120, 120)) * fov_per / GetFPS() * 5;
	SetMousePoint(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2);
	if (angles_.x > 10.0f)
	{
		angles_.x = 10.0f;
	}
	if (angles_.x < -20.0f)
	{
		angles_.x = -20.0f;
	}
}

VECTOR Camera::GetPos(void) const
{
	return pos_;
}

VECTOR Camera::GetAngles(void) const
{
	return angles_;
}

VECTOR Camera::GetTargetPos(void) const
{
	return targetPos_;
}

Quaternion Camera::GetQuaRot(void) const
{
	return rot_;
}

Quaternion Camera::GetQuaRotOutX(void) const
{
	return rotOutX_;
}

VECTOR Camera::GetForward(void) const
{
	return VNorm(VSub(targetPos_, pos_));
}

void Camera::ChangeMode(MODE mode)
{

	// カメラの初期設定
	SetDefault();

	// カメラモードの変更
	mode_ = mode;

	// 変更時の初期化処理
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetMouseDispFlag(TRUE);
		break;
	case Camera::MODE::FOLLOW:
		SetMouseDispFlag(TRUE);
		break;
	case Camera::MODE::FOLLOW_MOUSE:
		SetMouseDispFlag(FALSE);
		break;
	case Camera::MODE::FPS_MOUSE:
		SetMouseDispFlag(FALSE);
		break;
	}

}

void Camera::ChangeGameCamera(GAME_CAMERA gameCamera)
{
	gameCamera_ = gameCamera;

	// 変更時の初期化処理
	switch (gameCamera_)
	{
	case Camera::GAME_CAMERA::NONE:
		break;
	case Camera::GAME_CAMERA::MOUSE:
		//angles_.x = -9.0f;
		break;
	}
}

void Camera::SetDefault(void)
{

	// カメラの初期設定
	pos_ = DEFAULT_CAMERA_POS;

	// 注視点
	targetPos_ = AsoUtility::VECTOR_ZERO;

	// カメラの上方向
	cameraUp_ = AsoUtility::DIR_U;

	angles_.x = AsoUtility::Deg2RadF(30.0f);
	angles_.y = 0.0f;
	angles_.z = 0.0f;

	rot_ = Quaternion();

}

void Camera::SyncFollow(void)
{

	// 同期先の位置
	VECTOR pos = followTransform_->pos;

	// 重力の方向制御に従う
	// 正面から設定されたY軸分、回転させる
	rotOutX_ = Quaternion::AngleAxis(angles_.y, AsoUtility::AXIS_Y);

	// 正面から設定されたX軸分、回転させる
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, AsoUtility::AXIS_X));

	VECTOR localPos;

	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = rotOutX_.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos, localPos);

	// カメラ位置
	localPos = rot_.PosAxis(LOCAL_F2C_POS);
	pos_ = VAdd(pos, localPos);

	// カメラの上方向
	cameraUp_ = AsoUtility::DIR_U;

}

void Camera::ProcessRot(Input& input)
{

	auto& ins = InputManager::GetInstance();

	float movePow = 5.0f;

	// カメラ回転
	if (ins.IsNew(KEY_INPUT_RIGHT))
	{
		// 右回転
		angles_.y += AsoUtility::Deg2RadF(1.0f);
	}
	if (ins.IsNew(KEY_INPUT_LEFT))
	{
		// 左回転
		angles_.y += AsoUtility::Deg2RadF(-1.0f);
	}

	// 上回転
	if (ins.IsNew(KEY_INPUT_UP))
	{
		angles_.x += AsoUtility::Deg2RadF(1.0f);
		if (angles_.x > LIMIT_X_UP_RAD)
		{
			angles_.x = LIMIT_X_UP_RAD;
		}
	}

	// 下回転
	if (ins.IsNew(KEY_INPUT_DOWN))
	{
		angles_.x += AsoUtility::Deg2RadF(-1.0f);
		if (angles_.x < -LIMIT_X_DW_RAD)
		{
			angles_.x = -LIMIT_X_DW_RAD;
		}
	}

}

void Camera::SetBeforeDrawFixedPoint(void)
{
	// 何もしない
}

void Camera::SetBeforeDrawFollow(Input& input)
{

	// カメラ操作
	ProcessRot(input);

	// 追従対象との相対位置を同期
	SyncFollow();

}

void Camera::SetBeforeDrawFollowMouse(Input& input)
{
	auto& ins = InputManager::GetInstance();

	static bool prevRStickPressed = false;
	DINPUT_JOYSTATE pad;
	GetJoypadDirectInputState(DX_INPUT_PAD1, &pad);

	bool nowRStickPressed = pad.Buttons[9] != 0;

	bool isToggleTriggered = false;
	if (nowRStickPressed && !prevRStickPressed)
	{
		isToggleTriggered = true;
	}

	prevRStickPressed = nowRStickPressed;


	// 追従対象の位置
	VECTOR followPos = followTransform_->pos;

	Quaternion rotX = Quaternion::AngleAxis(AsoUtility::Deg2RadF(angles_.x), AsoUtility::AXIS_X);
	Quaternion rotY = Quaternion::AngleAxis(AsoUtility::Deg2RadF(angles_.y), AsoUtility::AXIS_Y);
	Quaternion mixRot = rotY.Mult(rotX);

	// 追従対象からカメラまでの相対座標
	VECTOR relativeCPos = rotY.PosAxis(RELATIVE_F2C_POS_FOLLOW);

	// カメラ位置の更新
	pos_ = VAdd(followPos, relativeCPos);

	// カメラ位置から注視点までの相対座標
	VECTOR relativeTPos = mixRot.PosAxis(RELATIVE_C2T_POS);

	// 注視点の更新
	targetPos_ = AsoUtility::Lerp(targetPos_, VAdd(pos_, relativeTPos), 0.3f);

	// カメラの上方向
	cameraUp_ = AsoUtility::DIR_U;

	angles_.z = 0.0f;
}

void Camera::SetBeforeDrawFPSMouse(void)
{
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	int centerX = Application::SCREEN_SIZE_X / 2;
	int centerY = Application::SCREEN_SIZE_Y / 2;

	int dx = mouseX - centerX;
	int dy = mouseY - centerY;

	// マウス感度
	float sensitivity = 0.002f;

	// 視点角度に反映
	angles_.y += dx * sensitivity;	// 水平回転
	angles_.x -= dy * sensitivity;	// 垂直回転

	// 垂直回転の制限
	if (angles_.x > AsoUtility::Deg2RadF(89.0f))angles_.x = AsoUtility::Deg2RadF(89.0f);
	if (angles_.x < AsoUtility::Deg2RadF(-89.0f))angles_.x = AsoUtility::Deg2RadF(-89.0f);

	// 位置(FPSなのでfollowTransformがあればそこを基準にする)
	pos_ = followTransform_ ? followTransform_->pos : pos_;

	// 前方方向ベクトルを計算
	VECTOR forward = {
		cosf(angles_.x) * sinf(angles_.y),
		sinf(angles_.x),
		cosf(angles_.x) * cosf(angles_.y)
	};

	// 注視点を更新(カメラ位置 + 前方方向ベクトル)
	targetPos_ = VAdd(pos_, VScale(forward, 100.0f));

	// 上方向は常にY+
	cameraUp_ = AsoUtility::DIR_U;

	// 毎フレームマウスを中央に戻す
	SetMousePoint(centerX, centerY);
}
