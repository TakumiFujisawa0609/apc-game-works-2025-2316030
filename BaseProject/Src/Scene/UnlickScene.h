#pragma once
#include "Scene.h"
class UnlickScene :
    public Scene
{
public:
    UnlickScene(SceneController& controller);
    ~UnlickScene(void);

    virtual void Init(Input& input)override;
    virtual void Update(Input& input)override;
    virtual void Draw(void)override;

private:
    int frame_ = 0;

    using UpdateFunc_t = void (UnlickScene::*)(Input& input);
    using DrawFunc_t = void (UnlickScene::*)(void);

    UpdateFunc_t update_;//毎フレーム更新関数ポインタ
    DrawFunc_t draw_;//毎フレーム描画関数ポインタ

    //更新関数
    void AppearUpdate(Input& input);//枠を広げて表示する関数
    void NormalUpdate(Input& input);//枠およびメニューを表示する関数
    void DisappearUpdate(Input& input);//枠を縮小して消す関数

    //描画関数
    void ProcessDraw(void);//枠の出現・消滅時の演出描画
    void NormalDraw(void);//通常のポーズメニュー描画

    void LockPickingDraw(void);// 開錠中の描画
};

