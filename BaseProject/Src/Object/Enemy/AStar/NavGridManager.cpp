#include "../../Common/Transform.h"
#include "AStarNode.h"
#include "NavGridManager.h"

void NavGridManager::InitGrid(int gridSizeX, int gridSizeZ, float nodeSize)
{
    gridSizeX_ = gridSizeX;
    gridSizeZ_ = gridSizeZ;
    nodeSize_ = nodeSize;

    navGrid_.resize(gridSizeX_, std::vector<AStarNode>(gridSizeZ_));

    for (int x = 0; x < gridSizeX_; ++x){
        for (int z = 0; z < gridSizeZ_; ++z){
            AStarNode& node = navGrid_[x][z];
            node.gridX_ = x;
            node.gridZ_ = z;

            // ワールド座標の中心を計算
            node.worldPos_.x = (x * nodeSize_) + (nodeSize_ / 2.0f);
            node.worldPos_.y = 0.0f; // 地面の高さに固定
            node.worldPos_.z = (z * nodeSize_) + (nodeSize_ / 2.0f);

            node.isWalkable_ = true; // 初期は全て通行可能
        }
    }
}

AStarNode* NavGridManager::NodeFromWorldPoint(VECTOR worldPos)
{
    // ワールド座標をグリッド座標に変換
    int x = (int)(worldPos.x / nodeSize_);
    int z = (int)(worldPos.z / nodeSize_);

    if (x >= 0 && x < gridSizeX_ && z >= 0 && z < gridSizeZ_){
        return &navGrid_[x][z];
    }
    return nullptr;
}

AStarNode* NavGridManager::GetNode(int x, int z)
{
    if (x >= 0 && x < gridSizeX_ && z >= 0 && z < gridSizeZ_){
        return &navGrid_[x][z];
    }
    return nullptr;
}

void NavGridManager::ResetAllNodes()
{
    for (int x = 0; x < gridSizeX_; ++x){
        for (int z = 0; z < gridSizeZ_; ++z){
            navGrid_[x][z].Reset();
        }
    }
}

void NavGridManager::CheckObstacles(const std::vector<Transform>& obstacles)
{
    // 障害物の座標とノードの座標を比較し
    // 障害物と重なるノードのisWalkable をfalseに設定する処理が必要

    // 既存の障害物は上書きするため、全てのノードをリセットして通行可能に戻す
    
    ResetAllNodes();

    for (int x = 0; x < gridSizeX_; ++x){
        for (int z = 0; z < gridSizeZ_; ++z){
            navGrid_[x][z].isWalkable_ = true;
        }
    }

    // 2. すべての障害物に対してループ
    for (const auto& obstacleTransform : obstacles){
        //if (obstacleTransform) continue;

        // 障害物の境界ボックスのパラメータを取得
        VECTOR obsPos = obstacleTransform.pos;
        VECTOR obsScale = obstacleTransform.scl;

        // 障害物のワールド座標での範囲 (XとZ軸のみ考慮)
        float minX = obsPos.x - (obsScale.x / 2.0f);
        float maxX = obsPos.x + (obsScale.x / 2.0f);
        float minZ = obsPos.z - (obsScale.z / 2.0f);
        float maxZ = obsPos.z + (obsScale.z / 2.0f);

        // 3. 全てのナビゲーションノードに対してループ
        for (int x = 0; x < gridSizeX_; ++x){
            for (int z = 0; z < gridSizeZ_; ++z){
                AStarNode& node = navGrid_[x][z];

                // ノードが既に通行不可とマークされていたらスキップ
                if (!node.isWalkable_) continue;

                // 4. ノードの中心座標が障害物の境界ボックス内にあるかを判定
                float nodeX = node.worldPos_.x;
                float nodeZ = node.worldPos_.z;

                bool isInsideX = (nodeX >= minX) && (nodeX <= maxX);
                bool isInsideZ = (nodeZ >= minZ) && (nodeZ <= maxZ);

                if (isInsideX && isInsideZ){
                    // ノードの中心が障害物の中にあるため、通行不可とマーク
                    node.isWalkable_ = false;
                }
            }
        }
    }
}
