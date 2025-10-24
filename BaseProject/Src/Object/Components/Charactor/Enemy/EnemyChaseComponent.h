#pragma once
#include "../Object/ObjectBase/ActorBase.h"
#include "../../../Player/Player.h"
#include "../../../Enemy/AStar/NavGridManager.h"
#include "../../../Enemy/AStar/AStarNode.h"
#include <vector>
#include <queue>
#include <unordered_set>

#include "EnemyComponent.h"
class Transform;
class EnemyChaseComponent :
    public EnemyComponent
{
public:

    EnemyChaseComponent(std::shared_ptr<Charactor> owner, Player& player, std::shared_ptr<NavGridManager> navManager);
    ~EnemyChaseComponent(void);

    // プレイヤー追跡のメイン処理
    void Chase(float deltaTime, Transform& transform,
        VECTOR& moveDir, VECTOR& movePow, float moveSpeed, Quaternion& outRotation);

    // 障害物情報をNavGridManagerに設定
    void SetObstacle(std::vector<Transform> obstacle);

    void SetTime(float time);
    float GetTime(void);

private:
    // A*のメイン関数
    std::vector<VECTOR> FindPath(VECTOR startPos, VECTOR endPos);

    // A*のヘルパー関数
    float GetHCost(AStarNode* a, AStarNode* b);
    std::vector<AStarNode*> GetNeighbors(AStarNode* node);
    float GetDistance(AStarNode* a, AStarNode* b);
    std::vector<VECTOR> RetracePath(AStarNode* start, AStarNode* end);

    // A*ノードを比較するための構造体（Fスコアが低い方を優先）
    struct CompareNode
    {
        bool operator()(const AStarNode* a, const AStarNode* b) const;
    };

private:
    std::vector<VECTOR> currentPath_;
    int currentPathIndex_ = 0;
    float pathRecalcTimer_ = 0.0f;
    float time_ = 0.0f;

    // NavGridManagerへのスマートポインタ
    std::shared_ptr<NavGridManager> navGridManager_;
};

