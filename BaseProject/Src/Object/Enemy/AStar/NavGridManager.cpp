#include "AStarNode.h"
#include "NavGridManager.h"

NavGridManager::NavGridManager(void)
    :
    gridSizeX_(50),
    gridSizeZ_(50),
    nodeSize_(100.0f)
{
}

NavGridManager::~NavGridManager(void)
{
}

void NavGridManager::InitGrid(std::vector<Transform>& obstacles)
{
    navGrid_.resize(gridSizeX_, std::vector<AStarNode>(gridSizeZ_));

    for (int x = 0; x < gridSizeX_; ++x)
    {
        for (int z = 0; z < gridSizeZ_; ++z)
        {
            AStarNode& node = navGrid_[x][z];
            node.gridX_ = x;
            node.gridZ_ = z;

            // グリッド座標からワールド座標の中心を計算 (例: Stageが(0,0,0)から始まる場合)
            node.worldPos_.x = (x * nodeSize_) + (nodeSize_ / 2.0f);
            node.worldPos_.y = 0.0f;
            node.worldPos_.z = (z * nodeSize_) + (nodeSize_ / 2.0f);

            // 初期は全て通行可能
            node.isWalkAble_ = true;

            // 【TODO】障害物との衝突判定を行い、isWalkableを更新
        }
    }
}

AStarNode* NavGridManager::NodeFromWorldPoint(VECTOR worldPos)
{
    int x = (int)(worldPos.x / nodeSize_);
    int z = (int)(worldPos.z / nodeSize_);

    if (x >= 0 && x < gridSizeX_ && z >= 0 && z < gridSizeZ_)
    {
        return &navGrid_[x][z];
    }
    return nullptr;
}
