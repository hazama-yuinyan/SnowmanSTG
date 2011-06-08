/*�X�R�A�̋L�^�ƕ`�������Score�N���X�̃w�b�_�[�t�@�C��*/

#ifndef _SCORE_H
#define _SCORE_H

#include "stdafx.h"
#include <iostream>
#include <fstream>

class Score{
	friend class Touch;

	const int ADD_SCORE_RATE;
	const int RANK_DRAW_START_POS;
	int ranking[10];
	int read_scores[10];
	float many_times;		//�X�R�A�̔{����ۑ�����ϐ�
	float tmp_times;		//�ꎞ�I�ɃX�R�A�̔{����ۑ����Ă����ϐ�

public:
	Score();
	int cur_score;
	void AddScore(bool &Score_Flag);
	void Draw(int FigHandle[]){
		FigDraw(cur_score, FigHandle);
	}
	void FigDraw(int Figs, int FigHandle[]) const;
	void WriteFile(void);
	void ReadFile(void);
	void PutInOrder(void);
	void RankDraw(int &Color);
	void MultiplierDraw(const int &Pic_Handle);
	void SaveInTemp(void);
	void Eliminator(int &NumOfFoes, bool &Score_Flag);
	void Eliminator_X(int &NumOfFoes, bool &Score_Flag);
	void Initialize(void);
};

#endif
