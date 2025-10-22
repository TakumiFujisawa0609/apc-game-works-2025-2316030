#pragma once
#include <vector>

#include "EnemyComponent.h"

class Transform;
class EnemyChaseComponent :
    public EnemyComponent
{
public:
    EnemyChaseComponent(std::shared_ptr<Charactor> owner, Player& player);
    virtual ~EnemyChaseComponent(void);

    virtual void Update(float deltaTime) override;

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
};

