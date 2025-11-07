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

    static constexpr VECTOR POS = {0.0f,300.0f,300.0f};
    static constexpr VECTOR SCALE = { 1.0f,1.0f ,1.0f };
    static constexpr VECTOR QUAROT_LOCAL = { 0.0f,180.0f,0.0f };

    static constexpr VECTOR TOP = {0.0f,160.0f,0.0f};
    static constexpr VECTOR DOWN = {0.0f,0.0f,0.0f};
    static constexpr float RADIUS = 20.0f;

    // 視野の広さ
    static constexpr float EYE_VIEW_RANGE = 450.0f;

    // 攻撃範囲の広さ
    static constexpr float ATTACK_RANGE = 175.0f;

    // 視野角
    static constexpr float VIEW_ANGLE = 30.0f;

    enum class STATE
    {
        PATROL, // 巡回
        CHASE,  // 追跡
        ATTACK, // 攻撃
        IDLE    // 待機
    };

    enum class ANIM
    {
        PATROL,
        CHASE,
        ATTACK,
        IDLE,
        LOOK
    };

    // 回転完了するまでの時間
    static constexpr float TIME_ROT = 1.0f;

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

    int frame_;

    EnemyPatrolComponent* patrolComponent_;     // 移動
    EnemyChaseComponent* chaseComponent_;   // 追跡

    STATE state_;       // 現在の行動パターン

    // 巡回パスの情報
    std::shared_ptr<PatrolPath> patrolPath_;
    int currentPatrolPathIndex_;                // 現在のパスの番号

    std::unique_ptr<AnimationController> animationController_;  // アニメーション

    // アニメーションの初期化
    virtual void InitAnimation(void);

    std::shared_ptr<NavGridManager> navGridManager_;

    bool isWaiting_;            // 待機中かどうか
    float currentWaitTime_;     // 現在の待機残り時間
    float dis_;
    int currentNode_;

private:

    // 回転処理
    void Rotate(void);

    void Collision(void);               // 衝突判定
    void CollisionCapsule(void);        // カプセルとの衝突判定
    void CollisionGravity(void);        // 重力との衝突判定
    void CalcGravityPow(void);          // 重力加算処理

    void ChangeState(STATE state);

    bool EyeSerch(void);

    bool HearingSound(void);

    void DrawDebug(void);

    void UpdatePatrol(float deltaTime);
    void UpdateChase(float deltaTime);

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

    std::vector<VECTOR> currentPath_;
    int currentPathIndex_ = 0;
    float pathRecalcTimer_ = 0.0f;
    float time_ = 0.0f;
};

