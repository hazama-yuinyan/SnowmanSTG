#include "stdafx.h"
#include "InputDevice.h"
#include "Sounds.h"
#include "Scenes.h"
#include "Memento.h"


InputDeviceKeyBoard::InputDeviceKeyBoard(void) : sp_ope_left(new OperatorGoLeft), sp_ope_up(new OperatorGoUp)
, sp_ope_right(new OperatorGoRight), sp_ope_down(new OperatorGoDown), menu_pos(START_GAME)
, MENU_CHANGE_INTERVAL(250)
{
	HAZAMA::draw_helper->GetAllKeysState(key_state_buf);
	p_timer = &TimerForGames::Instance();
	timer_id = p_timer->StartTimer();
	p_memento = new MementoKey;
}

bool InputDeviceKeyBoard::Update(void)
{
	char *p_key_state;
	Dix::sp<IOperator> sp_ope;

	HAZAMA::draw_helper->GetAllKeysState(key_state_buf);
	p_key_state = key_state_buf;

	for(int i = 0; i < 256; ++i){
		if(*p_key_state == 1){							//どのキーが押されたか判断して
			switch(i){									//適切な動作をする
			case HAZAMA::DrawHelper::KEY_A:
			case HAZAMA::DrawHelper::KEY_LEFT:
					sp_ope = sp_ope_left;
					p_snow_task->SetOperator(sp_ope);
					break;
			case HAZAMA::DrawHelper::KEY_W:
			case HAZAMA::DrawHelper::KEY_UP:
					sp_ope = sp_ope_up;
					p_snow_task->SetOperator(sp_ope);
					break;
			case HAZAMA::DrawHelper::KEY_D:
			case HAZAMA::DrawHelper::KEY_RIGHT:
					sp_ope = sp_ope_right;
					p_snow_task->SetOperator(sp_ope);
					break;
			case HAZAMA::DrawHelper::KEY_S:
			case HAZAMA::DrawHelper::KEY_DOWN:
					sp_ope = sp_ope_down;
					p_snow_task->SetOperator(sp_ope);
					break;
			case HAZAMA::DrawHelper::KEY_P:
					HAZAMA::draw_helper->SetDrawScreen(HAZAMA::DrawHelper::BACK);
					MainBGM::Stop();
					HAZAMA::draw_helper->SetDrawBlendMode(HAZAMA::DrawHelper::ALPHA, 255);
					HAZAMA::draw_helper->DrawString(300, 210, "PAUSED", GetColor(0, 0, 0));
					HAZAMA::draw_helper->DrawString(240, 250, "Just take a break!", GetColor(0, 0, 0));
					HAZAMA::draw_helper->FlipScreen();
					HAZAMA::draw_helper->WaitForKey();
					if(HAZAMA::draw_helper->IsKeyPressedWith(HAZAMA::DrawHelper::KEY_ESCAPE) == 1){		//ゲーム本編の時はスコア画面へ
						return false;
					}
					MainBGM::Resume();
					break;
			case HAZAMA::DrawHelper::KEY_SPACE:
					p_snow_task->Shoot();
			}
			++p_key_state;
			continue;
		}else{
			++p_key_state;
			continue;
		}
	}
	return true;
}

bool InputDeviceKeyBoard::Update(SceneTitle *Title, TitleObj *Obj)
{
	char *p_key_state;

	HAZAMA::draw_helper->GetAllKeysState(key_state_buf);
	p_key_state = key_state_buf;

	for(int i = 0; i < 256; ++i){
		if(*p_key_state == 1){							//どのキーが押されたか判断して
			switch(i){									//適切な動作をする
			case HAZAMA::DrawHelper::KEY_UP:
					p_timer->GetTotalTime();
					if(p_timer->IsTakenUp(timer_id, MENU_CHANGE_INTERVAL)){
						if(menu_pos == START_GAME){
							menu_pos = MENU_END;
						}
						menu_pos = static_cast<MENUS>(menu_pos - 1);
						Obj->SetStrColor(menu_pos);
						timer_id = p_timer->StartTimer();
					}
					break;
			case HAZAMA::DrawHelper::KEY_DOWN:
					p_timer->GetTotalTime();
					if(p_timer->IsTakenUp(timer_id, MENU_CHANGE_INTERVAL)){
						menu_pos = static_cast<MENUS>(menu_pos + 1);
						if(menu_pos == MENU_END){
							menu_pos = START_GAME;
						}
						Obj->SetStrColor(menu_pos);
						timer_id = p_timer->StartTimer();
					}
					break;
			case HAZAMA::DrawHelper::KEY_RETURN:
					Title->Invoke(menu_pos);
					return false;
					break;
			}
			++p_key_state;
			continue;
		}else{
			++p_key_state;
			continue;
		}
	}
	return true;
}