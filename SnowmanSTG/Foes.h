/*�G�����̃N���XFoes�Ƃ��̔h���N���X�̃w�b�_�[�t�@�C��*/

#ifndef _FOES_H
#define _FOES_H

#include "Snowman.h"
#include "TouchObj.h"
#include "Bullets.h"
#include "MyDrawObj.h"

/**
* �G�̈��ł���Zetsubou�N���X
* �G�̒��ł͍Ŏ�̑���
*/
class Zetsubou : public Character, public MyDrawObj, public TouchObj
{
private:
	double AMOUNT_FOE_MOVE;		//!< @brief �G��1�t���[���ɓ�����
	int launched_bullets;	//!< @brief �A���Ŕ��˂����e�̐�

	/**
	* �]�������
	*/
	bool Touch(Snowman *other);

	/**
	* �]�������
	*/
	bool Touch(SelfBullet *other);

	/**
	* ���ۂɔ��������
	*/
	bool Touch(ExploAnim *other);

public:

	/**
	* �R���X�g���N�^  �����ʒu�̐ݒ�Ȃǂ��s��
	*/
	Zetsubou();

	/**
	* �G����ʂɕ`�悷�郁�\�b�h
	*/
	bool Draw(void);

	bool Dispatch(TouchObj *other){
		return(other->Touch(this));
	}

	/**
	* ��Փx��ύX���郁�\�b�h
	* @param Difficulty ���݂̓�Փx
	*/
	//void ChangeDifficulty(DIFFICULTIES &Difficulty);
};

#endif
