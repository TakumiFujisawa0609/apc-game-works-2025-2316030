#pragma once
#include <memory>
#include <string>
class ActorBase;


/// <summary>
/// Actor に対しての振る舞いや機能を追加するための
/// コンポーネント基底クラス
/// 
/// このクラスは直接使用せず、派生クラスを作成して
/// Update()を実装すること前提とする
/// </summary>
class Component
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">コンポーネントの所有者</param>
	Component(std::shared_ptr<ActorBase> owner);
	virtual ~Component(void) = default;

	// 毎フレーム状態を更新する
	virtual void Update(float deltaTime) = 0;

	// このコンポーネントの所有者を取得
	std::weak_ptr<ActorBase> GetOwner(void) const;

protected:

	// このコンポーネントの所有者
	std::weak_ptr<ActorBase> mOwner_;

};

