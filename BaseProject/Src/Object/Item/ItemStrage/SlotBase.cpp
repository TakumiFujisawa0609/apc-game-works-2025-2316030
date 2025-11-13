#include "../Application.h"
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

	//DrawFormatString(10, 50, GetColor(255, 255, 255), L"Current: %ls", itemName.c_str());

	// アプリケーションのウィンドウサイズを取得
	auto size = Application::GetInstance().GetWindowSize();

	int numSlots = static_cast<int>(slots_.size());
	//if (numSlots == 0)
	//{
	//	// アイテムがない場合は何もしない
	//	DrawFormatString(10, 50, GetColor(255, 255, 255), L"Current: None");
	//	return;
	//}

	// スロット全体の幅を計算
	int totalWidth = numSlots * SLOT_WIDTH + (numSlots - 1) * SLOT_PADDING;

	// 描画開始X座標 (画面中央に配置)
	int startX = (size.width - totalWidth) / 2;
	// 描画Y座標 (画面下部から少し上に配置)
	int startY = size.height - SLOT_HEIGHT - 30;

	// --- 1. スロットとアイコンの描画 ---
	for (int i = 0; i < numSlots; ++i)
	{
		int drawX = startX + i * (SLOT_WIDTH + SLOT_PADDING);
		int drawY = startY;

		// アイテム
		auto& item = slots_[i];

		// --- 1-1. 背景ボックスの描画 ---
		// 半透明の暗い背景 (サバイバルゲームのHUD風)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
		DrawBox(drawX, drawY, drawX + SLOT_WIDTH, drawY + SLOT_HEIGHT, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// --- 1-2. アイコンの描画 (ここではモデル表示を省略し、仮の色で代替) ---
		// 実際にはここでアイテムのアイコン画像(item->GetIconHandle())を描画します
		int iconColor = GetColor(100, 100, 100); // 仮のアイコン色
		if (std::dynamic_pointer_cast<Battery>(item)) iconColor = GetColor(0, 255, 0);
		else if (std::dynamic_pointer_cast<HandLight>(item)) iconColor = GetColor(255, 255, 0);
		else if (std::dynamic_pointer_cast<Hemostatic>(item)) iconColor = GetColor(255, 0, 0);

		// アイテムが無効な場合は薄暗くする
		if (item->IsDisabledItem()) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		}

		// 仮のアイコンを描画 (内側の四角)
		DrawBox(drawX + 4, drawY + 4, drawX + SLOT_WIDTH - 4, drawY + SLOT_HEIGHT - 4, iconColor, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


		// --- 1-3. 選択中の強調表示 ---
		if (i == currentSelectedIndex_)
		{
			// 選択中のスロットを白い枠で強調
			int highlightColor = GetColor(255, 255, 255);
			int highlightThickness = 3;

			// 外側の枠を描画 (GetColor(255, 255, 255) は白)
			DrawBox(drawX, drawY, drawX + SLOT_WIDTH, drawY + highlightThickness, highlightColor, TRUE); // 上
			DrawBox(drawX, drawY + SLOT_HEIGHT - highlightThickness, drawX + SLOT_WIDTH, drawY + SLOT_HEIGHT, highlightColor, TRUE); // 下
			DrawBox(drawX, drawY, drawX + highlightThickness, drawY + SLOT_HEIGHT, highlightColor, TRUE); // 左
			DrawBox(drawX + SLOT_WIDTH - highlightThickness, drawY, drawX + SLOT_WIDTH, drawY + SLOT_HEIGHT, highlightColor, TRUE); // 右
		}

		// --- 1-4. スタック数/残数表示 (ここでは実装省略) ---
		// if (item->GetStackCount() > 1) { ... }
	}

	// --- 2. 選択アイテムのテキスト情報の表示 ---
	auto currentItem = GetCurrentItem();
	if (currentItem)
	{
		std::wstring itemName = GetCurrentItemType();

		// 選択スロットの真下に情報を表示
		int textDrawX = startX + currentSelectedIndex_ * (SLOT_WIDTH + SLOT_PADDING);
		int textDrawY = startY + SLOT_HEIGHT + 5; // スロットの下に配置

		// アイテム名を表示
		DrawFormatString(textDrawX, textDrawY, GetColor(255, 255, 255), L"%ls", itemName.c_str());

		// 状態表示 (例: HandLightの残量、ON/OFFなど)
		if (auto light = std::dynamic_pointer_cast<HandLight>(currentItem))
		{
			// HandLightのDrawUI()で残量ゲージが表示されるため、ここではON/OFFステータスを表示
			// DrawFormatString(textDrawX, textDrawY + FONT_SIZE + 2, GetColor(0, 255, 0), light->IsDisabled() ? L"OFF" : L"ON");
		}
	}
}

void SlotBase::UseCurrentItem(void)
{
	auto currentItem = GetCurrentItem();
	if (!currentItem)return;

	if (auto battery = std::dynamic_pointer_cast<Battery>(currentItem))
	{
		battery->Use();
	}
	if (auto hemostatic = std::dynamic_pointer_cast<Hemostatic>(currentItem))
	{
		hemostatic->Use();
	}
	if (auto tranquilizer = std::dynamic_pointer_cast<Tranquilizer>(currentItem))
	{
		tranquilizer->Use();
	}

	if (currentItem->IsDisabledItem())
	{
		RemoveItem(currentItem);
	}
}

std::shared_ptr<ItemBase> SlotBase::GetCurrentItem(void) const
{
	if (slots_.empty())
	{
		return nullptr;
	}

	// 現在のインデックスが有効な範囲内かをチェック
	if (currentSelectedIndex_ >= 0 && currentSelectedIndex_ < static_cast<int>(slots_.size()))
	{
		return slots_[currentSelectedIndex_];
	}
	return nullptr;
}

bool SlotBase::SelectItem(int index)
{
	if (index >= 0 && index < slots_.size())
	{
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
	if (slots_.empty())
	{
		currentSelectedIndex_ = -1;
		return;
	}

	if (currentSelectedIndex_ != -1)
	{
		if (auto prevItemBase = std::dynamic_pointer_cast<ItemBase>(slots_[currentSelectedIndex_]))
		{
			// 以前のアイテムを ININVENTORY 状態に戻す
			prevItemBase->ChangeState(ItemBase::STATE::ININVENTORY);
			prevItemBase->ChangeUse(ItemBase::USE::NONE);
		}
	}

	int size = static_cast<int>(slots_.size());
	int newIndex = currentSelectedIndex_ + direction;
	if (newIndex < 0)
	{
		newIndex = size - 1; // 最後のアイテムに戻る
	}
	else if (newIndex >= size)
	{
		newIndex = 0; // 最初のアイテムに戻る
	}
	currentSelectedIndex_ = newIndex;


	if (currentSelectedIndex_ != -1)
	{
		if (auto newItemBase = std::dynamic_pointer_cast<ItemBase>(slots_[currentSelectedIndex_]))
		{
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

	if (it != slots_.end())
	{
		// 削除する要素のインデックスを計算
		indexToRemove = static_cast<int>(std::distance(slots_.begin(), it));

		// 要素をリストから削除
		slots_.erase(it);

		// 現在選択されているインデックスを更新
		if (slots_.empty())
		{
			currentSelectedIndex_ = -1;
		}
		else if (indexToRemove == currentSelectedIndex_)
		{
			// currentSelectedIndex_ を新しいリストのサイズ内で循環させる
			int newIndex = currentSelectedIndex_;
			if (newIndex >= static_cast<int>(slots_.size())) // 削除により currentSelectedIndex_ が新しいサイズを超えた場合
			{
				// 末尾の要素を削除した場合、新しい末尾を選択 (サイズが1以上の場合)
				newIndex = static_cast<int>(slots_.size() - 1);
			}

			currentSelectedIndex_ = newIndex;

			// 選択が新しいインデックスに移ったため、新しいアイテムの状態をINUSEにする
			if (currentSelectedIndex_ != -1)
			{
				slots_[currentSelectedIndex_]->ChangeState(ItemBase::STATE::ININVENTORY);
				slots_[currentSelectedIndex_]->ChangeUse(ItemBase::USE::INUSE);
			}
		}
		else if (indexToRemove < currentSelectedIndex_)
		{
			// 削除されたアイテムが現在選択されているアイテムより前にある場合、インデックスを一つ減らす
			currentSelectedIndex_--;
		}

	}
}

std::wstring SlotBase::GetCurrentItemType(void) const
{
	auto currentItem = GetCurrentItem();
	if (!currentItem)
	{
		return L"None";
	}

	// 恒久アイテム
	if (std::dynamic_pointer_cast<HandLight>(currentItem))
	{
		return L"HandLight";
	}
	if (std::dynamic_pointer_cast<Lockpick>(currentItem))
	{
		return L"Lockpick";
	}
	// 消費アイテム
	if (std::dynamic_pointer_cast<Battery>(currentItem))
	{
		return L"Battery";
	}
	if (std::dynamic_pointer_cast<Hemostatic>(currentItem))
	{
		return L"HP回復";
	}
	if (std::dynamic_pointer_cast<Tranquilizer>(currentItem))
	{
		return L"正気度回復";
	}
	// その他
	if (std::dynamic_pointer_cast<Wire>(currentItem))
	{
		return L"Wire";
	}

	return L"UnknownItem";
}
