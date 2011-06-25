/*�l�X�Ȏd�����ɂȂ�Task�̃w�b�_�[�t�@�C��*/

#ifndef _TASK_H
#define _TASK_H

#include "TaskSystem.h"
#include "MyDrawObj.h"
#include "Factory.h"
#include "Operator.h"
#include "Foes.h"
#include "Snowman.h"
#include "Bullets.h"
#include "AnimObj.h"
#include "Items.h"

class InputDeviceKeyBoard;

enum TASKIDS{TASK_SNOWMAN, TASK_ZETSUBOU, TASK_SELFBULLET, TASK_FOEBULLET, TASK_ITEM, TASK_TOUCH, TASK_SCORE
, TASK_DRAW, TASK_REPLAY};

enum TASK_MESSAGES{NONE, DONT_MOVE, CAN_MOVE, ELIMINATING, ADD_OPE_APPLYGRAVITY, REMOVE_EXTRA_OPE, ENDING_GAME};

template<class T>
bool operator<(const Dix::sp<T> &lhs, const Dix::sp<T> &rhs)
{
	return(*lhs.GetPtr() < *rhs.GetPtr());
}

template<class T>
bool operator>(const Dix::sp<T> &lhs, const Dix::sp<T> &rhs)
{
	return(*lhs.GetPtr() > *rhs.GetPtr());
}


class TaskBase : public ITaskBase
{
protected:
	TASK_MESSAGES message;

public:
	TaskBase(void) : message(NONE){};
	TASK_MESSAGES GetTaskMessage(void){return message;}
	void ResetMessage(void){message = NONE;}
};

/**
* �^�X�N���Ǘ�����
*/
class TaskSystem : public ITaskSystem
{
private:
	bool continue_flag;		//!< @brief Update�̖߂�l�Ƃ��Ďg�p����ϐ�
	/**
	* �^�X�N�̃��b�Z�[�W���m�F���Ă��̓��e�ɉ������Ή�������
	*/
	void CheckTaskMessage(void);

	/**
	* �ړ��֘A�̃^�X�N�̏�Ԃ���Ăɕω�������
	*/
	void SwitchMovingTasks(bool OnOrOff = false);

public:
	/**
	* �R���X�g���N�^
	* �ړ��I�y���[�^�[���܂ރ^�X�N�͂����Ő�������
	*/
	TaskSystem(InputDeviceKeyBoard *p_key);
	virtual ~TaskSystem(){};

	/**
	* �^�X�N�S�̂�����������
	* �����ŃQ�[�����ŕK�v�ȏ����̏��������s��
	*/
	bool Init(void);

	/**
	* �o�^����Ă���^�X�N����^����ꂽ�^�X�NID�������̂���������
	* ���̃|�C���^��Ԃ�
	*/
	template<class DerivedTask>
	DerivedTask *GetTask(TASKIDS TaskID){
		TASK_MAP::iterator it;
		it = m_mapTask.find(TaskID);
		return dynamic_cast<DerivedTask*>((*it).second.GetPtr());
	}

	/**
	* �������̃^�X�N��S�ă��Z�b�g����
	*/
	void Reset(void){
		TASK_MAP::iterator it = m_mapActTask.begin();
		for(; it != m_mapActTask.end(); ++it){
			(*it).second->Reset();
		}
	}

	/**
	* �w�肵���^�X�N�̃��b�Z�[�W��Ԃ�
	*/
	TASK_MESSAGES GetTaskMessage(TASKIDS TaskID){
		TASK_MAP::iterator it = m_mapActTask.find(TaskID);
		if(it != m_mapActTask.end()){
			Dix::sp<TaskBase> sp_task;
			sp_task.DownCast((*it).second);
			return sp_task->GetTaskMessage();
		}
		return NONE;
	}

	/**
	* �w�肵���^�X�N�̃��b�Z�[�W������������
	*/
	void ResetMessage(TASKIDS TaskID){
		TASK_MAP::iterator it = m_mapActTask.find(TaskID);
		Dix::sp<TaskBase> sp_task;
		sp_task.DownCast((*it).second);
		sp_task->ResetMessage();
	}

	/**
	* �^�X�N���X�V����
	* @return �Q�[�����p�����Ă������ǂ���
	*/
	bool Update(void);

	void SetReplayMode(bool Flag);
};

/**
* �`��^�X�N�N���X
* �`���S������
*/
class DrawTask : public TaskBase
{
private:
	typedef Dix::sp<DrawObj> SPDrawObj;
	std::list<SPDrawObj> l_sp_draw_obj;		//!< @brief �`��I�u�W�F�N�g�N���X
	std::list<SPDrawObj> l_have_msgs_obj;	//!< @brief ���b�Z�[�W�𔭂������I�u�W�F�N�g���X�g

public:
	/**
	* �R���X�g���N�^
	*/
	DrawTask(){
		InittheList();
	}

	/**
	* �f�X�g���N�^
	*/
	virtual ~DrawTask(){
		Reset();
	};

	/**
	* �x�N�^�[�R���e�i�ɓo�^����Ă���I�u�W�F�N�g��`�悷��
	*/
	virtual bool Update(void);

	/**
	* ���b�Z�[�W�����I�u�W�F�N�g�̃��X�g������������
	*/
	void InittheList(void);

	void EraseDummies(void){
		l_have_msgs_obj.remove_if(boost::bind(&DrawTask::IsDummy, this, _1));
	}

	bool IsDummy(SPDrawObj &sp){return(sp.GetPtr() == NULL);}

	/**
	* ���b�Z�[�W�𔭂����I�u�W�F�N�g�����邩�ǂ���
	*/
	bool IsMessaged(void){
		std::list<SPDrawObj>::iterator it = l_have_msgs_obj.begin();
		return((*it).GetPtr() != NULL);
	}

	/**
	* �o�^����Ă���I�u�W�F�N�g��S�ĊO��
	*/
	void Reset(void){
		l_sp_draw_obj.clear();
		l_have_msgs_obj.clear();
	};

	template<class T>
	void Register(Dix::sp<T> &sp){
		SPDrawObj sp_draw(sp);
		std::list<SPDrawObj>::iterator it = std::upper_bound(l_sp_draw_obj.begin(), l_sp_draw_obj.end(), sp_draw);
		l_sp_draw_obj.insert(it, sp_draw);
	}
};

/**
* �I�y���[�^�[���g�p����^�X�N�̊��N���X
*/
class OperatorTask : public TaskBase
{
protected:
	std::vector<Dix::sp<IOperator> > v_sp_ope;	//�������l�C���^�[�t�F�C�X
	std::list<Dix::sp<IOperator> > l_sp_base_ope;	//�W���ݒ�I�y���[�^�[���X�g

	/**
	* �R���X�g���N�^�i�v���e�N�g�錾�j
	*/
	OperatorTask(void){};

public:
	/**
	* �f�X�g���N�^
	*/
	virtual ~OperatorTask(void){
		Reset();
	}

	/**
	* �Z�b�g����Ă���I�y���[�^�[�����ׂă��Z�b�g����
	*/
	void Reset(void){
		v_sp_ope.clear();
		l_sp_base_ope.clear();
	}

	virtual bool Update(void) = 0;

	/**
	* �W���ŃZ�b�g����Ă���I�y���[�^�[�ȊO�����Z�b�g����
	* @param EraseBaseOpeListFlag OperatorStraight�ȊO�̃I�y���[�^�[��W���ݒ胊�X�g����������ǂ��������߂�
	*/
	void ResetOpes(bool EraseBaseOpeListFlag = false){
		if(EraseBaseOpeListFlag && l_sp_base_ope.size() > 1){
			std::list<Dix::sp<IOperator> >::iterator it = l_sp_base_ope.begin();
			l_sp_base_ope.erase(++it, l_sp_base_ope.end());
		}

		unsigned repeat_times = v_sp_ope.size() - l_sp_base_ope.size();
		for(size_t i = 0; i < repeat_times; ++i){	//���X�g�ɓo�^����Ă�����̈ȊO�A�R���e�i����폜����
			v_sp_ope.pop_back();
		}
	}

	/**
	* �������l���Ԏ؂�
	* @param spOpe �Z�b�g����I�y���[�^�[�̃X�}�[�g�|�C���^
	* @param IsBaseFlag �W���ŃZ�b�g�����ׂ��I�y���[�^�[���ǂ���
	*/
	bool SetOperator(Dix::sp<IOperator> spOpe, bool IsBaseFlag = false){
		if(spOpe.GetPtr() == NULL) return false;
		v_sp_ope.push_back(spOpe);
		if(IsBaseFlag){
			l_sp_base_ope.push_back(spOpe);
		}
		m_bUsageFlag = true;
		return true;
	}
};

class SnowmanTask : public OperatorTask
{
private:
	void CreateNormalBullet(Dix::sp<Snowman> &SPSnowman);
	void CreateTriBullets(Dix::sp<Snowman> &SPSnowman);
	void CreateGravifiedBullet(Dix::sp<Snowman> &SPSnowman);

public:
	SnowmanTask(Dix::sp<IOperator> sp_ope = NULL){
		if(sp_ope.GetPtr() != NULL){
			SetOperator(sp_ope, true);
		}
	}

	/**
	* �X�y�[�X�L�[��������Ă�����V���ɒe�𐶐�����
	*/
	void Shoot(void);

	/**
	* �X�V����
	*/
	virtual bool Update();
};

class ZetsubouTask : public OperatorTask
{
private:
	int count_min;
	unsigned MAX_NUM_FOES;			//!< @brief ��ʂɏo���ł���G�̐�
	float EXTRA_MARGIN;		//!< @brief �e���˂̍ۍl������͈̗͂]���̕�
	TimerForGames *p_timer;
	TimerForGames::TimerID timer_id;
	
	/**
	* �V���ȓG���o��������
	*/
	void CreatNewZetsu(void);

public:
	ZetsubouTask(Dix::sp<IOperator> sp_ope = NULL) : count_min(0), EXTRA_MARGIN(20.0f), MAX_NUM_FOES(10), timer_id(0){
		p_timer = &TimerForGames::Instance();
		if(sp_ope.GetPtr() != NULL){
			SetOperator(sp_ope, true);
		}
	}

	/**
	* �����𖞂�������V���ɒe�𐶐�����
	*/
	void Shoot(Dix::sp<Zetsubou> SP);

	/**
	* ���@������͈͓��ɂق��̒e�����邩
	* @param Rect ���ׂ����͈͂̒��S�ɂ���I�u�W�F�N�g�̊e���_�̍��W
	* @return true:���� false:�Ȃ�
	*/
	bool IsOthersNearby(const HAZAMA::RECT &Rect);

	/**
	* �����������ׂ��I�u�W�F�N�g��S�ĊԐړI�ɍ폜����
	* ���ۂɍ폜�����̂́ADrawTask���A�b�v�f�[�g����ăI�u�W�F�N�g�̎Q�ƃJ�E���g��1�ɂȂ��Ă���
	*/
	void Eliminate(void){
		Dix::sp<Zetsubou> sp_zetsu;
		Dix::sp<ScoreObj> sp_dummy = Score::Instance().GetDummyObj();

		for(unsigned i = 0; i < MyFactory<Zetsubou>::Instance().Size(); ++i){
			Dix::sp<ScoreObj> sp_score_obj(new ScoreObj(50, sp_dummy->GetManyTimes()));
			Score::Instance().AddObj(sp_score_obj);
			MyFactory<Zetsubou>::Instance().Create(i, sp_zetsu);
			sp_zetsu->Clear();
		}
	}

	/**
	* �X�V����
	*/
	virtual bool Update();
};

class SelfBulletTask : public OperatorTask
{
public:
	SelfBulletTask(Dix::sp<IOperator> sp_ope = NULL){
		if(sp_ope.GetPtr() != NULL){
			SetOperator(sp_ope, true);
		}
	}

	/**
	* �X�V����
	*/
	virtual bool Update(void);
};

class FoeBulletTask : public OperatorTask
{
public:
	FoeBulletTask(Dix::sp<IOperator> sp_ope = NULL){
		if(sp_ope.GetPtr() != NULL){
			SetOperator(sp_ope, true);
		}
	}

	/**
	* �����������ׂ��I�u�W�F�N�g��S�ĊԐړI�ɍ폜����
	* ���ۂɍ폜�����̂́ADrawTask���A�b�v�f�[�g����ăI�u�W�F�N�g�̎Q�ƃJ�E���g��1�ɂȂ��Ă���
	*/
	void Eliminate(void){
		Dix::sp<FoeBullet> sp_foe_bul;
		for(unsigned i = 0; i < MyFactory<FoeBullet>::Instance().Size(); ++i){
			MyFactory<FoeBullet>::Instance().Create(i, sp_foe_bul);
			sp_foe_bul->Clear();
		}
	}

	/**
	* �X�V����
	*/
	virtual bool Update(void);
};

class ItemTask : public OperatorTask
{
private:
	int next_count;		//!< @brief ���ɃA�C�e�����o�ꂷ�鎞��
	int cur_score;		//!< @brief ���݂̃X�R�A
	TimerForGames *p_timer;
	TimerForGames::TimerID timer_id;
	
	bool CreateLifeEx(void){
		Dix::sp<LifeExtend> sp;
		MyFactory<LifeExtend>::Instance().Create(0, sp);
		ResetOpes(true);
		return true;
	}

	bool CreateBarrier(void){
		Dix::sp<Barrier> sp;
		if(cur_score >= 12000){
			MyFactory<Barrier>::Instance().Create(0, sp);
			ResetOpes(true);
			return true;
		}
		return false;
	}

	bool CreateQuick(void){
		Dix::sp<Quick> sp;
		if(cur_score >= 15000){
			MyFactory<Quick>::Instance().Create(0, sp);
			ResetOpes(true);
			return true;
		}
		return false;
	}

	bool CreateMultiplier(void){
		Dix::sp<Multiplier> sp;
		if(cur_score >= 20000){
			MyFactory<Multiplier>::Instance().Create(0, sp);
			ResetOpes(true);
			return true;
		}
		return false;
	}

	bool CreateEliminator(void){
		Dix::sp<Eliminator> sp;
		if(cur_score >= 25000){
			MyFactory<Eliminator>::Instance().Create(0, sp);
			ResetOpes(true);
			return true;
		}
		return false;
	}

	bool CreateTriBullets(void){
		Dix::sp<TriBullets> sp;
		if(cur_score >= 30000){
			MyFactory<TriBullets>::Instance().Create(0, sp);
			ResetOpes(true);
			return true;
		}
		return false;
	}

	bool CreateGravifiedBullet(void){
		Dix::sp<GravifiedBullet> sp;
		if(cur_score >= 0){
			MyFactory<GravifiedBullet>::Instance().Create(0, sp);
			ResetOpes(true);
			return true;
		}
		return false;
	}

	bool CreateEliminatorX(void){
		Dix::sp<EliminatorX> sp;
		if(cur_score >= 45000){
			MyFactory<EliminatorX>::Instance().Create(0, sp);
			Dix::sp<OperatorChaseAfter> sp_after(new OperatorChaseAfter);
			ResetOpes(true);
			SetOperator(sp_after, true);
			return true;
		}
		return false;
	}

public:
	ItemTask(Dix::sp<IOperator> sp_ope = NULL) : timer_id(0){
		next_count = GetRand(10000) + 20000;
		p_timer = &TimerForGames::Instance();
		if(sp_ope.GetPtr() != NULL){
			SetOperator(sp_ope, true);
		}
	}

	/**
	* �X�V����
	*/
	virtual bool Update(void);

	void CreateNewItem(void);
};

class TouchTask : public TaskBase
{
private:
	Dix::sp<Snowman> sp_snowman;
	Dix::sp<FoeBullet> sp_foe_bullet;
	Dix::sp<Items> sp_item;

public:
	TouchTask(void){
		m_bUsageFlag = true;
	}

	void Reset(void){};

	/**
	* �X�V����
	*/
	bool Update(void);

	/**
	* �v���C���[�ƓG�̓����蔻������
	*/
	void SelfAndFoe(void);

	/**
	* �v���C���[�̒e�ƓG�̓����蔻������
	*/
	void SelfBulletAndFoe(void);

	/**
	* �v���C���[�ƓG�̒e�̓����蔻������
	*/
	void SelfAndFoeBullet(void);

	/**
	* �v���C���[�ƃA�C�e���̓����蔻������
	*/
	void SelfAndItem(void);

	/**
	* ���̑����X�̓����蔻������
	*/
	void Others(void);
};

class ScoreTask : public TaskBase
{
public:
	ScoreTask(void){
		m_bUsageFlag = true;
	}

	void Reset(void){};

	/**
	* �X�V����
	*/
	bool Update(void);
};

#if 0
class ReplayTask : public TaskBase
{
private:
	bool replay_flag;
	std::ofstream o_file;
	std::ifstream i_file;
	Dix::sp<boost::archive::text_oarchive> sp_oarchive;
	Dix::sp<boost::archive::text_iarchive> sp_iarchive;
	InputDeviceKeyBoard *p_keyboard;

public:
	ReplayTask(InputDeviceKeyBoard *p_key);

	/**
	* ���ۂɃt�@�C���Ƀf�[�^��ۑ�����
	*/
	bool Update(void);

	/**
	* ���v���C���[�h�ɂ��邩�ǂ����؂�ւ���
	*/
	void TurnReplayFlag(bool ReplayFlag){
		replay_flag = ReplayFlag;
	}

	/**
	* ���v���C�Ɏg���܂��͋L�^����t�@�C�����J��
	*/
	void SetRecordingFile(const char src[]);
};
#endif

#endif
