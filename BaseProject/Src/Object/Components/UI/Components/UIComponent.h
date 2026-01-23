#pragma once
#include <vector>

#include "../../../Common/Component.h"

class UIElement;

class UIComponent :
    public Component
{
public:

    UIComponent(std::shared_ptr<ActorBase> owner);
    ~UIComponent(void);
    virtual void Update(float deltaTime) override = 0;
    void Draw(void);

    // UIÇÃí«â¡
    template<typename T,typename... Args>
    T* AddElement(Args&&... args) {
        auto elem = std::make_unique<T>(std::forward<Args>(args)...);
        T* rawPtr = elem.get();
        elements_.push_back(std::move(elem));
        return rawPtr;
    }

private:

    // äeUIÇÃèÓïÒ
    std::vector<std::unique_ptr<UIElement>> elements_;

};

