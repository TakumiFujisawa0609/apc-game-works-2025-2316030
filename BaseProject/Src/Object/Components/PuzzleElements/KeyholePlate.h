#pragma once
#include "../../ActorBase.h"
class ResourceManager;

class KeyholePlate :
    public ActorBase
{
public:

    KeyholePlate(void);
    virtual ~KeyholePlate(void);

    void Init(void);
    void Update(float deltaTime) override;
    void OnUpdate(float deltaTime)override;
    void Draw(void);

private:

};

