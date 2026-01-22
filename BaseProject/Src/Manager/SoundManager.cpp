#include <DxLib.h>
#include "SoundManager.h"


SoundManager& SoundManager::Instance()
{
    static SoundManager instance;
    return instance;
}

void SoundManager::Init()
{
    // メモリ圧縮形式でサウンドデータを作成
    // メモリ使用量を抑える目的
    SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMPRESS);
}

void SoundManager::Destory()
{
    // 再生中のBGMがあれば停止
    StopBGM();

    // BGM用サウンドハンドルの解放
    for (auto& bgm : bgmHandles_){
        DeleteSoundMem(bgm.second);
    }
    bgmHandles_.clear();

    // SE用サウンドハンドルの解放
    for (auto& se : seHandles_){
        DeleteSoundMem(se.second);
    }
    seHandles_.clear();
}

bool SoundManager::LoadBGM(const std::wstring& key, const std::wstring& filePath)
{
    // 既にロード済みなら省略
    if (bgmHandles_.count(key) > 0){
        return true;
    }

    // BGMサウンドデータのロード
    int handle = LoadSoundMem(filePath.c_str());
    if (handle == -1){
        return false;
    }

    // 初期音量の設定
    ChangeVolumeSoundMem(bgmVolume_, handle);
    
    // 管理マップに登録
    bgmHandles_[key] = handle;
    return true;
}

bool SoundManager::LoadSE(const std::wstring& key, const std::wstring& filePath)
{
    // 既にロード済みなら省略
    if (seHandles_.count(key) > 0){
        return true;
    }

    // SEサウンドデータのロード
    int handle = LoadSoundMem(filePath.c_str());
    if (handle == -1){
        return false;
    }

    // 初期音量の設定
    ChangeVolumeSoundMem(seVolume_, handle);
    
    // 管理マップに登録
    seHandles_[key] = handle;
    return true;
}

void SoundManager::PlayBGM(const std::wstring& key, bool loop)
{
    auto it = bgmHandles_.find(key);

    // 未ロードのBGMは生成させない
    if (it == bgmHandles_.end()){
        return;
    }

    // 別のBGMが再生中なら停止
    if (currentBGMH_ != -1){
        StopSoundMem(currentBGMH_);
    }

    currentBGMH_ = it->second;

    // ループ or 単発再生の切り替え
    PlaySoundMem(currentBGMH_, loop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK);
}

void SoundManager::StopBGM()
{
    if (currentBGMH_ != -1){
        StopSoundMem(currentBGMH_);
        currentBGMH_ = -1;
    }
}

void SoundManager::PlaySE(const std::wstring& key)
{
    auto it = seHandles_.find(key);
    if (it == seHandles_.end()){
        return;
    }

    // SEは重ねて再生するためBACK再生
    PlaySoundMem(it->second, DX_PLAYTYPE_BACK);
}

void SoundManager::FadeOutBGM(int frame)
{
    if (currentBGMH_ == -1){
        return;
    }

    // 即時音量を0にして停止
    SetVolumeSoundMem(0, currentBGMH_);
    StopSoundMem(currentBGMH_);
    currentBGMH_ = -1;
}

void SoundManager::SetBGMVolume(int volume)
{
    bgmVolume_ = volume;
    if (currentBGMH_ != -1){
        ChangeVolumeSoundMem(volume, currentBGMH_);
    }
}

void SoundManager::SetSEVolume(int volume)
{
    seVolume_ = volume;
    for (auto& se : seHandles_){
        ChangeVolumeSoundMem(volume, se.second);
    }
}