/*敵たちのクラスFoesとその派生クラスのヘッダーファイル*/

#ifndef _FOES_H
#define _FOES_H

#include "Snowman.h"
#include "TouchObj.h"
#include "Bullets.h"
#include "MyDrawObj.h"

/**
* 敵の一種であるZetsubouクラス
* 敵の中では最弱の存在
*/
class Zetsubou : public Character, public MyDrawObj, public TouchObj
{
private:
	double AMOUNT_FOE_MOVE;		//!< @brief 敵が1フレームに動く量
	int launched_bullets;	//!< @brief 連続で発射した弾の数

	/**
	* 従属する方
	*/
	bool Touch(Snowman *other);

	/**
	* 従属する方
	*/
	bool Touch(SelfBullet *other);

	/**
	* 実際に判定を取る方
	*/
	bool Touch(ExploAnim *other);

public:

	/**
	* コンストラクタ  初期位置の設定などを行う
	*/
	Zetsubou();

	/**
	* 敵を画面に描画するメソッド
	*/
	bool Draw(void);

	bool Dispatch(TouchObj *other){
		return(other->Touch(this));
	}

	/**
	* 難易度を変更するメソッド
	* @param Difficulty 現在の難易度
	*/
	//void ChangeDifficulty(DIFFICULTIES &Difficulty);
};

#endif
