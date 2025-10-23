#pragma once
#include <vector>
#include <queue>
#include <set>

#include "../../../Enemy/AStar/AStarNode.h"
class NavGridManager;

#include "EnemyComponent.h"
class Transform;
class EnemyChaseComponent :
    public EnemyComponent
{
public:
    EnemyChaseComponent(std::shared_ptr<Charactor> owner, Player& player);
    virtual ~EnemyChaseComponent(void);

    virtual void Update(float deltaTime) override;

    // 再計算
    void RecalcTime(float deltaTime);

    // 追跡処理
    void Chase(float deltaTime,
        Transform& transform,
        VECTOR& moveDir,
        VECTOR& movePow,
        float moveSpeed,
        Quaternion& outRotation);

    // 障害物を設定する
    void SetObstacle(std::vector<Transform> obstacle);

    // 現在追跡中のノード座標リストを設定する
    void SetCurrentPath(std::vector<VECTOR> currentPath);

    // 再計算する時間を取得する
    float GetRecalcTime(void);

    // 再計算時間を設定する
    void SetRecalcTime(float time);

    struct CompareNode
    {
        bool operator()(const AStarNode* a, const AStarNode* b) const
        {
            // Fスコアが低い方を優先
            if (a->F_Score() != b->F_Score())
            {
                return a->F_Score() > b->F_Score();
            }
            // Fスコアが同じ場合、Hスコアが低い方を優先
            return a->H_Score_ > b->H_Score_;
        }
    };

private:

    // 障害物
    std::vector<Transform> obstacle_;

    // メンバー変数
    std::vector<VECTOR> currentPath_; // 現在追跡中のノード座標リスト
    int currentPathIndex_;       // 現在の目標ノードのインデックス
    float pathRecalcTimer_;   // A*を再計算する間隔

    // カプセルキャスト
    void CapsuleCast(float deltaTime);
    
    /// <summary>
    /// 障害物回避のためのステアリングフォースを計算する 
    /// </summary>
    /// <param name="transform">自身のTransform</param>
    /// <param name="deltaTime">処理時間</param>
    /// <returns></returns>
    VECTOR CalculateAvoidanceForce(const Transform& transform, float lookAheadDistance, float deltaTime);

    // A*のメイン関数
    std::vector<VECTOR> FindPath(VECTOR startPos, VECTOR endPos);

    // A*のヘルパー関数（エラーを解消するために定義）
    float GetHCost(AStarNode* a, AStarNode* b); // Hコスト（ヒューリスティック）の計算

    // 8方向の隣接ノードを取得
    std::vector<AStarNode*> GetNeighbors(AStarNode* node);

    // ノード間の距離（Gコスト）を計算
    float GetDistance(AStarNode* a, AStarNode* b);

    // 経路復元関数
    std::vector<VECTOR> RetracePath(AStarNode* start, AStarNode* end);

    NavGridManager* navGridManager_;

    // A*計算中にノードの状態を一時的にリセットする関数（重要）
    void ResetAStarNodes();
};

