#ifndef _ANIMOBJ_H
#define _ANIMOBJ_H

#include "MyDrawObj.h"
#include "TouchObj.h"


enum ANIMATIONS{SELF_GOT_HIT, FIRE, EXPLOSION, MAX_FRAMES};

typedef int anim_handle;


/**
* �ʏ�A�j���[�V�����p�̊��N���X
* �Ɨ������A�j���[�V�����̕`��ɂ͂��̃N���X��P�ƌp�����Ď�������
*/
class CAnimObj : public CDrawObj
{
protected:
	static std::vector<Dix::sp<anim_handle> > v_sp_anim;
	HAZAMA::VECTOR2 pos;

	/**
	* �R���X�g���N�^�i�v���e�N�g�錾�j
	*/
	CAnimObj(const int PriorityNum) : CDrawObj(PriorityNum){};

public:
	/**
	* �f�R���X�g���N�^
	*/
	~CAnimObj(){};

	/*
	* �ʒu��ݒ肷��
	* @param Pos �ݒ肷��ʒu��\��VECTOR2�\����
	*/
	void SetPosition(const HAZAMA::VECTOR2 &Pos){
		pos = Pos;
	}

	/*
	* ���݂̈ʒu��Ԃ�
	* @return ���݂̈ʒu��\��VECTOR2�\����
	*/
	const HAZAMA::VECTOR2& GetPosition(void) const{return pos;}

	/**
	* �A�j���[�V�����Ɏg���摜��ǂݍ���
	*/
	static void LoadPic(void);
};

/*class ICooperativeAnim	//���̃N���X�̂�����Factory�N���X�Ɋi�[�����I�u�W�F�N�g�̈ꕔ�ɕs�����������\������
{
protected:
	ICooperativeAnim(void){};

public:
	virtual ~ICooperativeAnim(void){};
	/**
	* ���̃t���[���Ɉڂ�
	*/
	/*virtual void AdvanceFrame(void) = 0;

	/**
	* ���݂̃t���[����`�悷��
	*/
	/*virtual void DrawAFrame(void) = 0;
};*/

/**
* �ق��̃I�u�W�F�N�g��`�悵�A�����ɃA�j���[�V�������Đ��������ꍇ�ɂ͂��̃N���X���p������
*/
class CCooperativeAnimObj : public CAnimObj
{
protected:
	int cur_frame;		//!< @brief ���݂̃t���[����
	int MAX_FRAME;		//!< @brief �`�悷��A�j���[�V�����̍ő�t���[����
	Dix::wp<anim_handle> wp_anim;	//!< @brief �`�悷��A�j���[�V�����̉摜��ێ�����z��

	void SetAnim(ANIMATIONS Anim){
		wp_anim = v_sp_anim[Anim];
		MAX_FRAME = v_sp_anim[MAX_FRAMES][Anim] - 1;
	}

public:
	CCooperativeAnimObj(const int PriorityNum) : CAnimObj(PriorityNum), cur_frame(0){};
	virtual ~CCooperativeAnimObj(void){};
	
	void AdvanceFrame(void){++cur_frame;}

	virtual void DrawAFrame(void) = 0;
};

class CSelfGotHitAnim : public CAnimObj
{
public:
	CSelfGotHitAnim();

	/**
	* �`��֐�
	*/
	bool Draw(void);

	/**
	* �A�j���[�V�����Đ����I������̂ŁA�ق��̃^�X�N�̊������ĊJ����悤���b�Z�[�W�𔭂���
	*/
	void MessageGet(OBJ_MSG &msg){msg = MOVING;}
};

class CFireAnim : public CCooperativeAnimObj
{
private:
	bool play_elimx_SE_flag;

public:
	CFireAnim(void);
	virtual ~CFireAnim(void){};

	/**
	* �`��֐�
	*/
	bool Draw(void);

	/**
	* �A�j���[�V�����Đ����I������̂ŁA�ق��̃^�X�N���ēx���������Aeliminator�p�̓��ʂ�
	* �^�X�N�����s����悤�w������
	*/
	void MessageGet(OBJ_MSG &msg){msg = ELIMINATE;}

	void DrawAFrame(void);

	void SetSEType(bool Flag){play_elimx_SE_flag = Flag;}
};

class CExploAnim : public CCooperativeAnimObj, public CTouchObj
{
private:
	bool Touch(Snowman *other){return true;}
	bool Touch(Zetsubou *other){return true;}

public:
	CExploAnim(void);
	virtual ~CExploAnim(void){};

	/**
	* �`��֐�
	*/
	bool Draw(void);

	void DrawAFrame(void);
	bool Dispatch(CTouchObj *other){
		return(other->Touch(this));
	}
};

#endif
