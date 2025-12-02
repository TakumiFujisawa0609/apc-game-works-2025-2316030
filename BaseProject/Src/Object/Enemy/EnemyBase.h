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

    // 座標位置
    static constexpr VECTOR POS = { -1713.0f,200.0f,2580.0f };

    // 拡大率
    static constexpr VECTOR SCALE = { 1.0f,1.0f ,1.0f };

    // ローカル回転
    static constexpr VECTOR QUAROT_LOCAL = { 0.0f,180.0f,0.0f };

    // カプセルの上部
    static constexpr VECTOR TOP = {0.0f,160.0f,0.0f};

    // カプセルの下部
    static constexpr VECTOR DOWN = {0.0f,0.0f,0.0f};
    
    // カプセルの半径
    static constexpr float RADIUS = 20.0f;
    
    // 視野の広さ
    static constexpr float EYE_VIEW_RANGE = 250.0f;

    // 攻撃範囲の広さ
    static constexpr float ATTACK_RANGE = 175.0f;
    
    // 視野角
    static constexpr float VIEW_ANGLE = 30.0f;
    
    // 回転完了するまでの時間
    static constexpr float TIME_ROT = 1.0f;

    // 歩き速度
    static constexpr float WALK_SPEED = 8.0f;

    // 行動パターン
    enum class STATE{
        PATROL, // 巡回
        CHASE,  // 追跡
        ATTACK, // 攻撃
        IDLE    // 待機
    };

    // アニメーション状態
    enum class ANIM{
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

    // プレイヤー参照
    Player& player_;
    
    // 視界内にプレイヤーが入っているか
    bool isPlayerInSight_;

    // 音を検知したかどうか
    bool isHearingSound_;

    // フレーム
    int frame_;

    // 移動
    EnemyPatrolComponent* patrolComponent_;
    
    // 追跡
    EnemyChaseComponent* chaseComponent_;

    // 現在の行動パターン
    STATE state_;

    // 巡回パス
    std::shared_ptr<PatrolPath> patrolPath_;
    
    // 現在のパスの番号
    int currentPatrolPathIndex_;

    // アニメーション
    std::unique_ptr<AnimationController> animationController_;

    // モデル情報初期化
    virtual void InitModel(VECTOR pos, VECTOR scl, VECTOR quaRotLocal);

    // アニメーションの初期化
    virtual void InitAnimation(void);

    // A*経路探索
    std::shared_ptr<NavGridManager> navGridManager_;

    // 待機中かどうか
    bool isWaiting_;

    // 現在の待機残り時間
    float currentWaitTime_;

    // プレイヤーとの距離
    float dis_;

    // 現在の追跡ノード
    int currentNode_;

private:

    // 回転処理
    void Rotate(void);

    // 衝突判定
    void Collision(void);

    // カプセルとの衝突判定
    void CollisionCapsule(void);
    
    // 重力との衝突判定
    void CollisionGravity(void);
    
    // 重力加算処理
    void CalcGravityPow(void);

    // 状態の変更
    void ChangeState(STATE state);

    // 視覚判定
    bool EyeSerch(void);

    // 聴覚判定
    bool HearingSound(void);

    // デバック描画
    void DrawDebug(void);

    // 徘徊状態の更新
    void UpdatePatrol(float deltaTime);

    // 追跡状態の更新
    void UpdateChase(float deltaTime);

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

    // 攻撃判定
    bool isDamageCheckActive_;

    // プレイヤーに対して攻撃があたった場合
    bool hasPlayerBeenHit_;

    // ダメージ判定
    void HandleAttackCollsion(float deltaTime);

    // 攻撃判定
    bool isAttackRange(void);

    // 攻撃をするかどうか
    bool isAttack_;
};

