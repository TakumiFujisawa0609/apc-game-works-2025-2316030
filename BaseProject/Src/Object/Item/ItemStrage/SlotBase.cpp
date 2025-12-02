#include "../Application.h"
#include "../Manager/Config.h"
#include "../PermanentItems/PermanentItemBase.h"
#include "../ComsumableItems/Battery.h"
#include "../PermanentItems/HandLight.h"
#include "../PermanentItems/Lockpick.h"
#include "../ComsumableItems/Battery.h"
#include "../ComsumableItems/Hemostatic.h"
#include "../ComsumableItems/Tranquilizer.h"
#include "../Wire.h"
#include "SlotBase.h"

// 定数定義 (SlotBase.cpp または .h に含める)
namespace {
	constexpr int SLOT_WIDTH = 64;
	constexpr int SLOT_HEIGHT = 64;
	constexpr int SLOT_PADDING = 8;
	constexpr int FONT_SIZE = 16;
}

SlotBase::SlotBase(void)
	:
	currentSelectedIndex_(-1)
{
}

SlotBase::~SlotBase(void)
{
}

void SlotBase::Update(float deltaTime)
{
}

void SlotBase::Draw(void)
{
	std::wstring itemName = GetCurrentItemType();
	auto& size = Config::GetInstance().GetWindowSize();

	// --- 調整に使用する定数 ---
	const float SCALE = 0.6f; // サイズ縮小率 (3/5)

	// 新しいスロットとパディングの幅
	const float SLOT_WIDTH_RATIO = 0.10f * SCALE;         // 0.0600f
	const float SLOT_PADDING_RATIO = 0.0125f * SCALE;     // 0.0075f
	const float SLOT_TOTAL_STEP = SLOT_WIDTH_RATIO + SLOT_PADDING_RATIO; // 0.0675f

	// 新しいスロットの高さ
	const float SLOT_HEIGHT_RATIO = 0.175f * SCALE;       // 0.1050f

	// 新しいアイコンの縦横比率 (W * 0.0875 * SCALE と H * 0.15556 * SCALE はほぼ等しい)
	const float ICON_WIDTH_RATIO = 0.0875f * SCALE;       // 0.0525f
	const float ICON_HEIGHT_RATIO = 0.15556f * SCALE;     // 0.093336f

	// アイコンのオフセット
	const float ICON_OFFSET_X_START = 0.00625f * SCALE;   // 0.00375f
	const float ICON_OFFSET_Y_START = 0.00833f * SCALE;   // 0.004998f

	// 枠線の太さ (絶対値または相対値で調整)
	const float HIGHLIGHT_THICKNESS_W = 0.003125f * SCALE; // 0.001875f
	const float HIGHLIGHT_THICKNESS_H = 0.00416f * SCALE;  // 0.002496f


	int numSlots = static_cast<int>(slots_.size());

	// スロット全体の幅を計算
	int totalWidth = static_cast<int>(numSlots * size.width_ * SLOT_WIDTH_RATIO + (numSlots - 1) * size.width_ * SLOT_PADDING_RATIO);

	// 描画開始X座標 (画面中央に配置)
	int startX = (size.width_ - totalWidth) / 2;

	// 描画Y座標 (画面下部から少し上だったものを、画面下部から少し離れた位置に変更し、全体的に上へ移動)
	// 元の配置基準Y座標 0.19583f から、例えば 0.35f (画面下部から35%の位置) に変更して全体を上へ移動
	// または、画面下部から固定ピクセルを引くなど、調整が必要です。ここでは比率を大きめにして上へ移動させます
	int startY = static_cast<int>(size.height_ - size.height_ * 0.23f);

	// --- 1. スロットとアイコンの描画 ---
	for (int i = 0; i < numSlots; ++i){
		// 描画X座標 (SLOT_WIDTH + SLOT_PADDING) に合わせて修正
		int drawX = static_cast<int>(startX + i * size.width_ * SLOT_TOTAL_STEP);
		int drawY = startY;

		// アイテム
		auto& item = slots_[i];

		// --- 1-1. 背景ボックスの描画 ---
		// 半透明の暗い背景
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
		DrawBox(drawX, drawY,
			static_cast<int>(drawX + size.width_ * SLOT_WIDTH_RATIO),
			static_cast<int>(drawY + size.height_ * SLOT_HEIGHT_RATIO),
			GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// アイコンの描画
		int iconColor = GetColor(100, 100, 100);
		if (std::dynamic_pointer_cast<Battery>(item)) iconColor = GetColor(0, 255, 0);
		else if (std::dynamic_pointer_cast<HandLight>(item)) iconColor = GetColor(255, 255, 0);
		else if (std::dynamic_pointer_cast<Hemostatic>(item)) iconColor = GetColor(255, 0, 0);

		// アイテムが無効な場合は薄暗くする
		auto currentItem = GetCurrentItem();
		auto perItem = std::dynamic_pointer_cast<PermanentItemBase>(currentItem);
		if (item->IsDisabledItem()) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		}

		// 仮のアイコンを描画 (内側の四角)
		DrawBox(static_cast<int>(drawX + size.width_ * ICON_OFFSET_X_START),
			static_cast<int>(drawY + size.height_ * ICON_OFFSET_Y_START),
			static_cast<int>(drawX + size.width_ * (ICON_OFFSET_X_START + ICON_WIDTH_RATIO)),
			static_cast<int>(drawY + size.height_ * (ICON_OFFSET_Y_START + ICON_HEIGHT_RATIO)),
			iconColor, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


		// --- 1-3. 選択中の強調表示 ---
		if (i == currentSelectedIndex_){
			// 選択中のスロットを白い枠で強調
			int highlightColor = GetColor(255, 255, 255);

			// 外側の枠を描画 (DrawBoxのサイズも SCALE 倍に)
			// 上
			DrawBox(drawX, drawY, static_cast<int>(drawX + size.width_ * SLOT_WIDTH_RATIO), static_cast<int>(drawY + size.height_ * HIGHLIGHT_THICKNESS_H), highlightColor, TRUE);
			// 下
			DrawBox(drawX, static_cast<int>(drawY + size.height_ * (SLOT_HEIGHT_RATIO - HIGHLIGHT_THICKNESS_H)), static_cast<int>(drawX + size.width_ * SLOT_WIDTH_RATIO), static_cast<int>(drawY + size.height_ * SLOT_HEIGHT_RATIO), highlightColor, TRUE);
			// 左
			DrawBox(drawX, drawY, static_cast<int>(drawX + size.width_ * HIGHLIGHT_THICKNESS_W), static_cast<int>(drawY + size.height_ * SLOT_HEIGHT_RATIO), highlightColor, TRUE);
			// 右
			DrawBox(static_cast<int>(drawX + size.width_ * (SLOT_WIDTH_RATIO - HIGHLIGHT_THICKNESS_W)), drawY, static_cast<int>(drawX + size.width_ * SLOT_WIDTH_RATIO), static_cast<int>(drawY + size.height_ * SLOT_HEIGHT_RATIO), highlightColor, TRUE);
		}
	}

	// --- 2. 選択アイテムのテキスト情報の表示 ---
	auto currentItem = GetCurrentItem();
	if (currentItem){
		std::wstring itemName = GetCurrentItemType();

		// 選択スロットの真下に情報を表示
		int textDrawX = static_cast<int>(startX + currentSelectedIndex_ * size.width_ * SLOT_TOTAL_STEP);
		// スロットの縦幅の変更に伴い、テキスト描画Y座標を調整
		int textDrawY = static_cast<int>(startY + size.height_ * (SLOT_HEIGHT_RATIO + 0.01f));

		// アイテム名を表示 (フォントサイズも縮小する場合は、別途関数で調整が必要です)
		DrawFormatString(textDrawX, textDrawY, GetColor(255, 255, 255), L"%ls", itemName.c_str());
	}
}

void SlotBase::UseCurrentItem(void)
{
	auto currentItem = GetCurrentItem();
	if (!currentItem)return;

	if (auto battery = std::dynamic_pointer_cast<Battery>(currentItem)){
		battery->Use();
	}
	if (auto hemostatic = std::dynamic_pointer_cast<Hemostatic>(currentItem)){
		hemostatic->Use();
	}
	if (auto tranquilizer = std::dynamic_pointer_cast<Tranquilizer>(currentItem)){
		tranquilizer->Use();
	}

	if (currentItem->IsDisabledItem()){
		RemoveItem(currentItem);
	}
}

std::shared_ptr<ItemBase> SlotBase::GetCurrentItem(void) const
{
	if (slots_.empty()){
		return nullptr;
	}

	// 現在のインデックスが有効な範囲内かをチェック
	if (currentSelectedIndex_ >= 0 && currentSelectedIndex_ < static_cast<int>(slots_.size())){
		return slots_[currentSelectedIndex_];
	}
	return nullptr;
}

bool SlotBase::SelectItem(int index)
{
	if (index >= 0 && index < slots_.size()){
		currentSelectedIndex_ = index;
		return true;
	}
	return false;
}

void SlotBase::CycleByWheel(bool scrollUp)
{
	int direction = scrollUp ? -1 : 1;
	UpdateIndex(direction);
}

int SlotBase::GetCurrentSelectedIndex(void)
{
	return currentSelectedIndex_;
}

void SlotBase::UpdateIndex(int direction)
{
	if (slots_.empty()){
		currentSelectedIndex_ = -1;
		return;
	}

	if (currentSelectedIndex_ != -1){
		if (auto prevItemBase = std::dynamic_pointer_cast<ItemBase>(slots_[currentSelectedIndex_])){
			// 以前のアイテムを ININVENTORY 状態に戻す
			prevItemBase->ChangeState(ItemBase::STATE::ININVENTORY);
			prevItemBase->ChangeUse(ItemBase::USE::NONE);
		}
	}

	int size = static_cast<int>(slots_.size());
	int newIndex = currentSelectedIndex_ + direction;
	if (newIndex < 0){
		// 最後のアイテムに戻る
		newIndex = size - 1; 
	}
	else if (newIndex >= size){
		// 最初のアイテムに戻る
		newIndex = 0; 
	}
	currentSelectedIndex_ = newIndex;


	if (currentSelectedIndex_ != -1){
		if (auto newItemBase = std::dynamic_pointer_cast<ItemBase>(slots_[currentSelectedIndex_])){
			// 新しいアイテムを ININVENTORY 状態にする
			newItemBase->ChangeState(ItemBase::STATE::ININVENTORY);
			newItemBase->ChangeUse(ItemBase::USE::INUSE);
		}
	}
}

void SlotBase::RemoveItem(const std::shared_ptr<ItemBase>& item)
{
	int indexToRemove = -1;

	// 削除するアイテムと同じポインタを持つ要素を検索
	auto it = std::find_if(slots_.begin(), slots_.end(),
		[&item](const std::shared_ptr<ItemBase>& slotItem)
		{
			return slotItem == item;
		});

	if (it != slots_.end()){
		// 削除する要素のインデックスを計算
		indexToRemove = static_cast<int>(std::distance(slots_.begin(), it));

		// 要素をリストから削除
		slots_.erase(it);

		// 現在選択されているインデックスを更新
		if (slots_.empty()){
			currentSelectedIndex_ = -1;
		}
		else if (indexToRemove == currentSelectedIndex_){
			// currentSelectedIndex_ を新しいリストのサイズ内で循環させる
			int newIndex = currentSelectedIndex_;

			// 削除により currentSelectedIndex_ が新しいサイズを超えた場合
			if (newIndex >= static_cast<int>(slots_.size())){
				// 末尾の要素を削除した場合、新しい末尾を選択 (サイズが1以上の場合)
				newIndex = static_cast<int>(slots_.size() - 1);
			}

			currentSelectedIndex_ = newIndex;

			// 選択が新しいインデックスに移ったため、新しいアイテムの状態をINUSEにする
			if (currentSelectedIndex_ != -1){
				slots_[currentSelectedIndex_]->ChangeState(ItemBase::STATE::ININVENTORY);
				slots_[currentSelectedIndex_]->ChangeUse(ItemBase::USE::INUSE);
			}
		}
		else if (indexToRemove < currentSelectedIndex_){
			// 削除されたアイテムが現在選択されているアイテムより前にある場合、インデックスを一つ減らす
			currentSelectedIndex_--;
		}
	}
}

std::wstring SlotBase::GetCurrentItemType(void) const
{
	auto currentItem = GetCurrentItem();
	if (!currentItem){
		return L"None";
	}

	// 恒久アイテム
	if (std::dynamic_pointer_cast<HandLight>(currentItem)){
		return L"HandLight";
	}
	if (std::dynamic_pointer_cast<Lockpick>(currentItem)){
		return L"Lockpick";
	}
	// 消費アイテム
	if (std::dynamic_pointer_cast<Battery>(currentItem)){
		return L"Battery";
	}
	if (std::dynamic_pointer_cast<Hemostatic>(currentItem)){
		return L"HP回復";
	}
	if (std::dynamic_pointer_cast<Tranquilizer>(currentItem)){
		return L"正気度回復";
	}
	// その他
	if (std::dynamic_pointer_cast<Wire>(currentItem)){
		return L"Wire";
	}
	return L"UnknownItem";
}
