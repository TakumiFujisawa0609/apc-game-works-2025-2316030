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
    void Init(Input& input)override;
    void Update(Input& input)override;
    void Draw(void)override;
    void DrawUI(void)override;

private:

    // 設定
    Config& config_;

    // 設定状態
    enum class AdvancedSettingState {
        MainMenu,
        WindowSize
    };

    // メインメニューインデックス
    int mainMenuIndex_;

    // 環境設定メニューインデックス
    int prefMenuIndex_;

    // 詳細設定メニューインデックス
    int asMenuIndex_;
    
    // ウィンドウメニュー切り替え用インデックス
    int wMenuIndex_;

    // 初期化
    bool isInitialize_;

    // 設定状態
    AdvancedSettingState asState_;

    // フルスクリーンであるかどうか
    bool isFullS_;

    // スクリーンの幅、高さ
    int width_, height_;

    // マウス感度
    float mouseSensitivity_;

    // パッド操作
    bool isVibration_;

    // サウンド関係
    int bgmVolume_;
    int seVolume_;

    // メインメニューの文字列と関数テーブル
    using Menufunc_t = std::function<void(Input&)>;

    // メインメニュー文字列リスト
    std::vector<std::wstring> menuList_;

    // メインメニューテーブル
    std::map<std::wstring, Menufunc_t> mainMenuFuncTable_;

    // 環境設定メニューの文字列と関数テーブル
    using Preferences_t = std::function<void(Input&)>;

    // 環境設定メニュー文字列リスト
    std::vector<std::wstring> prefeMenuList_;
    
    // 環境設定メニューテーブル
    std::map<std::wstring, Preferences_t> prefeMenuTable_;

    // 詳細設定メニューの文字列と関数テーブル
    using AdvancedSetting_t = std::function<void(Input&)>;

    // 詳細設定メニュー文字列リスト
    std::vector<std::wstring> asmList_;

    // 詳細設定メニューテーブル
    std::map<std::wstring, AdvancedSetting_t> asmTable_;

    // メンバ関数ポインタ型の別名を定義
    using UpdateFunc_t = void (SystemSettingScene::*)(Input&);
    using DrawFunc_t = void(SystemSettingScene::*)();

    // Update用のメンバ関数ポインタ
    UpdateFunc_t update_;

    // Draw用のメンバ関数ポインタ
    DrawFunc_t draw_;

    // 設定(環境系)更新
    void PreferencesUpdate(Input& input);
    
    // 設定(PC詳細設定)更新
    void AdvancedSettingUpdate(Input& input);

    // 設定(環境系)描画
    void PreferencesDraw(void);

    // 設定(PC詳細設定)描画
    void AdvancedSettingDraw(void);

    // メニューの初期化
    void InitMenuName(std::wstring menuListIndex, std::map<std::wstring, std::function<void(Input&)>> table, Input& input);

    // ウインドウサイズ切り替え
    using WindowSize_t = std::function<void(Input&)>;
    
    // ウィンドウサイズ設定のメニューリスト
    std::vector<std::wstring> windowSizeList_;

    // ウィンドウサイズのテーブル
    std::map<std::wstring, WindowSize_t> windowSizeTable_;
};

