#ifndef _SCENE_H
#define _SCENE_H


class ScenesBase;
class TaskSystem;
class InputDeviceKeyBoard;
class BackObj;
class Score;
enum MENUS;

typedef Dix::sp<ScenesBase> SPScenes;
typedef Dix::sp<TaskSystem> SPTaskSystem;
typedef Dix::sp<InputDeviceKeyBoard> SPKeyBoard;

class IMenus
{
public:
	virtual void Execute(void) = 0;
};

class ScenesBase
{
protected:
	SPTaskSystem sp_task_system;
	SPKeyBoard sp_input_key;
	SPScenes sp_parent;			//親シーン
	bool active_flag;			//そのシーンが有効かどうか

public:
	ScenesBase(void) : active_flag(true){};
	virtual ~ScenesBase(void){};
	virtual void AddScene(SPScenes sp){};
	virtual void RemoveScenes(void){};
	
	bool IsNotActive(void){return !(active_flag);}
	/**
	* シーンを開始する
	*/
	virtual void Begin(void){};
};

class SceneParent : public ScenesBase
{
private:
	std::list<SPScenes> scene_list;	//子シーンリスト

public:
	void AddScene(SPScenes sp){
		scene_list.push_back(sp);
	}

	void RemoveScenes(void){
		scene_list.remove_if(boost::mem_fn(&ScenesBase::IsNotActive));
	}

	void Begin(void){
		std::list<SPScenes>::iterator it = scene_list.begin();
		for(; it != scene_list.end(); ++it){
			(*it)->Begin();
		}
	}
};

class SceneTitle : public ScenesBase
{
private:
	
	class IMainMenu: public IMenus
	{
	protected:
		SceneTitle *p_title;
	};

	class MenuStartGame : public IMainMenu
	{
	public:
		MenuStartGame(SceneTitle *p){
			p_title = p;
		}
		void Execute(void);
	};

	class MenuReplayGame : public IMainMenu
	{
	public:
		MenuReplayGame(SceneTitle *p){
			p_title = p;
		}
		void Execute(void);
	};

	IMainMenu *p_menu;

public:
	SceneTitle(SPScenes sp_paren, SPTaskSystem &sp_task, SPKeyBoard &sp_key);
	virtual ~SceneTitle(void){};
	void Invoke(MENUS &menu);
	void AddScene(SPScenes sp){};
	void RemoveScene(SPScenes sp){};
	SPScenes GetChild(int &Num){};
	void Begin(void);
};

class SceneGaming : public ScenesBase
{
private:
	Dix::sp<BackObj> sp_back;
	Dix::sp<Score> sp_score;

public:
	SceneGaming(SPScenes sp_paren, SPTaskSystem &sp_task, SPKeyBoard &sp_key);
	virtual ~SceneGaming(void){};
	void AddScene(SPScenes sp){};
	void RemoveScene(SPScenes sp){};
	SPScenes GetChild(int &Num){};
	void Begin(void);
};

class SceneScore : public ScenesBase
{
private:
	int white;

public:
	SceneScore(SPScenes sp_paren, SPTaskSystem &sp_task, SPKeyBoard &sp_key);
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
