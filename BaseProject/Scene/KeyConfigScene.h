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
private:
    void DrawInputList();
    std::wstring GetPeriphString(const PeripheralType& type);
    Input& input_;
    int currentIndex_ = 0;//現在選択中のインデックス
    
    std::array<char,256> lastKeyState_;//エディット用で、前にどのキーが押されたかを保存しておく
    std::array<char, 256> currentKeyState_;//エディット用

    int currentPadState_;
    int lastPadState_;

    using NameTable_t = std::unordered_map<int, std::wstring>;
    NameTable_t keyboardNameTable_;//キーボード名前テーブル
    NameTable_t padNameTable_;//パッド名前テーブル
    NameTable_t analogNameTable_;//アナログ入力名前テーブル

    using SystemMenuTable_t = std::unordered_map<std::wstring, std::function<void(void)>>;
    SystemMenuTable_t systemMenuTable_;
    std::vector<std::wstring> systemMenuStringList_;

    Input::InputTable_t tempInputTable_;//入力テーブルのダミーテーブル(更新はこっちをいじる)


    using UpdateFunc_t = void (KeyConfigScene::*)(Input& input);
    UpdateFunc_t update_;
    void NormalUpdate(Input& input);//非編集中状態
    void EdittingUpdate(Input& input);//編集状態

    
    void CommitInputTable();
    void ReloadTable();
public:
    KeyConfigScene(SceneController& controller,Input& input);
    
    virtual void Update(Input& input)override;
    virtual void Draw()override;
    
};

