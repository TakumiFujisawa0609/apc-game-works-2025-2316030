#pragma once
#include "../../ObjectBase/ActorBase.h"
class PlazzleElementBase :
    public ActorBase
{
public:

    PlazzleElementBase(void);
    virtual ~PlazzleElementBase(void);

    void Init(void);
    void Update(float deltaTime) override;
    void OnUpdate(float deltaTime)override;
    void Draw(void);

protected:

    void InitMoel(float distance, Quaternion quaRot, Quaternion quaLocal);

private:

};

