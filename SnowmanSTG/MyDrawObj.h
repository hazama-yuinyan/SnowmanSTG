/*���C�u�����ŗp�ӂ����`��Ǘ��ƕ`�悳���I�u�W�F�N�g�̊��N���X��
���̃Q�[���p�Ɋg�������N���X*/

#include "stdafx.h"

#ifndef _MYDRAWOBJ_H
#define _MYDRAWOBJ_H



enum PICTURES{TITLE_BACK, BACK, SNOWMAN, SNOW_BARRIER, ZETSUBOU, SELFBULLET, FOEBULLET, LIFE, ITEM_LIFE_EXTEND
, ITEM_BARRIER, ITEM_QUICK, ITEM_MULTIPLIER, ITEM_ELIMINATOR, ITEM_TRI_BULLETS, ITEM_GRAVIFIED, ITEM_ELIMINATOR_X1
, ITEM_ELIMINATOR_X2, PARA_MULTIPLIER, PIC_END};

enum OBJ_MSG{NOTHING, MOVING, ELIMINATE, GAMES_OVER};

enum MENUS{START_GAME, REPLAY_GAME, MENU_END};

/**
* �`����N���X
*/
class CDrawObj
{
protected:
	HAZAMA::RECT rect;	//!< @brief �`�悷��摜�̑傫��
	bool draw_flag;		//!< @brief �`�悷�邩�ǂ���
	int draw_priority_num;	//!< @brief �`�掞�ɍl�����ׂ��D�揇��(0�`5�܂ł�6�i�K)�ԍ����������قǐ�ɕ`�悳���

	/**
	* �R���X�g���N�^�i�v���e�N�g�錾�j
	*/
	CDrawObj(const int PriorityNum) : draw_flag(true), draw_priority_num(PriorityNum){};

public:
	virtual ~CDrawObj(void){};

	/**
	* �`�悵�Ȃ��悤�ɐݒ肷��
	*/
	void Clear(void){draw_flag = false;}

	bool IsNotDrawn(void){
		return HAZAMA::ReverseFlag(draw_flag);
	}
	/**
	* �摜�̑傫����Ԃ�
	*/
	const HAZAMA::RECT &GetRect(void) const{
		return rect;
	}

	/**
	* �`��֐�
	* @return �������b�Z�[�W�𔭂������ꍇ�ɂ�false��Ԃ�
	*/
	virtual bool Draw() = 0;

	/**
	* ��̓I�ȃ��b�Z�[�W�𔭂���
	* @param msg ��̓I�ȃ��b�Z�[�W������ϐ�
	*/
	virtual void MessageGet(OBJ_MSG &msg){msg = NOTHING;}

	/**
	* �`��D�揇�ʂ̔�r�̂��߂̉��Z�q�I�[�o�[���[�h
	*/
	bool operator<(CDrawObj &rhs){
		return(this->draw_priority_num < rhs.draw_priority_num);
	}
};

/**
* ���ʂ̉摜��`�悷��N���X�̊��N���X
*/
class CMyDrawObj : public CDrawObj
{
protected:
	static int pic_handles[PIC_END];					//!< @brief �摜�̃n���h����ێ�����z��
	static int fig_handles[10];				//!< @brief �X�R�A�p�̐����̉摜��ێ�����z��
	
	/**
	* �R���X�g���N�^�i�v���e�N�g�錾�j
	*/
	CMyDrawObj(const int PriorityNum) : CDrawObj(PriorityNum){};

public:
	/**
	* �f�R���X�g���N�^
	*/
	~CMyDrawObj(){};

	/**
	* �I�u�W�F�N�g�Ɏg���摜��z��ɓǂݍ���
	*/
	static void LoadPic(void);
};

class CTitleObj : public CMyDrawObj
{
private:
	int color[MENU_END];

public:
	/**
	* �R���X�g���N�^
	*/
	CTitleObj() : CMyDrawObj(0){
		color[0] = GetColor(255, 0, 0);
		color[1] = GetColor(0, 0, 0);
	};

	/**
	* �`��֐�
	*/
	bool Draw(void);
	void SetStrColor(MENUS menu);
};

class CBackObj : public CMyDrawObj
{
public:
	/**
	* �R���X�g���N�^
	*/
	CBackObj() : CMyDrawObj(0){};

	/**
	* �`��֐�
	*/
	bool Draw(void){
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);		//�d�˂ĕ`��ł���悤�ɂ���
		DrawGraph(0, 0, pic_handles[BACK], false);
		return true;
	}
};

class ScoreObj
{
private:
	const int ADD_SCORE_RATE;		//!< @brief ���̃I�u�W�F�N�g�̊�{�l���|�C���g
	const float OBJ_MANY_TIMES;		//!< @brief ���̃I�u�W�F�N�g�̊�{�{��

public:
	/**
	* �R���X�g���N�^
	* �X�R�A���[�g�Ɣ{����ݒ肷��
	*/
	ScoreObj(int ScoreRate, float ObjManyTimes) : ADD_SCORE_RATE(ScoreRate), OBJ_MANY_TIMES(ObjManyTimes){};

	/**
	* ���̃I�u�W�F�N�g�̊�{�{����Ԃ�
	* �_�~�[�p
	*/
	float GetManyTimes(void){return OBJ_MANY_TIMES;}

	/**
	* ���̃X�R�A�I�u�W�F�N�g�̍��v���_��Ԃ�
	*/
	float GetResult(void){
		return(ADD_SCORE_RATE * OBJ_MANY_TIMES);
	}
};

class Score : public CMyDrawObj
{
private:
	const int RANK_DRAW_START_POS;	//!< @brief �����L���O��`�悵�n�߂���W
	int ranking[10];			//!< @brief �X�R�A�̏��ʗp�̃_�~�[
	int read_scores[10];		//!< @brief �t�@�C������ǂݍ��񂾃x�X�g10�̃X�R�A
	int cur_score;				//!< @brief ���݂̃X�R�A
	float many_times;			//!< @brief ���݂̑S�̂̔{��
	std::stack<Dix::sp<ScoreObj> > s_score_obj;	//!< @brief ���ꂩ��ǉ�����X�R�A�̃I�u�W�F�N�g���ꎞ�ۑ����Ă����X�^�b�N
	Dix::sp<ScoreObj> sp_dummy;	//!< @brief Eliminator�ȂǗp�̃_�~�[

	/**
	* �R���X�g���N�^
	*/
	Score();

public:

	/**
	* �f�X�g���N�^
	*/
	~Score(void){};
	
	/**
	* �V���O���g���I�u�W�F�N�g���擾
	*/
	static Score &Instance(void){
		static Score inst;
		return inst;
	}

	/**
	*�X�^�b�N�ɕێ������I�u�W�F�N�g�̏������ɃX�R�A���X�V����
	*/
	void AddScore(void);

	/**
	* �X�R�A�̃I�u�W�F�N�g��ǉ�����
	*/
	void AddObj(Dix::sp<ScoreObj> &sp, bool IsDummy = false){
		IsDummy ? sp_dummy = sp : s_score_obj.push(sp);
	}

	/**
	* �_�~�[�̃I�u�W�F�N�g��Ԃ�
	*/
	Dix::sp<ScoreObj> &GetDummyObj(void){return sp_dummy;}

	/**
	* ���݂̔{����O�̔{����2�{�ɃZ�b�g����
	*/
	void SetManyTimes(void){
		many_times *= 2.0f;
	}

	int GetCurScore(void){return cur_score;}

	/**
	* �`��֐�
	*/
	bool Draw(void);

	/**
	* ���݂̔{������ʍ���ɃA�C�R���Ƃ��ĕ`�悷��
	*/
	void MultiplierDraw(void);

	/**
	* �e�L�X�g�t�@�C���Ƀ����L���O����������
	*/
	void WriteFile(void);

	/**
	* �e�L�X�g�t�@�C�����烉���L���O��ǂݍ���
	*/
	void ReadFile(void);

	/**
	* ���݂̃X�R�A�ƃ����L���O�̃X�R�A���ׂď��ʏ��ɕ��בւ���
	*/
	void PutInOrder(void);

	/**
	* �t�@�C������ǂݍ��񂾃X�R�A�̃����L���O����ʂɕ`�悷��
	*/
	void RankDraw(void);

	void Reset(void){
		cur_score = 0;
		many_times = SCORE_NORMAL;
	}

	void Initialize(void){
		many_times = SCORE_NORMAL;
	}
};

#endif
