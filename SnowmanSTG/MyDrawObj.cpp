#include "stdafx.h"
#include "MyDrawObj.h"
#include "Task.h"

int MyDrawObj::pic_handles[PIC_END];	//静的メンバ変数の実体
int MyDrawObj::fig_handles[10];


void MyDrawObj::LoadPic(void)
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

	pic_handles[TITLE_BACK] = HAZAMA::draw_helper->LoadImageToMemory(TITLE_BACK_PICTURE_NAME);	//タイトルの背景の画像をメモリーに読み込む
	pic_handles[BACK] = HAZAMA::draw_helper->LoadImageToMemory(BACK_PICTURE_NAME);		 //背景の画像をメモリーに読み込む
	pic_handles[SNOWMAN] = HAZAMA::draw_helper->LoadImageToMemory(SELF_PICTURE_NAME);		 //自機の画像をメモリーに読み込む
	pic_handles[SNOW_BARRIER] = HAZAMA::draw_helper->LoadImageToMemory(BARRIER_NAME);	//snowmanのバリアーの画像をメモリーに読み込む
	pic_handles[ZETSUBOU] = HAZAMA::draw_helper->LoadImageToMemory(FOE_PICTURE_NAME);		 //敵の画像をメモリーに読み込む
	pic_handles[SELFBULLET] = HAZAMA::draw_helper->LoadImageToMemory(SELF_BULLET_PICTURE_NAME);	 //弾丸の画像をメモリーに読み込む
	pic_handles[FOEBULLET] = HAZAMA::draw_helper->LoadImageToMemory(FOE_BULLET_PICTURE_NAME);		//敵の弾の画像をメモリーに読み込む
	pic_handles[LIFE] = HAZAMA::draw_helper->LoadImageToMemory(LIFE_PICTURE_NAME);		//残機数の画像をメモリーに読み込む
	pic_handles[ITEM_LIFE_EXTEND] = HAZAMA::draw_helper->LoadImageToMemory(ITEM_LIFE_EXTEND_NAME);
	pic_handles[ITEM_BARRIER] = HAZAMA::draw_helper->LoadImageToMemory(ITEM_BARRIER_NAME);
	pic_handles[ITEM_QUICK] = HAZAMA::draw_helper->LoadImageToMemory(ITEM_QUICK_NAME);
	pic_handles[ITEM_MULTIPLIER] = HAZAMA::draw_helper->LoadImageToMemory(ITEM_MULTIPLIER_NAME);
	pic_handles[ITEM_ELIMINATOR] = HAZAMA::draw_helper->LoadImageToMemory(ITEM_ELIMINATOR_NAME);
	pic_handles[ITEM_TRI_BULLETS] = HAZAMA::draw_helper->LoadImageToMemory(ITEM_TRI_BULETS_NAME);
	pic_handles[ITEM_GRAVIFIED] = HAZAMA::draw_helper->LoadImageToMemory(ITEM_GRAVIFIEDBULLET_NAME);
	pic_handles[ITEM_ELIMINATOR_X1] = HAZAMA::draw_helper->LoadImageToMemory(ITEM_ELIMINATOR_X1_NAME);
	pic_handles[ITEM_ELIMINATOR_X2] = HAZAMA::draw_helper->LoadImageToMemory(ITEM_ELIMINATOR_X2_NAME);
	pic_handles[PARA_MULTIPLIER] = HAZAMA::draw_helper->LoadImageToMemory(MULTIPLIER_NAME);

	HAZAMA::draw_helper->LoadDividedImage(FIG_PICTURE_NAME, 10, 1, 10, FIG_PIC_WIDTH, FIG_PIC_HEIGHT, fig_handles);
}

bool TitleObj::Draw(void)
{
	int *p_color = color;
	const char START_GAME_STR[] = "Start Game";
	const char REPLAY_GAME_STR[] = "Replay Game";
	HAZAMA::draw_helper->SetDrawBlendMode(HAZAMA::DrawHelper::NO_BLEND, 0);
	HAZAMA::draw_helper->DrawImage(0, 0, pic_handles[TITLE_BACK], false);
	HAZAMA::draw_helper->DrawString(280, 360, START_GAME_STR, *p_color);
	HAZAMA::draw_helper->DrawString(280, 400, REPLAY_GAME_STR, *(++p_color));
	return true;
}

void TitleObj::SetStrColor(MENUS menu)
{
	for(int i = 0; i < MENU_END; ++i){	//すでに赤いメニューの文字を白に戻す
		if(color[i] == HAZAMA::draw_helper->GetColorCode(255, 0, 0)){
			color[i] = HAZAMA::draw_helper->GetColorCode(0, 0, 0);
		}
	}

	color[menu] = HAZAMA::draw_helper->GetColorCode(255, 0, 0);	//指定された項目の文字を赤にする
}

Score::Score(void) : MyDrawObj(4), RANK_DRAW_START_POS(100), many_times(SCORE_NORMAL), cur_score(0)
{
	rect(0, 0, 32.0f, 32.0f);
}

void Score::AddScore(void)
{
	for(unsigned i = 0; i < s_score_obj.size(); ++i){
		Dix::sp<ScoreObj> sp_score_obj(s_score_obj.top());
		s_score_obj.pop();
		cur_score += static_cast<int>(sp_score_obj->GetResult() * many_times);
	}
}

bool Score::Draw(void)
{
	int i, j, beam_width, x, figs = cur_score;
	int *tmp_handle;

	//figsが十進数で何桁になるか調べる
	beam_width = 0;
	for(i = 10; figs >= i; i *= 10) beam_width++;

	//画面左上に描画する
	HAZAMA::draw_helper->SetDrawBlendMode(HAZAMA::DrawHelper::NO_BLEND, 0);
	x = beam_width * static_cast<int>(rect.right);
	for(i = 0; i <= beam_width; i++){
		tmp_handle = &fig_handles[0];
		for(j = 0 ; j < 10; j++){
			if(figs % 10 == j){
				HAZAMA::draw_helper->DrawImage(x, 0, *tmp_handle, true);		//実際に数字を描画する
				x-= static_cast<int>(rect.right);		//描画x座標を移動
				figs /= 10;							//1番下の桁の描画終わったので次の桁に移動する
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
			HAZAMA::draw_helper->DrawImage(x, 32, pic_handles[PARA_MULTIPLIER], true);
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
	int i, y = RANK_DRAW_START_POS, white = HAZAMA::draw_helper->GetColorCode(255, 255, 255);

	HAZAMA::draw_helper->DrawFormattedString(230, 40, white, "CURRENT SCORE: %d", cur_score);

	HAZAMA::draw_helper->DrawString(280, 80, "--RANKING--", white);

	for(i = 0; i < 10; i++){
		HAZAMA::draw_helper->DrawFormattedString(200, y, white, "%d: %d", i + 1, read_scores[i]);
		y += 20;
	}
}