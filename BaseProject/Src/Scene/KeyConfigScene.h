#pragma once
#include "Scene.h"
#include<unordered_map>
#include<string>
#include<array>
#include<functional>
#include"../Input.h"

class KeyConfigScene :
    public Scene
{
public:
    KeyConfigScene(SceneController& controller, Input& input);


    void Init(Input& input)override;
    void Update(Input& input)override;
    void Draw(void)override;
    void DrawUI(void)override;

private:
    void DrawInputList();
    std::wstring GetPeriphString(const PeripheralType& type);
    
    // 入力
    Input& input_;
    
    // 現在選択中のインデックス
    int currentIndex_ = 0;
    
    // エディット用で、前にどのキーが押されたかを保存しておく
    std::array<char,256> lastKeyState_;
    
    // エディット用
    std::array<char, 256> currentKeyState_;

    // 現在のゲームパッド
    int currentPadState_;

    // 最後に入力した時のゲームパッドの状態
    int lastPadState_;

    // 入力テーブル
    using NameTable_t = std::unordered_map<int, std::wstring>;
 
    //キーボード名前テーブル
    NameTable_t keyboardNameTable_;

    //パッド名前テーブル
    NameTable_t padNameTable_;

    //アナログ入力名前テーブル
    NameTable_t analogNameTable_;

    // システムメニューの文字列と関数テーブル
    using SystemMenuTable_t = std::unordered_map<std::wstring, std::function<void(void)>>;
    
    // システムメニューテーブル
    SystemMenuTable_t systemMenuTable_;

    // システムメニュー文字列
    std::vector<std::wstring> systemMenuStringList_;

    //入力テーブルのダミーテーブル
    Input::InputTable_t tempInputTable_;

    using UpdateFunc_t = void (KeyConfigScene::*)(Input& input);
    
    // 更新の関数ポインタ
    UpdateFunc_t update_;
    
    // 非編集中状態
    void NormalUpdate(Input& input);
    
    // 編集状態
    void EdittingUpdate(Input& input);

    // 入力テーブルの一任
    void CommitInputTable();
    
    // テーブルの初期化
    void ReloadTable();
};

