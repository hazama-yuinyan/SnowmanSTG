/*アイテムのクラスのヘッダーファイル*/

#ifndef _ITEMS_H
#define _ITEMS_H

#include "MyDrawObj.h"
#include "Snowman.h"
#include "TouchObj.h"


class Items : public MyDrawObj, public TouchObj, public Character
{
protected:
	double AMOUNT_ITEM_MOVE;

public:
	Items() : MyDrawObj(3){
		const double RAND = static_cast<double>(HAZAMA::draw_helper->GetRand(MAX_INITIAL_POS_Y));
		pos(INITIAL_FOE_POSITION, RAND);
		rect(0, 0, 40.0, 40.0);
		touch_rect(0, 0, 40.0, 40.0);
	}
	virtual ~Items(){};
	bool Dispatch(TouchObj *other){
		return(other->Touch(this));
	}
	virtual bool Touch(Snowman *other){return false;}
};

class LifeExtend : public Items
{
public:
	LifeExtend(void){
		AMOUNT_ITEM_MOVE = 480.0;
		speed = AMOUNT_ITEM_MOVE;
		vect(-1.0 * speed, 0);
	}
	bool Draw(void);

	/**
	* 実際に判定を取る方
	*/
	bool Touch(Snowman *other);
};

class Barrier : public Items
{
public:
	Barrier(void){
		AMOUNT_ITEM_MOVE = 660.0;
		speed = AMOUNT_ITEM_MOVE;
		vect(-1.0 * speed, 0);
	}
	bool Draw(void);

	/**
	* 実際に判定を取る方
	*/
	bool Touch(Snowman *other);
};

class Quick : public Items
{
public:
	Quick(void){
		AMOUNT_ITEM_MOVE = 720.0;
		speed = AMOUNT_ITEM_MOVE;
		vect(-1.0 * speed, 0);
	}
	bool Draw(void);

	/**
	* 実際に判定を取る方
	*/
	bool Touch(Snowman *other);
};

class TriBullets : public Items
{
public:
	TriBullets(void){
		AMOUNT_ITEM_MOVE = 1020.0;
		speed = AMOUNT_ITEM_MOVE;
		vect(-1.0 * speed, 0);
	}
	bool Draw(void);

	/**
	* 実際に判定を取る方
	*/
	bool Touch(Snowman *other);
};

class Eliminator : public Items
{
public:
	Eliminator(void){
		AMOUNT_ITEM_MOVE = 834.0;
		speed = AMOUNT_ITEM_MOVE;
		vect(-1.0 * speed, 0);
	}
	bool Draw(void);

	/**
	* 実際に判定を取る方
	*/
	bool Touch(Snowman *other);
};

class Multiplier : public Items
{
public:
	Multiplier(void){
		AMOUNT_ITEM_MOVE = 780.0;
		speed = AMOUNT_ITEM_MOVE;
		vect(-1.0 * speed, 0);
	}
	bool Draw(void);

	/**
	* 実際に判定を取る方
	*/
	bool Touch(Snowman *other);
};

class GravifiedBullet : public Items
{
public:
	GravifiedBullet(void){
		AMOUNT_ITEM_MOVE = 700.0;
		speed = AMOUNT_ITEM_MOVE;
		vect(-1.0 * speed, 0);
	}
	bool Draw(void);

	/**
	* 実際に判定を取る方
	*/
	bool Touch(Snowman *other);
};

class EliminatorX : public Items
{
private:
	bool destruct_flag;				//!< @brief eliminator_xの自爆装置作動フラグ
	int count_start;				//!< @brief eliminator_xの自爆装置の起動時間
	int *p_pic_handle;

public:
	EliminatorX(void) : destruct_flag(false), count_start(0){
		AMOUNT_ITEM_MOVE = 450.0;
		speed = AMOUNT_ITEM_MOVE;
		vect(-1.0 * speed, 0);
		p_pic_handle = &pic_handles[ITEM_ELIMINATOR_X1];
	}
	bool Draw(void);

	/**
	* 実際に判定を取る方
	*/
	bool Touch(Snowman *other);

	/**
	* 自爆装置が起動し、もう爆発するかどうかを返す
	* @return true:爆発する false:まだ爆発しない、または自爆装置が起動していない
	*/
	bool IsDestroyed(void){
		if(destruct_flag){
			return(static_cast<int>(TimerForGames::Instance().GetTotalTime()) - count_start >= 3000);
		}
		return false;
	}
};

#endif
