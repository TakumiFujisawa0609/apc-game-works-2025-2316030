#pragma once
#include <DxLib.h>
class Collider
{
public:

	// 衝突種別
	enum class TYPE
	{
		STAGE,
		PLAYER,
		ENEMY
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="type">衝突種別</param>
	/// <param name="modelId">モデルのハンドルID</param>
	Collider(TYPE type, int modelId);

	~Collider(void);

	/// <summary>
	/// どのような物と衝突したのか
	/// </summary>
	/// <param name="other">他のColliderクラス</param>
	/// <returns></returns>
	bool IsCollidiingWith(const Collider& other) const;

	/// <summary>
	/// 押出方向の取得
	/// </summary>
	/// <param name="other">他のColliderクラス</param>
	/// <returns></returns>
	VECTOR GetPushVector(const Collider& other) const;

	/// <summary>
	/// Colliderの中心座標設定
	/// </summary>
	/// <param name="pos">中心座標</param>
	void SetPosition(const VECTOR& pos);

	VECTOR GetPosition(void) const;

	// 衝突種別
	TYPE type_;

	// モデルのハンドルID
	int modelId_;

	// 中心座標
	VECTOR center_;

	// 幅/高さ/奥行きの半分
	VECTOR halfSize_;

};
