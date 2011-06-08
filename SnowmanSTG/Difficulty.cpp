/*“ïˆÕ“x‚ğŠÇ—‚·‚éDifficultyƒNƒ‰ƒX‚ÌÀ‘•*/

#include "Difficulty.h"

void Difficulty::ChangeDifficulty(int CurrentScore){
	if(CurrentScore <= 12500){
		difficulty = VERY_EASY;
	}else if((CurrentScore >= 12500) && (CurrentScore <= 25000) && (difficulty == VERY_EASY_HELD)){
		difficulty = EASY;
	}else if((CurrentScore >= 25000) && (CurrentScore <= 50000) && (difficulty == EASY_HELD)){
		difficulty = NORMAL;
	}else if((CurrentScore >= 50000) && (CurrentScore <= 100000) && (difficulty == NORMAL_HELD)){
		difficulty = HARD;
	}else if(difficulty == HARD_HELD){
		difficulty = VERY_HARD;
	}

	p_zetsu->ChangeDifficulty(difficulty);
	p_foe_bullet->ChangeDifficulty(difficulty);
}