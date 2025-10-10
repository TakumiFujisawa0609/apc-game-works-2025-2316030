#pragma once
#include <DxLib.h>
#include "../Common/Quaternion.h"
class ResourceManager;
class Input;
class Transform;

class Camera
{

public:

	// カメラスピード(度)
	static constexpr float SPEED = 1.0f;

	// カメラクリップ：NEAR
	static constexpr float CAMERA_NEAR = 10.0f;

	// カメラクリップ：NEAR
	static constexpr float CAMERA_FAR = 30000.0f;

	// カメラの初期座標
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// 追従位置からカメラ位置までの相対座標
	static constexpr VECTOR LOCAL_F2C_POS = { 0.0f, 50.0f, -400.0f };

	// 追従位置から注視点までの相対座標
	static constexpr VECTOR LOCAL_F2T_POS = { 0.0f, 0.0f, 500.0f };

	// カメラのX回転上限度角
	static constexpr float LIMIT_X_UP_RAD = 80.0f * (DX_PI_F / 180.0f);
	static constexpr float LIMIT_X_DW_RAD = 15.0f * (DX_PI_F / 180.0f);

	// カメラ位置から注視点までの相対座標
	static constexpr VECTOR RELATIVE_C2T_POS = { 0.0f, -100.0f, 220.0f };

	// 追従対象からカメラ位置までの相対座標(完全追従)
	static constexpr VECTOR RELATIVE_F2C_POS_FOLLOW = { 0.0f, 280.0f, -350.0f };

	// パットの時のカメラの制限
	static constexpr float PAT_CAMERA_MAX_RX = 100.0f;
	static constexpr float PAT_CAMERA_MIN_RX = -100.0f;
	static constexpr float PAT_CAMERA_MAX_RY = 100.0f;
	static constexpr float PAT_CAMERA_MIN_RY = -100.0f;

	// カメラアングルの制限
	static constexpr float CAMERA_MAX_ANGLE_X = 60.0f;
	static constexpr float CAMERA_MIN_ANGLE_X = -60.0f;

	// カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT,
		FREE,
		FOLLOW,
		FOLLOW_MOUSE,
		FPS_MOUSE
	};

	enum class GAME_CAMERA
	{
		NONE,
		MOUSE
	};

	Camera(void);
	~Camera(void);

	void Init(void);
	void Update(void);
	void SetBeforeDraw(Input& input);
	void Draw(void);

	// カメラ位置
	VECTOR GetPos(void) const;

	// カメラの操作角度
	VECTOR GetAngles(void) const;
	
	// カメラの注視点
	VECTOR GetTargetPos(void) const;

	// カメラ角度
	Quaternion GetQuaRot(void) const;
	
	// X回転を抜いたカメラ角度
	Quaternion GetQuaRotOutX(void) const;
	
	// カメラの前方方向
	VECTOR GetForward(void) const;

	// カメラの右方向
	VECTOR GetRight(void) const;

	// カメラの上方向
	VECTOR GetUp(void) const;

	// カメラモードの変更
	void ChangeMode(MODE mode, VECTOR angle, bool isAngles);
	void ChangeGameCamera(GAME_CAMERA gameCamera);

	// 追従対象の設定
	void SetFollow(const Transform* follow);

	void MouseMove(float* x_m, float* y_m, const float fov_per);

	// カメラモード取得
	MODE GetCameraMode(void);

	// カメラの角度を一時的に保持する
	void SaveAngles(VECTOR save);

	// 角度を保存した値に一度だけ復元する
	void RestoreAnglesOnce(void);

	// カメラの操作が可能かどうかを設定する
	void SetOperableCamera(bool operable);

private:

	// シングルトン参照
	ResourceManager& resMng_;

	// カメラが追従対象とするTransform
	const Transform* followTransform_;

	// カメラモード
	MODE mode_;

	// ゲーム中のカメラ
	GAME_CAMERA gameCamera_;

	// カメラの位置
	VECTOR pos_;

	// カメラ角度(rad)
	VECTOR angles_;

	// X軸回転が無い角度
	Quaternion rotOutX_;

	// カメラ角度
	Quaternion rot_;

	// 注視点
	VECTOR targetPos_;

	// カメラの上方向
	VECTOR cameraUp_;

	// カメラモード変更時のカメラアングル
	VECTOR changeCameraAngles_;

	// カメラの角度保存用
	VECTOR saveAngles_;

	// カメラのモードを切り替えたかどうか
	bool isChangeMode_;

	// カメラアングルの復元が必要かを示すフラグ
	bool isRestoreAngleNeeded_;

	// カメラが操作可能状態であるか
	bool isOperable_;

	// カメラを初期位置に戻す
	void SetDefault(void);

	// 追従対象との位置同期を取る
	void SyncFollow(void);

	// カメラ操作
	void ProcessRot(Input& input);

	// モード別更新ステップ
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFollow(Input& input);
	void SetBeforeDrawFollowMouse(Input& input);
	void SetBeforeDrawFPSMouse(void);
};

