/*自機と敵のBulletクラス*/

#ifndef _BULLETS_H
#define _BULLETS_H

#include "TouchObj.h"
#include "MyDrawObj.h"
#include "Snowman.h"

class SelfBullet : public CMyDrawObj, public CCharacter, public CTouchObj
{
protected:
	const float AMOUNT_BULLET_MOVE;

public:
	SelfBullet();
	~SelfBullet(){};
	bool Dispatch(CTouchObj *other){
		return(other->Touch(this));
	}
};

class SelfNormalBullet : public SelfBullet
{
public:
	bool Draw(void);
	/**
	* 実際に判定を取る方
	*/
	bool Touch(Zetsubou *other);
};

class SelfGravifiedBullet : public SelfBullet
{
public:
	bool Draw(void);
	/**
	* 実際に判定を取る方
	*/
	bool Touch(Zetsubou *other);
};

class FoeBullet : public CMyDrawObj, public CCharacter, public CTouchObj
{
private:
	float AMOUNT_BULLET_MOVE;
	const float AMOUNT_LAUNCHED_BULLETS;

public:
	FoeBullet();
	~FoeBullet(){};
	bool Draw(void);
	bool Dispatch(CTouchObj *other){
		return(other->Touch(this));
	}

	/**
	* 従属する方
	*/
	virtual bool Touch(Snowman *other);
	
	//void ChangeDifficulty(DIFFICULTIES &Difficulty);
};

#endif
