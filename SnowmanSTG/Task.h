/*様々な仕事をになうTaskのヘッダーファイル*/

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

class CInputDeviceKeyBoard;

enum TASKIDS{TASK_SNOWMAN, TASK_ZETSUBOU, TASK_SELFBULLET, TASK_FOEBULLET, TASK_ITEM, TASK_TOUCH, TASK_SCORE
, TASK_DRAW, TASK_REPLAY};

enum TASK_MESSAGES{NONE, DONT_MOVE, CAN_MOVE, ELIMINATING, ADD_OPE_APPLYGRAVITY, REMOVE_EXTRA_OPE, ENDING_GAME};


class CTaskBase : public ITaskBase
{
protected:
	TASK_MESSAGES message;

public:
	CTaskBase(void) : message(NONE){};
	TASK_MESSAGES GetTaskMessage(void){return message;}
	void ResetMessage(void){message = NONE;}
};

/**
* タスクを管理する
*/
class CTaskSystem : public ITaskSystem
{
private:
	bool continue_flag;		//!< @brief Updateの戻り値として使用する変数
	/**
	* タスクのメッセージを確認してその内容に応じた対応をする
	*/
	void CheckTaskMessage(void);

	/**
	* 移動関連のタスクの状態を一斉に変化させる
	*/
	void SwitchMovingTasks(bool OnOrOff = false);

public:
	/**
	* コンストラクタ
	* 移動オペレーターを含むタスクはここで生成する
	*/
	CTaskSystem(CInputDeviceKeyBoard *p_key);
	virtual ~CTaskSystem(){};

	/**
	* タスク全体を初期化する
	* ここでゲーム内で必要な処理の初期化を行う
	*/
	bool Init(void);

	/**
	* 登録されているタスクから与えられたタスクIDを持つものを検索して
	* そのポインタを返す
	*/
	template<class DerivedTask>
	DerivedTask *GetTask(TASKIDS TaskID){
		TASK_MAP::iterator it;
		it = m_mapTask.find(TaskID);
		return dynamic_cast<DerivedTask*>((*it).second.GetPtr());
	}

	/**
	* 活動中のタスクを全てリセットする
	*/
	void Reset(void){
		TASK_MAP::iterator it = m_mapActTask.begin();
		for(; it != m_mapActTask.end(); ++it){
			(*it).second->Reset();
		}
	}

	/**
	* 指定したタスクのメッセージを返す
	*/
	TASK_MESSAGES GetTaskMessage(TASKIDS TaskID){
		TASK_MAP::iterator it = m_mapActTask.find(TaskID);
		if(it != m_mapActTask.end()){
			Dix::sp<CTaskBase> sp_task;
			sp_task.DownCast((*it).second);
			return sp_task->GetTaskMessage();
		}
		return NONE;
	}

	/**
	* 指定したタスクのメッセージを初期化する
	*/
	void ResetMessage(TASKIDS TaskID){
		TASK_MAP::iterator it = m_mapActTask.find(TaskID);
		Dix::sp<CTaskBase> sp_task;
		sp_task.DownCast((*it).second);
		sp_task->ResetMessage();
	}

	/**
	* タスクを更新する
	* @return ゲームを継続していいかどうか
	*/
	bool Update(void);

	void SetReplayMode(bool Flag);
};

/**
* 描画タスククラス
* 描画を担当する
*/
class CDrawTask : public CTaskBase
{
private:
	typedef Dix::sp<CDrawObj> SPDrawObj;
	std::list<SPDrawObj> l_sp_draw_obj;		//!< @brief 描画オブジェクトクラス
	std::list<SPDrawObj> l_have_msgs_obj;	//!< @brief メッセージを発したいオブジェクトリスト

public:
	/**
	* コンストラクタ
	*/
	CDrawTask(){
		InittheList();
	}

	/**
	* デストラクタ
	*/
	virtual ~CDrawTask(){
		Reset();
	};

	/**
	* ベクターコンテナに登録されているオブジェクトを描画する
	*/
	virtual bool Update(void);

	/**
	* メッセージ持ちオブジェクトのリストを初期化する
	*/
	void InittheList(void);

	void EraseDummies(void){
		l_have_msgs_obj.remove_if(boost::bind(&CDrawTask::IsDummy, this, _1));
	}

	bool IsDummy(SPDrawObj &sp){return(sp.GetPtr() == NULL);}

	/**
	* メッセージを発したオブジェクトがあるかどうか
	*/
	bool IsMessaged(void){
		std::list<SPDrawObj>::iterator it = l_have_msgs_obj.begin();
		return((*it).GetPtr() != NULL);
	}

	/**
	* 登録されているオブジェクトを全て外す
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
* オペレーターを使用するタスクの基底クラス
*/
class COperatorTask : public CTaskBase
{
protected:
	std::vector<Dix::sp<IOperator> > v_sp_ope;	//動かす人インターフェイス
	std::list<Dix::sp<IOperator> > l_sp_base_ope;	//標準設定オペレーターリスト

	/**
	* コンストラクタ（プロテクト宣言）
	*/
	COperatorTask(void){};

public:
	/**
	* デストラクタ
	*/
	virtual ~COperatorTask(void){
		Reset();
	}

	/**
	* セットされているオペレーターをすべてリセットする
	*/
	void Reset(void){
		v_sp_ope.clear();
		l_sp_base_ope.clear();
	}

	virtual bool Update(void) = 0;

	/**
	* 標準でセットされているオペレーター以外をリセットする
	* @param EraseBaseOpeListFlag OperatorStraight以外のオペレーターを標準設定リストから消すかどうかを決める
	*/
	void ResetOpes(bool EraseBaseOpeListFlag = false){
		if(EraseBaseOpeListFlag && l_sp_base_ope.size() > 1){
			std::list<Dix::sp<IOperator> >::iterator it = l_sp_base_ope.begin();
			l_sp_base_ope.erase(++it, l_sp_base_ope.end());
		}

		unsigned repeat_times = v_sp_ope.size() - l_sp_base_ope.size();
		for(size_t i = 0; i < repeat_times; ++i){	//リストに登録されているもの以外、コンテナから削除する
			v_sp_ope.pop_back();
		}
	}

	/**
	* 動かす人を間借り
	* @param spOpe セットするオペレーターのスマートポインタ
	* @param IsBaseFlag 標準でセットされるべきオペレーターかどうか
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

class CSnowmanTask : public COperatorTask
{
private:
	void CreateNormalBullet(Dix::sp<Snowman> &SPSnowman);
	void CreateTriBullets(Dix::sp<Snowman> &SPSnowman);
	void CreateGravifiedBullet(Dix::sp<Snowman> &SPSnowman);

public:
	CSnowmanTask(Dix::sp<IOperator> sp_ope = NULL){
		if(sp_ope.GetPtr() != NULL){
			SetOperator(sp_ope, true);
		}
	}

	/**
	* スペースキーが押されていたら新たに弾を生成する
	*/
	void Shoot(void);

	/**
	* 更新する
	*/
	virtual bool Update();
};

class CZetsubouTask : public COperatorTask
{
private:
	int count_min;
	unsigned MAX_NUM_FOES;			//!< @brief 画面に出現できる敵の数
	float EXTRA_MARGIN;		//!< @brief 弾発射の際考慮する範囲の余白の幅
	CTimerForGames *p_timer;
	CTimerForGames::TimerID timer_id;
	
	/**
	* 新たな敵を出現させる
	*/
	void CreatNewZetsu(void);

public:
	CZetsubouTask(Dix::sp<IOperator> sp_ope = NULL) : count_min(0), EXTRA_MARGIN(20.0f), MAX_NUM_FOES(10), timer_id(0){
		p_timer = &CTimerForGames::Instance();
		if(sp_ope.GetPtr() != NULL){
			SetOperator(sp_ope, true);
		}
	}

	/**
	* 条件を満たしたら新たに弾を生成する
	*/
	void Shoot(Dix::sp<Zetsubou> SP);

	/**
	* 自機が入る範囲内にほかの弾があるか
	* @param Rect 調べたい範囲の中心にあるオブジェクトの各頂点の座標
	* @return true:ある false:ない
	*/
	bool IsOthersNearby(const HAZAMA::RECT &Rect);

	/**
	* 自分が扱うべきオブジェクトを全て間接的に削除する
	* 実際に削除されるのは、DrawTaskがアップデートされてオブジェクトの参照カウントが1になってから
	*/
	void Eliminate(void){
		Dix::sp<Zetsubou> sp_zetsu;
		Dix::sp<ScoreObj> sp_dummy = Score::Instance().GetDummyObj();

		for(unsigned i = 0; i < CMyFactory<Zetsubou>::Instance().Size(); ++i){
			Dix::sp<ScoreObj> sp_score_obj(new ScoreObj(50, sp_dummy->GetManyTimes()));
			Score::Instance().AddObj(sp_score_obj);
			CMyFactory<Zetsubou>::Instance().Create(i, sp_zetsu);
			sp_zetsu->Clear();
		}
	}

	/**
	* 更新する
	*/
	virtual bool Update();
};

class CSelfBulletTask : public COperatorTask
{
public:
	CSelfBulletTask(Dix::sp<IOperator> sp_ope = NULL){
		if(sp_ope.GetPtr() != NULL){
			SetOperator(sp_ope, true);
		}
	}

	/**
	* 更新する
	*/
	virtual bool Update(void);
};

class CFoeBulletTask : public COperatorTask
{
public:
	CFoeBulletTask(Dix::sp<IOperator> sp_ope = NULL){
		if(sp_ope.GetPtr() != NULL){
			SetOperator(sp_ope, true);
		}
	}

	/**
	* 自分が扱うべきオブジェクトを全て間接的に削除する
	* 実際に削除されるのは、DrawTaskがアップデートされてオブジェクトの参照カウントが1になってから
	*/
	void Eliminate(void){
		Dix::sp<FoeBullet> sp_foe_bul;
		for(unsigned i = 0; i < CMyFactory<FoeBullet>::Instance().Size(); ++i){
			CMyFactory<FoeBullet>::Instance().Create(i, sp_foe_bul);
			sp_foe_bul->Clear();
		}
	}

	/**
	* 更新する
	*/
	virtual bool Update(void);
};

class CItemTask : public COperatorTask
{
private:
	int next_count;		//!< @brief 次にアイテムが登場する時間
	int cur_score;		//!< @brief 現在のスコア
	CTimerForGames *p_timer;
	CTimerForGames::TimerID timer_id;
	
	bool CreateLifeEx(void){
		Dix::sp<LifeExtend> sp;
		CMyFactory<LifeExtend>::Instance().Create(0, sp);
		ResetOpes(true);
		return true;
	}

	bool CreateBarrier(void){
		Dix::sp<Barrier> sp;
		if(cur_score >= 12000){
			CMyFactory<Barrier>::Instance().Create(0, sp);
			ResetOpes(true);
			return true;
		}
		return false;
	}

	bool CreateQuick(void){
		Dix::sp<Quick> sp;
		if(cur_score >= 15000){
			CMyFactory<Quick>::Instance().Create(0, sp);
			ResetOpes(true);
			return true;
		}
		return false;
	}

	bool CreateMultiplier(void){
		Dix::sp<Multiplier> sp;
		if(cur_score >= 20000){
			CMyFactory<Multiplier>::Instance().Create(0, sp);
			ResetOpes(true);
			return true;
		}
		return false;
	}

	bool CreateEliminator(void){
		Dix::sp<Eliminator> sp;
		if(cur_score >= 25000){
			CMyFactory<Eliminator>::Instance().Create(0, sp);
			ResetOpes(true);
			return true;
		}
		return false;
	}

	bool CreateTriBullets(void){
		Dix::sp<TriBullets> sp;
		if(cur_score >= 30000){
			CMyFactory<TriBullets>::Instance().Create(0, sp);
			ResetOpes(true);
			return true;
		}
		return false;
	}

	bool CreateGravifiedBullet(void){
		Dix::sp<GravifiedBullet> sp;
		if(cur_score >= 0){
			CMyFactory<GravifiedBullet>::Instance().Create(0, sp);
			ResetOpes(true);
			return true;
		}
		return false;
	}

	bool CreateEliminatorX(void){
		Dix::sp<EliminatorX> sp;
		if(cur_score >= 45000){
			CMyFactory<EliminatorX>::Instance().Create(0, sp);
			Dix::sp<COperatorChaseAfter> sp_after(new COperatorChaseAfter);
			ResetOpes(true);
			SetOperator(sp_after, true);
			return true;
		}
		return false;
	}

public:
	CItemTask(Dix::sp<IOperator> sp_ope = NULL) : timer_id(0){
		next_count = GetRand(10000) + 20000;
		p_timer = &CTimerForGames::Instance();
		if(sp_ope.GetPtr() != NULL){
			SetOperator(sp_ope, true);
		}
	}

	/**
	* 更新する
	*/
	virtual bool Update(void);

	void CreateNewItem(void);
};

class CTouchTask : public CTaskBase
{
private:
	Dix::sp<Snowman> sp_snowman;
	Dix::sp<FoeBullet> sp_foe_bullet;
	Dix::sp<Items> sp_item;

public:
	CTouchTask(void){
		m_bUsageFlag = true;
	}

	void Reset(void){};

	/**
	* 更新する
	*/
	bool Update(void);

	/**
	* プレイヤーと敵の当たり判定を取る
	*/
	void SelfAndFoe(void);

	/**
	* プレイヤーの弾と敵の当たり判定を取る
	*/
	void SelfBulletAndFoe(void);

	/**
	* プレイヤーと敵の弾の当たり判定を取る
	*/
	void SelfAndFoeBullet(void);

	/**
	* プレイヤーとアイテムの当たり判定を取る
	*/
	void SelfAndItem(void);

	/**
	* その他諸々の当たり判定を取る
	*/
	void Others(void);
};

class CScoreTask : public CTaskBase
{
public:
	CScoreTask(void){
		m_bUsageFlag = true;
	}

	void Reset(void){};

	/**
	* 更新する
	*/
	bool Update(void);
};

class CReplayTask : public CTaskBase
{
private:
	bool replay_flag;
	std::ofstream o_file;
	std::ifstream i_file;
	Dix::sp<boost::archive::text_oarchive> sp_oarchive;
	Dix::sp<boost::archive::text_iarchive> sp_iarchive;
	CInputDeviceKeyBoard *p_keyboard;

public:
	CReplayTask(CInputDeviceKeyBoard *p_key);

	/**
	* 実際にファイルにデータを保存する
	*/
	bool Update(void);

	/**
	* リプレイモードにするかどうか切り替える
	*/
	void TurnReplayFlag(bool ReplayFlag){
		replay_flag = ReplayFlag;
	}

	/**
	* リプレイに使うまたは記録するファイルを開く
	*/
	void SetRecordingFile(const char src[]);
};

#endif
