#pragma once
#include "../../../Utility/AsoUtility.h"
#include <limits>

class AStarNode
{
public:

	AStarNode(void);
	~AStarNode(void);

	// ワールド座標
	VECTOR worldPos_;

	// グリッドのXZ座標
	int gridX_, gridZ_;

	// 通過可能か
	bool isWalkable_;

	// A*計算用

	// スタートからゴールまでの実コスト
	float G_Score_;

	// このノードからゴールまでの推定コスト(ヒューリスティック)
	float H_Score_;

	// 合計コスト
	float F_Score(void) const;

	// 復元のための親ノード
	AStarNode* parent_;

	// A*探索開始にリセットするための関数
	void Reset(void);
};

