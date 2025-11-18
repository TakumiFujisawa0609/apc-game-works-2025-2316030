#include <Windows.h>
#include <DxLib.h>
#include "Config.h"

Config* Config::instance_ = nullptr;

Config::Config(void)
	:
	isFullS_(false),
	window_()
{
}

void Config::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new Config();
		instance_->Init();
	}
}

Config& Config::GetInstance()
{
	return *instance_;
}

void Config::Init(void)
{
	if (!isFullS_)
	{
		window_.width_ = 1280;
		window_.height_ = 720;
	}
	SetGraphMode(window_.width_, window_.height_, 32);
	ChangeWindowMode(true);
}

const Config::WindowSize& Config::GetWindowSize(void) const
{
	return window_;
}

void Config::SetWindowSize(int width, int height)
{
	window_.width_ = width;
	window_.height_ = height;
	ScreenInitialize();
}

bool Config::IsFullScreen(void) const
{
	return isFullS_;
}

void Config::SetFullScreen(bool isFull)
{
	isFullS_ = isFull;
	ScreenInitialize();
}

void Config::ScreenInitialize(void)
{
	SetDrawScreen(DX_SCREEN_BACK);

	if (!isFullS_) {
	/*	window_.width_ = 1280;
		window_.height_ = 720;*/
		SetGraphMode(window_.width_, window_.height_, 32);
		ChangeWindowMode(true);
	}
	else {
		fullScreenWidth_ = GetSystemMetrics(SM_CXSCREEN);
		fullScreenHeight_ = GetSystemMetrics(SM_CYSCREEN);
		//SetGraphMode(fullScreenWidth_, fullScreenHeight_, 32);
		ChangeWindowMode(false);
		
		SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_DESKTOP);
		//SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_MAXIMUM);

	}
}

float Config::GetMouseSensitivity(void) const
{
	return mouseSensitivity_;
}

void Config::SetMouseSensitivity(float sensitivity)
{
	mouseSensitivity_ = sensitivity;
}

int Config::GetBGMVolume(void) const
{
	return bgmVolume_;
}

void Config::SetBGMVolume(int volume)
{
	bgmVolume_ = volume;
}

int Config::GetSEVolume(void) const
{
	return seVolume_;
}

void Config::SetSEVolume(int volume)
{
	seVolume_ = volume;
}

bool Config::isJoyPadVibrationFlag(void) const
{
	return isVibration_;
}

void Config::SetJoyPadVibration(bool flag)
{
	isVibration_ = flag;
	SetUseJoypadVibrationFlag(isVibration_);
}

void Config::Destroy(void)
{
	delete instance_;
}
