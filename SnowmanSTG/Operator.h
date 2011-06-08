#ifndef _OPERATOR_H
#define _OPERATOR_H


/**
* オブジェクトを実際に動かすオペレーターの基底クラス
*/
class IOperator
{
public:
	IOperator(void){};
	virtual ~IOperator(void){};
	virtual void Operate(void) = 0;
};

/**
* 移動関連のオペレーター基底クラス
*/
class CMoveOperator : public IOperator
{
protected:
	HAZAMA::VECTOR2 pos;
	HAZAMA::VECTOR2 vect;
	float speed;

	//以下、必要なときのみセットする変数群
	HAZAMA::RECT self_rect;			//自身の四角形の相対座標
	HAZAMA::VECTOR2 other_pos;		//自分以外のオブジェクトの位置
	HAZAMA::RECT other_rect;		//自分以外の四角形の相対座標

public:
	/**
	* 計算した結果を返す
	*/
	HAZAMA::VECTOR2 GetPosition(void){
		return pos;
	}

	HAZAMA::VECTOR2 GetVector(void){
		return vect;
	}

	/**
	* オブジェクトの現在位置を間借りする
	*/
	void SetPosition(const HAZAMA::VECTOR2 &Position){
		pos = Position;
	}

	/**
	* オブジェクトの速度ベクトルを間借りする
	*/
	void SetVector(const HAZAMA::VECTOR2 &Vect){
		vect = Vect;
	}

	/**
	* オブジェクトの既定速度を間借りする
	*/
	void SetSpeed(const float &Speed){
		speed = Speed;
	}

	/**
	* 過剰に加速しないように検査して訂正する
	* @param Const 基準となるスピード
	*/
	void AdjustVector(const float &Const){
		if(vect.x > Const){
			vect(Const, vect.y);
		}
		if(vect.x < -Const){
			vect(-Const, vect.y);
		}
		if(vect.y > Const){
			vect(vect.x, Const);
		}
		if(vect.y < -Const){
			vect(vect.x, -Const);
		}
	}

	//以下、必要なときのみ設定する関数群
	/**
	* 自分以外のオブジェクトの位置をセットする
	*/
	void SetTargetPosition(const HAZAMA::VECTOR2 &TargetPos){
		other_pos = TargetPos;
	}

	/**
	* 自分や他人の四角形の頂点の相対座標をセットする
	*/
	void SetRects(const HAZAMA::RECT &SelfRect, const HAZAMA::RECT &OtherRect){
		self_rect = SelfRect;
		other_rect = OtherRect;
	}
};

/**
* 現在の移動方向にまっすぐにオブジェクトを動かす
*/
class COperatorStraight : public CMoveOperator
{
private:
	CTimerForGames *p_timer;
	
public:
	COperatorStraight(void);
	
	/**
	* 移動方向と速さに従って現在地を変化させる
	*/
	void Operate(void);
};

/**
* 左に動くよう仕向ける
*/
class COperatorGoLeft : public CMoveOperator
{
public:
	COperatorGoLeft(void){};
	void Operate(void){
		vect.x += -1.0f * speed;
		AdjustVector(speed);
	}
};

/**
* 上に動くよう仕向ける
*/
class COperatorGoUp : public CMoveOperator
{
public:
	COperatorGoUp(void){};
	void Operate(void){
		vect.y += -1.0f * speed;
		AdjustVector(speed);
	}
};

/**
* 右に動くよう仕向ける
*/
class COperatorGoRight : public CMoveOperator
{
public:
	COperatorGoRight(void){};
	void Operate(void){
		vect.x += 1.0f * speed;
		AdjustVector(speed);
	}
};

/**
* 下に動くよう仕向ける
*/
class COperatorGoDown : public CMoveOperator
{
public:
	COperatorGoDown(void){};
	void Operate(void){
		vect.y += 1.0f * speed;
		AdjustVector(speed);
	}
};

/**
* ほかのオブジェクトを追いかけるような挙動を実現するクラス
*/
class COperatorChaseAfter : public CMoveOperator
{
private:
	/**
	* ターゲットが存在する方向を割り出す
	* 自分が回転して、ターゲットは回転しないときのみ使用可
	* @param Vect 現在の自分の速度ベクトル
	*/
	HAZAMA::VECTOR2 FigureTargetDirection(HAZAMA::VECTOR2 &Vect);

public:
	COperatorChaseAfter(void){};
	void Operate(void);
};

/**
* 重力を受けて下向きに加速する挙動を実現するクラス
*/
class COperatorApplyGravity : public CMoveOperator
{
private:
	const float AMOUNT_GRAVITY;		//重力の大きさ(単位はpixels/x)
	CTimerForGames *p_timer;

public:
	COperatorApplyGravity(float g = 294.0f) : AMOUNT_GRAVITY(g){
		p_timer = &CTimerForGames::Instance();
	}
	void Operate(void);
};

#endif
