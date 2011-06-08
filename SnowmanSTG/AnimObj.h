#ifndef _ANIMOBJ_H
#define _ANIMOBJ_H

#include "MyDrawObj.h"
#include "TouchObj.h"


enum ANIMATIONS{SELF_GOT_HIT, FIRE, EXPLOSION, MAX_FRAMES};

typedef int anim_handle;


/**
* 通常アニメーション用の基底クラス
* 独立したアニメーションの描画にはこのクラスを単独継承して実装する
*/
class CAnimObj : public CDrawObj
{
protected:
	static std::vector<Dix::sp<anim_handle> > v_sp_anim;
	HAZAMA::VECTOR2 pos;

	/**
	* コンストラクタ（プロテクト宣言）
	*/
	CAnimObj(const int PriorityNum) : CDrawObj(PriorityNum){};

public:
	/**
	* デコンストラクタ
	*/
	~CAnimObj(){};

	/*
	* 位置を設定する
	* @param Pos 設定する位置を表すVECTOR2構造体
	*/
	void SetPosition(const HAZAMA::VECTOR2 &Pos){
		pos = Pos;
	}

	/*
	* 現在の位置を返す
	* @return 現在の位置を表すVECTOR2構造体
	*/
	const HAZAMA::VECTOR2& GetPosition(void) const{return pos;}

	/**
	* アニメーションに使う画像を読み込む
	*/
	static void LoadPic(void);
};

/*class ICooperativeAnim	//このクラスのせいでFactoryクラスに格納されるオブジェクトの一部に不具合が発生する可能性あり
{
protected:
	ICooperativeAnim(void){};

public:
	virtual ~ICooperativeAnim(void){};
	/**
	* 次のフレームに移る
	*/
	/*virtual void AdvanceFrame(void) = 0;

	/**
	* 現在のフレームを描画する
	*/
	/*virtual void DrawAFrame(void) = 0;
};*/

/**
* ほかのオブジェクトを描画しつつ、同時にアニメーションも再生したい場合にはこのクラスを継承する
*/
class CCooperativeAnimObj : public CAnimObj
{
protected:
	int cur_frame;		//!< @brief 現在のフレーム数
	int MAX_FRAME;		//!< @brief 描画するアニメーションの最大フレーム数
	Dix::wp<anim_handle> wp_anim;	//!< @brief 描画するアニメーションの画像を保持する配列

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
	* 描画関数
	*/
	bool Draw(void);

	/**
	* アニメーション再生が終わったので、ほかのタスクの活動を再開するようメッセージを発する
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
	* 描画関数
	*/
	bool Draw(void);

	/**
	* アニメーション再生が終わったので、ほかのタスクを再度活動させ、eliminator用の特別な
	* タスクを実行するよう指示する
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
	* 描画関数
	*/
	bool Draw(void);

	void DrawAFrame(void);
	bool Dispatch(CTouchObj *other){
		return(other->Touch(this));
	}
};

#endif
