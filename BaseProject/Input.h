#pragma once

//#include<map>//対応表のため
#include<unordered_map>
#include<string>//対応キーのため
#include<vector>//デバイスを追加するため
#include<functional>
#include<DxLib.h>

/// <summary>
/// 周辺機器種別
/// </summary>
enum class PeripheralType {
	keyboard,//キーボード
	gamepad,//パッド
	mouse,//マウス
	x_analog,//Xboxコントローラのアナログ入力
	end,//for文で回すとき用に定義
};

/// <summary>
/// アナログ入力種別
/// </summary>
enum class AnalogInputType {
	none,//押してない
	l_up,//左スティックの上
	l_down,//左スティックの下
	l_left,//左スティックの左
	l_right,//左スティックの右
	r_up,//左スティックの上
	r_down,//左スティックの下
	r_left,//左スティックの左
	r_right,//左スティックの右
	l_trigger,//左トリガー
	r_trigger,//右トリガー
};
class KeyConfigScene;
class Input
{
	friend KeyConfigScene;//privateもいじっていいよ
private:
	struct InputState {
		PeripheralType type;//周辺機器種別
		uint32_t code;//入力コード(汎用)
	};
	using InputTable_t = std::unordered_map < std::string, std::vector<InputState>>;
	InputTable_t inputTable_;//イベントと実際の入力の対応表

	using InputData_t = std::unordered_map < std::string, bool>;
	InputData_t currentInput_;//そのイベントに対応するボタンが押されてる状態か
	InputData_t lastInput_;//そのイベントに対応するボタンが押されてる状態か(直前)

	std::vector<std::string> inputListForDisplay_;

	using AnalogInputTable_t = 
		std::unordered_map<AnalogInputType, std::function<bool(const XINPUT_STATE&)>>;
	AnalogInputTable_t analogInputTable_;

	/// <summary>
	/// 入力テーブルのセーブ
	/// </summary>
	void Save();
	/// <summary>
	/// 入力テーブルのロード
	/// </summary>
	void Load();

public:
	Input();
	void ResetTable();
	/// <summary>
	/// 指定されたコードが押された瞬間か
	/// </summary>
	/// <param name="eventcode">対象のイベントコード</param>
	/// <returns>押した瞬間：true / false 押されてないor押しっパ</returns>
	bool IsTriggered(const std::string& eventcode)const;
	bool IsPressed(const std::string& eventcode)const;
	void Update();//入力状態を更新する
};

