#pragma once
#include <vector>

#include "../../../Common/Component.h"

class UIElement;

/// <summary>
/// 単一のUI描画を管理する
/// コンポーネント
/// </summary>
class UIComponent :
    public Component
{
public:

    UIComponent(std::shared_ptr<ActorBase> owner);
    ~UIComponent(void);
    virtual void Update(float deltaTime) override = 0;
    void Draw(void);

    // UIの追加
    template<typename T,typename... Args>
    T* AddElement(Args&&... args) {
        auto elem = std::make_unique<T>(std::forward<Args>(args)...);
        T* rawPtr = elem.get();
        elements_.push_back(std::move(elem));
        return rawPtr;
    }

private:

    // 各UIの情報
    std::vector<std::unique_ptr<UIElement>> elements_;

};

