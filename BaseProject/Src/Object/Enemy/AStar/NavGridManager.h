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

	// X方向のグリットを取得
	int GetGridSizeX() const { return gridSizeX_; }
	
	// Z方向のグリットを取得
	int GetGridSizeZ() const { return gridSizeZ_; }

	// 障害物情報の受け取り
	void CheckObstacles(const std::vector<Transform>& obstacles);

private:

	// A*経路探索用のノード
	std::vector<std::vector<AStarNode>> navGrid_;

	// X,Z方向のグリッド数
	int gridSizeX_ = 50, gridSizeZ_ = 50;

	// １グリット当たりのワールドサイズ
	float nodeSize_ = 100.0f;
};

