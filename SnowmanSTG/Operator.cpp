#include "stdafx.h"
#include "Operator.h"


using namespace HAZAMA;

COperatorStraight::COperatorStraight(void)
{
	p_timer = &CTimerForGames::Instance();
}

void COperatorStraight::Operate(void)
{
	pos += vect * p_timer->GetFrameTime() / 1000.0f;
}

void COperatorChaseAfter::Operate(void)
{
	VECTOR2 the_right = FigureTargetDirection(vect);

	if(V2Cross(vect, the_right) > 0.0f){		//�ڕW�̃x�N�g����荶���ɂ���Ȃ�A�E������
		vect = Rotate(vect, 0.02f*M_PI);		//�E���ɂ���Ȃ�A�������֏�������]������
	}else if(V2Cross(vect, the_right) < 0.0f){
		vect = Rotate(vect, -(0.02f*M_PI));
	}

	vect *= speed;		//�����x�N�g�����瑬�x�x�N�g���ɕϊ�
}

VECTOR2 COperatorChaseAfter::FigureTargetDirection(VECTOR2 &Vect)
{
	if(pos > other_pos - 0.25f && pos < other_pos + 0.25f){	//������x�̋�����������ĂȂ��Ȃ珈�����Ȃ�
		VECTOR2 dummy;
		return (dummy(0, 0));
	}

	Vect = V2Normalize(Vect);	//�^����ꂽ�x�N�g���𐳋K������

	/*��̃I�u�W�F�N�g�̒��S�_�Ԃ�x���W,y���W�̍����o����
	���̍�����钼�p�O�p�`�̋t���ڂ����߂�*/
	float self_center[2], other_center[2];

	self_center[0] = pos.x + self_rect.right * Vect.x / 2.0f, self_center[1] = pos.y + self_rect.down * Vect.y / 2.0f;
	other_center[0] = other_pos.x + other_rect.right / 2.0f, other_center[1] = other_pos.y + other_rect.down / 2.0f;

	float direction = std::atan2(other_center[1] - self_center[1], other_center[0] - self_center[0]);

	//���߂��p�x�����ɕ����x�N�g�����o��
 	return ConvertAngleToVect(direction);
}

void COperatorApplyGravity::Operate(void)
{
	vect.y += AMOUNT_GRAVITY * p_timer->GetFrameTime() / 1000.0f;		//1�t���[��������������
}