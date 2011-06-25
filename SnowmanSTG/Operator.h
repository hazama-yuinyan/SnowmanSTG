#ifndef _OPERATOR_H
#define _OPERATOR_H


/**
* �I�u�W�F�N�g�����ۂɓ������I�y���[�^�[�̊��N���X
*/
class IOperator
{
public:
	IOperator(void){};
	virtual ~IOperator(void){};
	virtual void Operate(void) = 0;
};

/**
* �ړ��֘A�̃I�y���[�^�[���N���X
*/
class MoveOperator : public IOperator
{
protected:
	HAZAMA::VECTOR2 pos;
	HAZAMA::VECTOR2 vect;
	double speed;

	//�ȉ��A�K�v�ȂƂ��̂݃Z�b�g����ϐ��Q
	HAZAMA::RECT self_rect;			//���g�̎l�p�`�̑��΍��W
	HAZAMA::VECTOR2 other_pos;		//�����ȊO�̃I�u�W�F�N�g�̈ʒu
	HAZAMA::RECT other_rect;		//�����ȊO�̎l�p�`�̑��΍��W

public:
	/**
	* �v�Z�������ʂ�Ԃ�
	*/
	HAZAMA::VECTOR2 GetPosition(void){
		return pos;
	}

	HAZAMA::VECTOR2 GetVector(void){
		return vect;
	}

	/**
	* �I�u�W�F�N�g�̌��݈ʒu���Ԏ؂肷��
	*/
	void SetPosition(const HAZAMA::VECTOR2 &Position){
		pos = Position;
	}

	/**
	* �I�u�W�F�N�g�̑��x�x�N�g�����Ԏ؂肷��
	*/
	void SetVector(const HAZAMA::VECTOR2 &Vect){
		vect = Vect;
	}

	/**
	* �I�u�W�F�N�g�̊��葬�x���Ԏ؂肷��
	*/
	void SetSpeed(const double Speed){
		speed = Speed;
	}

	/**
	* �ߏ�ɉ������Ȃ��悤�Ɍ������Ē�������
	* @param Const ��ƂȂ�X�s�[�h
	*/
	void AdjustVector(const double Const){
		if(vect.x > Const){
			vect(Const, vect.y);
		}
		if(vect.x < -Const){
			vect(-Const, vect.y);
		}
		if(vect.y > Const){
			vect(vect.x, Const);
		}
		if(vect.y < -Const){
			vect(vect.x, -Const);
		}
	}

	//�ȉ��A�K�v�ȂƂ��̂ݐݒ肷��֐��Q
	/**
	* �����ȊO�̃I�u�W�F�N�g�̈ʒu���Z�b�g����
	*/
	void SetTargetPosition(const HAZAMA::VECTOR2 &TargetPos){
		other_pos = TargetPos;
	}

	/**
	* �����⑼�l�̎l�p�`�̒��_�̑��΍��W���Z�b�g����
	*/
	void SetRects(const HAZAMA::RECT &SelfRect, const HAZAMA::RECT &OtherRect){
		self_rect = SelfRect;
		other_rect = OtherRect;
	}
};

/**
* ���݂̈ړ������ɂ܂������ɃI�u�W�F�N�g�𓮂���
*/
class OperatorStraight : public MoveOperator
{
private:
	TimerForGames *p_timer;
	
public:
	OperatorStraight(void);
	
	/**
	* �ړ������Ƒ����ɏ]���Č��ݒn��ω�������
	*/
	void Operate(void);
};

/**
* ���ɓ����悤�d������
*/
class OperatorGoLeft : public MoveOperator
{
public:
	OperatorGoLeft(void){};
	void Operate(void){
		vect.x += -1.0f * speed;
		AdjustVector(speed);
	}
};

/**
* ��ɓ����悤�d������
*/
class OperatorGoUp : public MoveOperator
{
public:
	OperatorGoUp(void){};
	void Operate(void){
		vect.y += -1.0f * speed;
		AdjustVector(speed);
	}
};

/**
* �E�ɓ����悤�d������
*/
class OperatorGoRight : public MoveOperator
{
public:
	OperatorGoRight(void){};
	void Operate(void){
		vect.x += 1.0f * speed;
		AdjustVector(speed);
	}
};

/**
* ���ɓ����悤�d������
*/
class OperatorGoDown : public MoveOperator
{
public:
	OperatorGoDown(void){};
	void Operate(void){
		vect.y += 1.0f * speed;
		AdjustVector(speed);
	}
};

/**
* �ق��̃I�u�W�F�N�g��ǂ�������悤�ȋ�������������N���X
*/
class OperatorChaseAfter : public MoveOperator
{
private:
	/**
	* �^�[�Q�b�g�����݂������������o��
	* ��������]���āA�^�[�Q�b�g�͉�]���Ȃ��Ƃ��̂ݎg�p��
	* @param Vect ���݂̎����̑��x�x�N�g��
	*/
	HAZAMA::VECTOR2 FigureTargetDirection(HAZAMA::VECTOR2 &Vect);

public:
	OperatorChaseAfter(void){};
	void Operate(void);
};

/**
* �d�͂��󂯂ĉ������ɉ������鋓������������N���X
*/
class OperatorApplyGravity : public MoveOperator
{
private:
	const float AMOUNT_GRAVITY;		//�d�͂̑傫��(�P�ʂ�pixels/x)
	TimerForGames *p_timer;

public:
	OperatorApplyGravity(float g = 294.0f) : AMOUNT_GRAVITY(g){
		p_timer = &TimerForGames::Instance();
	}
	void Operate(void);
};

#endif
