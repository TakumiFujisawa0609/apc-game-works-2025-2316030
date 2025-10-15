#pragma once
#include "../ObjectBase/ActorBase.h"

class Player;

class Stage :
    public ActorBase
{
public:
	Stage(Player& player);
	virtual ~Stage(void);
	virtual void Init(void) override;
	virtual void Update(float deltaTime) override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void Draw(void) override;

private:
	Player& player_;

};

