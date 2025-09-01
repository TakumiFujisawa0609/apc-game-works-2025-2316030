#pragma once
#include<string>
#include<vector>
#include<map>
#include<functional>

#include "Scene.h"
class Input;
class PauseScene :
    public Scene
{
private:
    int frame_ = 0;
    int currentIndex_ = 0;
    //メニューの文字列と関数のテーブル
    using MenuFunc_t = std::function<void(Input&)>;
    std::vector<std::wstring> menuList_;
    std::map<std::wstring,MenuFunc_t> menuFuncTable_;

    using UpdateFunc_t = void (PauseScene::*)(Input& input);
    using DrawFunc_t = void (PauseScene::*)();

    UpdateFunc_t update_;//毎フレーム更新関数ポインタ
    DrawFunc_t draw_;//毎フレーム描画関数ポインタ

    //更新関数
    void AppearUpdate(Input& input);//枠を広げて表示する関数
    void NormalUpdate(Input& input);//枠およびメニューを表示する関数
    void DisappearUpdate(Input& input);//枠を縮小して消す関数
   
    //描画関数
    void ProcessDraw();//枠の出現・消滅時の演出描画
    void NormalDraw();//通常のポーズメニュー描画

    //メニューのリストを表示する
    void DrawMenuList();


public:
    PauseScene(SceneController& controller);
    virtual void Update(Input& input)override;
    virtual void Draw()override;

};

