#include "stdafx.h"
#include "Task.h"
#include "Sounds.h"
#include "InputDevice.h"



CTaskSystem::CTaskSystem(CInputDeviceKeyBoard *p_key) : continue_flag(true)
{
	Dix::sp<IOperator> sp_ope_straight(new COperatorStraight);
	Dix::sp<CDrawTask> sp_draw_task(new CDrawTask);
	Dix::sp<CSnowmanTask> sp_task_snow(new CSnowmanTask(sp_ope_straight));
	Dix::sp<CZetsubouTask> sp_task_zetsu(new CZetsubouTask(sp_ope_straight));
	Dix::sp<CSelfBulletTask> sp_task_sel_bul(new CSelfBulletTask(sp_ope_straight));
	Dix::sp<CFoeBulletTask> sp_task_foe_bul(new CFoeBulletTask(sp_ope_straight));
	Dix::sp<CItemTask> sp_task_item(new CItemTask(sp_ope_straight));
	Dix::sp<CReplayTask> sp_replay(new CReplayTask(p_key));

	m_mapTask.insert(TASK_PAIR(TASK_DRAW, sp_draw_task));
	m_mapTask.insert(TASK_PAIR(TASK_SNOWMAN, sp_task_snow));
	m_mapTask.insert(TASK_PAIR(TASK_ZETSUBOU, sp_task_zetsu));
	m_mapTask.insert(TASK_PAIR(TASK_SELFBULLET, sp_task_sel_bul));
	m_mapTask.insert(TASK_PAIR(TASK_FOEBULLET, sp_task_foe_bul));
	m_mapTask.insert(TASK_PAIR(TASK_ITEM, sp_task_item));
	m_mapTask.insert(TASK_PAIR(TASK_REPLAY, sp_replay));

	CMyObjectFactory::SetDrawTask(sp_draw_task.GetPtr());
	SwitchTask(TASK_DRAW);
}

bool CTaskSystem::Init(void)
{
	Dix::sp<CTouchTask> sp_task_touch(new CTouchTask);
	Dix::sp<CScoreTask> sp_task_score(new CScoreTask);
	
	CReplayTask *p_replay = GetTask<CReplayTask>(TASK_REPLAY);
	p_replay->SetRecordingFile("Data\\Replay\\replay.dat");

	m_mapTask.insert(TASK_PAIR(TASK_TOUCH, sp_task_touch));
	m_mapTask.insert(TASK_PAIR(TASK_SCORE, sp_task_score));
	return true;
}

bool CTaskSystem::Update(void)
{
	TASK_MAP::iterator it = m_mapActTask.begin();
	for(; it != m_mapActTask.end(); ++it){
		(*it).second->Update();
	}
	CheckTaskMessage();
	CTimerForGames::Instance().GetTotalTime();	//�^�C�}�[�X�V
	return continue_flag;
}

void CTaskSystem::SwitchMovingTasks(bool OnOrOff)
{
	SwitchTask(TASK_SNOWMAN, OnOrOff);
	SwitchTask(TASK_ZETSUBOU, OnOrOff);
	SwitchTask(TASK_SELFBULLET, OnOrOff);
	SwitchTask(TASK_FOEBULLET, OnOrOff);
	SwitchTask(TASK_ITEM, OnOrOff);
	SwitchTask(TASK_SCORE, OnOrOff);
	SwitchTask(TASK_REPLAY, OnOrOff);
}

void CTaskSystem::CheckTaskMessage(void)
{
	if(GetTaskMessage(TASK_TOUCH) == DONT_MOVE){
		SwitchMovingTasks();
		ResetMessage(TASK_TOUCH);
	}else if(GetTaskMessage(TASK_DRAW) != NONE){
		if(GetTaskMessage(TASK_DRAW) == ENDING_GAME){	//�Q�[���I���̂��m�点
			continue_flag = false;
			return;
		}

		SwitchMovingTasks(true);
		if(GetTaskMessage(TASK_DRAW) == ELIMINATING){	//�A�C�e��eliminator�擾���̏���
			CZetsubouTask *p_zetsu;
			CFoeBulletTask *p_foe_bullet;
			p_zetsu = GetTask<CZetsubouTask>(TASK_ZETSUBOU);
			p_foe_bullet = GetTask<CFoeBulletTask>(TASK_FOEBULLET);
			p_zetsu->Eliminate();
			p_foe_bullet->Eliminate();
		}

		ResetMessage(TASK_DRAW);
	}else if(GetTaskMessage(TASK_SNOWMAN) != NONE){		//SelfBullet�ύX��̏���
		CSelfBulletTask *p_self_bullet;
		p_self_bullet = GetTask<CSelfBulletTask>(TASK_SELFBULLET);
		p_self_bullet->ResetOpes(true);
		if(GetTaskMessage(TASK_SNOWMAN) == ADD_OPE_APPLYGRAVITY){
			Dix::sp<COperatorApplyGravity> sp_gravity(new COperatorApplyGravity);
			p_self_bullet->SetOperator(sp_gravity, true);
		}
	}
}

void CTaskSystem::SetReplayMode(bool Flag)
{
	CReplayTask *p_replay = GetTask<CReplayTask>(TASK_REPLAY);
	p_replay->TurnReplayFlag(Flag);
}

bool CDrawTask::Update(void)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	remove_copy_if(l_sp_draw_obj.begin(), l_sp_draw_obj.end(), l_have_msgs_obj.begin(), mem_fn(&CDrawObj::Draw));
		
	if(IsMessaged()){	//���b�Z�[�W�𔭂������I�u�W�F�N�g���������ꍇ
		EraseDummies();	//�_�~�[�̗v�f������

		OBJ_MSG msg = NOTHING;		//�I�u�W�F�N�g���烁�b�Z�[�W���󂯎��
		for_each(l_have_msgs_obj.begin(), l_have_msgs_obj.end(), boost::bind(&CDrawObj::MessageGet, _1, ref(msg)));
		if(msg == MOVING){
			message = CAN_MOVE;
		}else if(msg == ELIMINATE){
			message = ELIMINATING;
		}else{
			message = ENDING_GAME;
			return false;
		}

		InittheList();		//���b�Z�[�W���������I�u�W�F�N�g���X�g������������
	}

	/*��ʂɕ`�悳��Ȃ����̂̓��X�g����O��
		������factory�N���X��������������*/
	l_sp_draw_obj.remove_if(boost::mem_fn(&CDrawObj::IsNotDrawn));

	ScreenFlip();
	return true;
}

void CDrawTask::InittheList(void)
{
	unsigned i = 0;

	if(!l_have_msgs_obj.empty()){
		list<SPDrawObj>::iterator it = l_have_msgs_obj.begin();
		for(; i < l_have_msgs_obj.size(); ++i){		//���b�Z�[�W�����������I�u�W�F�N�g���_�~�[�ɂ���
			(*it++) = NULL;
		}
	}
	
	for(; i < 3; ++i){		//�c��̘g���_�~�[�Ŗ��߂�
		SPDrawObj dummy;
		l_have_msgs_obj.push_back(dummy);
	}
}

bool CSnowmanTask::Update(void)
{
	Dix::sp<CMoveOperator> sp_move_ope;
	Dix::sp<Snowman> sp_snowman;

	if(!CheckUsageFlag()){
		return false;	// �g���Ȃ�
	}

	CMyFactory<Snowman>::Instance().Create(0, sp_snowman);

	//�ړ��Ɋւ���I�y���[�^�[�𓮂���
	for(int i = v_sp_ope.size() - 1; i >= 0; --i){
		if(sp_move_ope.DownCast(v_sp_ope[i])){
			sp_move_ope->SetPosition(sp_snowman->GetPosition());
			sp_move_ope->SetVector(sp_snowman->GetVector());
			sp_move_ope->SetSpeed(sp_snowman->GetSpeed());
			sp_move_ope->Operate();
			sp_snowman->SetVector(sp_move_ope->GetVector());
			sp_snowman->SetPosition(sp_move_ope->GetPosition());
		}
	}

	ResetOpes();
	sp_snowman->SetVector(HAZAMA::V2Get(0, 0));
	return true;
}

void CSnowmanTask::Shoot(void)
{
	void (CSnowmanTask::*func_tbl[GRAVIFIED+1])(Dix::sp<Snowman>&) = {
		&CSnowmanTask::CreateNormalBullet,
		&CSnowmanTask::CreateTriBullets,
		&CSnowmanTask::CreateGravifiedBullet
	};
	Dix::sp<Snowman> sp_snowman;
	CMyFactory<Snowman>::Instance().Create(0, sp_snowman);

	int creating_bullet_num = static_cast<int>(sp_snowman->GetBulletType());
	(this->*func_tbl[creating_bullet_num])(sp_snowman);
}

void CSnowmanTask::CreateNormalBullet(Dix::sp<Snowman> &SPSnowman)
{
	Dix::sp<SelfNormalBullet> sp_self_bullet;
	OBJID objid = CMyFactory<SelfNormalBullet>::Instance().Size();

	 CMyFactory<SelfNormalBullet>::Instance().Create(objid, sp_self_bullet);

	//�e�̏����ʒu��ݒ肷��
	HAZAMA::VECTOR2 initial_pos = SPSnowman->GetPosition();
	initial_pos.x += SPSnowman->GetRect().right;
	initial_pos.y += 32.0f;
	sp_self_bullet->SetPosition(initial_pos);
	message = REMOVE_EXTRA_OPE;
}

void CSnowmanTask::CreateTriBullets(Dix::sp<Snowman> &SPSnowman)
{
	Dix::sp<SelfNormalBullet> sp_self_bullet;
	OBJID objid =  CMyFactory<SelfNormalBullet>::Instance().Size();

	for(int i = 0; i < 3; ++i){
		objid += i;
		CMyFactory<SelfNormalBullet>::Instance().Create(objid, sp_self_bullet);

		//�e�̏����ʒu��ݒ肷��
		HAZAMA::VECTOR2 initial_pos = SPSnowman->GetPosition();
		initial_pos.x += SPSnowman->GetRect().right;
		initial_pos.y += 32.0f;
		sp_self_bullet->SetPosition(initial_pos);

		switch(i){				//���˂���鏇�Ԃɂ����Y�������̏����x��^����
			case 1:
				sp_self_bullet->SetVector(300.0f, 1);
				break;
			case 2:
				sp_self_bullet->SetVector(-300.0f, 1);
				break;
			default:
				break;
		}
	}
	message = REMOVE_EXTRA_OPE;
}

void CSnowmanTask::CreateGravifiedBullet(Dix::sp<Snowman> &SPSnowman)
{
	Dix::sp<SelfGravifiedBullet> sp_self_bullet;
	OBJID objid =  CMyFactory<SelfGravifiedBullet>::Instance().Size();

	if(!CMyFactory<SelfGravifiedBullet>::Instance().HaveObj()){
		message = ADD_OPE_APPLYGRAVITY;
	}
	
	CMyFactory<SelfGravifiedBullet>::Instance().Create(objid, sp_self_bullet);

	//�e�̏����ʒu��ݒ肷��
	HAZAMA::VECTOR2 initial_pos = SPSnowman->GetPosition();
	initial_pos.x += SPSnowman->GetRect().right;
	initial_pos.y += 32.0f;
	sp_self_bullet->SetPosition(initial_pos);
}

bool CZetsubouTask::Update(void)
{
	Dix::sp<CMoveOperator> sp_move_ope;
	Dix::sp<Zetsubou> sp_zetsu;

	if(!CheckUsageFlag()){
		return false;	// �g���Ȃ�
	}

	CreatNewZetsu();

	for(unsigned i = 0; i < CMyFactory<Zetsubou>::Instance().Size(); ++i){ 
		
		CMyFactory<Zetsubou>::Instance().Create(i, sp_zetsu);
		if(!sp_zetsu->IsNotDrawn()){		//�`�悳���Zetsubou�Ȃ�e���˂̏���������
			Shoot(sp_zetsu);
		}

		//�ړ��Ɋւ���I�y���[�^�[�𓮂���
		for(int j = v_sp_ope.size() - 1; j >= 0; --j){
			if(sp_move_ope.DownCast(v_sp_ope[j])){
				sp_move_ope->SetPosition(sp_zetsu->GetPosition());
				sp_move_ope->SetVector(sp_zetsu->GetVector());
				sp_move_ope->SetSpeed(sp_zetsu->GetSpeed());
				sp_move_ope->Operate();
				sp_zetsu->SetVector(sp_move_ope->GetVector());
				sp_zetsu->SetPosition(sp_move_ope->GetPosition());
			}
		}

		sp_zetsu->SetVector(HAZAMA::V2Get(-1.0f * sp_zetsu->GetSpeed(), 0));
	}

	ResetOpes();
	return true;
}

void CZetsubouTask::Shoot(Dix::sp<Zetsubou> SP)
{
	Dix::sp<FoeBullet> sp_foe_bullet;
	HAZAMA::RECT zetsu_rect = SP->GetRect() + SP->GetPosition();	//Zetsubou�I�u�W�F�N�g�̒��_�̐�΍��W���o��
	if(GetNowCount() % 1000 >= 850 && !IsOthersNearby(zetsu_rect)){
		CMyFactory<FoeBullet> *p_factory = &CMyFactory<FoeBullet>::Instance();
		OBJID objid = p_factory->Size();
		p_factory->Create(objid, sp_foe_bullet);

		//�e�̏����ʒu��ݒ肷��
		HAZAMA::VECTOR2 initial_pos = SP->GetPosition();
		initial_pos.x -= 24.0f;
		initial_pos.y += 16.0f;
		sp_foe_bullet->SetPosition(initial_pos);
	}
}

bool CZetsubouTask::IsOthersNearby(const HAZAMA::RECT &Rect)
{
	bool return_bool = false;
	Dix::sp<FoeBullet> sp_existent;
	Dix::sp<Snowman> sp_snowman;
	HAZAMA::VECTOR2 bullet_pos;
	HAZAMA::RECT snowman_rect;

	CMyFactory<Snowman>::Instance().Create(0, sp_snowman);
	snowman_rect = sp_snowman->GetRect();

	for(unsigned i = 0; i < CMyFactory<FoeBullet>::Instance().Size(); ++i){
		CMyFactory<FoeBullet>::Instance().Create(i, sp_existent);
		bullet_pos = sp_existent->GetPosition();
		if(Rect.right + snowman_rect.right + EXTRA_MARGIN > bullet_pos.x
			&& Rect.left - snowman_rect.right - EXTRA_MARGIN < bullet_pos.x
			&& Rect.down + snowman_rect.down + EXTRA_MARGIN > bullet_pos.y
			&& Rect.up - snowman_rect.down - EXTRA_MARGIN < bullet_pos.y){

				return_bool = true;		//�͈͓��ɂ���΁A�߂�l�ɂ��邱�Ƃ��w�肵�ă��[�v�𔲂���
				break;
		}else{
				continue;				//�͈͓��ɂȂ���΁A����FoeBullet��
		}
	}
	return return_bool;
}

void CZetsubouTask::CreatNewZetsu(void)
{
	Dix::sp<Zetsubou> sp_zetsu;
	if(!p_timer->Exists(timer_id)){		//�V�K�^�C�}�[�X�^�[�g
		timer_id = p_timer->StartTimer();
	}
	
	if(count_min == 0){
		count_min = GetRand(1000);
	}else{
		if(p_timer->IsTakenUp(timer_id, count_min)){		//������x�̊Ԃ��������ēG���o��

			CMyFactory<Zetsubou> *p_factory = &CMyFactory<Zetsubou>::Instance();
			if(p_factory->Size() <= MAX_NUM_FOES){
				OBJID objid = p_factory->Size();
				p_factory->Create(objid, sp_zetsu);

				count_min = 0;
				timer_id = 0;
			}
		}
	}
}

bool CSelfBulletTask::Update(void)
{
	Dix::sp<CMoveOperator> sp_move_ope;
	Dix::sp<SelfBullet> sp_self_bullet;
	unsigned objid[2] = {0, 0};

	if(!CheckUsageFlag()){
		return false;	// �g���Ȃ�
	}

	//�S�Ă̎�ނ̒e���������I���܂Ń��[�v�𔲂��Ȃ�
	while(CMyFactory<SelfNormalBullet>::Instance().Exists(sp_self_bullet, objid[0]++)
		|| CMyFactory<SelfGravifiedBullet>::Instance().Exists(sp_self_bullet, objid[1]++)){ 
		
		//�ړ��Ɋւ���I�y���[�^�[�𓮂���
		for(int i = v_sp_ope.size() - 1; i >= 0; --i){
			if(sp_move_ope.DownCast(v_sp_ope[i])){
				sp_move_ope->SetPosition(sp_self_bullet->GetPosition());
				sp_move_ope->SetVector(sp_self_bullet->GetVector());
				sp_move_ope->SetSpeed(sp_self_bullet->GetSpeed());
				sp_move_ope->Operate();
				sp_self_bullet->SetVector(sp_move_ope->GetVector());
				sp_self_bullet->SetPosition(sp_move_ope->GetPosition());
			}
		}
	}

	ResetOpes();
	return true;
}

bool CFoeBulletTask::Update(void)
{
	Dix::sp<CMoveOperator> sp_move_ope;
	Dix::sp<FoeBullet> sp_foe_bullet;

	if(!CheckUsageFlag()){
		return false;	// �g���Ȃ�
	}

	for(size_t i = 0; i < CMyFactory<FoeBullet>::Instance().Size(); ++i){ 
		
		CMyFactory<FoeBullet>::Instance().Create(i, sp_foe_bullet);
		//�ړ��Ɋւ���I�y���[�^�[�𓮂���
		for(int j = v_sp_ope.size() - 1; j >= 0; --j){
			if(sp_move_ope.DownCast(v_sp_ope[j])){
				sp_move_ope->SetPosition(sp_foe_bullet->GetPosition());
				sp_move_ope->SetVector(sp_foe_bullet->GetVector());
				sp_move_ope->SetSpeed(sp_foe_bullet->GetSpeed());
				sp_move_ope->Operate();
				sp_foe_bullet->SetVector(sp_move_ope->GetVector());
				sp_foe_bullet->SetPosition(sp_move_ope->GetPosition());
			}
		}

		sp_foe_bullet->SetVector(HAZAMA::V2Get(-1.0f * sp_foe_bullet->GetSpeed(), 0));
	}

	ResetOpes();
	return true;
}

bool CItemTask::Update(void)
{
	Dix::sp<CMoveOperator> sp_move_ope;
	Dix::sp<Items> sp_item;
	Dix::sp<Snowman> sp_snowman;

	if(!CheckUsageFlag()){
		return false;	// �g���Ȃ�
	}

	if(!CMyFactory<LifeExtend>::Instance().Exists(sp_item, 0)
		&& !CMyFactory<Barrier>::Instance().Exists(sp_item, 0)
		&& !CMyFactory<Quick>::Instance().Exists(sp_item, 0)
		&& !CMyFactory<Multiplier>::Instance().Exists(sp_item, 0)
		&& !CMyFactory<Eliminator>::Instance().Exists(sp_item, 0)
		&& !CMyFactory<TriBullets>::Instance().Exists(sp_item, 0)
		&& !CMyFactory<GravifiedBullet>::Instance().Exists(sp_item, 0)
		&& !CMyFactory<EliminatorX>::Instance().Exists(sp_item, 0)){
			CreateNewItem();		//�A�C�e����1�����݂��ĂȂ�������A�V������������
	}

	CMyFactory<Snowman>::Instance().Create(0, sp_snowman);
	
	if(sp_item != NULL){
		//�ړ��Ɋւ���I�y���[�^�[�𓮂���
		for(int i = v_sp_ope.size() - 1; i >= 0; --i){
			if(sp_move_ope.DownCast(v_sp_ope[i])){
				sp_move_ope->SetPosition(sp_item->GetPosition());
				sp_move_ope->SetVector(sp_item->GetVector());
				sp_move_ope->SetSpeed(sp_item->GetSpeed());
				sp_move_ope->SetTargetPosition(sp_snowman->GetPosition());
				sp_move_ope->SetRects(sp_item->GetRect(), sp_snowman->GetRect());
				sp_move_ope->Operate();
				sp_item->SetVector(sp_move_ope->GetVector());
				sp_item->SetPosition(sp_move_ope->GetPosition());
			}
		}

		//EliminatorX�̎������u�N����̏������s��
		Dix::sp<EliminatorX> sp_elimx;
		if(sp_elimx.DownCast(sp_item) && sp_elimx->IsDestroyed()){
			Dix::sp<CExploAnim> sp_explo;
			CMyFactory<CExploAnim>::Instance().Create(0, sp_explo);
			HAZAMA::RECT anim_rect = sp_explo->GetRect();
			HAZAMA::VECTOR2 anim_pos = sp_elimx->GetPosition() - HAZAMA::V2Get(anim_rect.right / 2, anim_rect.down / 2);
			sp_explo->SetPosition(anim_pos);		//�����A�j���̍Đ��ʒu���A�C�e���̌��݈ʒu�ɐݒ�
			sp_elimx->Clear();
		}
	}

	ResetOpes();
	return true;
}

void CItemTask::CreateNewItem(void)
{
	bool (CItemTask::*func_tbl[NUM_ITEM_TYPES]) (void) = {				//�֐��|�C���^�e�[�u���̍쐬
		&CItemTask::CreateLifeEx,
		&CItemTask::CreateBarrier,
		&CItemTask::CreateQuick,
		&CItemTask::CreateMultiplier,
		&CItemTask::CreateEliminator,
		&CItemTask::CreateTriBullets,
		&CItemTask::CreateGravifiedBullet,
		&CItemTask::CreateEliminatorX
	};

	int type;

	if(!p_timer->Exists(timer_id)){		//�V�K�^�C�}�[�X�^�[�g
		timer_id = p_timer->StartTimer();
	}

	if(p_timer->IsTakenUp(timer_id, next_count)){
		next_count = GetRand(10000) + 20000;
		cur_score = Score::Instance().GetCurScore();

#ifndef _DEBUG
		if(cur_score < 5000){		//���݂̃X�R�A��5000�_�ȉ��Ȃ�A�ȉ��̏����������Ɋ֐��𔲂���
			return;
		}
#endif

		do{
			type = GetRand(NUM_ITEM_TYPES - 1);			/*���炩�̃A�C�e�������������܂�
											���������ނ�ς��Ă���*/
		}while(!(this->*func_tbl[type])() );
		timer_id = 0;
	}
}

bool CTouchTask::Update(void)
{
	if(!CheckUsageFlag()){
		return false;	// �g���Ȃ�
	}

	SelfBulletAndFoe();
	SelfAndFoe();
	SelfAndFoeBullet();
	SelfAndItem();
	Others();

	return true;
}

void CTouchTask::SelfBulletAndFoe(void)
{
	Dix::sp<Zetsubou> sp_zetsu;
	Dix::sp<SelfBullet> sp_self_bullet;

	for(unsigned i = 0; i < CMyFactory<Zetsubou>::Instance().Size(); ++i){
		
		CMyFactory<Zetsubou>::Instance().Create(i, sp_zetsu);
		if(sp_zetsu->IsNotDrawn()){
			continue;		//��ʂɕ`�悳��Ȃ�Zetsubou�̓X�L�b�v����
		}

		unsigned objid[2] = {0, 0};

		while(CMyFactory<SelfNormalBullet>::Instance().Exists(sp_self_bullet, objid[0]++)
			|| CMyFactory<SelfGravifiedBullet>::Instance().Exists(sp_self_bullet, objid[1]++)){

			if(sp_zetsu->Dispatch(sp_self_bullet.GetPtr())){
				sp_self_bullet->Dispatch(sp_zetsu.GetPtr());
				break;		//����Zetsubou��
			}
		}
	}
}

void CTouchTask::SelfAndFoe(void)
{
	Dix::sp<Zetsubou> sp_zetsu;

	CMyFactory<Snowman>::Instance().Create(0, sp_snowman);
	for(unsigned i = 0; i < CMyFactory<Zetsubou>::Instance().Size(); ++i){

		CMyFactory<Zetsubou>::Instance().Create(i, sp_zetsu);		//Zetsubou��Snowman�̓����蔻��
		if(sp_zetsu->IsNotDrawn()){
			continue;		//��ʂɕ`�悳��Ȃ�Zetsubou�̓X�L�b�v����
		}

		if(sp_zetsu->Dispatch(sp_snowman.GetPtr()) && sp_snowman->Dispatch(sp_zetsu.GetPtr())){
			Dix::sp<CSelfGotHitAnim> sp_self_got_hit;
			CMyFactory<CSelfGotHitAnim>::Instance().Create(0, sp_self_got_hit);
			sp_self_got_hit->SetPosition(sp_snowman->GetPosition());
			sp_snowman->Initialize();
			message = DONT_MOVE;
			break;
		}
	}
}

void CTouchTask::SelfAndFoeBullet(void)
{
	CMyFactory<Snowman>::Instance().Create(0, sp_snowman);
	for(unsigned i = 0; i < CMyFactory<FoeBullet>::Instance().Size(); ++i){

		CMyFactory<FoeBullet>::Instance().Create(i, sp_foe_bullet);		//FoeBullet��Snowman�̓����蔻��
		if(sp_foe_bullet->IsNotDrawn()){
			continue;		//��ʂɕ`�悳��Ȃ�FoeBullet�̓X�L�b�v����
		}

		if(sp_foe_bullet->Dispatch(sp_snowman.GetPtr()) && sp_snowman->Dispatch(sp_foe_bullet.GetPtr())){
			Dix::sp<CSelfGotHitAnim> sp_self_got_hit;
			CMyFactory<CSelfGotHitAnim>::Instance().Create(0, sp_self_got_hit);
			sp_self_got_hit->SetPosition(sp_snowman->GetPosition());
			sp_snowman->Initialize();
			message = DONT_MOVE;
			break;
		}
	}
}

void CTouchTask::SelfAndItem(void)
{
	CMyFactory<Snowman>::Instance().Create(0, sp_snowman);

	if(!CMyFactory<LifeExtend>::Instance().HaveObj()
		&& !CMyFactory<Barrier>::Instance().HaveObj()
		&& !CMyFactory<Quick>::Instance().HaveObj()
		&& !CMyFactory<Multiplier>::Instance().HaveObj()
		&& !CMyFactory<Eliminator>::Instance().HaveObj()
		&& !CMyFactory<TriBullets>::Instance().HaveObj()
		&& !CMyFactory<GravifiedBullet>::Instance().HaveObj()
		&& !CMyFactory<EliminatorX>::Instance().HaveObj()){
			return;			//�A�C�e�������݂��ĂȂ��Ȃ�A�����蔻������Ȃ�
	}

	if( sp_snowman->Dispatch(sp_item.GetPtr()) && sp_item->Dispatch(sp_snowman.GetPtr())){

		GotItemSE::Play();		//�A�C�e���l���̌��ʉ���炷
		if(typeid(*sp_item) == typeid(Eliminator) || typeid(*sp_item) == typeid(EliminatorX)){
			Dix::sp<CFireAnim> sp_fire_anim;
			CMyFactory<CFireAnim>::Instance().Create(0, sp_fire_anim);
			if(typeid(*sp_item) == typeid(EliminatorX)){
				sp_fire_anim->SetSEType(true);
			}else{
				sp_fire_anim->SetSEType(false);
			}
			message = DONT_MOVE;
		}
	}
}

void CTouchTask::Others(void)
{
	Dix::sp<Zetsubou> sp_zetsu;
	Dix::sp<CExploAnim> sp_explo;
	if(!CMyFactory<CExploAnim>::Instance().HaveObj()){	//�����̃A�j���[�V���������݂��Ȃ��Ȃ�A�������s��Ȃ�
		return;
	}

	CMyFactory<CExploAnim>::Instance().Create(0, sp_explo);
	CMyFactory<Snowman>::Instance().Create(0, sp_snowman);

	if(sp_explo->Dispatch(sp_snowman.GetPtr())){
		sp_snowman->Dispatch(sp_explo.GetPtr());
		Dix::sp<CSelfGotHitAnim> sp_self_got_hit;
		CMyFactory<CSelfGotHitAnim>::Instance().Create(0, sp_self_got_hit);
		sp_self_got_hit->SetPosition(sp_snowman->GetPosition());
		sp_snowman->Initialize();
		message = DONT_MOVE;
	}

	for(unsigned i = 0; i < CMyFactory<Zetsubou>::Instance().Size(); ++i){

		CMyFactory<Zetsubou>::Instance().Create(i, sp_zetsu);
		if(sp_zetsu->IsNotDrawn()){
			continue;		//��ʂɕ`�悳��Ȃ�Zetsubou�̓X�L�b�v����
		}

		if(sp_explo->Dispatch(sp_zetsu.GetPtr())){
			sp_zetsu->Dispatch(sp_explo.GetPtr());
		}
	}
}

bool CScoreTask::Update(void)
{
	if(!CheckUsageFlag()){
		return false;	// �g���Ȃ�
	}

	Score::Instance().AddScore();
	return true;
}

CReplayTask::CReplayTask(CInputDeviceKeyBoard *p_key) : replay_flag(false), o_file(), i_file(), sp_iarchive()
, sp_oarchive(), p_keyboard(p_key)
{}

bool CReplayTask::Update(void)
{
	if(!CheckUsageFlag()){
		return false;	// �g���Ȃ�
	}

	if(!replay_flag){		//���v���C�f�[�^���t�@�C���ɏo��
		*sp_oarchive << (const CTimerForGames&)CTimerForGames::Instance();
		*sp_oarchive << (const CInputDeviceKeyBoard&)p_keyboard;
	}else{					//���v���C�f�[�^���t�@�C���������
		*sp_iarchive >> CTimerForGames::Instance();
		*sp_iarchive >> p_keyboard;
	}
	return true;
}

void CReplayTask::SetRecordingFile(const char src[])
{
	if(!replay_flag){
		o_file.open(src);
		assert(o_file);
		Dix::sp<boost::archive::text_oarchive> tmp(new boost::archive::text_oarchive(o_file));
		sp_oarchive = tmp;
	}else{
		i_file.open(src);
		assert(i_file);
		Dix::sp<boost::archive::text_iarchive> tmp(new boost::archive::text_iarchive(i_file));
		sp_iarchive = tmp;
	}
}