#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <memory>
#include"Application.h"
#include<DxLib.h>

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

int WINAPI WinMain
	(
	_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow
	)
{
	
	// メモリリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// 垂直同期を有効にする
	SetWaitVSyncFlag(true);

	// インスタンスの生成
	Application::CreateInstance();

	// インスタンスの取得
	Application& app = Application::GetInstance();
	if (app.IsInitFail()) {
		return -1;
	}

	// メインループ
	app.Run();

	// 終了処理
	app.Terminate();

	return 0;
}