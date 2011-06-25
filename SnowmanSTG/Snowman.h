/*自機であるSnowmanクラスのヘッダーファイル*/

#ifndef _SNOWMAN_H
#define _SNOWMAN_H

#include "MyDrawObj.h"
#include "TouchObj.h"

/**
* キャラクター全部の基本クラス
*/
class Character
{
protected:
	HAZAMA::VECTOR2 pos;			//!< @brief キャラクターの位置（位置ベクトル）
	HAZAMA::VECTOR2 vect;		//!< @brief キャラクターの動く速度
	double speed;		//!< @brief キャラクターの移動速度(単位はpixels/s)

public:

	/**
	* コンストラクタ 位置や速度にとりあえず0を入れておく
	*/
	Character(){};
	
	/*
	* 位置を設定する
	* @param Pos 設定する位置を表すVECTOR2構造体
	*/
	void SetPosition(const HAZAMA::VECTOR2 &Pos){
		pos = Pos;
	}

	/**
	* ある座標軸方向のみの位置を設定する
	* @param Pos 設定する値
	* @param Ax 設定する軸（x=0, y=1)
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
	* 速度を設定する
	* @param Vect 設定する速度を表すVECTOR2構造体
	*/
	void SetVector(const HAZAMA::VECTOR2 &Vect){
		vect = Vect;
	}

	/**
	* ある座標軸方向のみの速度を設定する
	* @param V 設定する値
	* @param Ax 設定する軸（x=0, y=1)
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
	* 現在の位置を返す
	* @return 現在の位置を表すVECTOR2構造体
	*/
	const HAZAMA::VECTOR2& GetPosition(void) const{return pos;}

	/*
	* 現在の速度を返す
	* @return 現在の速度を表すVECTOR2構造体
	*/
	const HAZAMA::VECTOR2& GetVector(void) const{return vect;}

	const double &GetSpeed(void) const{return speed;}
};


enum BULLETTYPE{BULLET_NORMAL, TRIBULLETS, GRAVIFIED};

/**
* プレイヤーが操作するキャラクタークラス
*/
class Snowman : public Character, public MyDrawObj, public TouchObj
{
private:
	const double AMOUNT_SELF_MOVE;
	const int LIFE_PIC_WIDTH;
	int life;		  ///< @brief 残機数
	int barrier_off_time;	//!< @brief バリアーがオフになる時間を記録しておく変数
	bool barrier_flag;		//!< @brief バリアーがオンになっているかどうか判断する変数
	int foes_taken_down;	//!< @brief 自機が倒した敵の数(隠しパラメーター)
	BULLETTYPE bullet_type;		//!< @brief 発射する弾の種類

	/**
	* 実際に判定を取る方
	*/
	bool Touch(Zetsubou *other);

	/**
	* 実際に判定を取る方
	*/
	bool Touch(FoeBullet *other);

	/**
	* 従属する方
	*/
	bool Touch(Items *other){return true;}

	/**
	* 実際に判定を取る方
	*/
	bool Touch(ExploAnim *other);
	
public:
	Snowman();
	bool Draw(void);

	/**
	* 残機数が0になったので、ゲームを終了するよう指示を出す
	*/
	void MessageGet(OBJ_MSG &msg){msg = GAMES_OVER;}
	bool Dispatch(TouchObj *other){
		return(other->Touch(this));
	}

	void SetBulletType(BULLETTYPE Type){bullet_type = Type;}

	BULLETTYPE GetBulletType(void){return bullet_type;}

	/**
	* 現在の残機数を返す
	*/
	int &GetLife(void){return life;}

	/**
	* 残機数を一個増やす
	*/
	void LifeExtended(void){
		++life;
	}

	/**
	* 残機数を画面右上にアイコンで提示する
	*/
	void LifeDraw(void);

	/**
	* バリアーを有効化する
	*/
	void BarrierOn(void);

	/**
	* バリアーを無効化する
	* @param IsCompelled 強制的に解除するかどうか
	*/
	void BarrierOff(bool IsCompelled = false);

	/**
	* バリアーを画面に描画する
	*/
	void BarrierDraw(void);

	/**
	* バリアーが有効化されてるか
	*/
	bool IsBarrierOn(void){return barrier_flag;}

	/**
	* 初期化する
	*/
	void Initialize(void);
};

#endif
