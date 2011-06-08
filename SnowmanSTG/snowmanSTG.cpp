// snowmanSTG.cpp : メイン プロジェクト ファイルです。

#include "stdafx.h"
#include "InputDevice.h"
#include "Scenes.h"
#include "AnimObj.h"
#include "Sounds.h"
#include "Factory.h"
#include "Task.h"

using namespace Dix;

CDrawTask *CMyObjectFactory::p_draw_task;				//静的メンバ変数の実体
list<IObjectFactory*> CFactoryManager::m_pFactoryList;	// staticなリストをメモリに確保

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ){

	/*ウインドウの設定*/
	ChangeWindowMode(true);
	SetMainWindowText("snowmanシューティング☆");

#ifndef _DEBUG
	SetOutApplicationLogValidFlag(false);			//ログファイルの出力を行わない
#endif

	if(DxLib_Init() == -1){							 //DXライブラリーの初期化
		return -1;
	}

	/*描画の初期化*/
	CMyDrawObj::LoadPic();
	CAnimObj::LoadPic();
	CSounds::LoadSounds();

	CTimerForGames::SetTimerCounter();

	sp<CInputDeviceKeyBoard> sp_input_key(new CInputDeviceKeyBoard);
	sp<CTaskSystem> sp_task_system(new CTaskSystem(sp_input_key.GetPtr()));
	sp<CSceneParent> sp_scene(new CSceneParent);
	sp<CSceneTitle> sp_title(new CSceneTitle(sp_scene, sp_task_system, sp_input_key));
	sp_scene->AddScene(sp_title);
	sp_scene->Begin();

	InitGraph();	//読み込んだ画像データを全て削除する
	InitSoundMem();	//読み込んだサウンドデータを全て削除する

	DxLib_End();

	return 0;
}
