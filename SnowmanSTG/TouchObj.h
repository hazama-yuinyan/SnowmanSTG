/*当たり判定を取るTouchクラスのヘッダーファイル*/

#ifndef _TOUCH_OBJ_H
#define _TOUCH_OBJ_H

class Snowman;
class Zetsubou;
class SelfBullet;
class FoeBullet;
class Items;
class CExploAnim;


class CTouchObj : public HAZAMA::ITouch
{
protected:
	HAZAMA::RECT touch_rect;	//!< @brief 当たり判定用の矩形の頂点座標

public:
	CTouchObj(){};
	virtual ~CTouchObj(){};

	HAZAMA::RECT GetTouchRect(void){return touch_rect;}

	virtual bool Touch(Snowman *other){return false;}
	virtual bool Touch(Zetsubou *other){return false;}
	virtual bool Touch(SelfBullet *other){return false;}
	virtual bool Touch(FoeBullet *other){return false;}
	virtual bool Touch(Items *other){return false;}
	virtual bool Touch(CExploAnim *other){return false;}

	virtual bool Dispatch(CTouchObj *other) = 0;
};

#endif
