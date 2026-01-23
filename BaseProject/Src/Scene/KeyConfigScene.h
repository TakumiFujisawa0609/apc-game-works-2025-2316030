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

    virtual void Init(Input& input)override;
    virtual void Update(Input& input)override;
    virtual void Draw(void)override;
    virtual void DrawUI(void)override;

private:

    void DrawInputList();
    std::wstring GetPeriphString(const PeripheralType& type);
    Input& input_;

    // 現在選択中のインデックス
    int currentIndex_ = 0;
    
    // エディット用で、前にどのキーが押されたかを保存しておく
    std::array<char,256> lastKeyState_;

    // エディット用
    std::array<char, 256> currentKeyState_;

    int currentPadState_;
    int lastPadState_;

    using NameTable_t = std::unordered_map<int, std::wstring>;

    // キーボード名前テーブル
    NameTable_t keyboardNameTable_;

    // パッド名前テーブル
    NameTable_t padNameTable_;

    // アナログ入力名前テーブル
    NameTable_t analogNameTable_;

    using SystemMenuTable_t = std::unordered_map<std::wstring, std::function<void(void)>>;
    SystemMenuTable_t systemMenuTable_;
    std::vector<std::wstring> systemMenuStringList_;

    //入力テーブルのダミーテーブル(更新はこっちをいじる)
    Input::InputTable_t tempInputTable_;        

    using UpdateFunc_t = void (KeyConfigScene::*)(Input& input);
    UpdateFunc_t update_;

    // 非編集中状態
    void NormalUpdate(Input& input);

    // 編集状態
    void EdittingUpdate(Input& input);

    void CommitInputTable();
    void ReloadTable();
    
};

