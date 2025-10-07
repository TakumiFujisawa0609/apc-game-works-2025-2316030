#pragma once
#include "../../Common/Component.h"

class ResourceManager;

class KeyholePlate :
    public Component
{
public:
    
    struct INT2
    {
        int x, y;
    };

    static constexpr INT2 INIT_POS = { 0,0 };


    KeyholePlate(std::shared_ptr<ActorBase> owner);
    virtual ~KeyholePlate(void);

    void Init(void);
    virtual void Update(float deltaTime) override;
    void Draw(void);

private:
    // シングルトン参照
    ResourceManager& resMng_;

    int imgH_;          // 画像ハンドル
    INT2 pos_;          // 中心座標
    float extRate_;     // 拡大率
    float angle_;       // 角度
};

