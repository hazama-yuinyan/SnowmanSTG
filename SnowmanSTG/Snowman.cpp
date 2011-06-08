/*Snowmanクラスの実装*/

#include "stdafx.h"
#include "Snowman.h"
#include "Foes.h"
#include "Items.h"
#include "AnimObj.h"


Snowman::Snowman() : CMyDrawObj(1), AMOUNT_SELF_MOVE(300.0f), barrier_flag(false), LIFE_PIC_WIDTH(24), life(3)
, bullet_type(BULLET_NORMAL)
{
	Initialize();
	rect(0, 0, 96.0f, 96.0f);
	touch_rect(20.0f, 16.0f, 86.0f, 80.0f);
}

void Snowman::Initialize(void)
{
	SetSpeed(AMOUNT_SELF_MOVE);
	SetPosition(HAZAMA::V2Get(INITIAL_SELF_POSITION_X, INITIAL_SELF_POSITION_Y));
	vect(0, 0);
	bullet_type = BULLET_NORMAL;
}

bool Snowman::Draw(void)
{
	if(life <= 0){
		return false;
	}

	//画面からはみ出さないように検査する
	if(GetPosition().x < LEFT_EDGE_POSITION){
		SetPosition(LEFT_EDGE_POSITION, 0);
	}
	if(GetPosition().y < UPPER_EDGE_POSITION){
		SetPosition(UPPER_EDGE_POSITION, 1);
	}
	if(GetPosition().x + rect.right > RIGHT_EDGE_POSITION){
		SetPosition(RIGHT_EDGE_POSITION - rect.right, 0);
	}
	if(GetPosition().y + rect.down > LOWER_EDGE_POSITION){
		SetPosition(LOWER_EDGE_POSITION - rect.down, 1);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawGraph(static_cast<int>(pos.x), static_cast<int>(pos.y), pic_handles[SNOWMAN], true);

	LifeDraw();

	if(barrier_flag){
		BarrierOff();
		BarrierDraw();
	}
	return true;
}

bool Snowman::Touch(Zetsubou *other)
{
	HAZAMA::RECT other_rect = other->GetTouchRect() + other->GetPosition();
	if(!IsBarrierOn() && IsTouchRectAndRect(touch_rect + pos, other_rect)){
		--life;							//バリアーがオフなら普通の矩形の当たり判定を取る
		Score::Instance().Initialize();
		return true;
	}else if(IsBarrierOn() && IsTouchCircleAndRect(pos, rect, other_rect)){
		return true;					//バリアーがオンなら円形の当たり判定を取る
	}else{
		return false;
	}
}

bool Snowman::Touch(FoeBullet *other)
{
	HAZAMA::RECT other_rect = other->GetTouchRect() + other->GetPosition();
	if(!IsBarrierOn() && IsTouchRectAndRect(touch_rect + pos, other_rect)){
		--life;							//バリアーがオフなら普通の矩形の当たり判定を取る
		Score::Instance().Initialize();
		return true;
	}else if(IsBarrierOn() && IsTouchCircleAndRect(pos, rect, other_rect)){
		return true;					//バリアーがオンなら円形の当たり判定を取る
	}else{
		return false;
	}
}

bool Snowman::Touch(CExploAnim *other)
{
	HAZAMA::RECT other_rect = other->GetTouchRect() + other->GetPosition();
	if(IsTouchRectAndRect(touch_rect + pos, other_rect)){
		--life;
		return true;
	}
	return false;
}

void Snowman::LifeDraw(void)
{
	int i, x;
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	x = RIGHT_EDGE_POSITION - life * LIFE_PIC_WIDTH;
	for(i = 0; i < life; ++i){
		DrawGraph(x, 0, pic_handles[LIFE], true);
		x += LIFE_PIC_WIDTH;
	}
}

void Snowman::BarrierOn(void)
{
	barrier_flag= true;
	barrier_off_time = GetNowCount() + 10 * 1000;
}

void Snowman::BarrierOff(bool IsCompelled)
{
	if(IsCompelled){
		barrier_flag = false;
	}else{
		int now = GetNowCount();

		if(now >= barrier_off_time){
			barrier_flag = false;
		}
	}
}

void Snowman::BarrierDraw(void)
{
	int remaining_time = barrier_off_time - GetNowCount(), a;

	if((remaining_time <= 3000) && (remaining_time >= 1000)){	//バリアーの持続時間が残り3秒を切ったら点滅させる
		if(remaining_time % 200 >= 100){
			a = 0;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, a);
		}else{
			SetDrawBlendMode(DX_BLENDMODE_ADD, 128);
		}
	}else if(remaining_time <= 1000){
		if(remaining_time % 100 >= 50){
			a = 0;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, a);
		}else{
			SetDrawBlendMode(DX_BLENDMODE_ADD, 128);
		}
	}else{
		SetDrawBlendMode(DX_BLENDMODE_ADD, 128);
	}
		
	DrawGraph(static_cast<int>(pos.x), static_cast<int>(pos.y), pic_handles[SNOW_BARRIER], true);
}