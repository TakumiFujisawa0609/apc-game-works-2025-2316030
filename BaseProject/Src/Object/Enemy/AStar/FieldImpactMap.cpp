#include "FieldImpactMap.h"

AreaEdge::AreaEdge(int id, float c)
    :
    neighborAreaId_(id),
    cost_(c)
{
}

AreaEdge::~AreaEdge(void)
{
}

FieldImpactMap::FieldImpactMap(void)
{
}

FieldImpactMap::~FieldImpactMap(void)
{
}

std::queue<int> FieldImpactMap::SearchAreaRouteIndices(const VECTOR& startPos, const VECTOR& targetPos)
{
    return std::queue<int>();
}

int FieldImpactMap::GetAreaIdFromPostion(const VECTOR& pos) const
{
    // 全てのエリアに対して座標判定を行い、最初にヒットしたエリアIDを返す
    for (const auto& area : areas_)
    {
        if (area.Contains(pos))
        {
            return area.id_;
        }
    }
    return -1; // エリア外
}

VECTOR FieldImpactMap::GetConnectionPointToArea(const VECTOR& currentPos, int nextAreaId) const
{
    return VECTOR();
}

void FieldImpactMap::AddArea(const Area& area)
{
    // エリアリストに追加
    areas_.push_back(area);
}

void FieldImpactMap::AddConnection(int areaIdA, int areaIdB, const VECTOR& connectionPointA, const VECTOR& connectionPointB)
{
    // エリア間のユークリッド距離をコストとする
    VECTOR diff = VSub(connectionPointA, connectionPointB);
    float cost = VSize(diff);

    // エッジ (Area A -> Area B) の追加
    adjacencyList_[areaIdA].emplace_back(areaIdB, cost);
    // エッジ (Area B -> Area A) の追加 (双方向移動を想定)
    adjacencyList_[areaIdB].emplace_back(areaIdA, cost);
}

std::vector<int> FieldImpactMap::FindAreaPath(int startAreaId, int targetAreaId)
{
    return std::vector<int>();
}

float FieldImpactMap::GetHCost(int areaIdA, int areaIdB)
{
    // AreaのIDとインデックスが一致しない場合があるので、エリアリストから検索
    const Area* areaA = nullptr;
    const Area* areaB = nullptr;

    for (const auto& area : areas_) {
        if (area.id_ == areaIdA) areaA = &area;
        if (area.id_ == areaIdB) areaB = &area;
    }

    if (areaA && areaB) {
        // エリアの中心点間の水平距離を推定コストとする
        VECTOR diff = VSub(areaA->center_, areaB->center_);
        // Y軸（高さ）を無視
        diff.y = 0.0f;
        return VSize(diff);
    }
    return std::numeric_limits<float>::max();
}
