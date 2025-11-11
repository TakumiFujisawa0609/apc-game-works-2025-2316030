#pragma once
#include <vector>

class PatrolNode;
class PatrolPath
{
public:

	// 巡回タイプ
	enum class PATHTYPE
	{
		NONE,
		LOOP,
		ROUNDTRIP
	};

	PatrolPath(std::vector<PatrolNode>& nodes, PATHTYPE type);
	~PatrolPath(void);

	// 次に移動すべきノードを返す
	const PatrolNode& GetNextNode(int& currentIndex);

	// 指定されたインデックスのノードを返す
	const PatrolNode& GetNodeIndex(int index) const;

	// ノードの総数を返す
	size_t GetNodeCound(void);

	// 巡回タイプを返す
	PATHTYPE GetPathType(void);

	// 保有するノードを返す
	const std::vector<PatrolNode>& GetNodes(void) const;

	// 現在の位置から最も近いPatrolNodeのインデックスを返す
	int FindClosestNodeIndex(const VECTOR& currentPos) const;

private:
	std::vector<PatrolNode> nodes;		// 巡回ノードを格納したvector配列
	PATHTYPE pathType_;					// 巡回タイプ
	int direction_;					// ROUNDTRIPタイプの進行方向

};

