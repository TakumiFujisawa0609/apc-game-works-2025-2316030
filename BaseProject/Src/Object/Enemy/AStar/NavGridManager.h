#pragma once

#include <vector>
#include "AStarNode.h"

class Transform;
class AStarNode;

class NavGridManager
{
public:

	// 　グリッドマップの初期化
	void InitGrid(int gridSizeX, int gridSizeZ, float nodeSize);

	// ワールド座標からノードを取得する
	AStarNode* NodeFromWorldPoint(VECTOR worldPos);

	// 特定のグリッド座標からノードを取得
	AStarNode* GetNode(int x, int z);

	// A*探索前に全てのノードをリセット
	void ResetAllNodes();

	int GetGridSizeX() const { return gridSizeX_; }
	int GetGridSizeZ() const { return gridSizeZ_; }

	// 【TODO】障害物情報 (Transformなど) を受け取り、isWalkableを設定する関数が必要
	void CheckObstacles(const std::vector<Transform>& obstacles);

private:

	std::vector<std::vector<AStarNode>> navGrid_;

	// X,Z方向のグリッド数
	int gridSizeX_ = 50, gridSizeZ_ = 50;

	// １グリット当たりのワールドサイズ
	float nodeSize_ = 100.0f;
};