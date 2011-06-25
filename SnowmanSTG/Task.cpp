#include "stdafx.h"
#include "Task.h"
#include "Sounds.h"
#include "InputDevice.h"



TaskSystem::TaskSystem(InputDeviceKeyBoard *p_key) : continue_flag(true)
{
	Dix::sp<IOperator> sp_ope_straight(new OperatorStraight);
	Dix::sp<DrawTask> sp_draw_task(new DrawTask);
	Dix::sp<SnowmanTask> sp_task_snow(new SnowmanTask(sp_ope_straight));
	Dix::sp<ZetsubouTask> sp_task_zetsu(new ZetsubouTask(sp_ope_straight));
	Dix::sp<SelfBulletTask> sp_task_sel_bul(new SelfBulletTask(sp_ope_straight));
	Dix::sp<FoeBulletTask> sp_task_foe_bul(new FoeBulletTask(sp_ope_straight));
	Dix::sp<ItemTask> sp_task_item(new ItemTask(sp_ope_straight));
	//Dix::sp<ReplayTask> sp_replay(new ReplayTask(p_key));

	m_mapTask.insert(TASK_PAIR(TASK_DRAW, sp_draw_task));
	m_mapTask.insert(TASK_PAIR(TASK_SNOWMAN, sp_task_snow));
	m_mapTask.insert(TASK_PAIR(TASK_ZETSUBOU, sp_task_zetsu));
	m_mapTask.insert(TASK_PAIR(TASK_SELFBULLET, sp_task_sel_bul));
	m_mapTask.insert(TASK_PAIR(TASK_FOEBULLET, sp_task_foe_bul));
	m_mapTask.insert(TASK_PAIR(TASK_ITEM, sp_task_item));
	//m_mapTask.insert(TASK_PAIR(TASK_REPLAY, sp_replay));

	MyObjectFactory::SetDrawTask(sp_draw_task.GetPtr());
	SwitchTask(TASK_DRAW);
}

bool TaskSystem::Init(void)
{
	Dix::sp<TouchTask> sp_task_touch(new TouchTask);
	Dix::sp<ScoreTask> sp_task_score(new ScoreTask);
	
	//ReplayTask *p_replay = GetTask<ReplayTask>(TASK_REPLAY);
	//p_replay->SetRecordingFile("Data\\Replay\\replay.dat");

	m_mapTask.insert(TASK_PAIR(TASK_TOUCH, sp_task_touch));
	m_mapTask.insert(TASK_PAIR(TASK_SCORE, sp_task_score));
	return true;
}

bool TaskSystem::Update(void)
{
	TASK_MAP::iterator it = m_mapActTask.begin();
	for(; it != m_mapActTask.end(); ++it){
		(*it).second->Update();
	}
	CheckTaskMessage();
	TimerForGames::Instance().GetTotalTime();	//�^�C�}�[�X�V
	return continue_flag;
}

void TaskSystem::SwitchMovingTasks(bool OnOrOff)
{
	SwitchTask(TASK_SNOWMAN, OnOrOff);
	SwitchTask(TASK_ZETSUBOU, OnOrOff);
	SwitchTask(TASK_SELFBULLET, OnOrOff);
	SwitchTask(TASK_FOEBULLET, OnOrOff);
	SwitchTask(TASK_ITEM, OnOrOff);
	SwitchTask(TASK_SCORE, OnOrOff);
	SwitchTask(TASK_REPLAY, OnOrOff);
}

void TaskSystem::CheckTaskMessage(void)
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
			ZetsubouTask *p_zetsu;
			FoeBulletTask *p_foe_bullet;
			p_zetsu = GetTask<ZetsubouTask>(TASK_ZETSUBOU);
			p_foe_bullet = GetTask<FoeBulletTask>(TASK_FOEBULLET);
			p_zetsu->Eliminate();
			p_foe_bullet->Eliminate();
		}

		ResetMessage(TASK_DRAW);
	}else if(GetTaskMessage(TASK_SNOWMAN) != NONE){		//SelfBullet�ύX��̏���
		SelfBulletTask *p_self_bullet;
		p_self_bullet = GetTask<SelfBulletTask>(TASK_SELFBULLET);
		p_self_bullet->ResetOpes(true);
		if(GetTaskMessage(TASK_SNOWMAN) == ADD_OPE_APPLYGRAVITY){
			Dix::sp<OperatorApplyGravity> sp_gravity(new OperatorApplyGravity);
			p_self_bullet->SetOperator(sp_gravity, true);
		}
	}
}

void TaskSystem::SetReplayMode(bool Flag)
{
	//ReplayTask *p_replay = GetTask<ReplayTask>(TASK_REPLAY);
	//p_replay->TurnReplayFlag(Flag);
}

bool DrawTask::Update(void)
{
	HAZAMA::draw_helper->SetDrawScreen(HAZAMA::DrawHelper::BACK);
	HAZAMA::draw_helper->InitializeScreen();

	remove_copy_if(l_sp_draw_obj.begin(), l_sp_draw_obj.end(), l_have_msgs_obj.begin(), mem_fn(&DrawObj::Draw));
		
	if(IsMessaged()){	//���b�Z�[�W�𔭂������I�u�W�F�N�g���������ꍇ
		EraseDummies();	//�_�~�[�̗v�f������

		OBJ_MSG msg = NOTHING;		//�I�u�W�F�N�g���烁�b�Z�[�W���󂯎��
		std::for_each(l_have_msgs_obj.begin(), l_have_msgs_obj.end(), boost::bind(&DrawObj::MessageGet, _1, boost::ref(msg)));
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
	l_sp_draw_obj.remove_if(boost::mem_fn(&DrawObj::IsNotDrawn));

	HAZAMA::draw_helper->FlipScreen();
	return true;
}

void DrawTask::InittheList(void)
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

bool SnowmanTask::Update(void)
{
	Dix::sp<MoveOperator> sp_move_ope;
	Dix::sp<Snowman> sp_snowman;

	if(!CheckUsageFlag()){
		return false;	// �g���Ȃ�
	}

	MyFactory<Snowman>::Instance().Create(0, sp_snowman);

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

void SnowmanTask::Shoot(void)
{
	void (SnowmanTask::*func_tbl[GRAVIFIED+1])(Dix::sp<Snowman>&) = {
		&SnowmanTask::CreateNormalBullet,
		&SnowmanTask::CreateTriBullets,
		&SnowmanTask::CreateGravifiedBullet
	};
	Dix::sp<Snowman> sp_snowman;
	MyFactory<Snowman>::Instance().Create(0, sp_snowman);

	int creating_bullet_num = static_cast<int>(sp_snowman->GetBulletType());
	(this->*func_tbl[creating_bullet_num])(sp_snowman);
}

void SnowmanTask::CreateNormalBullet(Dix::sp<Snowman> &SPSnowman)
{
	Dix::sp<SelfNormalBullet> sp_self_bullet;
	OBJID objid = MyFactory<SelfNormalBullet>::Instance().Size();

	 MyFactory<SelfNormalBullet>::Instance().Create(objid, sp_self_bullet);

	//�e�̏����ʒu��ݒ肷��
	HAZAMA::VECTOR2 initial_pos = SPSnowman->GetPosition();
	initial_pos.x += SPSnowman->GetRect().right;
	initial_pos.y += 32.0;
	sp_self_bullet->SetPosition(initial_pos);
	message = REMOVE_EXTRA_OPE;
}

void SnowmanTask::CreateTriBullets(Dix::sp<Snowman> &SPSnowman)
{
	Dix::sp<SelfNormalBullet> sp_self_bullet;
	OBJID objid =  MyFactory<SelfNormalBullet>::Instance().Size();

	for(int i = 0; i < 3; ++i){
		objid += i;
		MyFactory<SelfNormalBullet>::Instance().Create(objid, sp_self_bullet);

		//�e�̏����ʒu��ݒ肷��
		HAZAMA::VECTOR2 initial_pos = SPSnowman->GetPosition();
		initial_pos.x += SPSnowman->GetRect().right;
		initial_pos.y += 32.0f;
		sp_self_bullet->SetPosition(initial_pos);

		switch(i){				//���˂���鏇�Ԃɂ����Y�������̏����x��^����
			case 1:
				sp_self_bullet->SetVector(300.0, 1);
				break;
			case 2:
				sp_self_bullet->SetVector(-300.0, 1);
				break;
			default:
				break;
		}
	}
	message = REMOVE_EXTRA_OPE;
}

void SnowmanTask::CreateGravifiedBullet(Dix::sp<Snowman> &SPSnowman)
{
	Dix::sp<SelfGravifiedBullet> sp_self_bullet;
	OBJID objid =  MyFactory<SelfGravifiedBullet>::Instance().Size();

	if(!MyFactory<SelfGravifiedBullet>::Instance().HaveObj()){
		message = ADD_OPE_APPLYGRAVITY;
	}
	
	MyFactory<SelfGravifiedBullet>::Instance().Create(objid, sp_self_bullet);

	//�e�̏����ʒu��ݒ肷��
	HAZAMA::VECTOR2 initial_pos = SPSnowman->GetPosition();
	initial_pos.x += SPSnowman->GetRect().right;
	initial_pos.y += 32.0;
	sp_self_bullet->SetPosition(initial_pos);
}

bool ZetsubouTask::Update(void)
{
	Dix::sp<MoveOperator> sp_move_ope;
	Dix::sp<Zetsubou> sp_zetsu;

	if(!CheckUsageFlag()){
		return false;	// �g���Ȃ�
	}

	CreatNewZetsu();

	for(unsigned i = 0; i < MyFactory<Zetsubou>::Instance().Size(); ++i){ 
		
		MyFactory<Zetsubou>::Instance().Create(i, sp_zetsu);
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

		sp_zetsu->SetVector(HAZAMA::V2Get(-1.0 * sp_zetsu->GetSpeed(), 0));
	}

	ResetOpes();
	return true;
}

void ZetsubouTask::Shoot(Dix::sp<Zetsubou> SP)
{
	Dix::sp<FoeBullet> sp_foe_bullet;
	HAZAMA::RECT zetsu_rect = SP->GetRect() + SP->GetPosition();	//Zetsubou�I�u�W�F�N�g�̒��_�̐�΍��W���o��
	if(static_cast<int>(p_timer->GetTotalTime()) % 1000 >= 850 && !IsOthersNearby(zetsu_rect)){
		MyFactory<FoeBullet> *p_factory = &MyFactory<FoeBullet>::Instance();
		OBJID objid = p_factory->Size();
		p_factory->Create(objid, sp_foe_bullet);

		//�e�̏����ʒu��ݒ肷��
		HAZAMA::VECTOR2 initial_pos = SP->GetPosition();
		initial_pos.x -= 24.0;
		initial_pos.y += 16.0;
		sp_foe_bullet->SetPosition(initial_pos);
	}
}

bool ZetsubouTask::IsOthersNearby(const HAZAMA::RECT &Rect)
{
	bool return_bool = false;
	Dix::sp<FoeBullet> sp_existent;
	Dix::sp<Snowman> sp_snowman;
	HAZAMA::VECTOR2 bullet_pos;
	HAZAMA::RECT snowman_rect;

	MyFactory<Snowman>::Instance().Create(0, sp_snowman);
	snowman_rect = sp_snowman->GetRect();

	for(unsigned i = 0; i < MyFactory<FoeBullet>::Instance().Size(); ++i){
		MyFactory<FoeBullet>::Instance().Create(i, sp_existent);
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

void ZetsubouTask::CreatNewZetsu(void)
{
	Dix::sp<Zetsubou> sp_zetsu;
	if(!p_timer->Exists(timer_id)){		//�V�K�^�C�}�[�X�^�[�g
		timer_id = p_timer->StartTimer();
	}
	
	if(count_min == 0){
		count_min = HAZAMA::draw_helper->GetRand(1000);
	}else{
		if(p_timer->IsTakenUp(timer_id, count_min)){		//������x�̊Ԃ��������ēG���o��

			MyFactory<Zetsubou> *p_factory = &MyFactory<Zetsubou>::Instance();
			if(p_factory->Size() <= MAX_NUM_FOES){
				OBJID objid = p_factory->Size();
				p_factory->Create(objid, sp_zetsu);

				count_min = 0;
				timer_id = 0;
			}
		}
	}
}

bool SelfBulletTask::Update(void)
{
	Dix::sp<MoveOperator> sp_move_ope;
	Dix::sp<SelfBullet> sp_self_bullet;
	unsigned objid[2] = {0, 0};

	if(!CheckUsageFlag()){
		return false;	// �g���Ȃ�
	}

	//�S�Ă̎�ނ̒e���������I���܂Ń��[�v�𔲂��Ȃ�
	while(MyFactory<SelfNormalBullet>::Instance().Exists(sp_self_bullet, objid[0]++)
		|| MyFactory<SelfGravifiedBullet>::Instance().Exists(sp_self_bullet, objid[1]++)){ 
		
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

bool FoeBulletTask::Update(void)
{
	Dix::sp<MoveOperator> sp_move_ope;
	Dix::sp<FoeBullet> sp_foe_bullet;

	if(!CheckUsageFlag()){
		return false;	// �g���Ȃ�
	}

	for(size_t i = 0; i < MyFactory<FoeBullet>::Instance().Size(); ++i){ 
		
		MyFactory<FoeBullet>::Instance().Create(i, sp_foe_bullet);
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

bool ItemTask::Update(void)
{
	Dix::sp<MoveOperator> sp_move_ope;
	Dix::sp<Items> sp_item;
	Dix::sp<Snowman> sp_snowman;

	if(!CheckUsageFlag()){
		return false;	// �g���Ȃ�
	}

	if(!MyFactory<LifeExtend>::Instance().Exists(sp_item, 0)
		&& !MyFactory<Barrier>::Instance().Exists(sp_item, 0)
		&& !MyFactory<Quick>::Instance().Exists(sp_item, 0)
		&& !MyFactory<Multiplier>::Instance().Exists(sp_item, 0)
		&& !MyFactory<Eliminator>::Instance().Exists(sp_item, 0)
		&& !MyFactory<TriBullets>::Instance().Exists(sp_item, 0)
		&& !MyFactory<GravifiedBullet>::Instance().Exists(sp_item, 0)
		&& !MyFactory<EliminatorX>::Instance().Exists(sp_item, 0)){
			CreateNewItem();		//�A�C�e����1�����݂��ĂȂ�������A�V������������
	}

	MyFactory<Snowman>::Instance().Create(0, sp_snowman);
	
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
			Dix::sp<ExploAnim> sp_explo;
			MyFactory<ExploAnim>::Instance().Create(0, sp_explo);
			HAZAMA::RECT anim_rect = sp_explo->GetRect();
			HAZAMA::VECTOR2 anim_pos = sp_elimx->GetPosition() - HAZAMA::V2Get(anim_rect.right / 2, anim_rect.down / 2);
			sp_explo->SetPosition(anim_pos);		//�����A�j���̍Đ��ʒu���A�C�e���̌��݈ʒu�ɐݒ�
			sp_elimx->Clear();
		}
	}

	ResetOpes();
	return true;
}

void ItemTask::CreateNewItem(void)
{
	bool (ItemTask::*func_tbl[NUM_ITEM_TYPES]) (void) = {				//�֐��|�C���^�e�[�u���̍쐬
		&ItemTask::CreateLifeEx,
		&ItemTask::CreateBarrier,
		&ItemTask::CreateQuick,
		&ItemTask::CreateMultiplier,
		&ItemTask::CreateEliminator,
		&ItemTask::CreateTriBullets,
		&ItemTask::CreateGravifiedBullet,
		&ItemTask::CreateEliminatorX
	};

	int type;

	if(!p_timer->Exists(timer_id)){		//�V�K�^�C�}�[�X�^�[�g
		timer_id = p_timer->StartTimer();
	}

	if(p_timer->IsTakenUp(timer_id, next_count)){
		next_count = HAZAMA::draw_helper->GetRand(10000) + 20000;
		cur_score = Score::Instance().GetCurScore();

#ifndef _DEBUG
		if(cur_score < 5000){		//���݂̃X�R�A��5000�_�ȉ��Ȃ�A�ȉ��̏����������Ɋ֐��𔲂���
			return;
		}
#endif

		do{
			type = HAZAMA::draw_helper->GetRand(NUM_ITEM_TYPES - 1);			/*���炩�̃A�C�e�������������܂�
											���������ނ�ς��Ă���*/
		}while(!(this->*func_tbl[type])() );
		timer_id = 0;
	}
}

bool TouchTask::Update(void)
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

void TouchTask::SelfBulletAndFoe(void)
{
	Dix::sp<Zetsubou> sp_zetsu;
	Dix::sp<SelfBullet> sp_self_bullet;

	for(unsigned i = 0; i < MyFactory<Zetsubou>::Instance().Size(); ++i){
		
		MyFactory<Zetsubou>::Instance().Create(i, sp_zetsu);
		if(sp_zetsu->IsNotDrawn()){
			continue;		//��ʂɕ`�悳��Ȃ�Zetsubou�̓X�L�b�v����
		}

		unsigned objid[2] = {0, 0};

		while(MyFactory<SelfNormalBullet>::Instance().Exists(sp_self_bullet, objid[0]++)
			|| MyFactory<SelfGravifiedBullet>::Instance().Exists(sp_self_bullet, objid[1]++)){

			if(sp_zetsu->Dispatch(sp_self_bullet.GetPtr())){
				sp_self_bullet->Dispatch(sp_zetsu.GetPtr());
				break;		//����Zetsubou��
			}
		}
	}
}

void TouchTask::SelfAndFoe(void)
{
	Dix::sp<Zetsubou> sp_zetsu;

	MyFactory<Snowman>::Instance().Create(0, sp_snowman);
	for(unsigned i = 0; i < MyFactory<Zetsubou>::Instance().Size(); ++i){

		MyFactory<Zetsubou>::Instance().Create(i, sp_zetsu);		//Zetsubou��Snowman�̓����蔻��
		if(sp_zetsu->IsNotDrawn()){
			continue;		//��ʂɕ`�悳��Ȃ�Zetsubou�̓X�L�b�v����
		}

		if(sp_zetsu->Dispatch(sp_snowman.GetPtr()) && sp_snowman->Dispatch(sp_zetsu.GetPtr())){
			Dix::sp<SelfGotHitAnim> sp_self_got_hit;
			MyFactory<SelfGotHitAnim>::Instance().Create(0, sp_self_got_hit);
			sp_self_got_hit->SetPosition(sp_snowman->GetPosition());
			sp_snowman->Initialize();
			message = DONT_MOVE;
			break;
		}
	}
}

void TouchTask::SelfAndFoeBullet(void)
{
	MyFactory<Snowman>::Instance().Create(0, sp_snowman);
	for(unsigned i = 0; i < MyFactory<FoeBullet>::Instance().Size(); ++i){

		MyFactory<FoeBullet>::Instance().Create(i, sp_foe_bullet);		//FoeBullet��Snowman�̓����蔻��
		if(sp_foe_bullet->IsNotDrawn()){
			continue;		//��ʂɕ`�悳��Ȃ�FoeBullet�̓X�L�b�v����
		}

		if(sp_foe_bullet->Dispatch(sp_snowman.GetPtr()) && sp_snowman->Dispatch(sp_foe_bullet.GetPtr())){
			Dix::sp<SelfGotHitAnim> sp_self_got_hit;
			MyFactory<SelfGotHitAnim>::Instance().Create(0, sp_self_got_hit);
			sp_self_got_hit->SetPosition(sp_snowman->GetPosition());
			sp_snowman->Initialize();
			message = DONT_MOVE;
			break;
		}
	}
}

void TouchTask::SelfAndItem(void)
{
	MyFactory<Snowman>::Instance().Create(0, sp_snowman);

	if(!MyFactory<LifeExtend>::Instance().HaveObj()
		&& !MyFactory<Barrier>::Instance().HaveObj()
		&& !MyFactory<Quick>::Instance().HaveObj()
		&& !MyFactory<Multiplier>::Instance().HaveObj()
		&& !MyFactory<Eliminator>::Instance().HaveObj()
		&& !MyFactory<TriBullets>::Instance().HaveObj()
		&& !MyFactory<GravifiedBullet>::Instance().HaveObj()
		&& !MyFactory<EliminatorX>::Instance().HaveObj()){
			return;			//�A�C�e�������݂��ĂȂ��Ȃ�A�����蔻������Ȃ�
	}

	if( sp_snowman->Dispatch(sp_item.GetPtr()) && sp_item->Dispatch(sp_snowman.GetPtr())){

		GotItemSE::Play();		//�A�C�e���l���̌��ʉ���炷
		if(typeid(*sp_item) == typeid(Eliminator) || typeid(*sp_item) == typeid(EliminatorX)){
			Dix::sp<FireAnim> sp_fire_anim;
			MyFactory<FireAnim>::Instance().Create(0, sp_fire_anim);
			if(typeid(*sp_item) == typeid(EliminatorX)){
				sp_fire_anim->SetSEType(true);
			}else{
				sp_fire_anim->SetSEType(false);
			}
			message = DONT_MOVE;
		}
	}
}

void TouchTask::Others(void)
{
	Dix::sp<Zetsubou> sp_zetsu;
	Dix::sp<ExploAnim> sp_explo;
	if(!MyFactory<ExploAnim>::Instance().HaveObj()){	//�����̃A�j���[�V���������݂��Ȃ��Ȃ�A�������s��Ȃ�
		return;
	}

	MyFactory<ExploAnim>::Instance().Create(0, sp_explo);
	MyFactory<Snowman>::Instance().Create(0, sp_snowman);

	if(sp_explo->Dispatch(sp_snowman.GetPtr())){
		sp_snowman->Dispatch(sp_explo.GetPtr());
		Dix::sp<SelfGotHitAnim> sp_self_got_hit;
		MyFactory<SelfGotHitAnim>::Instance().Create(0, sp_self_got_hit);
		sp_self_got_hit->SetPosition(sp_snowman->GetPosition());
		sp_snowman->Initialize();
		message = DONT_MOVE;
	}

	for(unsigned i = 0; i < MyFactory<Zetsubou>::Instance().Size(); ++i){

		MyFactory<Zetsubou>::Instance().Create(i, sp_zetsu);
		if(sp_zetsu->IsNotDrawn()){
			continue;		//��ʂɕ`�悳��Ȃ�Zetsubou�̓X�L�b�v����
		}

		if(sp_explo->Dispatch(sp_zetsu.GetPtr())){
			sp_zetsu->Dispatch(sp_explo.GetPtr());
		}
	}
}

bool ScoreTask::Update(void)
{
	if(!CheckUsageFlag()){
		return false;	// �g���Ȃ�
	}

	Score::Instance().AddScore();
	return true;
}

/*ReplayTask::ReplayTask(InputDeviceKeyBoard *p_key) : replay_flag(false), o_file(), i_file(), sp_iarchive()
, sp_oarchive(), p_keyboard(p_key)
{}

bool ReplayTask::Update(void)
{
	if(!CheckUsageFlag()){
		return false;	// �g���Ȃ�
	}

	if(!replay_flag){		//���v���C�f�[�^���t�@�C���ɏo��
		*sp_oarchive << (const TimerForGames&)TimerForGames::Instance();
		*sp_oarchive << (const InputDeviceKeyBoard&)p_keyboard;
	}else{					//���v���C�f�[�^���t�@�C���������
		*sp_iarchive >> TimerForGames::Instance();
		*sp_iarchive >> p_keyboard;
	}
	return true;
}

void ReplayTask::SetRecordingFile(const char src[])
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
}*/