#pragma once
#include "../../Common/Component.h"
#include "PlazzleElementBase.h"


class Keyhole :
    public PlazzleElementBase
{
public:

    // 距離
    static constexpr float DISTANCE = 200.0f;
    
    // 拡大率
    static constexpr VECTOR SIZE = { 66.0f,66.0f,66.0f };
    
    // ローカル回転
    static constexpr VECTOR QUAROT_LOCAL = { 0.0f,90.0f,0.0f };

    Keyhole(void);
    virtual ~Keyhole(void);

    void Init(void)override;
    void OnUpdate(float deltaTime)override;
    void Draw(void)override;

    // 角度を設定
    void SetAngle(float angle);

    // 成功したかどうかを設定する
    void SetIsSuccess(bool isSuccess);

    // 成功状態を返す
    bool GetIsSuccess(void) const;

private:

    // 解錠成功フラグ
    bool isSuccess_;

    // 最終角度
    float goalAngle_;
};

