//-----------------------------------------------------------------------------
//�L�����N�^�ėp�X�[�p�[�N���X
//-----------------------------------------------------------------------------
#include "BChara.h"
#include  "MyPG.h"
#include  "Task_Map2D.h"



//-----------------------------------------------------------------------------
//���[�V�������X�V�i�ύX�Ȃ��̏ꍇ	false)
bool  BChara::UpdateMotion(Motion  nm_)
{
	if (nm_ == this->motion) {
		return  false;
	}
	else {
		this->preMotion = this->motion;
		this->preMoveCnt = this->moveCnt;

		this->motion = nm_;		//���[�V�����ύX
		this->moveCnt = 0;		//�s���J�E���^�N���A
		this->animCnt = 0;		//�A�j���[�V�����J�E���^�̃N���A
		return  true;
	}
}

//-----------------------------------------------------------------------------
//����ڐG����
bool  BChara::CheckHead()
{
	//�����蔻�����ɂ��ē����`�𐶐�
	ML::Box2D  head(this->hitBase.x,
		this->hitBase.y - 1,
		this->hitBase.w,
		1);
	head.Offset((int)this->pos.x, (int)this->pos.y);


	auto   map = ge->GetTask<Map2D::Object>(Map2D::defGroupName, Map2D::defName);
	if (nullptr == map) { return false; }//�}�b�v��������Δ��肵�Ȃ�(�o���Ȃ��j
	return map->CheckHit(head);
}
//-----------------------------------------------------------------------------
//�߂荞�܂Ȃ��ړ�����
void BChara::CheckMove(ML::Vec2& e_)
{
	//�}�b�v�����݂��邩���ׂĂ���A�N�Z�X
	auto   map = ge->GetTask<Map2D::Object>(Map2D::defGroupName, Map2D::defName);
	if (nullptr == map) { return; }//�}�b�v��������Δ��肵�Ȃ�(�o���Ȃ��j

	//�����ɑ΂���ړ�
	while (e_.x != 0) {
		float  preX = this->pos.x;
		if (e_.x >= 1) { this->pos.x += 1;		e_.x -= 1; }
		else if (e_.x <= -1) { this->pos.x -= 1;		e_.x += 1; }
		else { this->pos.x += e_.x;		e_.x = 0; }
		ML::Box2D  hit = this->hitBase.OffsetCopy(this->pos);
		if (true == map->CheckHit(hit)) {
			this->pos.x = preX;		//�ړ����L�����Z��
			break;
		}
	}
	//�c���ɑ΂���ړ�
	while (e_.y != 0) {
		float  preY = this->pos.y;
		if (e_.y >= 1) { this->pos.y += 1;		e_.y -= 1; }
		else if (e_.y <= -1) { this->pos.y -= 1;		e_.y += 1; }
		else { this->pos.y += e_.y;		e_.y = 0; }
		ML::Box2D  hit = this->hitBase.OffsetCopy(this->pos);
		if (true == map->CheckHit(hit)) {
			this->pos.y = preY;		//�ړ����L�����Z��
			break;
		}
	}
}
//-----------------------------------------------------------------------------
//�����ڐG����
bool  BChara::CheckFoot()
{
	//�����蔻�����ɂ��đ�����`�𐶐�
	ML::Box2D  foot(this->hitBase.x,
		this->hitBase.y + this->hitBase.h,
		this->hitBase.w,
		1);
	foot.Offset(this->pos);

	auto   map = ge->GetTask<Map2D::Object>(Map2D::defGroupName, Map2D::defName);
	if (nullptr == map) { return  false; }//�}�b�v��������Δ��肵�Ȃ�(�o���Ȃ��j
	//�}�b�v�ƐڐG����
	return map->CheckHit(foot);
}

//���ʐڐG����i�T�C�h�r���[�Q�[����p�j 
bool  BChara::CheckFront_LR()
{
	//�����蔻�����ɂ��ċ�`�𐶐�(�Ƃ肠�����A���������P�ɂȂ�����`��p�ӂ���) 
	ML::Box2D  front(
		this->hitBase.x,
		hitBase.y,
		1,
		this->hitBase.h);
	//�L�����N�^�̌����ɂ���`�̈ʒu�𒲐� 
	if (this->angle_LR == Angle_LR::Left) {
		front.Offset(-1, 0); //�����Ɉړ� 
	}
	else {
		front.Offset(this->hitBase.w, 0); //�E���Ɉړ� 
	}
	//���݈ʒu�ɍ��킹�� 
	front.Offset((int)this->pos.x, (int)this->pos.y);
	auto   map = ge->GetTask<Map2D::Object>(Map2D::defGroupName, Map2D::defName);
	if (nullptr == map) { return false; }//�}�b�v��������Δ��肵�Ȃ�(�o���Ȃ��j 
	return map->CheckHit(front);
}

void BChara::Received(BChara* from_, AttackInfo at_) {
	ML::MsgBox("Received ��������Ă��܂���");
}

bool BChara::CheckHit(const ML::Box2D& hit_) {
	ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
	return me.Hit(hit_);
 }

bool BChara::Attack_Std(const string& targetGroup,  AttackInfo& at) {
	ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
	auto targets = ge->GetTasks<BChara>(targetGroup);
	for (auto it = targets->begin(); it != targets->end(); ++it) {
		if ((*it)->CheckHit(me)) {
			(*it)->Received(this, at);
			return true; // 
		}
	}
	return false; // 
}


