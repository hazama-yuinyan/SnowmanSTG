#include "stdafx.h"
#include "MyDrawObj.h"
#include "Task.h"

int CMyDrawObj::pic_handles[PIC_END];	//�ÓI�����o�ϐ��̎���
int CMyDrawObj::fig_handles[10];


void CMyDrawObj::LoadPic(void)
{
	const char TITLE_BACK_PICTURE_NAME[] = "Data\\Images\\snowman_title.png";
	const char BACK_PICTURE_NAME[] = "Data\\Images\\back.png";
	const char LIFE_PICTURE_NAME[] = "Data\\Images\\snowman_s.png";
	const char SELF_BULLET_PICTURE_NAME[] = "Data\\Images\\snow_ball.bmp";
	const char FOE_PICTURE_NAME[] = "Data\\Images\\ZETSUBOU.png";
	const char SELF_PICTURE_NAME[] = "Data\\Images\\snowman.bmp";
	const char BARRIER_NAME[] = "Data\\Images\\snowman_barrier.bmp";
	const char FOE_BULLET_PICTURE_NAME[] = "Data\\Images\\foe_bullet.png";
	const char SELF_GOT_HIT_ANIM_NAME[] = "Data\\Images\\self_got_hit_anim.bmp";
	const char ITEM_LIFE_EXTEND_NAME[] = "Data\\Images\\item_life_extend.bmp";
	const char ITEM_BARRIER_NAME[] = "Data\\Images\\item_barrier.bmp";
	const char ITEM_QUICK_NAME[] = "Data\\Images\\item_quick.bmp";
	const char ITEM_MULTIPLIER_NAME[] = "Data\\Images\\item_multiplier.bmp";
	const char ITEM_TRI_BULETS_NAME[] = "Data\\Images\\item_tri_bullets.bmp";
	const char ITEM_ELIMINATOR_NAME[] = "Data\\Images\\item_eliminator.bmp";
	const char ITEM_GRAVIFIEDBULLET_NAME[] = "Data\\Images\\item_gravified_bullet.bmp";
	const char ITEM_ELIMINATOR_X1_NAME[] = "Data\\Images\\item_eliminator_x1.bmp";
	const char ITEM_ELIMINATOR_X2_NAME[] = "Data\\Images\\item_eliminator_x2.bmp";
	const char MULTIPLIER_NAME[] = "Data\\Images\\para_multiplier.bmp";
	const char FIG_PICTURE_NAME[] = "Data\\Images\\figures.bmp";

	const int FIG_PIC_WIDTH = 32, FIG_PIC_HEIGHT = 32;

	pic_handles[TITLE_BACK] = LoadGraph(TITLE_BACK_PICTURE_NAME);	//�^�C�g���̔w�i�̉摜���������[�ɓǂݍ���
	pic_handles[BACK] = LoadGraph(BACK_PICTURE_NAME);		 //�w�i�̉摜���������[�ɓǂݍ���
	pic_handles[SNOWMAN] = LoadGraph(SELF_PICTURE_NAME);		 //���@�̉摜���������[�ɓǂݍ���
	pic_handles[SNOW_BARRIER] = LoadGraph(BARRIER_NAME);	//snowman�̃o���A�[�̉摜���������[�ɓǂݍ���
	pic_handles[ZETSUBOU] = LoadGraph(FOE_PICTURE_NAME);		 //�G�̉摜���������[�ɓǂݍ���
	pic_handles[SELFBULLET] = LoadGraph(SELF_BULLET_PICTURE_NAME);	 //�e�ۂ̉摜���������[�ɓǂݍ���
	pic_handles[FOEBULLET] = LoadGraph(FOE_BULLET_PICTURE_NAME);		//�G�̒e�̉摜���������[�ɓǂݍ���
	pic_handles[LIFE] = LoadGraph(LIFE_PICTURE_NAME);		//�c�@���̉摜���������[�ɓǂݍ���
	pic_handles[ITEM_LIFE_EXTEND] = LoadGraph(ITEM_LIFE_EXTEND_NAME);
	pic_handles[ITEM_BARRIER] = LoadGraph(ITEM_BARRIER_NAME);
	pic_handles[ITEM_QUICK] = LoadGraph(ITEM_QUICK_NAME);
	pic_handles[ITEM_MULTIPLIER] = LoadGraph(ITEM_MULTIPLIER_NAME);
	pic_handles[ITEM_ELIMINATOR] = LoadGraph(ITEM_ELIMINATOR_NAME);
	pic_handles[ITEM_TRI_BULLETS] = LoadGraph(ITEM_TRI_BULETS_NAME);
	pic_handles[ITEM_GRAVIFIED] = LoadGraph(ITEM_GRAVIFIEDBULLET_NAME);
	pic_handles[ITEM_ELIMINATOR_X1] = LoadGraph(ITEM_ELIMINATOR_X1_NAME);
	pic_handles[ITEM_ELIMINATOR_X2] = LoadGraph(ITEM_ELIMINATOR_X2_NAME);
	pic_handles[PARA_MULTIPLIER] = LoadGraph(MULTIPLIER_NAME);

	LoadDivGraph(FIG_PICTURE_NAME, 10, 1, 10, FIG_PIC_WIDTH, FIG_PIC_HEIGHT, fig_handles);
}

bool CTitleObj::Draw(void)
{
	int *p_color = color;
	const char START_GAME_STR[] = "Start Game";
	const char REPLAY_GAME_STR[] = "Replay Game";
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawGraph(0, 0, pic_handles[TITLE_BACK], false);
	DrawString(280, 360, START_GAME_STR, *p_color);
	DrawString(280, 400, REPLAY_GAME_STR, *(++p_color));
	return true;
}

void CTitleObj::SetStrColor(MENUS menu)
{
	for(int i = 0; i < MENU_END; ++i){	//���łɐԂ����j���[�̕����𔒂ɖ߂�
		if(color[i] == GetColor(255, 0, 0)){
			color[i] = GetColor(0, 0, 0);
		}
	}

	color[menu] = GetColor(255, 0, 0);	//�w�肳�ꂽ���ڂ̕�����Ԃɂ���
}

Score::Score(void) : CMyDrawObj(4), RANK_DRAW_START_POS(100), many_times(SCORE_NORMAL), cur_score(0)
{
	rect(0, 0, 32.0f, 32.0f);
}

void Score::AddScore(void)
{
	for(int i = 0; i < s_score_obj.size(); ++i){
		sp<ScoreObj> sp_score_obj(s_score_obj.top());
		s_score_obj.pop();
		cur_score += static_cast<int>(sp_score_obj->GetResult() * many_times);
	}
}

bool Score::Draw(void)
{
	int i, j, beam_width, x, figs = cur_score;
	int *tmp_handle;

	//figs���\�i���ŉ����ɂȂ邩���ׂ�
	beam_width = 0;
	for(i = 10; figs >= i; i *= 10) beam_width++;

	//��ʍ���ɕ`�悷��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	x = beam_width * static_cast<int>(rect.right);
	for(i = 0; i <= beam_width; i++){
		tmp_handle = &fig_handles[0];
		for(j = 0 ; j < 10; j++){
			if(figs % 10 == j){
				DrawGraph(x, 0, *tmp_handle, true);		//���ۂɐ�����`�悷��
				x-= static_cast<int>(rect.right);		//�`��x���W���ړ�
				figs /= 10;							//1�ԉ��̌��̕`��I������̂Ŏ��̌��Ɉړ�����
				break;
			}else{
				++tmp_handle;
				continue;
			}
		}
	}

	MultiplierDraw();
	return true;
}

void Score::MultiplierDraw(void)
{
	int x = 0 , i;

	if(many_times >= SCORE_DOUBLE){
		for(i = 0; i < many_times / 2; ++i){
			DrawGraph(x, 32, pic_handles[PARA_MULTIPLIER], true);
			x += 32;
		}
	}
}

void Score::WriteFile(void)
{
	int i;
	std::ofstream File("Data\\Scores\\scores.txt");

	for(i = 0; i < 10; i++){
		File << i + 1 << " " << read_scores[i] << std::endl;
	}
}

void Score::ReadFile(void)
{
	int i;
	std::ifstream File("Data\\Scores\\scores.txt");

	for(i = 0; i < 10; i++){
		File >> ranking[i];
		File >> read_scores[i];
	}
}

void Score::PutInOrder(void)
{
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

void Score::RankDraw(void)
{
	int i, y = RANK_DRAW_START_POS, white = GetColor(255, 255, 255);

	DrawFormatString(230, 40, white, "CURRENT SCORE: %d", cur_score);

	DrawString(280, 80, "--RANKING--", white);

	for(i = 0; i < 10; i++){
		DrawFormatString(200, y, white, "%d: %d", i + 1, read_scores[i]);
		y += 20;
	}
}