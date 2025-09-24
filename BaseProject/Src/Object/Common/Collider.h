#pragma once
#include <DxLib.h>
class Collider
{

public:

	// 衝突種別
	enum class TYPE
	{
		STAGE
	};

	// コンストラクタ
	Collider(TYPE type, int modelId);

	// デストラクタ
	~Collider(void);

	bool IsCollidiingWith(const Collider& other) const;
	VECTOR GetPushVector(const Collider& other) const;
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
