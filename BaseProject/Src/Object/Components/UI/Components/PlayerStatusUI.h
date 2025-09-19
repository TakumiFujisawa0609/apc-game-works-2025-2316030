#pragma once
#include "UIComponent.h"

class Player;
class OxygenGauge;


class PlayerStatusUI :
    public UIComponent
{
public:
    PlayerStatusUI(std::shared_ptr<ActorBase> owner, Player& player);
    ~PlayerStatusUI(void);

    void Update(float deltaTime)override;
    void Draw(void);

private:

    // プレイヤー情報取得用
    Player& player_;

    // 酸素ゲージUI
    OxygenGauge* oxygen_;

};

