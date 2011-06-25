#include "stdafx.h"
#include "Operator.h"


using namespace HAZAMA;

OperatorStraight::OperatorStraight(void)
{
	p_timer = &TimerForGames::Instance();
}

void OperatorStraight::Operate(void)
{
	pos += vect * p_timer->GetFrameTime() / 1000.0;
}

void OperatorChaseAfter::Operate(void)
{
	VECTOR2 the_right = FigureTargetDirection(vect);

	if(V2Cross(vect, the_right) > 0.0){		//�ڕW�̃x�N�g����荶���ɂ���Ȃ�A�E������
		vect = Rotate(vect, 0.02 * M_PI);		//�E���ɂ���Ȃ�A�������֏�������]������
	}else if(V2Cross(vect, the_right) < 0.0){
		vect = Rotate(vect, -(0.02 * M_PI));
	}

	vect *= speed;		//�����x�N�g�����瑬�x�x�N�g���ɕϊ�
}

VECTOR2 OperatorChaseAfter::FigureTargetDirection(VECTOR2 &Vect)
{
	if(pos > other_pos - 0.25 && pos < other_pos + 0.25){	//������x�̋�����������ĂȂ��Ȃ珈�����Ȃ�
		VECTOR2 dummy;
		return (dummy(0, 0));
	}

	Vect = V2Normalize(Vect);	//�^����ꂽ�x�N�g���𐳋K������

	/*��̃I�u�W�F�N�g�̒��S�_�Ԃ�x���W,y���W�̍����o����
	���̍�����钼�p�O�p�`�̋t���ڂ����߂�*/
	double self_center[2], other_center[2];

	self_center[0] = pos.x + self_rect.right * Vect.x / 2.0, self_center[1] = pos.y + self_rect.down * Vect.y / 2.0;
	other_center[0] = other_pos.x + other_rect.right / 2.0, other_center[1] = other_pos.y + other_rect.down / 2.0;

	double direction = std::atan2(other_center[1] - self_center[1], other_center[0] - self_center[0]);

	//���߂��p�x�����ɕ����x�N�g�����o��
 	return ConvertAngleToVect(direction);
}

void OperatorApplyGravity::Operate(void)
{
	vect.y += AMOUNT_GRAVITY * p_timer->GetFrameTime() / 1000.0;		//1�t���[��������������
}