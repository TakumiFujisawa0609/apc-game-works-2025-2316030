#pragma once
#include "ItemBase.h"
class Wire :
    public ItemBase
{
public:
    static constexpr VECTOR INIT_POS = { 0.0f,0.0f,0.0f };              // ‰ŠúÀ•W
    static constexpr VECTOR INIT_SCL = { 0.05f,0.05f,0.05f };           // ‰ŠúŠg‘å—¦
    static constexpr VECTOR INIT_QUAROTLOCAL = { 0.0f,-90.0f,0.0f };    // ‰Šúƒ[ƒJƒ‹‰ñ“]

    Wire(std::shared_ptr<ActorBase> owner);
    ~Wire();

    void Init(void) override;
    void Update(float deltaTime) override;
    void Draw(void) override;

    // Œ»İ‚ÌŠp“x‚ğæ“¾
    float GetCurrentAngle(void);

    // ³‰ğ‚ÌŠp“x‚ğæ“¾
    float GetGoalAngle(void);


private:


    float currentAngle_;        // Œ»İ‚ÌŠp“x
    float goalAngle_;           // ³‰ğ‚ÌŠp“x

    virtual void UpdateOnStage(float deltaTime) override;
    virtual void UpdateInVentory(float deltaTime) override;
    virtual void UpdateInUse(float deltaTime) override;
    virtual void UpdateUsedUp(float deltaTime) override;
    virtual void UpdateDisabled(float deltaTime) override;
};

