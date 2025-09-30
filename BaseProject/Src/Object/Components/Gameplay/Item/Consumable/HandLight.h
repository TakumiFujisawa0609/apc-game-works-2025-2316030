#pragma once
#include "ItemBase.h"
class HandLight :
    public ItemBase
{
public:
    // í≤êÆópç¿ïW
    static constexpr VECTOR TARGET_POS = { 25.0f,-20.0f, 50.0f };

    HandLight(std::shared_ptr<ActorBase> owner);
    ~HandLight(void);

    void Init(void) override;
    void Update(float deltaTime) override;
    void Draw(void) override;

private:

    virtual void UpdateOnStage(float deltaTime) override;
    virtual void UpdateInVentory(float deltaTime) override;
    virtual void UpdateInUse(float deltaTime) override;
    virtual void UpdateUsedUp(float deltaTime) override;
    virtual void UpdateDisabled(float deltaTime) override;

};

