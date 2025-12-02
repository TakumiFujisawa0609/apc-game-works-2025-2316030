#pragma once
#include "ItemBase.h"
class Wire :
    public ItemBase
{
public:

    // 初期座標
    static constexpr VECTOR INIT_POS = { -500.0f, 150.0f, 150.0f };

    // 初期拡大率
    static constexpr VECTOR INIT_SCL = { 0.3f,0.3f,0.3f };

    // 初期ローカル回転
    static constexpr VECTOR INIT_QUAROTLOCAL = { 0.0f,90.0f,0.0f };

    // マウス感度
    static constexpr float MOUSE_SENSITIVITY = 0.35f;

    // 最大回転度数
    static constexpr float MAX_ROT_Z = 180.0f;

    // 最小回転度数
    static constexpr float MIN_ROT_Z = 0.0f;

    // 一つ目のロック
    static constexpr float FIRST_LOCK = 20.f;

    // 2つ目のロック
    static constexpr float SECOND_LOCK = 15.0f;

    // 3つ目のロック
    static constexpr float THIRD_LOCK = 8.0f;

    // 調整用座標
    static constexpr VECTOR TARGET_POS = { 25.0f,-20.0f, 50.0f };


    Wire(Player& player);
    ~Wire(void);

    void Init(void) override;
    void Update(float deltaTime) override;
    void Draw(void) override;

    // 現在の角度を取得
    float GetCurrentAngle(void);

    // 正解の角度を取得
    float GetGoalAngle(void);

    // ワイヤーを動かして探る
    void UpdateExplore(float deltaTime);

    // 差分が0の時にtrueを返す
    bool IsDifference(void);

    // ロックの段階取得
    int GetLockLevel(void);

    // 初期状態に戻すかどうか
    bool GetDefault(void);

    // 初期状態を設定
    void SetIsDefault(bool flag);

    // クリア判定の取得
    bool isGameClear(void);

    // 鍵回転率の取得
    float GetLockRotationRate(void) const;

    void DrawDebug(void);

private:

    float currentAngle_;        // 現在の角度
    float goalAngle_;           // 正解の角度

    int lastMousePosX_;         // 前フレームのマウスx座標
    bool isDifference_;         // 現在の角度と正解の角度の差分が0であるか

    int lockLevel_;             // 解錠レベル
    bool isDefault_;            // 初期状態に戻すかどうか

    bool isGameClear_;          // クリアの判定

    virtual void OnUpdate(float deltaTime)override;

    virtual void UpdateOnStage(float deltaTime) override;
    virtual void UpdateInVentory(float deltaTime) override;
    virtual void UpdateInUse(float deltaTime) override;
    virtual void UpdateUsedUp(float deltaTime) override;

    // 初期状態に戻す
    void SetDefault(void);
};

