#pragma once

class ResourceManager;
class SoundManager
{
public:

	enum class SOUNDID {
		// title
		// bottom
		// kurikku
		// aruki 
		// hasiri
		// siyou
		// tukaikiru
		// hasittatokinokoe
		// 
	};

	// 明示的にインスタンスを生成
	static void CreateInstance();

	// 静的インスタンスの取得
	static SoundManager& GetInstance();

	// 初期化
	void Init(void);

	// 読み込み
	void Load(void);

	// 再生
	void Play(SOUNDID soundID);

	// リソースの削除
	void Destory(void);

private:

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SoundManager(void);
	~SoundManager(void) = default;

	// インスタンス
	static SoundManager* instance_;

	// リソース元
	ResourceManager& resManager_;

	// サウンドID
	SOUNDID soudID_;
};

