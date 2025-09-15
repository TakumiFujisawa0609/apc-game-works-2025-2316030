#pragma once
#include "../ActorBase.h"

class Stage :
    public ActorBase
{
public:
	Stage(void);
	virtual ~Stage(void);
	virtual void Init(void) override;
	virtual void Update(float deltaTime) override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void Draw(void) override;

private:

};

