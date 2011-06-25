// snowmanSTG.cpp : ���C�� �v���W�F�N�g �t�@�C���ł��B

#include "stdafx.h"
#include "InputDevice.h"
#include "Scenes.h"
#include "AnimObj.h"
#include "Sounds.h"
#include "Factory.h"
#include "Task.h"


DrawTask *MyObjectFactory::p_draw_task;				//�ÓI�����o�ϐ��̎���
std::list<IObjectFactory*> FactoryManager::m_pFactoryList;	// static�ȃ��X�g���������Ɋm��
#ifdef USE_GLUT
Dix::sp<HAZAMA::DrawHelper> HAZAMA::draw_helper = Dix::sp<HAZAMA::DrawHelper>(new HAZAMA::Glut);
#endif
#ifdef USE_DXLIB
Dix::sp<HAZAMA::DrawHelper> HAZAMA::draw_helper = Dix::sp<HAZAMA::DrawHelper>(new HAZAMA::DxLibFunctions);
#endif

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ){

	/*�E�C���h�E�̐ݒ�*/
	HAZAMA::draw_helper->ChangeWindowMode(true);
	HAZAMA::draw_helper->SetWindowTitle("snowman�V���[�e�B���O��");

	/*�`��̏�����*/
	MyDrawObj::LoadPic();
	AnimObj::LoadPic();
	Sounds::LoadSounds();

	TimerForGames::SetTimerCounter();

	Dix::sp<InputDeviceKeyBoard> sp_input_key(new InputDeviceKeyBoard);
	Dix::sp<TaskSystem> sp_task_system(new TaskSystem(sp_input_key.GetPtr()));
	Dix::sp<SceneParent> sp_scene(new SceneParent);
	Dix::sp<SceneTitle> sp_title(new SceneTitle(sp_scene, sp_task_system, sp_input_key));
	sp_scene->AddScene(sp_title);
	sp_scene->Begin();

	return 0;
}
