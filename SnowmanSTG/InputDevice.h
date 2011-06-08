#ifndef _INPUTDEVICE_H
#define _INPUTDEVICE_H

#include "Operator.h"
#include "Task.h"

class CSceneTitle;
class CTitleObj;
class CSnowmanTask;
class CMementoKey;
enum MENUS;


class IInputDevice
{
public:
	IInputDevice(void){};
	virtual ~IInputDevice(void){};
	virtual bool Update(void) = 0;
};


class CInputDeviceKeyBoard : public IInputDevice
{
private:
	const int MENU_CHANGE_INTERVAL;
	char key_state_buf[256];
	CMementoKey *p_memento;
	MENUS menu_pos;
	CTimerForGames *p_timer;
	CTimerForGames::TimerID timer_id;
	CSnowmanTask *p_snow_task;
	Dix::sp<COperatorGoLeft> sp_ope_left;
	Dix::sp<COperatorGoUp> sp_ope_up;
	Dix::sp<COperatorGoRight> sp_ope_right;
	Dix::sp<COperatorGoDown> sp_ope_down;

	/**
	* シリアライズ処理
	*/
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &Ar, unsigned version);

public:
	CInputDeviceKeyBoard(void);

	void SetSnowmanTask(CSnowmanTask *p){
		p_snow_task = p;
	}

	bool Update(void);

	bool Update(CSceneTitle *Title, CTitleObj *Obj);
};

#endif
