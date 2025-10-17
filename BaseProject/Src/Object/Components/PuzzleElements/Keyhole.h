#pragma once
#include "../../Common/Component.h"
#include "PlazzleElementBase.h"


class Keyhole :
    public PlazzleElementBase
{
public:


    static constexpr float DISTANCE = 200.0f;
    static constexpr VECTOR SIZE = { 66.0f,66.0f,66.0f };
    static constexpr VECTOR QUAROT_LOCAL = { 0.0f,90.0f,0.0f };

    Keyhole(void);
    virtual ~Keyhole(void);

    void Init(void);
	void OnUpdate(float deltaTime)override;
    void Draw(void);

    // Šp“x‚ğİ’è
    void SetAngle(float angle);

	void SetIsSuccess(bool isSuccess);

	bool GetIsSuccess(void) const;

private:

	bool isSuccess_; // ‰ğù¬Œ÷ƒtƒ‰ƒO

};

