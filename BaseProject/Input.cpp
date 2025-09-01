#include "Input.h"
#include<DxLib.h>
#include<cassert>

namespace {
	//ヘッダ部(データ数)
	struct Header {
		char signature[4];
		float version;
		uint32_t dataSize;//4バイト
	};
}

void Input::Save()
{
	FILE* fp = nullptr;
	auto err = fopen_s(&fp, "input.conf", "wb");
	if (fp == nullptr) {
		return;
	}

	//ヘッダ部
	Header header = {};
	header.version = 1.00f;
	header.dataSize= inputTable_.size();
	std::copy_n("kycf", 4, header.signature);
	fwrite(&header, sizeof(header), 1, fp);
	//データ部
	for (const auto& row : inputTable_) {
		//イベント名
		auto eventName = row.first;
		uint8_t nameSize = static_cast<uint8_t>(eventName.size());
		fwrite(&nameSize, sizeof(nameSize), 1, fp);
		fwrite(eventName.data(), sizeof(char), nameSize, fp);
		//データ部
		uint32_t rowSize = row.second.size();
		fwrite(&rowSize, sizeof(rowSize), 1, fp);
		fwrite(row.second.data(), sizeof(InputState), rowSize, fp);
	}

	fclose(fp);
}

void Input::Load()
{
	int handle = FileRead_open(L"input.conf");
	if (handle == 0) {
		return;
	}
	assert(handle != 0);
	//次回は初回起動時の対応を行う
	//ヘッダ部よみこみ
	Header header = {};
	FileRead_read(&header, sizeof(header), handle);
	//データ部
	for (int i = 0; i < header.dataSize;++i) {
		//イベント名
		uint8_t nameSize = 0;
		FileRead_read(&nameSize, sizeof(nameSize),handle);
		std::string eventName;
		eventName.resize(nameSize);
		FileRead_read(eventName.data(), eventName.size(),handle);
		//データ部
		uint32_t rowSize = 0;
		FileRead_read(&rowSize, sizeof(rowSize), handle);
		std::vector<InputState> rowData(rowSize);
		FileRead_read(rowData.data(), sizeof(InputState)*rowSize,handle);
		inputTable_[eventName] = rowData;
	}
	FileRead_close(handle);
}

Input::Input()
{
	ResetTable();
	Load();
	inputListForDisplay_ = {
		"ok","pause","jump","shot"
	};

	analogInputTable_[AnalogInputType::l_up] = [](const XINPUT_STATE& state) {
		return state.ThumbLY > 10000;
	};
	analogInputTable_[AnalogInputType::l_down] = [](const XINPUT_STATE& state) {
		return state.ThumbLY < -10000;
	};
	analogInputTable_[AnalogInputType::l_right] = [](const XINPUT_STATE& state) {
		return state.ThumbLX > 10000;
	};
	analogInputTable_[AnalogInputType::l_left] = [](const XINPUT_STATE& state) {
		return state.ThumbLX < -10000;
	};
	analogInputTable_[AnalogInputType::r_up] = [](const XINPUT_STATE& state) {
		return state.ThumbRY > 10000;
	};
	analogInputTable_[AnalogInputType::r_down] = [](const XINPUT_STATE& state) {
		return state.ThumbRY < -10000;
	};
	analogInputTable_[AnalogInputType::r_right] = [](const XINPUT_STATE& state) {
		return state.ThumbRX > 10000;
	};
	analogInputTable_[AnalogInputType::r_left] = [](const XINPUT_STATE& state) {
		return state.ThumbRX < -10000;
	};
	analogInputTable_[AnalogInputType::l_trigger] = [](const XINPUT_STATE& state) {
		return state.LeftTrigger > 128;
	};
	analogInputTable_[AnalogInputType::r_trigger] = [](const XINPUT_STATE& state) {
		return state.RightTrigger > 128;
	};

	for (const auto& keyvalue : inputTable_) {
		currentInput_[keyvalue.first] = false;
	}
	lastInput_ = currentInput_;
}

void Input::ResetTable()
{
	inputTable_ = {
		{ "ok",{
			{ PeripheralType::keyboard, KEY_INPUT_RETURN },
				{ PeripheralType::gamepad,PAD_INPUT_A },//Aボタン
				//{ PeripheralType::mouse,MOUSE_INPUT_LEFT }
		}//左クリック
		},
		{ "pause",{
			{ PeripheralType::keyboard, KEY_INPUT_P },
		{ PeripheralType::gamepad,PAD_INPUT_R } }//STARTボタン
	},
	};
	inputTable_["jump"] = { { PeripheralType::keyboard, KEY_INPUT_Z },
		{ PeripheralType::gamepad, PAD_INPUT_B },
		{PeripheralType::x_analog,(int)AnalogInputType::l_trigger} };

	inputTable_["shot"] = { { PeripheralType::keyboard, KEY_INPUT_X },
		{ PeripheralType::gamepad, PAD_INPUT_Y } ,
		{PeripheralType::x_analog,(int)AnalogInputType::r_trigger} };


	inputTable_["up"] = { { PeripheralType::keyboard, KEY_INPUT_UP },
		{ PeripheralType::gamepad, PAD_INPUT_UP },
		{PeripheralType::x_analog,(int)AnalogInputType::l_up} };
	inputTable_["down"] = { { PeripheralType::keyboard, KEY_INPUT_DOWN },
		{ PeripheralType::gamepad, PAD_INPUT_DOWN } ,
		{PeripheralType::x_analog,(int)AnalogInputType::l_down} };
	inputTable_["right"] = { { PeripheralType::keyboard, KEY_INPUT_RIGHT },
		{ PeripheralType::gamepad, PAD_INPUT_RIGHT },
		{PeripheralType::x_analog,(int)AnalogInputType::l_right} };
	inputTable_["left"] = { { PeripheralType::keyboard, KEY_INPUT_LEFT},
		{ PeripheralType::gamepad, PAD_INPUT_LEFT} ,
		{PeripheralType::x_analog,(int)AnalogInputType::l_left} };
	

}

bool Input::IsTriggered(const std::string& eventcode) const
{
#ifdef NDEBUG
	if (!currentInput_.contains(eventcode)) {
		return false;
	}
#endif
	return currentInput_.at(eventcode) && !lastInput_.at(eventcode);
}

bool Input::IsPressed(const std::string& eventcode) const
{
	return currentInput_.at(eventcode);
}

void Input::Update()
{	
	//押したか押してないか記録する部分
	lastInput_ = currentInput_;//前のプッシュ情報を記憶

	//キーボード情報
	char keyState[256] = {};
	GetHitKeyStateAll(keyState);
	//パッド情報
	int padState = GetJoypadInputState(DX_INPUT_PAD1);
	//マウス情報
	int mouseState = GetMouseInput();
	//アナログ入力情報
	XINPUT_STATE xinputState = {};
	GetJoypadXInputState(DX_INPUT_PAD1, &xinputState);
	
	for (const auto& keyvalue : inputTable_) {//テーブルの行を回す
		for (auto input : keyvalue.second) {//特定のキーの入力情報を取得
			bool pressed = false;//押してない状態で初期化
			if (input.type == PeripheralType::keyboard) {
				pressed = keyState[input.code];
			}
			else if (input.type == PeripheralType::gamepad) {
				pressed = padState & input.code;
			}
			else if (input.type == PeripheralType::mouse) {
				pressed = mouseState & input.code;
			}
			else if (input.type == PeripheralType::x_analog) {
				pressed = analogInputTable_[(AnalogInputType)input.code](xinputState);
			}
			currentInput_[keyvalue.first] = pressed;
			if (pressed) {
				break;
			}
		}
	}
}
