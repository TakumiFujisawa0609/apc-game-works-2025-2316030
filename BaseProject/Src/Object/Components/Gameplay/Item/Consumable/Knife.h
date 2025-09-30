#pragma once
#include "ItemBase.h"
class Knife :
    public ItemBase
{
public:

    // í≤êÆópç¿ïW
    static constexpr VECTOR TARGET_POS = { 15.0f,-15.0f, 35.0f };

    Knife(std::shared_ptr<ActorBase> owner);
    ~Knife(void);

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

