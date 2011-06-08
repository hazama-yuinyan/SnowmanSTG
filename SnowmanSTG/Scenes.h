#ifndef _SCENE_H
#define _SCENE_H


class CScenesBase;
class CTaskSystem;
class CInputDeviceKeyBoard;
class CBackObj;
class Score;
enum MENUS;

typedef Dix::sp<CScenesBase> SPScenes;
typedef Dix::sp<CTaskSystem> SPTaskSystem;
typedef Dix::sp<CInputDeviceKeyBoard> SPKeyBoard;

class IMenus
{
public:
	virtual void Execute(void) = 0;
};

class CScenesBase
{
protected:
	SPTaskSystem sp_task_system;
	SPKeyBoard sp_input_key;
	SPScenes sp_parent;			//親シーン
	bool active_flag;			//そのシーンが有効かどうか

public:
	CScenesBase(void) : active_flag(true){};
	virtual ~CScenesBase(void){};
	virtual void AddScene(SPScenes sp){};
	virtual void RemoveScenes(void){};
	
	bool IsNotActive(void){return !(active_flag);}
	/**
	* シーンを開始する
	*/
	virtual void Begin(void){};
};

class CSceneParent : public CScenesBase
{
private:
	std::list<SPScenes> scene_list;	//子シーンリスト

public:
	void AddScene(SPScenes sp){
		scene_list.push_back(sp);
	}

	void RemoveScenes(void){
		scene_list.remove_if(boost::mem_fn(&CScenesBase::IsNotActive));
	}

	void Begin(void){
		std::list<SPScenes>::iterator it = scene_list.begin();
		for(; it != scene_list.end(); ++it){
			(*it)->Begin();
		}
	}
};

class CSceneTitle : public CScenesBase
{
private:
	
	class IMainMenu: public IMenus
	{
	protected:
		CSceneTitle *p_title;
	};

	class CMenuStartGame : public IMainMenu
	{
	public:
		CMenuStartGame(CSceneTitle *p){
			p_title = p;
		}
		void Execute(void);
	};

	class CMenuReplayGame : public IMainMenu
	{
	public:
		CMenuReplayGame(CSceneTitle *p){
			p_title = p;
		}
		void Execute(void);
	};

	IMainMenu *p_menu;

public:
	CSceneTitle(SPScenes sp_paren, SPTaskSystem &sp_task, SPKeyBoard &sp_key);
	virtual ~CSceneTitle(void){};
	void Invoke(MENUS &menu);
	void AddScene(SPScenes sp){};
	void RemoveScene(SPScenes sp){};
	SPScenes GetChild(int &Num){};
	void Begin(void);
};

class CSceneGaming : public CScenesBase
{
private:
	Dix::sp<CBackObj> sp_back;
	Dix::sp<Score> sp_score;

public:
	CSceneGaming(SPScenes sp_paren, SPTaskSystem &sp_task, SPKeyBoard &sp_key);
	virtual ~CSceneGaming(void){};
	void AddScene(SPScenes sp){};
	void RemoveScene(SPScenes sp){};
	SPScenes GetChild(int &Num){};
	void Begin(void);
};

class CSceneScore : public CScenesBase
{
private:
	int white;

public:
	CSceneScore(SPScenes sp_paren, SPTaskSystem &sp_task, SPKeyBoard &sp_key);
	void AddScene(SPScenes sp){};
	void RemoveScene(SPScenes sp){};
	SPScenes GetChild(int &Num){};
	void Begin(void);

	/**
	* スコアのランキング関連の処理をする
	*/
	void ScoreRanking(void);
};

#endif
