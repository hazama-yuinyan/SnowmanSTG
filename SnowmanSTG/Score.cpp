/*スコアの記録と描画をするScoreクラスの実装*/

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

void Score::FigDraw(int Figs, int FigHandle[]) const {					//数値をグラフィック表示する関数
	int i, j, beam_width, x, *p_temp = FigHandle;

	//Figsが十進数で何桁になるか調べる
	beam_width = 0;
	for(i = 10; Figs >= i; i *= 10) beam_width++;

	//画面左上に描画する
	x = beam_width * FIG_PIC_WIDTH;
	for(i = 0; i <= beam_width; i++){
		FigHandle = p_temp;
		for(j = 0 ; j < 10; j++){
			if(Figs % 10 == j){
				DrawGraph(x, 0, *FigHandle, true);		//実際に数字を描画する
				x-= FIG_PIC_WIDTH;					//描画x座標を移動
				Figs /= 10;							//1番下の桁の描画終わったので次の桁に移動する
				break;
			}else{
				++FigHandle;
				continue;
			}
		}
	}
}

void Score::WriteFile(void){			//テキストファイルにランキングを書き込む
	int i;
	std::ofstream File("Data\\Scores\\scores.txt");

	for(i = 0; i < 10; i++){
		File << i + 1 << " " << read_scores[i] << std::endl;
	}
}

void Score::ReadFile(void){				//テキストファイルからランキングを読み込む
	int i;
	std::ifstream File("Data\\Scores\\scores.txt");

	for(i = 0; i < 10; i++){
		File >> ranking[i];
		File >> read_scores[i];
	}
}

void Score::PutInOrder(void){			//現在のスコアとランキングのスコアを比べて順位順に並べ替える
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

void Score::RankDraw(int &Color){			//ファイルから読み込んだスコアのランキングを画面に描画する
	int i, y = RANK_DRAW_START_POS;

	DrawString(280, 80, "--RANKING--", Color);

	for(i = 0; i < 10; i++){
		DrawFormatString(200, y, Color, "%d: %d", i + 1, read_scores[i]);
		y += 20;
	}
}

void Score::MultiplierDraw(const int &Pic_Handle){		//現在のスコアの倍率を画面に表示する
	int x = 0 , i;

	if(many_times >= SCORE_DOUBLE){
		for(i = 0; i < many_times / 2; i++){
			DrawGraph(x, 32, Pic_Handle, true);
			x += 32;
		}
	}
}

void Score::SaveInTemp(void){			//tmp_timesに倍率を一時的に保存する
	if(many_times >= SCORE_NORMAL){
		tmp_times = many_times;
	}else{
		return;
	}
}

void Score::Eliminator(int &NumOfFoes, bool &Score_Flag){		//アイテムeliminatorを取ったときのスコア追加動作
	int i;

	for(i = 0; i < NumOfFoes; i++){
		SaveInTemp();
		Score_Flag = true;
		many_times = SCORE_HALF;
		AddScore(Score_Flag);
	}
}

void Score::Eliminator_X(int &NumOfFoes, bool &Score_Flag){		//アイテムeliminator_xを取ったときのスコア追加動作
	int i;

	for(i = 0; i < NumOfFoes; i++){
		SaveInTemp();
		Score_Flag = true;
		many_times = SCORE_QAURTER_HALF;
		AddScore(Score_Flag);
	}
	Initialize();			//multiplierの効果を消す
}

void Score::Initialize(void){				//初期化する
	many_times = 1;
	tmp_times = 0;
}