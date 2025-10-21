#pragma once
#include <DxLib.h>

class PatrolNode
{
public:
	struct Vector3
	{
		float x, y, z;
	};

	enum class ACTTYPE
	{
		NONE,
		LOCKAROUND,
		LISTEN,
		TRIGGEREVENT
	};

	PatrolNode(VECTOR pos, float time, ACTTYPE type);
	~PatrolNode(void);

	// 座標を取得する
	VECTOR GetPos(void) const;

	// 次の行動に移すまでの残り時間を取得する
	float GetWaitTime(void) const;

	// このノードで実行する特殊行動を取得する
	ACTTYPE GetActType(void) const;

	// デバック用描画
	void DebugDraw(void) const;

private:

	VECTOR pos_;		//	ワールド座標
	float waitTime_;	// 到達した後、次に移動を開始するまでの時間
	ACTTYPE actionType_;	// このノードで実行する特殊行動
};

