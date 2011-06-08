#include "stdafx.h"
#include "Sounds.h"

int CSounds::sound_handles[SOUND_END];		//ê√ìIÉÅÉìÉoïœêîÇÃé¿ëÃ

void CSounds::LoadSounds(void)
{
	const char SELF_GOT_HIT_SE_NAME[] = "Data\\Sounds\\SelfGotHit.mp3";
	const char GOT_ITEM_SE_NAME[] = "Data\\Sounds\\GotItem.mp3";
	const char ELIMINATOR_SE_NAME[] = "Data\\Sounds\\Eliminator.mp3";
	const char ELIMINATOR_X_SE_NAME[] = "Data\\Sounds\\Eliminator_x.mp3";
	const char EXPLOSION_SE_NAME[] = "Data\\Sounds\\Explosion.mp3";
	const char BGM_NAME[] = "Data\\Sounds\\shooting_master.mp3";

	sound_handles[SELF_GOT_HIT_SE] = LoadSoundMem(SELF_GOT_HIT_SE_NAME);
	sound_handles[GOT_ITEM_SE] = LoadSoundMem(GOT_ITEM_SE_NAME);
	sound_handles[ELIMINATOR_SE] = LoadSoundMem(ELIMINATOR_SE_NAME);
	sound_handles[ELIMINATOR_X_SE] = LoadSoundMem(ELIMINATOR_X_SE_NAME);
	sound_handles[EXPLOSION_SE] = LoadSoundMem(EXPLOSION_SE_NAME);
	sound_handles[MAIN_BGM] = LoadSoundMem(BGM_NAME);
}