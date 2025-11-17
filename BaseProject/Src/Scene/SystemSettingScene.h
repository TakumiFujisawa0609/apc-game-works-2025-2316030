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

    Config& config_;

    int frame_;

    int mainMenuIndex_;
    int prefMenuIndex_;
    int asMenuIndex_;

    bool isFullS_;	// フルスクリーンであるかどうか
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

    UpdateFunc_t update_;//Update用のメンバ関数ポインタ
    DrawFunc_t draw_;//Draw用のメンバ関数ポインタ

    void PreferencesUpdate(Input& input);    //設定(環境系)
    void AdvancedSettingUpdate(Input& input);//設定(PC詳細設定)

    void PreferencesDraw(void);
    void AdvancedSettingDraw(void);

    void InitMenuName(std::wstring menuListIndex, std::map<std::wstring, std::function<void(Input&)>> table, Input& input);
};

