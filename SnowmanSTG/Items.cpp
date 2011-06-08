/*�A�C�e���̃N���XItems�̎���*/

#include "stdafx.h"
#include "Items.h"

using namespace Dix;


bool LifeExtend::Draw(void)
{
	if(pos.x < LEFT_EDGE_POSITION - rect.right){		//��ʂ̍��[�ɒB���������
		Clear();
		return true;
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawGraph(static_cast<int>(pos.x), static_cast<int>(pos.y), pic_handles[ITEM_LIFE_EXTEND], true);
	return true;
}

bool LifeExtend::Touch(Snowman *other)
{
	HAZAMA::RECT other_rect = other->GetTouchRect() + other->GetPosition();
	if(IsTouchRectAndRect(touch_rect + pos, other_rect)){
		other->LifeExtended();		//���C�t1�ǉ�
		Clear();
		return true;
	}
	return false;
}

bool Barrier::Draw(void)
{
	if(pos.x < LEFT_EDGE_POSITION - rect.right){		//��ʂ̍��[�ɒB���������
		Clear();
		return true;
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawGraph(static_cast<int>(pos.x), static_cast<int>(pos.y), pic_handles[ITEM_BARRIER], true);
	return true;
}

bool Barrier::Touch(Snowman *other)
{
	HAZAMA::RECT other_rect = other->GetTouchRect() + other->GetPosition();
	if(IsTouchRectAndRect(touch_rect + pos, other_rect)){
		other->BarrierOn();				//�o���A�[���I���ɂ���
		Clear();
		return true;
	}
	return false;
}

bool Quick::Draw(void)
{
	if(pos.x < LEFT_EDGE_POSITION - rect.right){		//��ʂ̍��[�ɒB���������
		Clear();
		return true;
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawGraph(static_cast<int>(pos.x), static_cast<int>(pos.y), pic_handles[ITEM_QUICK], true);
	return true;
}

bool Quick::Touch(Snowman *other)
{
	HAZAMA::RECT other_rect = other->GetTouchRect() + other->GetPosition();
	if(IsTouchRectAndRect(touch_rect + pos, other_rect)){
		other->SetSpeed(600.0f);
		Clear();
		return true;
	}
	return false;
}

bool TriBullets::Draw(void)
{
	if(pos.x < LEFT_EDGE_POSITION - rect.right){		//��ʂ̍��[�ɒB���������
		Clear();
		return true;
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawGraph(static_cast<int>(pos.x), static_cast<int>(pos.y), pic_handles[ITEM_TRI_BULLETS], true);
	return true;
}

bool TriBullets::Touch(Snowman *other)
{
	HAZAMA::RECT other_rect = other->GetTouchRect() + other->GetPosition();
	if(IsTouchRectAndRect(touch_rect + pos, other_rect)){
		other->SetBulletType(BULLETTYPE::TRIBULLETS);
		Clear();
		return true;
	}
	return false;
}

bool Eliminator::Draw(void)
{
	if(pos.x < LEFT_EDGE_POSITION - rect.right){		//��ʂ̍��[�ɒB���������
		Clear();
		return true;
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawGraph(static_cast<int>(pos.x), static_cast<int>(pos.y), pic_handles[ITEM_ELIMINATOR], true);
	return true;
}

bool Eliminator::Touch(Snowman *other)
{
	HAZAMA::RECT other_rect = other->GetTouchRect() + other->GetPosition();
	if(IsTouchRectAndRect(touch_rect + pos, other_rect)){
		sp<ScoreObj> sp_dummy(new ScoreObj(50, SCORE_HALF));
		Score::Instance().AddObj(sp_dummy, true);		//�_�~�[�̃I�u�W�F�N�g���Z�b�g
		Clear();
		return true;
	}
	return false;
}

bool Multiplier::Draw(void)
{
	if(pos.x < LEFT_EDGE_POSITION - rect.right){		//��ʂ̍��[�ɒB���������
		Clear();
		return true;
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawGraph(static_cast<int>(pos.x), static_cast<int>(pos.y), pic_handles[ITEM_MULTIPLIER], true);
	return true;
}

bool Multiplier::Touch(Snowman *other)
{
	HAZAMA::RECT other_rect = other->GetTouchRect() + other->GetPosition();
	if(IsTouchRectAndRect(touch_rect + pos, other_rect)){
		Score::Instance().SetManyTimes();
		Clear();
		return true;
	}
	return false;
}

bool GravifiedBullet::Draw(void)
{
	if(pos.x < LEFT_EDGE_POSITION - rect.right){		//��ʂ̍��[�ɒB���������
		Clear();
		return true;
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawGraph(static_cast<int>(pos.x), static_cast<int>(pos.y), pic_handles[ITEM_GRAVIFIED], true);
	return true;
}

bool GravifiedBullet::Touch(Snowman *other)
{
	HAZAMA::RECT other_rect = other->GetTouchRect() + other->GetPosition();
	if(IsTouchRectAndRect(touch_rect + pos, other_rect)){
		other->SetBulletType(BULLETTYPE::GRAVIFIED);
		Clear();
		return true;
	}
	return false;
}

bool EliminatorX::Draw(void)
{
	if(pos.x < LEFT_EDGE_POSITION - rect.right || (pos.y <= UPPER_EDGE_POSITION - rect.down) 
		|| (pos.y >= LOWER_EDGE_POSITION)){	//��ʂ̍��[�܂��͏�[�A���[�ɒB���������
			Clear();
			return true;
	}

	if(vect.x > 0 && count_start == 0){		//�v���C���[���������E���ɂ�����A�������u�N��
		p_pic_handle = &pic_handles[ITEM_ELIMINATOR_X2];
		destruct_flag = true;
		count_start = GetNowCount();
		SetVector(HAZAMA::V2Get(0, 0));			//�Î~������
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawGraph(static_cast<int>(pos.x), static_cast<int>(pos.y), *p_pic_handle, true);
	return true;
}

bool EliminatorX::Touch(Snowman *other)
{
	HAZAMA::RECT other_rect = other->GetTouchRect() + other->GetPosition();
	if(!destruct_flag && IsTouchRectAndRotatedRect(other_rect, rect + pos)){
		sp<ScoreObj> sp_dummy(new ScoreObj(50, SCORE_QAURTER_HALF));
		Score::Instance().AddObj(sp_dummy, true);		//�_�~�[�̃I�u�W�F�N�g���Z�b�g
		other->BarrierOff(true);
		Clear();
		return true;
	}
	return false;
}