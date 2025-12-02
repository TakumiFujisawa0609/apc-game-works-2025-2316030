#pragma once
#include <DxLib.h>

class PatrolNode
{
public:

	enum class ACTTYPE{
		NONE,			// なし
		LOCKAROUND,		// 見渡す
		LISTEN,			// 音を聞く
		TRIGGEREVENT	// イベント発生
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

	//	ワールド座標
	VECTOR pos_;

	// 到達した後、次に移動を開始するまでの時間
	float waitTime_;

	// このノードで実行する特殊行動
	ACTTYPE actionType_;
};

