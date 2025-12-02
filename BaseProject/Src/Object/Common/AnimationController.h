#pragma once
#include <string>
#include <map>
class SceneController;

class AnimationController
{
	
public :

	// アニメーションデータ
	struct Animation{
		int model = -1;
		int attachNo = -1;
		int animIndex = 0;
		float speed = 0.0f;
		float totalTime = 0.0f;
		float step = 0.0f;
	};

	// コンストラクタ
	AnimationController(int modelId);
	// デストラクタ
	~AnimationController(void);

	// アニメーション追加
	void Add(int type, const std::wstring& path, float speed);

	// アニメーション再生
	void Play(int type, bool isLoop = true,float blendRate=1.0f,
		float startStep = 0.0f, float endStep = -1.0f, bool isStop = false, bool isForce = false);

	// アニメーション更新
	void Update(void);

	// アニメーション終了後に繰り返すループステップ
	void SetEndLoop(float startStep, float endStep, float speed);

	// 再生中のアニメーション
	int GetPlayType(void) const;

	// 再生終了
	bool IsEnd(void) const;

private :

	// モデルのハンドルID
	int modelId_;

	// 種類別のアニメーションデータ
	std::map<int, Animation> animations_;

	// 再生する型
	int playType_;

	// アニメーションデータ
	Animation playAnim_;

	// アニメーションをループするかしないか
	bool isLoop_;

	// アニメーションを止めたままにする
	bool isStop_;

	// アニメーション終了後に繰り返すループステップ
	// ループステップの始まり
	float stepEndLoopStart_;

	// ループステップの終わり
	float stepEndLoopEnd_;

	// ループスピード
	float endLoopSpeed_;

	// 逆再生
	float switchLoopReverse_;

};

