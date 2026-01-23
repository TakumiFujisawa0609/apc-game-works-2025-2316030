#pragma once
#include "../../Common/Component.h"

class ActorBase;
class Charactor;
class CharactorComponent :
    public Component
{
public:
	CharactorComponent(std::shared_ptr<Charactor> owner);
	virtual ~CharactorComponent(void) = default;

	// 毎フレーム状態を更新する
	virtual void Update(float deltaTime) = 0;

    // Charactorを取得する
	std::weak_ptr<Charactor> GetCharactor(void);

protected:

	// Charactorを継承したクラス
	std::weak_ptr<Charactor> charaOwner_;	

};

