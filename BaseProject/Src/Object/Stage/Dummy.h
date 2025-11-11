#pragma once
#include "../ObjectBase/ActorBase.h"
class Dummy :
	public ActorBase
{
public:
	Dummy(void);
	virtual ~Dummy(void);
	virtual void Init(void) override;
	virtual void Update(float deltaTime) override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void Draw(void) override;

private:


};

