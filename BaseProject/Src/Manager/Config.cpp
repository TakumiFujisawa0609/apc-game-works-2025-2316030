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
		window_.width_ = 640;
		window_.height_ = 480;
	}
}

const Config::WindowSize& Config::GetWindowSize(void) const
{
	return window_;
}

void Config::SetWindowSize(int width, int height)
{
	window_.width_ = width;
	window_.height_ = height;
}

bool Config::IsFullScreen(void) const
{
	return isFullS_;
}

void Config::SetFullScreen(bool isFull)
{
	isFullS_ = isFull;
}

void Config::FullScreenWindow(void)
{
	window_.height_ = GetSystemMetrics(SM_CXSCREEN);
	window_.width_ = GetSystemMetrics(SM_CYSCREEN);
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
