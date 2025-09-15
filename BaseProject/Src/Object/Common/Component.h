#pragma once
#include <memory>
#include <string>
class ActorBase;

class Component
{
public:
	Component(std::shared_ptr<ActorBase> owner, const std::wstring name = L"Component");
	virtual ~Component() = default;

	// 毎フレーム状態を更新する
	virtual void Update(float deltaTime) = 0;

	// コンポーネントの識別名を取得
	const std::wstring& GetName(void) const;

	// このコンポーネントを持つアクターを取得
	std::weak_ptr<ActorBase> GetOwner(void) const;

protected:
	std::weak_ptr<ActorBase> mOwner_;	// このコンポーネントを持つアクター
	std::wstring mName_; // コンポーネントの識別名
};

