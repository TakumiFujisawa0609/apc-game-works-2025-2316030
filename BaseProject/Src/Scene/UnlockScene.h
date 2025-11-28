#pragma once
#include "Scene.h"

class Player;
class Lockpick;
class Wire;
class KeyholePlate;
class Keyhole;

class UnlockScene :
    public Scene
{
public:
    UnlockScene(SceneController& controller);
    ~UnlockScene(void);

    void Init(Input& input)override;
    void Update(Input& input)override;
    void Draw(void)override;
    void DrawUI(void)override;

    // ゲームシーンからプレイヤーを設定する
    void SetPlayer(std::shared_ptr<Player> player);

    // ゲームシーンからワイヤーを設定する
    void SetWire(std::shared_ptr<Wire> wire);

    // ゲームシーンからロックピックを設定する
    void SetLockPick(std::shared_ptr<Lockpick> lPick);

private:

    // プレイヤー
    std::shared_ptr<Player> player_;
    
    // ワイヤー
    std::shared_ptr<Wire> wire_;
    
    // ロックピック
    std::shared_ptr<Lockpick> lockpick_;
    
    // 鍵穴プレート
    std::shared_ptr<KeyholePlate> keyholePlate_;
    
    // 鍵穴
    std::shared_ptr<Keyhole> keyhole_;

    using UpdateFunc_t = void (UnlockScene::*)(Input& input);
    using DrawFunc_t = void (UnlockScene::*)(void);

    //毎フレーム更新関数ポインタ
    UpdateFunc_t update_;

    //毎フレーム描画関数ポインタ
    DrawFunc_t draw_;

    // 更新関数
    //枠を広げて表示する関数
    void AppearUpdate(Input& input);

    //枠およびメニューを表示する関数
    void NormalUpdate(Input& input);

    //枠を縮小して消す関数
    void DisappearUpdate(Input& input);

    //描画関数
    //枠の出現・消滅時の演出描画
    void ProcessDraw(void);

    //通常のポーズメニュー描画
    void NormalDraw(void);

    // 開錠中の描画
    void LockPickingDraw(void);
};

