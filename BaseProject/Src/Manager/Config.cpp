#include <Windows.h>
#include "Config.h"

Config::Config(void)
{
}

Config::~Config(void)
{
}

void Config::Init(void)
{
	window_.height_ = GetSystemMetrics(SM_CXSCREEN);
	window_.width_ = GetSystemMetrics(SM_CYSCREEN);
}

void Config::Update(void)
{


}

const WindowSize& Config::GetWindowSize(void) const
{
	return window_;
}
