#pragma once
#include <string>
#include <map>
class SceneController;

class AnimationController
{
public :

	// アニメーションデータ
	struct Animation
	{
		int model = -1;
		int attachNo = -1;
		int animIndex = 0;
		float speed = 0.0f;
		float totalTime = 0.0f;
		float step = 0.0f;
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="modelId">モデルのハンドルID</param>
	AnimationController(int modelId);

	~AnimationController(void);

	/// <summary>
	/// アニメーション追加
	/// </summary>
	/// <param name="type">どの状態に追加するか</param>
	/// <param name="path">アニメーションのファイル名</param>
	/// <param name="speed">アニメーションの再生速度</param>
	void Add(int type, const std::wstring& path, float speed);

	/// <summary>
	/// アニメーション再生
	/// </summary>
	/// <param name="type">どの状態のアニメーションか</param>
	/// <param name="isLoop">ループ生成するかどうか</param>
	/// <param name="blendRate">アニメーションブレンド率の指定</param>
	/// <param name="startStep">再生されるアニメーションの再生開始位置</param>
	/// <param name="endStep">再生されるアニメーションの再生終了位置</param>
	/// <param name="isStop">アニメーションの停止させるかどうか</param>
	/// <param name="isForce">強制的にに再生させるかどうか</param>
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

	// 再生されるアニメーション種別
	int playType_;

	// アニメーションデータ
	Animation playAnim_;

	// アニメーションをループするかしないか
	bool isLoop_;

	// アニメーションを止めたままにする
	bool isStop_;

	// アニメーション終了後に繰り返すループステップ
	float stepEndLoopStart_;
	float stepEndLoopEnd_;
	float endLoopSpeed_;

	// 逆再生
	float switchLoopReverse_;

};

