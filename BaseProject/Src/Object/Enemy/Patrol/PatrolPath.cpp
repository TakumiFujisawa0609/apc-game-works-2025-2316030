#include <algorithm>
#include <functional>
#define NOMINMAX
#include "PatrolNode.h"
#include "PatrolPath.h"

PatrolPath::PatrolPath(std::vector<PatrolNode>& nodes, PATHTYPE type)
    :
    nodes(nodes),
    pathType_(type),
    direction_(1)
{
}

PatrolPath::~PatrolPath(void)
{
}

const PatrolNode& PatrolPath::GetNextNode(int& currentIndex)
{
    if (pathType_ == PATHTYPE::LOOP)
    {
        // 次のインデックスを計算し、リストのサイズで割った余りを取る
        currentIndex = (currentIndex + 1) % nodes.size();

    }
    else if (pathType_ == PATHTYPE::ROUNDTRIP)
    {
        // 次のインデックスを計算
        int nextIndex = currentIndex + direction_;

        // 1. nextIndexがリストの有効範囲外に出たとき (終点N or 始点-1) に反転
        // nodes.size()はリストの要素数。最後のインデックスは nodes.size() - 1
        if (nextIndex >= nodes.size() || nextIndex < 0)
        {
            // 進行方向を反転
            direction_ *= -1;

            // 現在のインデックス (0またはsize-1) から、反転後の方向に進む
            currentIndex += direction_;
        }
        else
        {
            // 通常の前進
            currentIndex = nextIndex;
        }
    }

    // 新しいインデックスに対応するノードを返す
    return nodes[currentIndex];
}

const PatrolNode& PatrolPath::GetNodeIndex(int index) const
{
    return nodes[index];
}

size_t PatrolPath::GetNodeCound(void)
{
    return nodes.size();
}

PatrolPath::PATHTYPE PatrolPath::GetPathType(void)
{
    return pathType_;
}

const std::vector<PatrolNode>& PatrolPath::GetNodes(void) const
{
    return nodes;
}

int PatrolPath::FindClosestNodeIndex(const VECTOR& currentPos) const
{
    if (nodes.empty()) return -1;

    float minDistanceSq = std::numeric_limits<float>::max();
    int closestIndex = -1;

    for (int i = 0; i < nodes.size(); ++i)
    {
        VECTOR nodePos = nodes[i].GetPos();
        // 水平距離のみを比較するためY座標を無視
        VECTOR diff = VSub({ nodePos.x, 0.0f, nodePos.z }, { currentPos.x, 0.0f, currentPos.z });
        float distanceSq = VSquareSize(diff);

        if (distanceSq < minDistanceSq)
        {
            minDistanceSq = distanceSq;
            closestIndex = i;
        }
    }
    return closestIndex;
}
