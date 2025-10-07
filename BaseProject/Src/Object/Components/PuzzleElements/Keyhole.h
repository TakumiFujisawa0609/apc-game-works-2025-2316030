#pragma once
#include "../../Common/Component.h"

class ResourceManager;
class Keyhole :
    public Component
{
public:

    struct INT2
    {
        int x, y;
    };

    Keyhole(std::shared_ptr<ActorBase> owner);
    virtual ~Keyhole(void);

    void Init(void);
    virtual void Update(float deltaTime)override;
    void Draw(void);

    // 角度を設定
    void SetAngle(float angle);

private:
    // シングルトン参照
    ResourceManager& resMng_;

    int imgH_;          // 画像ハンドル
    INT2 pos_;          // 中心座標
    float extRate_;     // 拡大率
    float angle_;       // 角度

};

