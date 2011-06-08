/*�X�R�A�̋L�^�ƕ`�������Score�N���X�̎���*/

#include "Score.h"

Score::Score() : cur_score(0), ADD_SCORE_RATE(50), RANK_DRAW_START_POS(100), many_times(1.0f), tmp_times(0){
	ReadFile();
}

void Score::AddScore(bool &Score_Flag){
	if(Score_Flag == true){
		cur_score += ADD_SCORE_RATE * many_times;
		Score_Flag = false;
	}

	if(tmp_times != 0){
		many_times = tmp_times;
		tmp_times = 0;
	}
}

void Score::FigDraw(int Figs, int FigHandle[]) const {					//���l���O���t�B�b�N�\������֐�
	int i, j, beam_width, x, *p_temp = FigHandle;

	//Figs���\�i���ŉ����ɂȂ邩���ׂ�
	beam_width = 0;
	for(i = 10; Figs >= i; i *= 10) beam_width++;

	//��ʍ���ɕ`�悷��
	x = beam_width * FIG_PIC_WIDTH;
	for(i = 0; i <= beam_width; i++){
		FigHandle = p_temp;
		for(j = 0 ; j < 10; j++){
			if(Figs % 10 == j){
				DrawGraph(x, 0, *FigHandle, true);		//���ۂɐ�����`�悷��
				x-= FIG_PIC_WIDTH;					//�`��x���W���ړ�
				Figs /= 10;							//1�ԉ��̌��̕`��I������̂Ŏ��̌��Ɉړ�����
				break;
			}else{
				++FigHandle;
				continue;
			}
		}
	}
}

void Score::WriteFile(void){			//�e�L�X�g�t�@�C���Ƀ����L���O����������
	int i;
	std::ofstream File("Data\\Scores\\scores.txt");

	for(i = 0; i < 10; i++){
		File << i + 1 << " " << read_scores[i] << std::endl;
	}
}

void Score::ReadFile(void){				//�e�L�X�g�t�@�C�����烉���L���O��ǂݍ���
	int i;
	std::ifstream File("Data\\Scores\\scores.txt");

	for(i = 0; i < 10; i++){
		File >> ranking[i];
		File >> read_scores[i];
	}
}

void Score::PutInOrder(void){			//���݂̃X�R�A�ƃ����L���O�̃X�R�A���ׂď��ʏ��ɕ��בւ���
	int i, j;

	for(i = 0; i < 10; i++){
		if(cur_score >= read_scores[i]){
			for(j = 9; j > i; j--){
				read_scores[j] = read_scores[j-1];
			}
			read_scores[i] = cur_score;
			break;
		}
	}
}

void Score::RankDraw(int &Color){			//�t�@�C������ǂݍ��񂾃X�R�A�̃����L���O����ʂɕ`�悷��
	int i, y = RANK_DRAW_START_POS;

	DrawString(280, 80, "--RANKING--", Color);

	for(i = 0; i < 10; i++){
		DrawFormatString(200, y, Color, "%d: %d", i + 1, read_scores[i]);
		y += 20;
	}
}

void Score::MultiplierDraw(const int &Pic_Handle){		//���݂̃X�R�A�̔{������ʂɕ\������
	int x = 0 , i;

	if(many_times >= SCORE_DOUBLE){
		for(i = 0; i < many_times / 2; i++){
			DrawGraph(x, 32, Pic_Handle, true);
			x += 32;
		}
	}
}

void Score::SaveInTemp(void){			//tmp_times�ɔ{�����ꎞ�I�ɕۑ�����
	if(many_times >= SCORE_NORMAL){
		tmp_times = many_times;
	}else{
		return;
	}
}

void Score::Eliminator(int &NumOfFoes, bool &Score_Flag){		//�A�C�e��eliminator��������Ƃ��̃X�R�A�ǉ�����
	int i;

	for(i = 0; i < NumOfFoes; i++){
		SaveInTemp();
		Score_Flag = true;
		many_times = SCORE_HALF;
		AddScore(Score_Flag);
	}
}

void Score::Eliminator_X(int &NumOfFoes, bool &Score_Flag){		//�A�C�e��eliminator_x��������Ƃ��̃X�R�A�ǉ�����
	int i;

	for(i = 0; i < NumOfFoes; i++){
		SaveInTemp();
		Score_Flag = true;
		many_times = SCORE_QAURTER_HALF;
		AddScore(Score_Flag);
	}
	Initialize();			//multiplier�̌��ʂ�����
}

void Score::Initialize(void){				//����������
	many_times = 1;
	tmp_times = 0;
}