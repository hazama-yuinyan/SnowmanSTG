/*���@�ł���Snowman�N���X�̃w�b�_�[�t�@�C��*/

#ifndef _SNOWMAN_H
#define _SNOWMAN_H

#include "MyDrawObj.h"
#include "TouchObj.h"

/**
* �L�����N�^�[�S���̊�{�N���X
*/
class Character
{
protected:
	HAZAMA::VECTOR2 pos;			//!< @brief �L�����N�^�[�̈ʒu�i�ʒu�x�N�g���j
	HAZAMA::VECTOR2 vect;		//!< @brief �L�����N�^�[�̓������x
	double speed;		//!< @brief �L�����N�^�[�̈ړ����x(�P�ʂ�pixels/s)

public:

	/**
	* �R���X�g���N�^ �ʒu�⑬�x�ɂƂ肠����0�����Ă���
	*/
	Character(){};
	
	/*
	* �ʒu��ݒ肷��
	* @param Pos �ݒ肷��ʒu��\��VECTOR2�\����
	*/
	void SetPosition(const HAZAMA::VECTOR2 &Pos){
		pos = Pos;
	}

	/**
	* ������W�������݂̂̈ʒu��ݒ肷��
	* @param Pos �ݒ肷��l
	* @param Ax �ݒ肷�鎲�ix=0, y=1)
	*/
	void SetPosition(const double Pos, const int Ax){
		switch(Ax){
			case 0:
				pos.x = Pos;
				break;
			case 1:
				pos.y = Pos;
				break;
		}
	}

	/*
	* ���x��ݒ肷��
	* @param Vect �ݒ肷�鑬�x��\��VECTOR2�\����
	*/
	void SetVector(const HAZAMA::VECTOR2 &Vect){
		vect = Vect;
	}

	/**
	* ������W�������݂̂̑��x��ݒ肷��
	* @param V �ݒ肷��l
	* @param Ax �ݒ肷�鎲�ix=0, y=1)
	*/
	void SetVector(const double V, const int Ax){
		switch(Ax){
			case 0:
				vect.x = V;
				break;
			case 1:
				vect.y = V;
				break;
		}
	}

	void SetSpeed(const double Speed){
		speed = Speed;
	}

	/*
	* ���݂̈ʒu��Ԃ�
	* @return ���݂̈ʒu��\��VECTOR2�\����
	*/
	const HAZAMA::VECTOR2& GetPosition(void) const{return pos;}

	/*
	* ���݂̑��x��Ԃ�
	* @return ���݂̑��x��\��VECTOR2�\����
	*/
	const HAZAMA::VECTOR2& GetVector(void) const{return vect;}

	const double &GetSpeed(void) const{return speed;}
};


enum BULLETTYPE{BULLET_NORMAL, TRIBULLETS, GRAVIFIED};

/**
* �v���C���[�����삷��L�����N�^�[�N���X
*/
class Snowman : public Character, public MyDrawObj, public TouchObj
{
private:
	const double AMOUNT_SELF_MOVE;
	const int LIFE_PIC_WIDTH;
	int life;		  ///< @brief �c�@��
	int barrier_off_time;	//!< @brief �o���A�[���I�t�ɂȂ鎞�Ԃ��L�^���Ă����ϐ�
	bool barrier_flag;		//!< @brief �o���A�[���I���ɂȂ��Ă��邩�ǂ������f����ϐ�
	int foes_taken_down;	//!< @brief ���@���|�����G�̐�(�B���p�����[�^�[)
	BULLETTYPE bullet_type;		//!< @brief ���˂���e�̎��

	/**
	* ���ۂɔ��������
	*/
	bool Touch(Zetsubou *other);

	/**
	* ���ۂɔ��������
	*/
	bool Touch(FoeBullet *other);

	/**
	* �]�������
	*/
	bool Touch(Items *other){return true;}

	/**
	* ���ۂɔ��������
	*/
	bool Touch(ExploAnim *other);
	
public:
	Snowman();
	bool Draw(void);

	/**
	* �c�@����0�ɂȂ����̂ŁA�Q�[�����I������悤�w�����o��
	*/
	void MessageGet(OBJ_MSG &msg){msg = GAMES_OVER;}
	bool Dispatch(TouchObj *other){
		return(other->Touch(this));
	}

	void SetBulletType(BULLETTYPE Type){bullet_type = Type;}

	BULLETTYPE GetBulletType(void){return bullet_type;}

	/**
	* ���݂̎c�@����Ԃ�
	*/
	int &GetLife(void){return life;}

	/**
	* �c�@��������₷
	*/
	void LifeExtended(void){
		++life;
	}

	/**
	* �c�@������ʉE��ɃA�C�R���Œ񎦂���
	*/
	void LifeDraw(void);

	/**
	* �o���A�[��L��������
	*/
	void BarrierOn(void);

	/**
	* �o���A�[�𖳌�������
	* @param IsCompelled �����I�ɉ������邩�ǂ���
	*/
	void BarrierOff(bool IsCompelled = false);

	/**
	* �o���A�[����ʂɕ`�悷��
	*/
	void BarrierDraw(void);

	/**
	* �o���A�[���L��������Ă邩
	*/
	bool IsBarrierOn(void){return barrier_flag;}

	/**
	* ����������
	*/
	void Initialize(void);
};

#endif
