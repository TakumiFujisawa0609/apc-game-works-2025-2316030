#pragma once
#include <vector>
#include "../Object/Common/Transform.h"
class ResourceManager;
class Component;

class Collider;
class Capsule;

class ActorBase : public std::enable_shared_from_this<ActorBase>
{
public:

	// 頂点シェーダータイプの識別用
	enum class TYPE
	{	
		NONE,
		REGIDBODY,
		SKINING
	};

	// コンストラクタ
	ActorBase(void);

	// デストラクタ
	virtual ~ActorBase(void);

	virtual void Init(void) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Draw(void) = 0;

	const Transform& GetTransform(void) const;

	// コンポーネントの追加
	template<typename T, typename... Args>
	T* AddComponent(Args&&... args)
	{
		auto comp = std::make_unique<T>(shared_from_this(), std::forward<Args>(args)...);
		T* ptr = comp.get();
		components_.emplace_back(std::move(comp));
		return ptr;
	}

	// モデルのタイプ
	virtual TYPE GetModelType(void);

protected:

	// シングルトン参照
	ResourceManager& resMng_;

	// モデル制御の基本情報
	Transform transform_;

	// モデルタイプ
	TYPE type_;

	// 派生クラスで独自処理
	virtual void OnUpdate(float deltaTime) = 0;

private:

	// アクターコンポーネント群
	std::vector<std::unique_ptr<Component>> components_;
};
