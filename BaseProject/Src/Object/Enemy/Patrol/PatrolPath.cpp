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

        // リストの終点または始点にて到達した場合
        if (nextIndex >= nodes.size() - 1 || nextIndex <= 0)
        {
            // 進行方向を反転
            direction_ *= -1;

            // インデックスを終点または始点に留める
            if (nextIndex >= nodes.size() - 1)
            {
                currentIndex = static_cast<int>(nodes.size() - 1);
            }
            else
            {
                currentIndex = 0;
            }
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
