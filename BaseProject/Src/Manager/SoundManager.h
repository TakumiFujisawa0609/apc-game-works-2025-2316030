#pragma once
#include <string>
#include <unordered_map>


class SoundManager
{
public:

	// シングルトン
	static SoundManager& Instance(void);

	// 初期化
	void Init(void);
	
	// サウンド終了

	/// <summary>
	/// サウンド管理の終了処理
	/// ロードした全てのサウンドを開放する
	/// </summary>
	/// <param name=""></param>
	void Destory(void);

	// ロード
	bool LoadBGM(const std::wstring& key, const std::wstring& filePath);
	bool LoadSE(const std::wstring& key, const std::wstring& filePath);

	// 再生
	void PlayBGM(const std::wstring& key, bool loop = true);
	void StopBGM(void);
	void PlaySE(const std::wstring& key);

	// フェード
	void FadeOutBGM(int frame);

	// 音量
	void SetBGMVolume(int volume);
	void SetSEVolume(int volume);

private:

	SoundManager(void) = default;
	~SoundManager(void) = default;
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = default;

	std::unordered_map<std::wstring, int> bgmHandles_;
	std::unordered_map<std::wstring, int> seHandles_;

	int currentBGMH_;
	int bgmVolume_;
	int seVolume_;

};

