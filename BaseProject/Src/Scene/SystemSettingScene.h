#pragma once
#include<string>
#include<vector>
#include<map>
#include<functional>

class Config;
#include "Scene.h"
class SystemSettingScene :
    public Scene
{

public:
    SystemSettingScene(SceneController& controller);
    virtual void Init(Input& input)override;
    virtual void Update(Input& input)override;
    virtual void Draw(void)override;
    virtual void DrawUI(void)override;

private:

    // 設定
    Config& config_;

    // 
    enum class AdvancedSettingState {
        MainMenu,
        WindowSize
    };

    int frame_;

    int mainMenuIndex_;                 // メイン設定のインデックス
    int prefMenuIndex_;                 // 
    int asMenuIndex_;                   // 
    int wMenuIndex_;                    // 

    bool isInitialize_;                 // 初期化するかどうか

    AdvancedSettingState asState_;      // 設定状態

    bool isFullS_;	                    // フルスクリーンであるかどうか
    int width_, height_;                // ウィンドウのサイズの縦、横

    // マウス感度
    float mouseSensitivity_;            // マウス感度

    // パッド操作
    bool isVibration_;                  // パッドを振動させるかどうか

    // サウンド関係
    int bgmVolume_;                     // bgmの音量
    int seVolume_;                      // seの音量

    // メインメニューの文字列と関数テーブル
    using Menufunc_t = std::function<void(Input&)>;
    std::vector<std::wstring> menuList_;
    std::map<std::wstring, Menufunc_t> mainMenuFuncTable_;

    // 環境設定メニューの文字列と関数テーブル
    using Preferences_t = std::function<void(Input&)>;
    std::vector<std::wstring> prefeMenuList_;
    std::map<std::wstring, Preferences_t> prefeMenuTable_;

    // 詳細設定メニューの文字列と関数テーブル
    using AdvancedSetting_t = std::function<void(Input&)>;
    std::vector<std::wstring> asmList_;
    std::map<std::wstring, AdvancedSetting_t> asmTable_;

    //メンバ関数ポインタ型の別名を定義
    using UpdateFunc_t = void (SystemSettingScene::*)(Input&);
    using DrawFunc_t = void(SystemSettingScene::*)();

    UpdateFunc_t update_;                       //Update用のメンバ関数ポインタ
    DrawFunc_t draw_;                           //Draw用のメンバ関数ポインタ

    void PreferencesUpdate(Input& input);       //設定(環境系)
    void AdvancedSettingUpdate(Input& input);   //設定(PC詳細設定)

    void PreferencesDraw(void);                 // 描画(環境)
    void AdvancedSettingDraw(void);             // 描画(PC詳細設定)

    // メニューリストの初期化
    void InitMenuName(std::wstring menuListIndex, std::map<std::wstring, std::function<void(Input&)>> table, Input& input);

    // ウインドウサイズ切り替え
    using WindowSize_t = std::function<void(Input&)>;           // ウィンドウ設定の関数ポインタ
    std::vector<std::wstring> windowSizeList_;                  // ウィンドウ設定のリスト
    std::map<std::wstring, WindowSize_t> windowSizeTable_;      // ウィンドウ設定のテーブル
};

