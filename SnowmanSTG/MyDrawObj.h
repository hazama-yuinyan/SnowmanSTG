/*ライブラリで用意した描画管理と描画されるオブジェクトの基底クラスを
このゲーム用に拡張したクラス*/

#include "stdafx.h"

#ifndef _MYDRAWOBJ_H
#define _MYDRAWOBJ_H



enum PICTURES{TITLE_BACK, BACK, SNOWMAN, SNOW_BARRIER, ZETSUBOU, SELFBULLET, FOEBULLET, LIFE, ITEM_LIFE_EXTEND
, ITEM_BARRIER, ITEM_QUICK, ITEM_MULTIPLIER, ITEM_ELIMINATOR, ITEM_TRI_BULLETS, ITEM_GRAVIFIED, ITEM_ELIMINATOR_X1
, ITEM_ELIMINATOR_X2, PARA_MULTIPLIER, PIC_END};

enum OBJ_MSG{NOTHING, MOVING, ELIMINATE, GAMES_OVER};

enum MENUS{START_GAME, REPLAY_GAME, MENU_END};

/**
* 描画基底クラス
*/
class CDrawObj
{
protected:
	HAZAMA::RECT rect;	//!< @brief 描画する画像の大きさ
	bool draw_flag;		//!< @brief 描画するかどうか
	int draw_priority_num;	//!< @brief 描画時に考慮すべき優先順位(0～5までの6段階)番号が小さいほど先に描画される

	/**
	* コンストラクタ（プロテクト宣言）
	*/
	CDrawObj(const int PriorityNum) : draw_flag(true), draw_priority_num(PriorityNum){};

public:
	virtual ~CDrawObj(void){};

	/**
	* 描画しないように設定する
	*/
	void Clear(void){draw_flag = false;}

	bool IsNotDrawn(void){
		return HAZAMA::ReverseFlag(draw_flag);
	}
	/**
	* 画像の大きさを返す
	*/
	const HAZAMA::RECT &GetRect(void) const{
		return rect;
	}

	/**
	* 描画関数
	* @return 何かメッセージを発したい場合にはfalseを返す
	*/
	virtual bool Draw() = 0;

	/**
	* 具体的なメッセージを発する
	* @param msg 具体的なメッセージを入れる変数
	*/
	virtual void MessageGet(OBJ_MSG &msg){msg = NOTHING;}

	/**
	* 描画優先順位の比較のための演算子オーバーロード
	*/
	bool operator<(CDrawObj &rhs){
		return(this->draw_priority_num < rhs.draw_priority_num);
	}
};

/**
* 普通の画像を描画するクラスの基底クラス
*/
class CMyDrawObj : public CDrawObj
{
protected:
	static int pic_handles[PIC_END];					//!< @brief 画像のハンドルを保持する配列
	static int fig_handles[10];				//!< @brief スコア用の数字の画像を保持する配列
	
	/**
	* コンストラクタ（プロテクト宣言）
	*/
	CMyDrawObj(const int PriorityNum) : CDrawObj(PriorityNum){};

public:
	/**
	* デコンストラクタ
	*/
	~CMyDrawObj(){};

	/**
	* オブジェクトに使う画像を配列に読み込む
	*/
	static void LoadPic(void);
};

class CTitleObj : public CMyDrawObj
{
private:
	int color[MENU_END];

public:
	/**
	* コンストラクタ
	*/
	CTitleObj() : CMyDrawObj(0){
		color[0] = GetColor(255, 0, 0);
		color[1] = GetColor(0, 0, 0);
	};

	/**
	* 描画関数
	*/
	bool Draw(void);
	void SetStrColor(MENUS menu);
};

class CBackObj : public CMyDrawObj
{
public:
	/**
	* コンストラクタ
	*/
	CBackObj() : CMyDrawObj(0){};

	/**
	* 描画関数
	*/
	bool Draw(void){
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);		//重ねて描画できるようにする
		DrawGraph(0, 0, pic_handles[BACK], false);
		return true;
	}
};

class ScoreObj
{
private:
	const int ADD_SCORE_RATE;		//!< @brief このオブジェクトの基本獲得ポイント
	const float OBJ_MANY_TIMES;		//!< @brief このオブジェクトの基本倍率

public:
	/**
	* コンストラクタ
	* スコアレートと倍率を設定する
	*/
	ScoreObj(int ScoreRate, float ObjManyTimes) : ADD_SCORE_RATE(ScoreRate), OBJ_MANY_TIMES(ObjManyTimes){};

	/**
	* このオブジェクトの基本倍率を返す
	* ダミー用
	*/
	float GetManyTimes(void){return OBJ_MANY_TIMES;}

	/**
	* このスコアオブジェクトの合計得点を返す
	*/
	float GetResult(void){
		return(ADD_SCORE_RATE * OBJ_MANY_TIMES);
	}
};

class Score : public CMyDrawObj
{
private:
	const int RANK_DRAW_START_POS;	//!< @brief ランキングを描画し始める座標
	int ranking[10];			//!< @brief スコアの順位用のダミー
	int read_scores[10];		//!< @brief ファイルから読み込んだベスト10のスコア
	int cur_score;				//!< @brief 現在のスコア
	float many_times;			//!< @brief 現在の全体の倍率
	std::stack<Dix::sp<ScoreObj> > s_score_obj;	//!< @brief これから追加するスコアのオブジェクトを一時保存しておくスタック
	Dix::sp<ScoreObj> sp_dummy;	//!< @brief Eliminatorなど用のダミー

	/**
	* コンストラクタ
	*/
	Score();

public:

	/**
	* デストラクタ
	*/
	~Score(void){};
	
	/**
	* シングルトンオブジェクトを取得
	*/
	static Score &Instance(void){
		static Score inst;
		return inst;
	}

	/**
	*スタックに保持したオブジェクトの情報を元にスコアを更新する
	*/
	void AddScore(void);

	/**
	* スコアのオブジェクトを追加する
	*/
	void AddObj(Dix::sp<ScoreObj> &sp, bool IsDummy = false){
		IsDummy ? sp_dummy = sp : s_score_obj.push(sp);
	}

	/**
	* ダミーのオブジェクトを返す
	*/
	Dix::sp<ScoreObj> &GetDummyObj(void){return sp_dummy;}

	/**
	* 現在の倍率を前の倍率の2倍にセットする
	*/
	void SetManyTimes(void){
		many_times *= 2.0f;
	}

	int GetCurScore(void){return cur_score;}

	/**
	* 描画関数
	*/
	bool Draw(void);

	/**
	* 現在の倍率を画面左上にアイコンとして描画する
	*/
	void MultiplierDraw(void);

	/**
	* テキストファイルにランキングを書き込む
	*/
	void WriteFile(void);

	/**
	* テキストファイルからランキングを読み込む
	*/
	void ReadFile(void);

	/**
	* 現在のスコアとランキングのスコアを比べて順位順に並べ替える
	*/
	void PutInOrder(void);

	/**
	* ファイルから読み込んだスコアのランキングを画面に描画する
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
