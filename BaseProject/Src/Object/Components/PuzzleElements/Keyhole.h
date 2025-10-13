#pragma once
#include "../../Common/Component.h"
#include "../../ActorBase.h"


class Keyhole :
    public ActorBase
{
public:

    Keyhole(void);
    virtual ~Keyhole(void);

    void Init(void);
    void Update(float deltaTime)override;
	void OnUpdate(float deltaTime)override;
    void Draw(void);

    // Šp“x‚ğİ’è
    void SetAngle(float angle);

	void SetIsSuccess(bool isSuccess);

	bool GetIsSuccess(void) const;

private:

	bool isSuccess_; // ‰ğù¬Œ÷ƒtƒ‰ƒO

};

