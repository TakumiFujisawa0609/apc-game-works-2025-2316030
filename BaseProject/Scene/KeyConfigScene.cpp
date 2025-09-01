#include<DxLib.h>
#include "KeyConfigScene.h"
#include"../Application.h"
#include"SceneController.h"
#include"../Input.h"
#include"../StringUtiligy.h"

#include<algorithm>

namespace {
	constexpr int appear_interval = 20;//出現までのフレーム
	constexpr int input_list_row_height = 40;//イベントの１つあたりの高さ
	constexpr int input_list_col_width = 150;//割り当てコード列に要する幅
	constexpr int margin_size = 20;//ポーズメニュー枠の余白
	constexpr int input_list_top = 100;//入力情報の上端
	constexpr int input_list_left = 70;//入力情報の左端
	constexpr int analog_lever_threshold = 10000;//アナログレバーを倒した閾値
	constexpr int analog_trigger_threshold = 128;//アナログトリガーの閾値
}

void KeyConfigScene::DrawInputList()
{
	int rowY = input_list_top;//最初のY座標
	constexpr uint32_t normal_text_color = 0x000000;//文字の通常カラー
	constexpr uint32_t selected_text_color = 0x0000aa;//現在選択中の文字カラー
	constexpr uint32_t editting_text_color = 0xcc0000;//現在編集中の文字カラー
	constexpr uint32_t indicator_offset = 20;//矢印のオフセット
	
	for (const auto& keyStr : input_.inputListForDisplay_) {
		auto eventColor = normal_text_color;
		auto eventName = StringUtiligy::GetWStringFromString(keyStr);//イベント名	
		int colX = input_list_left;
		//現在のインデックスがイベントをさしていれば、その行の色を選択中に変更する
		if (currentIndex_ < input_.inputListForDisplay_.size()) {
			if (input_.inputListForDisplay_[currentIndex_] == keyStr) {
				DrawString(input_list_left - indicator_offset + 1, rowY + 1, L"⇒", 0x000000);
				DrawString(input_list_left - indicator_offset, rowY, L"⇒", 0xaa0000, 0xfffffff);
				colX += 10;
				//ただし、現在が編集モードに入っていれば、また別の色にする
				if (update_ == &KeyConfigScene::EdittingUpdate) {
					eventColor = editting_text_color;
					colX += 10;
				}
				else {
					eventColor = selected_text_color;
				}
			}
		}
		DrawFormatString(colX, rowY, eventColor, L"%s", eventName.c_str());
		colX += 70;
		
		for (auto& record : tempInputTable_[keyStr]) {
			std::wstring periName = GetPeriphString(record.type);//周辺機器名
			if (record.type == PeripheralType::keyboard) {
				DrawFormatString(colX, rowY, eventColor,
					L"%s:%s", periName.c_str(), keyboardNameTable_[record.code].c_str());
			}
			else if (record.type == PeripheralType::gamepad) {
				DrawFormatString(colX, rowY, eventColor,
					L"%s:%s", periName.c_str(), padNameTable_[record.code].c_str());
			}
			else if (record.type == PeripheralType::mouse) {
				DrawFormatString(colX, rowY, eventColor,
					L"%s:%x", periName.c_str(), record.code);
			}
			else if (record.type == PeripheralType::x_analog) {
				DrawFormatString(colX, rowY, eventColor,
					L"%s:%s", periName.c_str(), analogNameTable_[ record.code].c_str());
			}
			colX += input_list_col_width;
		}
		rowY += input_list_row_height;
	}

	rowY += 30;//システムメニューはキーコンフィグ部分からちょっと離す

	//キーコンフィグシステムメニューを表示
	for (const auto& menuName : systemMenuStringList_) {
		uint32_t texCol = normal_text_color;
		int colX = input_list_left + 100;
		int idx = currentIndex_ - input_.inputListForDisplay_.size();
		if (idx >= 0) {
			if (systemMenuStringList_[idx]==menuName) {
				DrawString(input_list_left - indicator_offset + 100 + 1, rowY + 1, L"⇒", 0x000000);
				DrawString(input_list_left - indicator_offset + 100, rowY, L"⇒", 0xaa0000, 0xfffffff);
				colX += 10;
				texCol = selected_text_color;
			}
		}
		DrawString(colX, rowY, menuName.c_str(), texCol);
		rowY += input_list_row_height;
	}
	
}

std::wstring 
KeyConfigScene::GetPeriphString(const PeripheralType& type)
{
	switch (type) {
	case PeripheralType::keyboard:
		return L"keybd";
	case PeripheralType::gamepad:
		return L"pad";
	case PeripheralType::mouse:
		return L"mouse";
	case PeripheralType::x_analog:
		return L"x_analog";
	default:
		return L"";
	}
}

void KeyConfigScene::NormalUpdate(Input& input)
{
	const auto keyEventSize = input.inputListForDisplay_.size();
	const auto inputRowSize = keyEventSize + systemMenuStringList_.size();//システムメニューを足す
	if (input.IsTriggered("ok")) {
		if (currentIndex_ < keyEventSize) {
			GetHitKeyStateAll(lastKeyState_.data());
			lastPadState_= GetJoypadInputState(DX_INPUT_PAD1);
			update_ = &KeyConfigScene::EdittingUpdate;//編集モード
			return;
		}
		else {
			const auto& menuStr = systemMenuStringList_[currentIndex_ - keyEventSize];
			systemMenuTable_[menuStr]();
		}
	}
	
	if (input.IsTriggered("up")) {
		currentIndex_ = (currentIndex_ + inputRowSize - 1) % inputRowSize;
	}
	if (input.IsTriggered("down")) {
		currentIndex_ = (currentIndex_ + 1) % inputRowSize;
	}


}

void KeyConfigScene::EdittingUpdate(Input& input)
{
	if (input.IsTriggered("ok")) {
		update_ = &KeyConfigScene::NormalUpdate;
		currentKeyState_ = {};
		lastKeyState_ = {};
		currentPadState_ = 0;
		lastPadState_ = 0;
		return;
	}
	GetHitKeyStateAll(currentKeyState_.data());
	int pressedKeyCode = -1;
	for (int i = 0; i < 256; ++i) {
		if (currentKeyState_[i]&&!lastKeyState_[i]) {
			pressedKeyCode = i;
			break;
		}
	}
	currentPadState_ = GetJoypadInputState(DX_INPUT_PAD1);
	int pressedPadCode = 0;
	for (int i = 0; i < 32; ++i) {
		if ((currentPadState_ & (0b1 << i)) != 0 && (lastPadState_ & (0b1 << i)) == 0) {
			pressedPadCode = (0b1 << i);
		}
	}
	//アナログ
	XINPUT_STATE xinputState = {};
	AnalogInputType pressedAnalogCode = AnalogInputType::none;
	GetJoypadXInputState(DX_INPUT_PAD1, &xinputState);
	if (abs(xinputState.ThumbLX) > analog_lever_threshold) {//左レバーチェック
		pressedAnalogCode = xinputState.ThumbLX > 0 ? AnalogInputType::l_right : AnalogInputType::l_left;
	}else if (abs(xinputState.ThumbLY) > analog_lever_threshold) {
		pressedAnalogCode = xinputState.ThumbLY > 0 ? AnalogInputType::l_up : AnalogInputType::l_down;
	}else if (abs(xinputState.ThumbRX) > analog_lever_threshold) {
		pressedAnalogCode = xinputState.ThumbRX > 0 ? AnalogInputType::r_right : AnalogInputType::r_left;
	}else if (abs(xinputState.ThumbRY) > analog_lever_threshold) {
		pressedAnalogCode = xinputState.ThumbRY > 0 ? AnalogInputType::r_up : AnalogInputType::r_down;
	}else if (abs(xinputState.LeftTrigger) > analog_trigger_threshold) {
		pressedAnalogCode = AnalogInputType::l_trigger;
	}else if (abs(xinputState.RightTrigger) > analog_trigger_threshold) {
		pressedAnalogCode = AnalogInputType::r_trigger;
	}

	
	auto& record = tempInputTable_[input.inputListForDisplay_[currentIndex_]];

	
	//for (auto& state : record) {
	for(int i=0;i<(int)PeripheralType::end; ++i) {
		auto it = std::find_if(record.begin(), record.end(), [i](const Input::InputState& state) {
			return PeripheralType(i) == state.type;
			});
		if (it != record.end()) {
			auto& state = *it;
			if (state.type == PeripheralType::keyboard && pressedKeyCode >= 0) {
				state.code = pressedKeyCode;
			}
			else if (state.type == PeripheralType::gamepad && pressedPadCode > 0) {
				state.code = pressedPadCode;
			}
			else if (state.type == PeripheralType::x_analog && pressedAnalogCode != AnalogInputType::none) {
				state.code = (int)pressedAnalogCode;
			}
		}
		else {
			Input::InputState state = {};
			
			if ((PeripheralType)i == PeripheralType::keyboard && pressedKeyCode >= 0) {
				state.code = pressedKeyCode;
			}
			else if ((PeripheralType)i  == PeripheralType::gamepad && pressedPadCode > 0) {
				state.code = pressedPadCode;
			}
			else if ((PeripheralType)i == PeripheralType::x_analog && pressedAnalogCode != AnalogInputType::none) {
				state.code = (int)pressedAnalogCode;
			}
			if (state.code > 0) {
				state.type = (PeripheralType)i;
				record.push_back(state);
			}
			
		}
	}
	lastKeyState_ = currentKeyState_;
	lastPadState_ = currentPadState_;
}

KeyConfigScene::KeyConfigScene(SceneController& controller, Input& input) :
	Scene(controller),
	input_(input),
	update_(&KeyConfigScene::NormalUpdate)
{
	ReloadTable();

	keyboardNameTable_[KEY_INPUT_A] = L"Ａキー";
	keyboardNameTable_[KEY_INPUT_B] = L"Ｂキー";
	keyboardNameTable_[KEY_INPUT_C] = L"Ｃキー";
	keyboardNameTable_[KEY_INPUT_D] = L"Ｄキー";
	keyboardNameTable_[KEY_INPUT_E] = L"Ｅキー";
	keyboardNameTable_[KEY_INPUT_F] = L"Ｆキー";
	keyboardNameTable_[KEY_INPUT_G] = L"Ｇキー";
	keyboardNameTable_[KEY_INPUT_H] = L"Ｈキー";
	keyboardNameTable_[KEY_INPUT_I] = L"Ｉキー";
	keyboardNameTable_[KEY_INPUT_J] = L"Ｊキー";
	keyboardNameTable_[KEY_INPUT_K] = L"Ｋキー";
	keyboardNameTable_[KEY_INPUT_L] = L"Ｌキー";
	keyboardNameTable_[KEY_INPUT_M] = L"Ｍキー";
	keyboardNameTable_[KEY_INPUT_N] = L"Ｎキー";
	keyboardNameTable_[KEY_INPUT_O] = L"Ｏキー";
	keyboardNameTable_[KEY_INPUT_P] = L"Ｐキー";
	keyboardNameTable_[KEY_INPUT_Q] = L"Ｑキー";
	keyboardNameTable_[KEY_INPUT_R] = L"Ｒキー";
	keyboardNameTable_[KEY_INPUT_S] = L"Ｓキー";
	keyboardNameTable_[KEY_INPUT_T] = L"Ｔキー";
	keyboardNameTable_[KEY_INPUT_U] = L"Ｕキー";
	keyboardNameTable_[KEY_INPUT_V] = L"Ｖキー";
	keyboardNameTable_[KEY_INPUT_W] = L"Ｗキー";
	keyboardNameTable_[KEY_INPUT_X] = L"Ｘキー";
	keyboardNameTable_[KEY_INPUT_Y] = L"Ｙキー";
	keyboardNameTable_[KEY_INPUT_Z] = L"Ｚキー";
	keyboardNameTable_[KEY_INPUT_BACK] = L"BSキー";
	keyboardNameTable_[KEY_INPUT_TAB] = L"Tabキー";
	keyboardNameTable_[KEY_INPUT_RETURN] = L"Enterキー";
	keyboardNameTable_[KEY_INPUT_ESCAPE] = L"Escキー";
	keyboardNameTable_[KEY_INPUT_SPACE] = L"スペースキー";
	keyboardNameTable_[KEY_INPUT_F1] = L"Ｆ１キー";
	keyboardNameTable_[KEY_INPUT_F2] = L"Ｆ２キー";
	keyboardNameTable_[KEY_INPUT_F3] = L"Ｆ３キー";
	keyboardNameTable_[KEY_INPUT_F4] = L"Ｆ４キー";
	keyboardNameTable_[KEY_INPUT_F5] = L"Ｆ５キー";

	padNameTable_[PAD_INPUT_A] = L"Ａボタン";
	padNameTable_[PAD_INPUT_B] = L"Ｂボタン";
	padNameTable_[PAD_INPUT_C] = L"Ｘボタン";
	padNameTable_[PAD_INPUT_X] = L"Ｙボタン";
	padNameTable_[PAD_INPUT_Y] = L"Ｌボタン";
	padNameTable_[PAD_INPUT_Z] = L"Ｒボタン";
	padNameTable_[PAD_INPUT_L] = L"セレクトボタン";
	padNameTable_[PAD_INPUT_R] = L"スタートボタン";
	padNameTable_[PAD_INPUT_START] = L"Ｌスティック";
	padNameTable_[PAD_INPUT_M] = L"Ｒスティック";

	analogNameTable_[(int)AnalogInputType::l_up] = L"左レバー上";
	analogNameTable_[(int)AnalogInputType::l_down] = L"左レバー下";
	analogNameTable_[(int)AnalogInputType::l_left] = L"左レバー左";
	analogNameTable_[(int)AnalogInputType::l_right] = L"左レバー右";
	analogNameTable_[(int)AnalogInputType::r_up] = L"右レバー上";
	analogNameTable_[(int)AnalogInputType::r_down] = L"右レバー下";
	analogNameTable_[(int)AnalogInputType::r_left] = L"右レバー左";
	analogNameTable_[(int)AnalogInputType::r_right] = L"右レバー右";
	analogNameTable_[(int)AnalogInputType::l_trigger] = L"左トリガー";
	analogNameTable_[(int)AnalogInputType::r_trigger] = L"右トリガー";
	
	systemMenuTable_[L"SAVE&EXIT"] = [this]() {
		CommitInputTable();
		controller_.PopScene();
	};
	systemMenuTable_[L"CANCEL&EXIT"] = [this]() {
		controller_.PopScene();
	};
	systemMenuTable_[L"RESET"] = [this]() {
		input_.ResetTable();
		ReloadTable();
	};

	systemMenuStringList_ = { L"RESET",L"SAVE&EXIT" ,L"CANCEL&EXIT" };

}

void KeyConfigScene::ReloadTable()
{
	tempInputTable_ = input_.inputTable_;
}

void KeyConfigScene::Update(Input& input)
{
	(this->*update_)(input);

}

void KeyConfigScene::Draw()
{
	const Size& wsize = Application::GetInstance().GetWindowSize();
	//緑っぽいセロファン
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 240);
	DrawBox(margin_size, margin_size,
		wsize.width - margin_size, wsize.height - margin_size,
		0xaaffaa, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//緑
	DrawBoxAA(margin_size, margin_size,
		wsize.width - margin_size, wsize.height - margin_size,
		0x00ff00, false, 3.0f);
	DrawString(margin_size + 10, margin_size + 10, L"キーコンフィグ", 0x000000);
	
	DrawInputList();
}

void KeyConfigScene::CommitInputTable()
{
	input_.inputTable_ = tempInputTable_;
	input_.Save();
}
