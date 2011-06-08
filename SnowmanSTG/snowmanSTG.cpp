// snowmanSTG.cpp : ���C�� �v���W�F�N�g �t�@�C���ł��B

#include "stdafx.h"
#include "InputDevice.h"
#include "Scenes.h"
#include "AnimObj.h"
#include "Sounds.h"
#include "Factory.h"
#include "Task.h"

using namespace Dix;

CDrawTask *CMyObjectFactory::p_draw_task;				//�ÓI�����o�ϐ��̎���
list<IObjectFactory*> CFactoryManager::m_pFactoryList;	// static�ȃ��X�g���������Ɋm��

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ){

	/*�E�C���h�E�̐ݒ�*/
	ChangeWindowMode(true);
	SetMainWindowText("snowman�V���[�e�B���O��");

#ifndef _DEBUG
	SetOutApplicationLogValidFlag(false);			//���O�t�@�C���̏o�͂��s��Ȃ�
#endif

	if(DxLib_Init() == -1){							 //DX���C�u�����[�̏�����
		return -1;
	}

	/*�`��̏�����*/
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

	InitGraph();	//�ǂݍ��񂾉摜�f�[�^��S�č폜����
	InitSoundMem();	//�ǂݍ��񂾃T�E���h�f�[�^��S�č폜����

	DxLib_End();

	return 0;
}
