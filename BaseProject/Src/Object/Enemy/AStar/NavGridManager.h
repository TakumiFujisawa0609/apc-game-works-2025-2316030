#pragma once
#include <DxLib.h>
#include <vector>

class Transform;
class AStarNode;

class NavGridManager
{
public:

	NavGridManager(void);
	~NavGridManager(void);

	void InitGrid(std::vector<Transform>& obstacles);

	// ワールド座標をグリット座標に変換する
	AStarNode* NodeFromWorldPoint(VECTOR worldPos);

private:

	std::vector<std::vector<AStarNode>> navGrid_;
	int gridSizeX_, gridSizeZ_;			// X,Z方向のグリッド数
	float nodeSize_;					// １グリット当たりのワールドサイズ
};

