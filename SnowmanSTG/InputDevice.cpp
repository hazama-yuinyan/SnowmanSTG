#include "stdafx.h"
#include "InputDevice.h"
#include "Sounds.h"
#include "Scenes.h"
#include "Memento.h"


CInputDeviceKeyBoard::CInputDeviceKeyBoard(void) : sp_ope_left(new COperatorGoLeft), sp_ope_up(new COperatorGoUp)
, sp_ope_right(new COperatorGoRight), sp_ope_down(new COperatorGoDown), menu_pos(START_GAME)
, MENU_CHANGE_INTERVAL(250)
{
	GetHitKeyStateAll(key_state_buf);
	p_timer = &CTimerForGames::Instance();
	timer_id = p_timer->StartTimer();
	p_memento = new CMementoKey;
}

template<class Archive>
void CInputDeviceKeyBoard::serialize(Archive &Ar, unsigned int version)
{
	Ar & key_state_buf;
}

bool CInputDeviceKeyBoard::Update(void)
{
	char *p_key_state;
	Dix::sp<IOperator> sp_ope;

	GetHitKeyStateAll(key_state_buf);
	p_key_state = key_state_buf;

	for(int i = 0; i < 256; ++i){
		if(*p_key_state == 1){							//どのキーが押されたか判断して
			switch(i){									//適切な動作をする
				case KEY_INPUT_A:
				case KEY_INPUT_LEFT:
					sp_ope = sp_ope_left;
					p_snow_task->SetOperator(sp_ope);
					break;
				case KEY_INPUT_W:
				case KEY_INPUT_UP:
					sp_ope = sp_ope_up;
					p_snow_task->SetOperator(sp_ope);
					break;
				case KEY_INPUT_D:
				case KEY_INPUT_RIGHT:
					sp_ope = sp_ope_right;
					p_snow_task->SetOperator(sp_ope);
					break;
				case KEY_INPUT_S:
				case KEY_INPUT_DOWN:
					sp_ope = sp_ope_down;
					p_snow_task->SetOperator(sp_ope);
					break;
				case KEY_INPUT_P:
					SetDrawScreen(DX_SCREEN_BACK);
					MainBGM::Stop();
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
					DrawString(300, 210, "PAUSED", GetColor(0, 0, 0));
					DrawString(240, 250, "Just take a break!", GetColor(0, 0, 0));
					ScreenFlip();
					WaitKey();
					if(CheckHitKey(KEY_INPUT_ESCAPE) == 1){		//ゲーム本編の時はスコア画面へ
						return false;
					}
					MainBGM::Resume();
					break;
				case KEY_INPUT_SPACE:
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

bool CInputDeviceKeyBoard::Update(CSceneTitle *Title, CTitleObj *Obj)
{
	char *p_key_state;

	GetHitKeyStateAll(key_state_buf);
	p_key_state = key_state_buf;

	for(int i = 0; i < 256; ++i){
		if(*p_key_state == 1){							//どのキーが押されたか判断して
			switch(i){									//適切な動作をする
				case KEY_INPUT_UP:
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
				case KEY_INPUT_DOWN:
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
				case KEY_INPUT_RETURN:
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