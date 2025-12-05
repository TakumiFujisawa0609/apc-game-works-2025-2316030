#pragma once
#include <vector>
#include <queue>
#include <map>
#include <DxLib.h>
#include "Area.h"

// エリア間の接続情報(エッジ)を表す構造体
struct AreaEdge {

	AreaEdge(int id, float c);
	~AreaEdge(void);

	// 隣接間のエッジID
	int neighborAreaId_;

	// 接続点間の距離
	float cost_;
};

class FieldImpactMap
{
public:
	FieldImpactMap(void);
	~FieldImpactMap(void);

	// エリア間のルートを計算する(グローバル経路探索)
	// エリアIDのキューを返す
	std::queue<int> SearchAreaRouteIndices(const VECTOR& startPos, const VECTOR& targetPos);

	// 指定座標が属するエリアIDを取得
	int GetAreaIdFromPostion(const VECTOR& pos) const;

	// 現在位置から次の目標エリアへ向かうための最適な接続の座標を取得
	VECTOR GetConnectionPointToArea(const VECTOR& currentPos, int nextAreaId) const;

	// エリアデータの追加(マップ構築用)
	void AddArea(const Area& area);
	void AddConnection(int areaIdA, int areaIdB, const VECTOR& connectionPointA, const VECTOR& connectionPointB);

private:

	// エリア
	std::vector<Area> areas_;

	// 隣接リスト:[エリアID] → [AreaEdgeのリスト]
	std::map<int,std::vector<AreaEdge>> adjacencyList_;

	// グローバル経路探索(エリアグラフに対するA*)
	std::vector<int> FindAreaPath(int startAreaId, int targetAreaId);

	// 推定コスト(ヒューリスティック関数)
	float GetHCost(int areaIdA, int areaIdB);
};

