/*難易度のクラスDifficultyクラスのヘッダーファイル*/

#ifndef _DIFFICULTY_H
#define _DIFFICULTY_H

#include "stdafx.h"
#include "Zetsubou.h"
#include "Bullets.h"

class Difficulty{
	DIFFICULTIES difficulty;
	Zetsubou *p_zetsu;
	FoeBullet *p_foe_bullet;

public:
	Difficulty() : difficulty(VERY_EASY){};
	void ChangeDifficulty(int CurrentScore);
	void SetFoe(Zetsubou *Zetsubou){
		p_zetsu = Zetsubou;
	}
	void SetFoeBullet(FoeBullet *FoeBullet){
		p_foe_bullet = FoeBullet;
	}
};

#endif
