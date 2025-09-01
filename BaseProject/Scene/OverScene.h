#pragma once
#include "Scene.h"
class OverScene :
    public Scene
{
	private:
		int imgH_ = 0;
		int frame_ = 0;
		using UpdateFunc_t = void(OverScene::*)(Input& input);
		using DrawFunc_t = void(OverScene::*)();

		UpdateFunc_t update_;
		DrawFunc_t draw_;

		void FadeInUpadte(Input&);
		void NormalUpadte(Input&);
		void FadeOutUpadte(Input&);

		void NormalDraw();
		void FadeDraw();

	public:
		OverScene(SceneController& controller);
		~OverScene();
		/// <summary>
		/// 毎フレーム状態を更新する
		/// </summary>
		/// <param name="input">入力クラス</param>
		virtual void Update(Input& input)override;

		/// <summary>
		/// 毎フレーム描画する
		/// </summary>
		virtual void Draw()override;
};

