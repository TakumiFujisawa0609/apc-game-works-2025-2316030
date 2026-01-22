#pragma once

class PatrolPath;

#include "EnemyComponent.h"
class EnemyPatrolComponent :
    public EnemyComponent
{
public:
    EnemyPatrolComponent(std::shared_ptr<Charactor> owner, Player& player);
    virtual ~EnemyPatrolComponent(void);

    virtual void Update(float deltaTime) override;

    
   /// <summary>
   /// 巡回処理
   /// </summary>
   /// <param name="deltaTime">処理時間</param>
   /// <param name="transform">現在の座標・回転</param>
   /// <param name="path">巡回パス情報</param>
   /// <param name="currentIndex">現在の巡回ノードの番号</param>
   /// <param name="moveDir">移動方向</param>
   /// <param name="movePow">移動量</param>
   /// <param name="moveSpeed">移動速度</param>
   /// <param name="outRotation">モデルの回転角度</param>
   void Patrol(
       float deltaTime,
       Transform& transform,
       std::shared_ptr<PatrolPath> path,
       int& currentIndex,
       VECTOR& moveDir,
       VECTOR& movePow,
       float moveSpeed,
       Quaternion& outRotation);

   float GetDis(void);
   int GetCurrentNode(void);

   void DrawDebug(std::shared_ptr<PatrolPath> path, int currentIndex);

private:
    bool isWaiting_;            // 待機中かどうか
    float currentWaitTime_;     // 現在の待機残り時間
    Quaternion outRotation_;    // 回転角度

    float dis_;
    int currentNode_;

    // 周囲を見回す
    // 待機アニメーション
    // 
};

