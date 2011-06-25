#include "stdafx.h"
#include "Scenes.h"
#include "Factory.h"
#include "Sounds.h"
#include "AnimObj.h"
#include "Task.h"
#include "InputDevice.h"


SceneTitle::SceneTitle(SPScenes sp_paren, SPTaskSystem &sp_task, SPKeyBoard &sp_key)
{
	sp_task_system = sp_task;
	sp_input_key = sp_key;
	sp_parent = sp_paren;
}

void SceneTitle::Begin(void)
{
	Dix::sp<TitleObj> sp_title;
	MyFactory<TitleObj>::Instance().Create(0, sp_title);

	while(sp_input_key->Update(this, sp_title.GetPtr())){
		sp_task_system->Update();
		if(!HAZAMA::draw_helper->ProcessMessage()){
			return;
		}
	}
	HAZAMA::draw_helper->SetDrawScreen(HAZAMA::DrawHelper::BACK);
	HAZAMA::draw_helper->InitializeScreen();
	HAZAMA::draw_helper->DrawString(200, 220, "‚Æ‚É‚©‚­¶‚«‰„‚Ñ‚ë...", HAZAMA::draw_helper->GetColorCode(255, 255, 255));
	HAZAMA::draw_helper->FlipScreen();
	sp_title->Clear();
	HAZAMA::draw_helper->WaitTimer(3000);
	TimerForGames::Instance().Stop(true);
	Dix::sp<SceneGaming> sp_gaming(new SceneGaming(sp_parent, sp_task_system, sp_input_key));
	sp_parent->AddScene(sp_gaming);
	active_flag = false;
}

void SceneTitle::MenuStartGame::Execute(void)
{
	p_title->sp_task_system->SetReplayMode(false);
}

void SceneTitle::MenuReplayGame::Execute(void)
{
	p_title->sp_task_system->SetReplayMode(true);
}

void SceneTitle::Invoke(MENUS &menu)
{
	switch(menu){
		case START_GAME:
			p_menu = new MenuStartGame(this);
			break;
		case REPLAY_GAME:
			p_menu = new MenuReplayGame(this);
			break;
	}

	p_menu->Execute();
}

SceneGaming::SceneGaming(SPScenes sp_paren, SPTaskSystem &sp_task, SPKeyBoard &sp_key)
{
	sp_task_system = sp_task;
	sp_input_key = sp_key;
	SnowmanTask *snowman_task = sp_task_system->GetTask<SnowmanTask>(TASK_SNOWMAN);
	sp_input_key->SetSnowmanTask(snowman_task);
	MyFactory<BackObj>::Instance().Create(0, sp_back);
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
	DrawTask *p_draw_task = sp_task_system->GetTask<DrawTask>(TASK_DRAW);
	sp_score.SetPtr(p_score);
	Dix::sp<DrawObj> sp_draw = sp_score;
	p_draw_task->Register(sp_draw);
	sp_parent = sp_paren;
}

void SceneGaming::Begin(void)
{
	while(1){
		MainBGM::Play();	//BGM‚ÌÄ¶
		if(!sp_input_key->Update() || !sp_task_system->Update()){
			break;		//‘±s‹ÖŽ~‚Ì‡}‚ª‚ ‚Á‚½‚çAƒXƒRƒA‰æ–Ê‚Ö
		}

		FactoryManager::Instance().Optimize();
		if(!HAZAMA::draw_helper->ProcessMessage()){
			return;
		}
	}
	Dix::sp<SceneScore> sp_scene_score(new SceneScore(sp_parent, sp_task_system, sp_input_key));
	sp_parent->AddScene(sp_scene_score);
	MainBGM::Stop();		//BGM‚Ì’âŽ~
	TimerForGames::Instance().Stop(true);
	active_flag = false;
}

SceneScore::SceneScore(SPScenes sp_paren, SPTaskSystem &sp_task, SPKeyBoard &sp_key)
{
	sp_task_system = sp_task;
	sp_input_key = sp_key;
	white = HAZAMA::draw_helper->GetColorCode(255, 255, 255);
	sp_parent = sp_paren;
}

void SceneScore::Begin(void)
{
	const char CONTINUE_SUGGESTION[] = "Press c to try it AGAIN!";
	const char QUIT_SUGGESTION[] = "Press q to quit the game!";

	while(1){
		HAZAMA::draw_helper->SetDrawScreen(HAZAMA::DrawHelper::BACK);
		HAZAMA::draw_helper->InitializeScreen();
		ScoreRanking();
		HAZAMA::draw_helper->DrawString(200, 400, CONTINUE_SUGGESTION, white);
		HAZAMA::draw_helper->DrawString(200, 430, QUIT_SUGGESTION, white);
		HAZAMA::draw_helper->FlipScreen();

		if(!HAZAMA::draw_helper->ProcessMessage()){
			return;
		}

		HAZAMA::draw_helper->WaitForKey();
		if(HAZAMA::draw_helper->IsKeyPressedWith(HAZAMA::DrawHelper::KEY_C)){
			sp_task_system->Reset();
			FactoryManager::Instance().ClearAll();
			sp_parent->RemoveScenes();
			Dix::sp<SceneGaming> sp_gaming(new SceneGaming(sp_parent, sp_task_system, sp_input_key));
			sp_parent->AddScene(sp_gaming);
			break;
		}else if(HAZAMA::draw_helper->IsKeyPressedWith(HAZAMA::DrawHelper::KEY_Q)){
			break;
		}
	}
}

void SceneScore::ScoreRanking(void)
{
	Score *p_score = &Score::Instance();
	p_score->ReadFile();
	p_score->PutInOrder();
	p_score->WriteFile();
	p_score->RankDraw();
	p_score->Reset();
}