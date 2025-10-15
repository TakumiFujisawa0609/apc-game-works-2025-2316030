#pragma once
#include "UIComponent.h"

class Player;
class TimeGauge;


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
    TimeGauge* tGauge_;

};

