#ifndef _INPUTDEVICE_H
#define _INPUTDEVICE_H

#include "Operator.h"
#include "Task.h"

class SceneTitle;
class TitleObj;
class SnowmanTask;
class MementoKey;
enum MENUS;


class IInputDevice
{
public:
	IInputDevice(void){};
	virtual ~IInputDevice(void){};
	virtual bool Update(void) = 0;
};


class InputDeviceKeyBoard : public IInputDevice
{
private:
	const int MENU_CHANGE_INTERVAL;
	char key_state_buf[256];
	MementoKey *p_memento;
	MENUS menu_pos;
	TimerForGames *p_timer;
	TimerForGames::TimerID timer_id;
	SnowmanTask *p_snow_task;
	Dix::sp<OperatorGoLeft> sp_ope_left;
	Dix::sp<OperatorGoUp> sp_ope_up;
	Dix::sp<OperatorGoRight> sp_ope_right;
	Dix::sp<OperatorGoDown> sp_ope_down;

	/**
	* シリアライズ処理
	*/
	/*friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &Ar, unsigned version)
	{
		Ar & key_state_buf;
	}*/

public:
	InputDeviceKeyBoard(void);

	void SetSnowmanTask(SnowmanTask *p){
		p_snow_task = p;
	}

	bool Update(void);

	bool Update(SceneTitle *Title, TitleObj *Obj);
};

#endif
