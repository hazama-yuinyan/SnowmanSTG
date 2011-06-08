/*���@�ƓG��Bullet�N���X�̎���*/

#include "stdafx.h"
#include "Bullets.h"
#include "Foes.h"

using namespace HAZAMA;


SelfBullet::SelfBullet(void) : CMyDrawObj(3), AMOUNT_BULLET_MOVE(600.0f)
{
	speed = AMOUNT_BULLET_MOVE;
	vect(1.0f * speed, 0);
	rect(0, 0, 32.0f, 32.0f);
	touch_rect(4.0f, 4.0f, 28.0f, 28.0f);
}

bool SelfNormalBullet::Draw(void)
{
	if((pos.x >= RIGHT_EDGE_POSITION) || (pos.y <= UPPER_EDGE_POSITION - 32.0f) 
			|| (pos.y >= LOWER_EDGE_POSITION)){	//��ʂ̉E�[�܂��͏�[�A���[�ɒB���������
				Clear();
			return true;
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawGraph(static_cast<int>(pos.x), static_cast<int>(pos.y), pic_handles[SELFBULLET], true);
	return true;
}

bool SelfNormalBullet::Touch(Zetsubou *other)
{
	HAZAMA::RECT other_rect = other->GetTouchRect() + other->GetPosition();
	if(IsTouchRectAndRect(touch_rect + pos, other_rect)){
		Clear();
		return true;
	}
	return false;
}

bool SelfGravifiedBullet::Draw(void)
{
	if((pos.x >= RIGHT_EDGE_POSITION) || (pos.y <= UPPER_EDGE_POSITION - 32.0f) 
			|| (pos.y >= LOWER_EDGE_POSITION)){	//��ʂ̉E�[�܂��͏�[�A���[�ɒB���������
				Clear();
			return true;
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawGraph(static_cast<int>(pos.x), static_cast<int>(pos.y), pic_handles[SELFBULLET], true);
	return true;
}

bool SelfGravifiedBullet::Touch(Zetsubou *other)
{
	HAZAMA::RECT other_rect = other->GetTouchRect() + other->GetPosition();
	if(IsTouchRectAndRect(touch_rect + pos, other_rect)){
		Clear();
		return true;
	}
	return false;
}

/*��������G�̒e�̃N���X*/
FoeBullet::FoeBullet(void) : CMyDrawObj(4), AMOUNT_BULLET_MOVE(600.0f), AMOUNT_LAUNCHED_BULLETS(3)
{
	speed = AMOUNT_BULLET_MOVE;
	vect(-1.0f * speed, 0);
	rect(0, 0, 24.0f, 10.0f);
	touch_rect(6.0f, 0, 6.0f, 0);
}

/*void FoeBullet::Move(void){				//���݂̓G�̈ʒu�ɐV�����e��\������
	
	while(p){
		if((homing_flag == false) || (p->pos_x <= 100.0f)){
			p->pos_x -= AMOUNT_BULLET_MOVE;
		}else{
			float f;
			GetSelfDirection(p, &f);
			if((p->direction - M_PI <= f) && ((f < p->direction) || (p->direction + M_PI <= f))){	//�G�̒e�����@�̕����Ɍ�����
				p->direction -= 0.02f * M_PI;
				if(p->direction < -M_PI){
					p->direction += 2 * M_PI;
				}
			}else{
				p->direction += 0.02f * M_PI;
				if(p->direction >= M_PI){
					p->direction -= 2 * M_PI;
				}
			}
			p->pos_x += AMOUNT_BULLET_MOVE * sin(p->direction);
			p->pos_y += AMOUNT_BULLET_MOVE * cos(p->direction);
		}
		if(p->pos_x <= LEFT_EDGE_POSITION){			//��ʂ̍��[�ɒB���������
			p = EraseNext(p);
		}else{
			p = p->p_next;
		}
	}
}*/

bool FoeBullet::Draw(void){
	if((pos.x <= LEFT_EDGE_POSITION - rect.right) || (pos.y <= UPPER_EDGE_POSITION - rect.down) 
			|| (pos.y >= LOWER_EDGE_POSITION)){	//��ʂ̍��[�܂��͏�[�A���[�ɒB���������
				Clear();
			return true;
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawGraph(static_cast<int>(pos.x), static_cast<int>(pos.y), pic_handles[FOEBULLET], true);
	return true;
}

bool FoeBullet::Touch(Snowman *other)
{
	if(!other->IsBarrierOn()){	//�o���A�[���I�t�̎��́APlayer���ꏏ�ɏ���
		Clear();
		return true;
	}else if(other->IsBarrierOn()){	//�o���A�[���I���̎��́A��������������
		Clear();
	}

	return false;
}


/*void FoeBullet::ChangeDifficulty(DIFFICULTIES &Difficulty){
	switch(Difficulty){
		case VERY_EASY:
			Difficulty = VERY_EASY_HELD;
			break;
		case EASY:
			EXTRA_MARGIN -= 3.75f;
			Difficulty = EASY_HELD;
			break;
		case NORMAL:
			EXTRA_MARGIN -= 3.75f;
			Difficulty = NORMAL_HELD;
			break;
		case HARD:
			AMOUNT_BULLET_MOVE = 8.0f;
			homing_flag = true;
			Difficulty = HARD_HELD;
			EXTRA_MARGIN -= 3.75f;
			break;
		case VERY_HARD:
			EXTRA_MARGIN -= 3.75f;
			Difficulty = VERY_HARD_HELD;
		default:
			return;
	}
}*/