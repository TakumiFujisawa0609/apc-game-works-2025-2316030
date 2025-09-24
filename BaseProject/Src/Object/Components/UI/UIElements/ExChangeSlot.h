#pragma once
#include <functional>
#include "../UIElement.h"
class ExChangeSlot :
    public UIElement
{
public:
    ExChangeSlot(int front, int back);

    void Update(float deltaTime)override;
    void Draw(void);

    // 表示を切り替える
    void Toggle(void);

    // 使用可能なスロットのアイテムIDを返す
    int GetActiveItemId(void) const;

    // 外部へ切り替え通知を行うためのハンドラ登録
    void SetToggleCallback(std::function<void(int itemId, bool isFrontNow)> callBack);

    // デフォルトで最初に来るスロットの中身を取得する
    int GetFront(void) const;

    // デフォルトで最後に来るスロットの中身を取得する
    int GetBack(void) const;

private:
    int front_;  // デフォルトで最初に来るスロット
    int back_;   // デフォルトで最後に来るスロット

    // 現在frontを表示しているかどうか
    bool showFront_;    

    // 切り替えが起きた時に通知するコールバック
    std::function<void(int, bool)> onToggle_;

    // 内部で通知する処理する専用関数
    void NotifyToggle(void);
};

