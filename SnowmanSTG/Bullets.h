/*���@�ƓG��Bullet�N���X*/

#ifndef _BULLETS_H
#define _BULLETS_H

#include "TouchObj.h"
#include "MyDrawObj.h"
#include "Snowman.h"

class SelfBullet : public MyDrawObj, public Character, public TouchObj
{
protected:
	const double AMOUNT_BULLET_MOVE;

public:
	SelfBullet();
	~SelfBullet(){};
	bool Dispatch(TouchObj *other){
		return(other->Touch(this));
	}
};

class SelfNormalBullet : public SelfBullet
{
public:
	bool Draw(void);
	/**
	* ���ۂɔ��������
	*/
	bool Touch(Zetsubou *other);
};

class SelfGravifiedBullet : public SelfBullet
{
public:
	bool Draw(void);
	/**
	* ���ۂɔ��������
	*/
	bool Touch(Zetsubou *other);
};

class FoeBullet : public MyDrawObj, public Character, public TouchObj
{
private:
	double AMOUNT_BULLET_MOVE;
	const int AMOUNT_LAUNCHED_BULLETS;

public:
	FoeBullet();
	~FoeBullet(){};
	bool Draw(void);
	bool Dispatch(TouchObj *other){
		return(other->Touch(this));
	}

	/**
	* �]�������
	*/
	virtual bool Touch(Snowman *other);
	
	//void ChangeDifficulty(DIFFICULTIES &Difficulty);
};

#endif
