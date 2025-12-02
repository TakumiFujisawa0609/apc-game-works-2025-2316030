#pragma once
#include "PlazzleElementBase.h"
class ResourceManager;

class KeyholePlate :
    public PlazzleElementBase
{
public:

    // ãóó£
    static constexpr float DISTANCE = 300.0f;
    
    // ägëÂó¶
    static constexpr VECTOR SIZE = { 200.0f,200.0f,200.0f };
    
    // ÉçÅ[ÉJÉãâÒì]
    static constexpr VECTOR QUAROT_LOCAL = { 0.0f,90.0f,0.0f };

    KeyholePlate(void);
    virtual ~KeyholePlate(void);

    void Init(void)override;
    void OnUpdate(float deltaTime)override;
    void Draw(void)override;

private:

};

