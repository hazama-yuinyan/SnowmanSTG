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

	if(V2Cross(vect, the_right) > 0.0){		//目標のベクトルより左側にあるなら、右方向へ
		vect = Rotate(vect, 0.02 * M_PI);		//右側にあるなら、左方向へ少しずつ回転させる
	}else if(V2Cross(vect, the_right) < 0.0){
		vect = Rotate(vect, -(0.02 * M_PI));
	}

	vect *= speed;		//方向ベクトルから速度ベクトルに変換
}

VECTOR2 OperatorChaseAfter::FigureTargetDirection(VECTOR2 &Vect)
{
	if(pos > other_pos - 0.25 && pos < other_pos + 0.25){	//ある程度の距離しか離れてないなら処理しない
		VECTOR2 dummy;
		return (dummy(0, 0));
	}

	Vect = V2Normalize(Vect);	//与えられたベクトルを正規化する

	/*二つのオブジェクトの中心点間のx座標,y座標の差を出して
	その差が作る直角三角形の逆正接を求める*/
	double self_center[2], other_center[2];

	self_center[0] = pos.x + self_rect.right * Vect.x / 2.0, self_center[1] = pos.y + self_rect.down * Vect.y / 2.0;
	other_center[0] = other_pos.x + other_rect.right / 2.0, other_center[1] = other_pos.y + other_rect.down / 2.0;

	double direction = std::atan2(other_center[1] - self_center[1], other_center[0] - self_center[0]);

	//求めた角度を元に方向ベクトルを出す
 	return ConvertAngleToVect(direction);
}

void OperatorApplyGravity::Operate(void)
{
	vect.y += AMOUNT_GRAVITY * p_timer->GetFrameTime() / 1000.0;		//1フレーム分加速させる
}