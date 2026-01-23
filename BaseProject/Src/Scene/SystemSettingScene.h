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

    // 詳細設定
    enum class AdvancedSettingState {
        MainMenu,
        WindowSize
    };

    // メイン設定のインデックス
    int mainMenuIndex_;

    // 環境設定のインデックス
    int prefMenuIndex_;

    // 詳細設定のインデックス
    int asMenuIndex_;

    // ウィンドウメニューのインデックス
    int wMenuIndex_;

    // 初期化するかどうか
    bool isInitialize_;

    // 設定状態
    AdvancedSettingState asState_;

    // フルスクリーンであるかどうか
    bool isFullS_;

    // ウィンドウのサイズの縦、横
    int width_, height_;

    // マウス感度
    float mouseSensitivity_;

    // パッドを振動させるかどうか
    bool isVibration_;

    // bgmの音量
    int bgmVolume_;

    // seの音量
    int seVolume_;

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

    // メンバ関数ポインタ型の別名を定義
    using UpdateFunc_t = void (SystemSettingScene::*)(Input&);
    using DrawFunc_t = void (SystemSettingScene::*)();

    // Update用のメンバ関数ポインタ
    UpdateFunc_t update_;

    // Draw用のメンバ関数ポインタ
    DrawFunc_t draw_;

    // 設定(環境系)
    void PreferencesUpdate(Input& input);

    // 設定(PC詳細設定)
    void AdvancedSettingUpdate(Input& input);

    // 描画(環境)
    void PreferencesDraw(void);

    // 描画(PC詳細設定)
    void AdvancedSettingDraw(void);

    // メニューリストの初期化
    void InitMenuName(std::wstring menuListIndex, std::map<std::wstring, std::function<void(Input&)>> table, Input& input);

    // ウインドウサイズ切り替え

    // ウィンドウ設定の関数ポインタ
    using WindowSize_t = std::function<void(Input&)>;
    
    // ウィンドウ設定のリスト
    std::vector<std::wstring> windowSizeList_;

    // ウィンドウ設定のテーブル
    std::map<std::wstring, WindowSize_t> windowSizeTable_;
};

