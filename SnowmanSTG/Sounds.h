#ifndef _SOUND_H
#define _SOUND_H

enum SOUNDS{SELF_GOT_HIT_SE, GOT_ITEM_SE, ELIMINATOR_SE, ELIMINATOR_X_SE, EXPLOSION_SE, MAIN_BGM, SOUND_END};

class CSounds
{
protected:
	static int sound_handles[SOUND_END];		//!< @brief �T�E���h�̃n���h����ێ�����z��

	/**
	* �R���X�g���N�^�i�v���e�N�g�錾�j
	*/
	CSounds(void){};

public:
	/**
	* �f�R���X�g���N�^
	*/
	~CSounds(void){};

	/**
	* ���݈ʒu����p������ʂ����߂�
	*/
	static int GetAmountPan(float Pos){
		Pos -= 320.0f;
		int tmp = static_cast<int>(Pos * 31.25f);
		return tmp;
	}

	/**
	* �T�E���h�f�[�^���������[�ɓǂݍ���
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
		SetLoopPosSoundMem(8000, sound_handles[MAIN_BGM]);			//���C����BGM�̃��[�v�ʒu��ݒ肷��
		ChangeVolumeSoundMem(255 * 60 / 100, sound_handles[MAIN_BGM]);
		if(CheckSoundMem(sound_handles[MAIN_BGM]) == 0){
			PlaySoundMem(sound_handles[MAIN_BGM], DX_PLAYTYPE_LOOP);
		}
	}

	/**
	* Stop���Ăяo���Ă��炱�̊֐����Ăяo���ƁAStop���Ăяo�����Ƃ��납��Ȃ̍Đ����ĊJ�����
	*/
	static void Resume(void){
		PlaySoundMem(sound_handles[MAIN_BGM], DX_PLAYTYPE_LOOP, false);
	}

	/**
	* �Ȃ̍Đ����~����
	* �ꎞ��~�ł��i�v��~�ł�����
	*/
	static void Stop(void){
		StopSoundMem(sound_handles[MAIN_BGM]);
	}

	/**
	* BGM�̃{�����[����������
	*/
	static void TurnDownVolume(void){
		ChangeVolumeSoundMem(255 * 20 / 100, sound_handles[MAIN_BGM]);
	}

	/**
	* BGM�̃{�����[�������ɖ߂�
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
