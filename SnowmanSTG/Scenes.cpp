#include "stdafx.h"
#include "Scenes.h"
#include "Factory.h"
#include "Sounds.h"
#include "AnimObj.h"
#include "Task.h"
#include "InputDevice.h"


CSceneTitle::CSceneTitle(SPScenes sp_paren, SPTaskSystem &sp_task, SPKeyBoard &sp_key)
{
	sp_task_system = sp_task;
	sp_input_key = sp_key;
	sp_parent = sp_paren;
}

void CSceneTitle::Begin(void)
{
	Dix::sp<CTitleObj> sp_title;
	CMyFactory<CTitleObj>::Instance().Create(0, sp_title);

	while(sp_input_key->Update(this, sp_title.GetPtr())){
		sp_task_system->Update();
		if(ProcessMessage() == -1){
			return;
		}
	}
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	DrawString(200, 220, "Ç∆Ç…Ç©Ç≠ê∂Ç´âÑÇ—ÇÎ...", GetColor(255, 255, 255));
	ScreenFlip();
	sp_title->Clear();
	WaitTimer(3000);
	CTimerForGames::Instance().Stop(true);
	Dix::sp<CSceneGaming> sp_gaming(new CSceneGaming(sp_parent, sp_task_system, sp_input_key));
	sp_parent->AddScene(sp_gaming);
	active_flag = false;
}

void CSceneTitle::CMenuStartGame::Execute(void)
{
	p_title->sp_task_system->SetReplayMode(false);
}

void CSceneTitle::CMenuReplayGame::Execute(void)
{
	p_title->sp_task_system->SetReplayMode(true);
}

void CSceneTitle::Invoke(MENUS &menu)
{
	switch(menu){
		case START_GAME:
			p_menu = new CMenuStartGame(this);
			break;
		case REPLAY_GAME:
			p_menu = new CMenuReplayGame(this);
			break;
	}

	p_menu->Execute();
}

CSceneGaming::CSceneGaming(SPScenes sp_paren, SPTaskSystem &sp_task, SPKeyBoard &sp_key)
{
	sp_task_system = sp_task;
	sp_input_key = sp_key;
	CSnowmanTask *snowman_task = sp_task_system->GetTask<CSnowmanTask>(TASK_SNOWMAN);
	sp_input_key->SetSnowmanTask(snowman_task);
	CMyFactory<CBackObj>::Instance().Create(0, sp_back);
	Score *p_score = &Score::Instance();
	sp_task_system->Init();
	sp_task_system->SwitchTask(TASK_SNOWMAN);
	sp_task_system->SwitchTask(TASK_ZETSUBOU);
	sp_task_system->SwitchTask(TASK_SELFBULLET);
	sp_task_system->SwitchTask(TASK_FOEBULLET);
	sp_task_system->SwitchTask(TASK_ITEM);
	sp_task_system->SwitchTask(TASK_TOUCH);
	sp_task_system->SwitchTask(TASK_SCORE);
	sp_task_system->SwitchTask(TASK_REPLAY);
	CDrawTask *p_draw_task = sp_task_system->GetTask<CDrawTask>(TASK_DRAW);
	sp_score.SetPtr(p_score);
	Dix::sp<CDrawObj> sp_draw = sp_score;
	p_draw_task->Register(sp_draw);
	sp_parent = sp_paren;
}

void CSceneGaming::Begin(void)
{
	while(1){
		MainBGM::Play();	//BGMÇÃçƒê∂
		if(!sp_input_key->Update() || !sp_task_system->Update()){
			break;		//ë±çsã÷é~ÇÃçáê}Ç™Ç†Ç¡ÇΩÇÁÅAÉXÉRÉAâÊñ Ç÷
		}

		CFactoryManager::Instance().Optimize();
		if(ProcessMessage() == -1){
			return;
		}
	}
	Dix::sp<CSceneScore> sp_scene_score(new CSceneScore(sp_parent, sp_task_system, sp_input_key));
	sp_parent->AddScene(sp_scene_score);
	MainBGM::Stop();		//BGMÇÃí‚é~
	CTimerForGames::Instance().Stop(true);
	active_flag = false;
}

CSceneScore::CSceneScore(SPScenes sp_paren, SPTaskSystem &sp_task, SPKeyBoard &sp_key)
{
	sp_task_system = sp_task;
	sp_input_key = sp_key;
	white = GetColor(255, 255, 255);
	sp_parent = sp_paren;
}

void CSceneScore::Begin(void)
{
	const char CONTINUE_SUGGESTION[] = "Press c to try it AGAIN!";
	const char QUIT_SUGGESTION[] = "Press q to quit the game!";

	while(1){
		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		ScoreRanking();
		DrawString(200, 400, CONTINUE_SUGGESTION, white);
		DrawString(200, 430, QUIT_SUGGESTION, white);
		ScreenFlip();

		if(ProcessMessage() == -1){
			return;
		}

		WaitKey();
		if(CheckHitKey(KEY_INPUT_C) == 1){
			sp_task_system->Reset();
			CFactoryManager::Instance().ClearAll();
			sp_parent->RemoveScenes();
			Dix::sp<CSceneGaming> sp_gaming(new CSceneGaming(sp_parent, sp_task_system, sp_input_key));
			sp_parent->AddScene(sp_gaming);
			break;
		}else if(CheckHitKey(KEY_INPUT_Q) == 1){
			break;
		}
	}
}

void CSceneScore::ScoreRanking(void)
{
	Score *p_score = &Score::Instance();
	p_score->ReadFile();
	p_score->PutInOrder();
	p_score->WriteFile();
	p_score->RankDraw();
	p_score->Reset();
}