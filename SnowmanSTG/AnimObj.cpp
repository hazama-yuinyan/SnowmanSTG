#include "stdafx.h"
#include "AnimObj.h"
#include "Sounds.h"
#include "Task.h"



std::vector<Dix::sp<anim_handle> > AnimObj::v_sp_anim;		//静的メンバ変数の実体

void AnimObj::LoadPic(void)
{
	const char SELF_GOT_HIT_ANIM_NAME[] = "Data\\Images\\self_got_hit_anim.bmp";
	const char EXPLOSION_ANIM_NAME[] = "Data\\Images\\Explosion.bmp";
	const char FIRE_ANIM_NAME[] = "Data\\Images\\Fire.bmp";
	const int SELF_PIC_WIDTH = 96, SELF_PIC_HEIGHT = 96;

	Dix::sp<anim_handle> self_got_hit(new anim_handle[8], true), fire(new anim_handle[64], true)
		, explo(new anim_handle[16], true), max_frames(new anim_handle[MAX_FRAMES], true);
	
	HAZAMA::draw_helper->LoadDividedImage(SELF_GOT_HIT_ANIM_NAME, 8, 4, 2, SELF_PIC_WIDTH, SELF_PIC_HEIGHT, self_got_hit.GetPtr());
	HAZAMA::draw_helper->LoadDividedImage(FIRE_ANIM_NAME, 64, 8, 8, 80, 80, fire.GetPtr());
	HAZAMA::draw_helper->LoadDividedImage(EXPLOSION_ANIM_NAME, 16, 5, 4, 120, 120, explo.GetPtr());

	max_frames[0] = 8;
	max_frames[1] = 64;
	max_frames[2] = 16;

	v_sp_anim.push_back(self_got_hit);
	v_sp_anim.push_back(fire);
	v_sp_anim.push_back(explo);
	v_sp_anim.push_back(max_frames);
}

SelfGotHitAnim::SelfGotHitAnim(void) : AnimObj(1)
{
	rect(0, 0, 96.0f, 96.0f);
}

bool SelfGotHitAnim::Draw(void)
{
	Dix::sp<anim_handle> sp_tmp(v_sp_anim[SELF_GOT_HIT]);
	anim_handle *p_anim = &sp_tmp[0];

	SelfGotHitSE::Play(pos.x);		//やられたときの効果音を鳴らす

	for(int i = 0; i < 8; ++i){
		HAZAMA::draw_helper->SetDrawScreen(HAZAMA::DrawHelper::BACK);
		HAZAMA::draw_helper->InitializeScreen();
		HAZAMA::draw_helper->DrawImage(static_cast<int>(pos.x), static_cast<int>(pos.y), *p_anim, true);
		++p_anim;
		HAZAMA::draw_helper->FlipScreen();
		HAZAMA::draw_helper->WaitTimer(100);
		if(!HAZAMA::draw_helper->ProcessMessage()){
			return true;
		}
	}
	Clear();
	return false;
}

FireAnim::FireAnim(void) : CooperativeAnimObj(5), play_elimx_SE_flag(false)
{
	rect(0, 0, 80.0f, 80.0f);
	SetAnim(FIRE);
}

bool FireAnim::Draw(void)
{
	if(cur_frame == 0){			//効果音を鳴らす
		if(play_elimx_SE_flag){
			EliminatorXSE::Play();
		}else{
			EliminatorSE::Play();
		}
	}

	DrawAFrame();

	if(cur_frame == MAX_FRAME){
		MainBGM::TurnUpVolume();		//効果音を鳴らすときに音量を下げたBGMの音量を元に戻す
		Clear();
		return false;
	}
	return true;
}

void FireAnim::DrawAFrame(void)
{
	pos.x = 0, pos.y = 0;
	anim_handle *p_anim = wp_anim + cur_frame;		//今回描画するフレームの画像を取得

	for(int j = 0; j <= 8; ++j){
		pos.y = 0;
		for(int k = 0; k <= 8; ++k){
			HAZAMA::draw_helper->DrawImage(static_cast<int>(pos.x), static_cast<int>(pos.y), *p_anim, true);
			pos.y += 80;
		}
		pos.x += 80;
	}

	AdvanceFrame();
	HAZAMA::draw_helper->WaitTimer(50 - static_cast<int>(TimerForGames::Instance().GetCurDiffTime()));	//1コマ50msにするため、不足分待つ

	if(!HAZAMA::draw_helper->ProcessMessage()){
		return;
	}
}

ExploAnim::ExploAnim(void) : CooperativeAnimObj(4)
{
	rect(0, 0, 120.0f, 120.0f);
	touch_rect(0, 0, 120.0f, 120.0f);
	SetAnim(EXPLOSION);
}

bool ExploAnim::Draw(void)
{
	if(cur_frame == 0){
		ExplosionSE::Play();		//効果音を鳴らす
	}

	DrawAFrame();

	if(cur_frame == MAX_FRAME){
		MainBGM::TurnUpVolume();		//効果音を鳴らすときに音量を下げたBGMの音量を元に戻す
		Clear();
	}
	return true;
}

void ExploAnim::DrawAFrame(void)
{
	anim_handle *p_anim = wp_anim + cur_frame;		//今回描画するフレームの画像を取得

	HAZAMA::draw_helper->DrawImage(static_cast<int>(pos.x), static_cast<int>(pos.y), *p_anim, true);

	AdvanceFrame();

	if(!HAZAMA::draw_helper->ProcessMessage()){
		return;
	}
}