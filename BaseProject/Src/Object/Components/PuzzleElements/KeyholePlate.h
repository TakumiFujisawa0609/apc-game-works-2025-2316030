#pragma once
#include "PlazzleElementBase.h"
class ResourceManager;

class KeyholePlate :
    public PlazzleElementBase
{
public:

    static constexpr float DISTANCE = 300.0f;
    static constexpr VECTOR SIZE = { 200.0f,200.0f,200.0f };
    static constexpr VECTOR QUAROT_LOCAL = { 0.0f,90.0f,0.0f };

    KeyholePlate(void);
    virtual ~KeyholePlate(void);

    void Init(void);
    void OnUpdate(float deltaTime)override;
    void Draw(void);

private:

};

