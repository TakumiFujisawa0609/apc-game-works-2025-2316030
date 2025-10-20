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

Camera::Camera(void) 
	: 
	resMng_(ResourceManager::GetInstance()),
	cameraUp_({}),
	gameCamera_(GAME_CAMERA::NONE),
	targetPos_({}),
	followTransform_(nullptr),
	changeCameraAngles_({}),
	saveAngles_({}),
	isChangeMode_(false),
	isRestoreAngleNeeded_(false),
	isOperable_(true)
{
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
	SetCameraNearFar(1.0f, 50.0f);
	ChangeMode(MODE::FIXED_POINT, AsoUtility::VECTOR_ZERO, false);
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

	//DrawSphere3D(targetPos_, 5.0f, 8, GetColor(0, 0, 0), GetColor(255, 255, 0), true);

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

Camera::MODE Camera::GetCameraMode(void)
{
	return mode_;
}

void Camera::SaveAngles(VECTOR save)
{
	saveAngles_ = save;
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

VECTOR Camera::GetRight(void) const
{

	VECTOR forward = GetForward();
	VECTOR up = VGet(0.0f, 1.0f, 0.0f);	// ワールド座標系の上方向
	VECTOR right = VNorm(VCross(up, forward));

	return right;
}

VECTOR Camera::GetUp(void) const
{
	VECTOR forward = GetForward();
	VECTOR right = GetRight();
	VECTOR up = VNorm(VCross(forward, right));
	return up;
}

void Camera::ChangeMode(MODE mode, VECTOR angle, bool isAngles)
{
	//if (mode_ == MODE::FPS_MOUSE)
	//{
	//	saveAngles_ = angle;
	//}

	// カメラの初期設定
	SetDefault();

	// カメラモードの変更
	mode_ = mode;

	// カメラアングルの変更
	changeCameraAngles_ = angle;

	// 角度を保存した値に一度だけ復元する
	if (isAngles)
	{
		RestoreAnglesOnce();
	}
	
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

	isChangeMode_ = true;
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

	angles_.x = AsoUtility::Deg2RadF(0.0f);
	angles_.y = AsoUtility::Deg2RadF(110.0f);
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

	// 操作可能時のみ
	if (isOperable_)
	{
		// 視点角度に反映
		angles_.y += dx * sensitivity;	// 水平回転
		angles_.x -= dy * sensitivity;	// 垂直回転

		// 垂直回転の制限
		if (angles_.x > AsoUtility::Deg2RadF(89.0f))angles_.x = AsoUtility::Deg2RadF(89.0f);
		if (angles_.x < AsoUtility::Deg2RadF(-89.0f))angles_.x = AsoUtility::Deg2RadF(-89.0f);

		// 位置(FPSなのでfollowTransformがあればそこを基準にする)
		pos_ = followTransform_ ? followTransform_->pos : pos_;

		pos_.y += 160.0f;
		// 前方方向ベクトルを計算
		VECTOR forward = {
			cosf(angles_.x) * sinf(angles_.y),
			sinf(angles_.x),
			cosf(angles_.x) * cosf(angles_.y)
		};

		// 注視点を更新(カメラ位置 + 前方方向ベクトル)
		targetPos_ = VAdd(pos_, VScale(forward, 200.0f));

		// 上方向は常にY+
		cameraUp_ = AsoUtility::DIR_U;

		// 毎フレームマウスを中央に戻す
		SetMousePoint(centerX, centerY);



		// 値を小さくするほど、回転速度が遅くなります。
		const float GAMEPAD_SENSITIVITY = 0.025f;

		// 【要定義】スティックの最大値で割るための定数（-1000 ～ 1000 の範囲を想定）
		const float MAX_ANALOG_VALUE = 1000.0f;

		// 【要定義】デッドゾーンの閾値（スティックの遊び、ノイズ対策）
		const int DEADZONE_THRESHOLD = 200; // -200 から 200 の範囲は無視

		DINPUT_JOYSTATE input;

		// 入力状態を取得
		if (GetJoypadDirectInputState(DX_INPUT_PAD1, &input) == 0)
		{
			int rx = input.Rx;

			// デッドゾーン処理
			if (abs(rx) < DEADZONE_THRESHOLD)
			{
				rx = 0;
			}

			if (rx != 0)
			{
				// スティック値を正規化 (-1.0f ～ 1.0f)
				float normalizedRx = (float)rx / MAX_ANALOG_VALUE;

				// 視点角度に反映: angles_.y は水平回転
				angles_.y += normalizedRx * GAMEPAD_SENSITIVITY;
			}

			// -----------------------------------------------------------------
			// 2. 垂直回転 (右スティックの Y軸: Ry)
			// -----------------------------------------------------------------
			int ry = input.Ry;

			// デッドゾーン処理
			if (abs(ry) < DEADZONE_THRESHOLD)
			{
				ry = 0;
			}

			if (ry != 0)
			{
				// スティック値を正規化 (-1.0f ～ 1.0f)
				float normalizedRy = (float)ry / MAX_ANALOG_VALUE;

				// 垂直回転の加算（元のコードの角度制限部分を統合し、感度を適用）
				// Rx, Ryは正の値で右/下方向を指すことが多いが、
				// カメラの Y軸回転(angles_.x)は '下方向' が角度減少、'上方向' が角度増加になるように調整が必要
				// マウス操作と合わせる場合: マウス 'dy' は '下移動' で正、'angles_.x' は減少 (-dy)
				// ゲームパッド 'ry' は '下倒し' で正、'angles_.x' は減少 (-normalizedRy) とします。
				float deltaAngleX = -normalizedRy * GAMEPAD_SENSITIVITY;

				// 角度制限を考慮して加算
				float newAngleX = angles_.x + deltaAngleX;

				// 垂直回転の制限
				float minRad = AsoUtility::Deg2RadF(-89.0f);
				float maxRad = AsoUtility::Deg2RadF(89.0f);

				if (newAngleX > maxRad)
				{
					angles_.x = maxRad;
				}
				else if (newAngleX < minRad)
				{
					angles_.x = minRad;
				}
				else
				{
					angles_.x = newAngleX;
				}
			}
		}
	}
}

void Camera::RestoreAnglesOnce(void)
{
	angles_ = saveAngles_;

	saveAngles_ = AsoUtility::VECTOR_ONE;
}

void Camera::SetOperableCamera(bool operable)
{
	isOperable_ = operable;
}
