#pragma once
#include "../ObjectBase/ActorBase.h"
class LightBase :
    public ActorBase
{
public:
    enum LIGHTTYPE
    {
        NONE,
        POINT,
        SPOT
    };

    virtual ~LightBase(void) = default;
    virtual LIGHTTYPE GetType(void)const = 0;
    virtual void Update(void) = 0;

protected:

};

