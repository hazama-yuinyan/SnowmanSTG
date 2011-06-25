#ifndef _SOUND_H
#define _SOUND_H

enum SOUNDS{SELF_GOT_HIT_SE, GOT_ITEM_SE, ELIMINATOR_SE, ELIMINATOR_X_SE, EXPLOSION_SE, MAIN_BGM, SOUND_END};

class Sounds
{
protected:
	static int sound_handles[SOUND_END];		//!< @brief �T�E���h�̃n���h����ێ�����z��

	/**
	* �R���X�g���N�^�i�v���e�N�g�錾�j
	*/
	Sounds(void){};

public:
	/**
	* �f�R���X�g���N�^
	*/
	~Sounds(void){};

	/**
	* ���݈ʒu����p������ʂ����߂�
	*/
	static int GetAmountPan(const double Pos){
		double tmp = Pos - 320.0;
		return static_cast<int>(Pos * 31.25);
	}

	/**
	* �T�E���h�f�[�^���������[�ɓǂݍ���
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
		HAZAMA::draw_helper->SetLoopPointOfSoundInMemory(8000, sound_handles[MAIN_BGM]);			//���C����BGM�̃��[�v�ʒu��ݒ肷��
		HAZAMA::draw_helper->ChangeVolumeOfSoundInMemory(255 * 60 / 100, sound_handles[MAIN_BGM]);
		if(!HAZAMA::draw_helper->IsSoundPlayingWith(sound_handles[MAIN_BGM])){
			HAZAMA::draw_helper->PlaySoundInMemory(sound_handles[MAIN_BGM], DX_PLAYTYPE_LOOP);
		}
	}

	/**
	* Stop���Ăяo���Ă��炱�̊֐����Ăяo���ƁAStop���Ăяo�����Ƃ��납��Ȃ̍Đ����ĊJ�����
	*/
	static void Resume(void){
		HAZAMA::draw_helper->PlaySoundInMemory(sound_handles[MAIN_BGM], DX_PLAYTYPE_LOOP, false);
	}

	/**
	* �Ȃ̍Đ����~����
	* �ꎞ��~�ł��i�v��~�ł�����
	*/
	static void Stop(void){
		HAZAMA::draw_helper->StopSoundInMemory(sound_handles[MAIN_BGM]);
	}

	/**
	* BGM�̃{�����[����������
	*/
	static void TurnDownVolume(void){
		HAZAMA::draw_helper->ChangeVolumeOfSoundInMemory(255 * 20 / 100, sound_handles[MAIN_BGM]);
	}

	/**
	* BGM�̃{�����[�������ɖ߂�
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
