#ifndef _SOUND_H
#define _SOUND_H

enum SOUNDS{SELF_GOT_HIT_SE, GOT_ITEM_SE, ELIMINATOR_SE, ELIMINATOR_X_SE, EXPLOSION_SE, MAIN_BGM, SOUND_END};

class Sounds
{
protected:
	static int sound_handles[SOUND_END];		//!< @brief サウンドのハンドルを保持する配列

	/**
	* コンストラクタ（プロテクト宣言）
	*/
	Sounds(void){};

public:
	/**
	* デコンストラクタ
	*/
	~Sounds(void){};

	/**
	* 現在位置からパンする量を求める
	*/
	static int GetAmountPan(const double Pos){
		double tmp = Pos - 320.0;
		return static_cast<int>(Pos * 31.25);
	}

	/**
	* サウンドデータをメモリーに読み込む
	*/
	static void LoadSounds(void);
};

class SelfGotHitSE : public Sounds
{
private:
	SelfGotHitSE(void){};

public:
	static void Play(const double PosX){
		HAZAMA::draw_helper->SetPanOfSoundInMemory(GetAmountPan(PosX), sound_handles[SELF_GOT_HIT_SE]);
		HAZAMA::draw_helper->PlaySoundInMemory(sound_handles[SELF_GOT_HIT_SE], DX_PLAYTYPE_BACK);
	}
};

class GotItemSE : public Sounds
{
private:
	GotItemSE(void){};

public:
	static void Play(void){
		HAZAMA::draw_helper->PlaySoundInMemory(sound_handles[GOT_ITEM_SE], DX_PLAYTYPE_BACK);
	}
};

class MainBGM : public Sounds
{
private:
	MainBGM(void){};

public:
	static void Play(void){
		HAZAMA::draw_helper->SetLoopPointOfSoundInMemory(8000, sound_handles[MAIN_BGM]);			//メインのBGMのループ位置を設定する
		HAZAMA::draw_helper->ChangeVolumeOfSoundInMemory(255 * 60 / 100, sound_handles[MAIN_BGM]);
		if(!HAZAMA::draw_helper->IsSoundPlayingWith(sound_handles[MAIN_BGM])){
			HAZAMA::draw_helper->PlaySoundInMemory(sound_handles[MAIN_BGM], DX_PLAYTYPE_LOOP);
		}
	}

	/**
	* Stopを呼び出してからこの関数を呼び出すと、Stopを呼び出したところから曲の再生が再開される
	*/
	static void Resume(void){
		HAZAMA::draw_helper->PlaySoundInMemory(sound_handles[MAIN_BGM], DX_PLAYTYPE_LOOP, false);
	}

	/**
	* 曲の再生を停止する
	* 一時停止でも永久停止でもある
	*/
	static void Stop(void){
		HAZAMA::draw_helper->StopSoundInMemory(sound_handles[MAIN_BGM]);
	}

	/**
	* BGMのボリュームを下げる
	*/
	static void TurnDownVolume(void){
		HAZAMA::draw_helper->ChangeVolumeOfSoundInMemory(255 * 20 / 100, sound_handles[MAIN_BGM]);
	}

	/**
	* BGMのボリュームを元に戻す
	*/
	static void TurnUpVolume(void){
		HAZAMA::draw_helper->ChangeVolumeOfSoundInMemory(255 * 60 / 100, sound_handles[MAIN_BGM]);
	}
};

class EliminatorSE : public Sounds
{
private:
	EliminatorSE(void){};

public:
	static void Play(void){
		MainBGM::TurnDownVolume();
		HAZAMA::draw_helper->PlaySoundInMemory(sound_handles[ELIMINATOR_SE], DX_PLAYTYPE_BACK);
	}
};

class EliminatorXSE : public Sounds
{
private:
	EliminatorXSE(void){};

public:
	static void Play(void){
		MainBGM::TurnDownVolume();
		HAZAMA::draw_helper->PlaySoundInMemory(sound_handles[ELIMINATOR_X_SE], DX_PLAYTYPE_BACK);
	}
};

class ExplosionSE : public Sounds
{
private:
	ExplosionSE(void){};

public:
	static void Play(void){
		MainBGM::TurnDownVolume();
		HAZAMA::draw_helper->PlaySoundInMemory(sound_handles[EXPLOSION_SE], DX_PLAYTYPE_BACK);
	}
};

#endif
