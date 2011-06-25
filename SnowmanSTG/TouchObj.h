/*当たり判定を取るTouchクラスのヘッダーファイル*/

#ifndef _TOUCH_OBJ_H
#define _TOUCH_OBJ_H

class Snowman;
class Zetsubou;
class SelfBullet;
class FoeBullet;
class Items;
class ExploAnim;


class TouchObj : public HAZAMA::ITouch
{
protected:
	HAZAMA::RECT touch_rect;	//!< @brief 当たり判定用の矩形の頂点座標

public:
	TouchObj(){};
	virtual ~TouchObj(){};

	HAZAMA::RECT GetTouchRect(void){return touch_rect;}

	virtual bool Touch(Snowman *other){return false;}
	virtual bool Touch(Zetsubou *other){return false;}
	virtual bool Touch(SelfBullet *other){return false;}
	virtual bool Touch(FoeBullet *other){return false;}
	virtual bool Touch(Items *other){return false;}
	virtual bool Touch(ExploAnim *other){return false;}

	virtual bool Dispatch(TouchObj *other) = 0;
};

#endif
