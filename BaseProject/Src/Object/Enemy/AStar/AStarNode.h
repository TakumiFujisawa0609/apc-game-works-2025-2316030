#pragma once
#include <DxLib.h>

class AStarNode
{
public:

	AStarNode(void);
	~AStarNode(void);

	// 物理情報
	VECTOR worldPos_;		// ワールド座標
	int gridX_, gridZ_;		// グリッドのXZ座標
	bool isWalkAble_;		// 通過可能か

	// A*計算用

	float G_Score_;			// スタートからゴールまでの実コスト
	float H_Score_;			// このノードからゴールまでの推定コスト(ヒューリスティック)
	float F_Score(void) const; // 合計コスト


	AStarNode* parent_;		// 復元のための親ノード
};

