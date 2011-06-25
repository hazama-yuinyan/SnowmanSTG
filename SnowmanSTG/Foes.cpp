/*Zetsubouクラスの実装*/

#include "stdafx.h"
#include "Foes.h"
#include "AnimObj.h"



Zetsubou::Zetsubou() : MyDrawObj(1), AMOUNT_FOE_MOVE(300.0), launched_bullets(0)
{
	const double RAND = static_cast<double>(HAZAMA::draw_helper->GetRand(MAX_INITIAL_POS_Y));
	speed = AMOUNT_FOE_MOVE;
	pos(INITIAL_FOE_POSITION, RAND);
	vect(-1.0 * speed, 0);
	rect(0, 0, 48.0, 64.0);
	touch_rect(0, 0, 48.0, 64.0);
}

bool Zetsubou::Draw(void)
{
	if(pos.x < LEFT_EDGE_POSITION - rect.right){		//画面の左端に達したら消す
		Clear();
		return true;
	}

	HAZAMA::draw_helper->SetDrawBlendMode(HAZAMA::DrawHelper::NO_BLEND, 0);
	HAZAMA::draw_helper->DrawImage(static_cast<int>(pos.x), static_cast<int>(pos.y), pic_handles[ZETSUBOU], true);
	return true;
}

bool Zetsubou::Touch(Snowman *other)
{
	if(!other->IsBarrierOn()){
		Clear();				
		return true;
	}else if(other->IsBarrierOn()){
		Clear();				
		Dix::sp<ScoreObj> sp_score_obj(new ScoreObj(50, SCORE_HALF));
		Score::Instance().AddObj(sp_score_obj);
	}

	return false;
}

bool Zetsubou::Touch(SelfBullet *other)
{
	Clear();
	Dix::sp<ScoreObj> sp_score_obj(new ScoreObj(50, SCORE_NORMAL));
	Score::Instance().AddObj(sp_score_obj);
	return true;
}

bool Zetsubou::Touch(ExploAnim *other)
{
	HAZAMA::RECT other_rect = other->GetTouchRect() + other->GetPosition();
	if(IsTouchRectAndRect(touch_rect + pos, other_rect)){
		Clear();
		return true;
	}
	return false;
}


/*
void Zetsubou::ChangeDifficulty(DIFFICULTIES &Difficulty){
	switch(Difficulty){
		case EASY:
			MAX_NUM_FOES += 3;
			AMOUNT_FOE_MOVE += 1.25f;
			break;
		case NORMAL:
			MAX_NUM_FOES += 3;
			AMOUNT_FOE_MOVE += 1.25f;
			break;
		case HARD:
			MAX_NUM_FOES += 3;
			AMOUNT_FOE_MOVE += 1.25f;
			break;
		case VERY_HARD:
			MAX_NUM_FOES += 3;
			AMOUNT_FOE_MOVE += 1.25f;
			break;
		default:
			return;
	}
}*/