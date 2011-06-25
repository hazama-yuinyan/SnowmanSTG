#include "stdafx.h"
#include "Sounds.h"

int Sounds::sound_handles[SOUND_END];		//Ã“Iƒƒ“ƒo•Ï”‚ÌŽÀ‘Ì

void Sounds::LoadSounds(void)
{
	const char SELF_GOT_HIT_SE_NAME[] = "Data\\Sounds\\SelfGotHit.mp3";
	const char GOT_ITEM_SE_NAME[] = "Data\\Sounds\\GotItem.mp3";
	const char ELIMINATOR_SE_NAME[] = "Data\\Sounds\\Eliminator.mp3";
	const char ELIMINATOR_X_SE_NAME[] = "Data\\Sounds\\Eliminator_x.mp3";
	const char EXPLOSION_SE_NAME[] = "Data\\Sounds\\Explosion.mp3";
	const char BGM_NAME[] = "Data\\Sounds\\shooting_master.mp3";

	sound_handles[SELF_GOT_HIT_SE] = HAZAMA::draw_helper->LoadSoundToMemory(SELF_GOT_HIT_SE_NAME);
	sound_handles[GOT_ITEM_SE] = HAZAMA::draw_helper->LoadSoundToMemory(GOT_ITEM_SE_NAME);
	sound_handles[ELIMINATOR_SE] = HAZAMA::draw_helper->LoadSoundToMemory(ELIMINATOR_SE_NAME);
	sound_handles[ELIMINATOR_X_SE] = HAZAMA::draw_helper->LoadSoundToMemory(ELIMINATOR_X_SE_NAME);
	sound_handles[EXPLOSION_SE] = HAZAMA::draw_helper->LoadSoundToMemory(EXPLOSION_SE_NAME);
	sound_handles[MAIN_BGM] = HAZAMA::draw_helper->LoadSoundToMemory(BGM_NAME);
}