#ifndef _SOUND_H
#define _SOUND_H

enum SOUNDS{SELF_GOT_HIT_SE, GOT_ITEM_SE, ELIMINATOR_SE, ELIMINATOR_X_SE, EXPLOSION_SE, MAIN_BGM, SOUND_END};

class CSounds
{
protected:
	static int sound_handles[SOUND_END];		//!< @brief サウンドのハンドルを保持する配列

	/**
	* コンストラクタ（プロテクト宣言）
	*/
	CSounds(void){};

public:
	/**
	* デコンストラクタ
	*/
	~CSounds(void){};

	/**
	* 現在位置からパンする量を求める
	*/
	static int GetAmountPan(float Pos){
		Pos -= 320.0f;
		int tmp = static_cast<int>(Pos * 31.25f);
		return tmp;
	}

	/**
	* サウンドデータをメモリーに読み込む
	*/
	static void LoadSounds(void);
};

class SelfGotHitSE : public CSounds
{
private:
	SelfGotHitSE(void){};

public:
	static void Play(const float &PosX){
		SetPanSoundMem(GetAmountPan(PosX), sound_handles[SELF_GOT_HIT_SE]);
		PlaySoundMem(sound_handles[SELF_GOT_HIT_SE], DX_PLAYTYPE_BACK);
	}
};

class GotItemSE : public CSounds
{
private:
	GotItemSE(void){};

public:
	static void Play(void){
		PlaySoundMem(sound_handles[GOT_ITEM_SE], DX_PLAYTYPE_BACK);
	}
};

class MainBGM : public CSounds
{
private:
	MainBGM(void){};

public:
	static void Play(void){
		SetLoopPosSoundMem(8000, sound_handles[MAIN_BGM]);			//メインのBGMのループ位置を設定する
		ChangeVolumeSoundMem(255 * 60 / 100, sound_handles[MAIN_BGM]);
		if(CheckSoundMem(sound_handles[MAIN_BGM]) == 0){
			PlaySoundMem(sound_handles[MAIN_BGM], DX_PLAYTYPE_LOOP);
		}
	}

	/**
	* Stopを呼び出してからこの関数を呼び出すと、Stopを呼び出したところから曲の再生が再開される
	*/
	static void Resume(void){
		PlaySoundMem(sound_handles[MAIN_BGM], DX_PLAYTYPE_LOOP, false);
	}

	/**
	* 曲の再生を停止する
	* 一時停止でも永久停止でもある
	*/
	static void Stop(void){
		StopSoundMem(sound_handles[MAIN_BGM]);
	}

	/**
	* BGMのボリュームを下げる
	*/
	static void TurnDownVolume(void){
		ChangeVolumeSoundMem(255 * 20 / 100, sound_handles[MAIN_BGM]);
	}

	/**
	* BGMのボリュームを元に戻す
	*/
	static void TurnUpVolume(void){
		ChangeVolumeSoundMem(255 * 60 / 100, sound_handles[MAIN_BGM]);
	}
};

class EliminatorSE : public CSounds
{
private:
	EliminatorSE(void){};

public:
	static void Play(void){
		MainBGM::TurnDownVolume();
		PlaySoundMem(sound_handles[ELIMINATOR_SE], DX_PLAYTYPE_BACK);
	}
};

class EliminatorXSE : public CSounds
{
private:
	EliminatorXSE(void){};

public:
	static void Play(void){
		MainBGM::TurnDownVolume();
		PlaySoundMem(sound_handles[ELIMINATOR_X_SE], DX_PLAYTYPE_BACK);
	}
};

class ExplosionSE : public CSounds
{
private:
	ExplosionSE(void){};

public:
	static void Play(void){
		MainBGM::TurnDownVolume();
		PlaySoundMem(sound_handles[EXPLOSION_SE], DX_PLAYTYPE_BACK);
	}
};

#endif
