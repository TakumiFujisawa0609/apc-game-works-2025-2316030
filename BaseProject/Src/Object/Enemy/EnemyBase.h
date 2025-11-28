#pragma once
#include "../ObjectBase/Charactor.h"
#include "AStar/NavGridManager.h"
#include "AStar/AStarNode.h"
#include <vector>
#include <queue>
#include <unordered_set>

class Collider;
class Capsule;
class AnimationController;

class EnemyPatrolComponent;
class EnemyChaseComponent;

class PatrolNode;
class PatrolPath;

class NavGridManager;

class Player;
class EnemyBase :
    public Charactor
{
public:

    static constexpr VECTOR POS = { -1713.0f,200.0f,2580.0f };          // 座標位置
    static constexpr VECTOR SCALE = { 1.0f,1.0f ,1.0f };                // 拡大率
    static constexpr VECTOR QUAROT_LOCAL = { 0.0f,180.0f,0.0f };        // ローカル回転

    static constexpr VECTOR TOP = {0.0f,160.0f,0.0f};                   // カプセルの上部
    static constexpr VECTOR DOWN = {0.0f,0.0f,0.0f};                    // カプセルの下部
    static constexpr float RADIUS = 20.0f;                              // カプセルの半径
    static constexpr float EYE_VIEW_RANGE = 250.0f;                     // 視野の広さ
    static constexpr float ATTACK_RANGE = 175.0f;                       // 攻撃範囲の広さ
    static constexpr float VIEW_ANGLE = 30.0f;                          // 視野角
    static constexpr float TIME_ROT = 1.0f;                             // 回転完了するまでの時間

    enum class STATE
    {
        PATROL, // 巡回
        CHASE,  // 追跡
        ATTACK, // 攻撃
        IDLE    // 待機
    };

    enum class ANIM
    {
        PATROL, // 巡回
        CHASE,  // 追跡
        ATTACK, // 攻撃
        IDLE,   // 待機
        LOOK    // 警戒
    };

    EnemyBase(Player& player);
    ~EnemyBase(void);

    void Init(void) override;
    void InitComponents(void) override;
    void Update(float deltaTime) override;
    void OnUpdate(float deltaTime) override;
    void Draw(void) override;

    // 移動中に障害物として認識される物を設定する
    void SetObstacle(std::vector<Transform> obstacle);

    // 徘徊用のノードパスをステージから取得する
    void SetPatrolPath(std::shared_ptr<PatrolPath> path);

    // グリッドマネージャーをステージから取得する
    void SetNavGridManagedr(std::shared_ptr<NavGridManager> navGridManager);

protected:

    // モデル情報初期化
    virtual void InitModel(VECTOR pos, VECTOR scl, VECTOR quaRotLocal);

    Player& player_;

    // 視界内にプレイヤーが入っているか
    bool isPlayerInSight_;

    // 音を検知したかどうか
    bool isHearingSound_;

    int frame_;                             // フレーム

    EnemyPatrolComponent* patrolComponent_; // 移動
    EnemyChaseComponent* chaseComponent_;   // 追跡

    STATE state_;       // 現在の行動パターン

    std::shared_ptr<PatrolPath> patrolPath_;    // 巡回パス
    int currentPatrolPathIndex_;                // 現在のパスの番号

    std::unique_ptr<AnimationController> animationController_;  // アニメーション

    // アニメーションの初期化
    virtual void InitAnimation(void);

    std::shared_ptr<NavGridManager> navGridManager_;        // A*経路探索

    bool isWaiting_;            // 待機中かどうか
    float currentWaitTime_;     // 現在の待機残り時間
    float dis_;                 // プレイヤーとの距離
    int currentNode_;           // 現在の追跡ノード

private:

    // 回転処理
    void Rotate(void);

    void Collision(void);               // 衝突判定
    void CollisionCapsule(void);        // カプセルとの衝突判定
    void CollisionGravity(void);        // 重力との衝突判定
    void CalcGravityPow(void);          // 重力加算処理

    void ChangeState(STATE state);      // 状態の変更

    bool EyeSerch(void);                // 視覚判定

    bool HearingSound(void);            // 聴覚判定

    void DrawDebug(void);

    void UpdatePatrol(float deltaTime); // 徘徊状態の更新
    void UpdateChase(float deltaTime);  // 追跡状態の更新

    // A*でのパスを探す
    std::vector<VECTOR> FindPath(VECTOR startPos, VECTOR endPos);

    // A*のノードのコスト計算
    float GetHCost(AStarNode* a, AStarNode* b);
    
    // 隣り合うノードの位置の取得
    std::vector<AStarNode*> GetNeighbors(AStarNode* node);
    
    // A*の隣り合うノード同士の距離
    float GetDistance(AStarNode* a, AStarNode* b);

    // ノードを辿らせる
    std::vector<VECTOR> RetracePath(AStarNode* start, AStarNode* end);

    // A*ノードを比較するための構造体（Fスコアが低い方を優先）
    struct CompareNode
    {
        bool operator()(const AStarNode* a, const AStarNode* b) const;
    };

    bool isDamageCheckActive_;              // 攻撃判定
    bool hasPlayerBeenHit_;                 // プレイヤーに対して攻撃があたった場合

    void HandleAttackCollsion(float deltaTime);     // ダメージ判定

    bool isAttackRange(void);           // 攻撃判定

    bool isAttack_;                     // 攻撃をするかどうか
};

