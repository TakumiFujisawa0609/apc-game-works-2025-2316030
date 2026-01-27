#pragma once

/// <summary>
/// UIの単一要素の描画
/// コンポーネント基底クラス
/// </summary>
class UIElement
{
public:

	UIElement(void);
    virtual ~UIElement(void) = default;
    
    virtual void Update(float deltaTime) = 0;
    virtual void Draw(void) = 0;

private:

};

