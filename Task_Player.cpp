//-------------------------------------------------------------------
// �v���C��
//-------------------------------------------------------------------
#include "Task_Player.h"

#include "MyPG.h"
#include "Task_Effect00.h"
#include "Task_Map2D.h"
#include "Task_Shot00.h"
#include "Task_Shot01.h"

namespace Player {
Resource::WP Resource::instance;
//-------------------------------------------------------------------
// ���\�[�X�̏�����
bool Resource::Initialize() {
  this->img = DG::Image::Create("./data/image/chara02.png");
  return true;
}
//-------------------------------------------------------------------
// ���\�[�X�̉��
bool Resource::Finalize() {
  this->img.reset();
  return true;
}
//-------------------------------------------------------------------
// �u�������v�^�X�N�������ɂP�񂾂��s������
bool Object::Initialize() {
  // �X�[�p�[�N���X������
  __super::Initialize(defGroupName, defName, true);
  // ���\�[�X�N���X����or���\�[�X���L
  this->res = Resource::Create();

  // ���f�[�^������
  this->render2D_Priority[1] = 0.5f;
  this->hitBase = ML::Box2D(-16, -40, 32, 80);
  this->angle_LR = Angle_LR::Right;
  this->controller = ge->in1;

  this->motion = Motion::Stand;  // �L�����������
  this->maxSpeed = 5.0f;         // �ő�ړ����x�i���j
  this->addSpeed = 1.0f;  // ���s�����x�i�n�ʂ̉e���ł�����x�ł��������
  this->decSpeed = 0.5f;       // hack �ڒn��Ԃ̎��̑��x�����ʁi���C
  this->maxFallSpeed = 10.0f;  // �ő嗎�����x
  this->jumpPow = -10.0f;      // �W�����v�́i�����j
  this->gravity = ML::Gravity(32) * 5;  // �d�͉����x�����ԑ��x�ɂ����Z��

  // ���^�X�N�̐���

  return true;
}
//-------------------------------------------------------------------
// �u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
bool Object::Finalize() {
  // ���f�[�^���^�X�N���

  if (!ge->QuitFlag() && this->nextTaskCreate) {
    // �������p���^�X�N�̐���
  }

  return true;
}
//-------------------------------------------------------------------
// �u�X�V�v�P�t���[�����ɍs������
void Object::UpDate() {
  this->moveCnt++;
  this->animCnt++;
  // �v�l�E�󋵔��f
  this->Think();
  // �����[�V�����ɑΉ���������
  this->Move();
  // �߂荞�܂Ȃ��ړ�
  ML::Vec2 est = this->moveVec;
  this->CheckMove(est);
}
//-------------------------------------------------------------------
// �u�Q�c�`��v�P�t���[�����ɍs������
void Object::Render2D_AF() {
  BChara::DrawInfo di = this->Anim();
  di.draw.Offset(this->pos);
  // �X�N���[���Ή�
  di.draw.Offset(-ge->camera2D.x, -ge->camera2D.y);

  this->res->img->Draw(di.draw, di.src);
}
//-----------------------------------------------------------------------------
// �v�l���󋵔��f�@���[�V��������
void Object::Think() {
  auto inp = this->controller->GetState();
  BChara::Motion nm = this->motion;  // �Ƃ肠�������̏�Ԃ��w��

  // �v�l�i���́j��󋵂ɉ����ă��[�V������ύX���鎖��ړI�Ƃ��Ă���B
  // ���[�V�����̕ύX�ȊO�̏����͍s��Ȃ�
  switch (nm) {
    case Motion::Stand:  // �����Ă���
      if (inp.LStick.BL.on) nm = Motion::Walk;
      if (inp.LStick.BR.on) nm = Motion::Walk;
      if (inp.B1.down) nm = Motion::TakeOff;
      if (this->CheckFoot() == false) nm = Motion::Fall;  // ���� ��Q�@����
      break;

    case Motion::Walk:  // �����Ă���
      if (inp.LStick.BL.off && inp.LStick.BR.off) nm = Motion::Stand;
      if (inp.B1.down) nm = Motion::TakeOff;
      if (this->CheckFoot() == false) nm = Motion::Fall;  // ���� ��Q�@����
      break;

    case Motion::Jump:           // �㏸��
      if (this->moveVec.y >= 0)  
      {
        nm = Motion::Fall;
      }
      break;

    case Motion::Fall:  // ������
      if (this->CheckFoot() == true) {
        nm = Motion::Landing;
      }
      break;

    case Motion::Attack:  // �U����
      break;
    case Motion::TakeOff:  // ��ї���
        if (inp.B1.up) nm = Motion::Jump;
        if (this->moveCnt >= 30) nm = Motion::Jump;
      if (this->CheckFoot() == false) nm = Motion::Fall;  // ���� ��Q�@����
      break;
    case Motion::Landing:  // ���n
      if (this->moveCnt >= 3) nm = Motion::Stand;
      if (this->CheckFoot() == false) nm = Motion::Fall;  // ���� ��Q�@����
      break;
  }
  // ���[�V�����X�V
  this->UpdateMotion(nm);
}
//-----------------------------------------------------------------------------
// ���[�V�����ɑΉ���������
//(���[�V�����͕ύX���Ȃ��j
void Object::Move() {
  // �d�͉���
  switch (this->motion) {
    default:
      // �㏸���������͑����ɒn�ʂ�����
      if (this->moveVec.y < 0 || this->CheckFoot() == false) {
        // this->moveVec.y = 1.0f;//������
        this->moveVec.y =
            min(this->moveVec.y + this->gravity, this->maxFallSpeed);
      }
      // �n�ʂɐڐG���Ă���
      else {
        this->moveVec.y = 0.0f;
      }
      break;
      // �d�͉����𖳌�������K�v�����郂�[�V�����͉���case�������i���ݑΏۖ����j
    case Motion::Unnon:
      break;
  }

  // �ړ����x����
  switch (this->motion) {
    default:
      if (this->moveVec.x < 0) {
        this->moveVec.x = min(this->moveVec.x + this->decSpeed, 0);
      } else {
        this->moveVec.x = max(this->moveVec.x - this->decSpeed, 0);
      }

      break;
      // �ړ����x�����𖳌�������K�v�����郂�[�V�����͉���case�������i���ݑΏۖ����j
    case Motion::Jump:
      if (this->moveCnt == 0) {
        this->moveVec.y = this->jumpPow+(-preMoveCnt/10);
      }
      break;
  }
  //-----------------------------------------------------------------
  // ���[�V�������ɌŗL�̏���
  auto inp = this->controller->GetState();  // TODO:���͏�Ԃ��擾
  switch (this->motion) {
    case Motion::Stand:  // �����Ă���
      break;
    case Motion::Walk:  // �����Ă���
      if (inp.LStick.BL.on) {
        this->angle_LR = Angle_LR::Left;
        this->moveVec.x = -this->maxSpeed;
      } else if (inp.LStick.BR.on) {
        this->angle_LR = Angle_LR::Right;
        this->moveVec.x = this->maxSpeed;
      }
      break;
    case Motion::Fall:  // ������
      if (inp.LStick.BL.on) {
        this->moveVec.x = -this->maxSpeed;
      } else if (inp.LStick.BR.on) {
        this->moveVec.x = this->maxSpeed;
      }
      break;
    case Motion::Jump:  // �㏸��
      if (this->CheckHead()) moveVec.y = 0;

      if (inp.LStick.BL.on) {
        this->moveVec.x = -this->maxSpeed;
      } else if (inp.LStick.BR.on) {
        this->moveVec.x = this->maxSpeed;
      }
      break;
    case Motion::Attack:  // �U����
      break;
  }
}
//-----------------------------------------------------------------------------
// �A�j���[�V��������
BChara::DrawInfo Object::Anim() {
  ML::Color defColor(1, 1, 1, 1);
  BChara::DrawInfo imageTable[] = {
      // draw							src
      {ML::Box2D(-16, -40, 32, 80), ML::Box2D(0, 0, 32, 80), defColor},  // ��~
      {ML::Box2D(-4, -40, 32, 80), ML::Box2D(32, 0, 32, 80),
       defColor},  // ���s�P
      {ML::Box2D(-20, -40, 48, 80), ML::Box2D(64, 0, 48, 80),
       defColor},  // ���s�Q
      {ML::Box2D(-20, -40, 48, 80), ML::Box2D(112, 0, 48, 80),
       defColor},  // ���s�R
      {ML::Box2D(-24, -40, 48, 80), ML::Box2D(48, 80, 48, 80),
       defColor},  // �W�����v
      {ML::Box2D(-24, -40, 48, 80), ML::Box2D(96, 80, 48, 80),
       defColor},  // ����
      {ML::Box2D(-24, -24, 48, 64), ML::Box2D(0, 80, 48, 64),
       defColor},  // ��ї����O
      {ML::Box2D(-24, -24, 48, 64), ML::Box2D(144, 80, 48, 64),
       defColor},  // ���n
  };
  BChara::DrawInfo rtv;
  int work;
  switch (this->motion) {
    default:
      rtv = imageTable[0];
      break;
      //	�W�����v------------------------------------------------------------------------
    case Motion::Jump:
      rtv = imageTable[4];
      break;
      //	��~----------------------------------------------------------------------------
    case Motion::Stand:
      rtv = imageTable[0];
      break;
      //	���s----------------------------------------------------------------------------
    case Motion::Walk:
      work = this->animCnt / 8;
      work %= 3;
      rtv = imageTable[work + 1];
      break;
      //	����----------------------------------------------------------------------------
    case Motion::Fall:
      rtv = imageTable[5];
      break;
      //	��ї����O--------------------------------------------------------------------
    case Motion::TakeOff:
      rtv = imageTable[6];
      break;
  }
  //	�����ɉ����ĉ摜�����E���]����
  if (Angle_LR::Left == this->angle_LR) {
    rtv.draw.x = -rtv.draw.x;
    rtv.draw.w = -rtv.draw.w;
  }
  return rtv;
}
// ������������������������������������������������������������������������������������
// �ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
// ������������������������������������������������������������������������������������
//-------------------------------------------------------------------
// �^�X�N��������
Object::SP Object::Create(bool flagGameEnginePushBack_) {
  Object::SP ob = Object::SP(new Object());
  if (ob) {
    ob->me = ob;
    if (flagGameEnginePushBack_) {
      ge->PushBack(ob);  // �Q�[���G���W���ɓo�^
      // �i���\�b�h�����ςȂ̂͋��o�[�W�����̃R�s�[�ɂ��o�O��������邽��
    }
    if (!ob->B_Initialize()) {
      ob->Kill();  // �C�j�V�����C�Y�Ɏ��s������Kill
    }
    return ob;
  }
  return nullptr;
}
//-------------------------------------------------------------------
bool Object::B_Initialize() { return this->Initialize(); }
//-------------------------------------------------------------------
Object::~Object() { this->B_Finalize(); }
bool Object::B_Finalize() {
  auto rtv = this->Finalize();
  return rtv;
}
//-------------------------------------------------------------------
Object::Object() {}
//-------------------------------------------------------------------
// ���\�[�X�N���X�̐���
Resource::SP Resource::Create() {
  if (auto sp = instance.lock()) {
    return sp;
  } else {
    sp = Resource::SP(new Resource());
    if (sp) {
      sp->Initialize();
      instance = sp;
    }
    return sp;
  }
}
//-------------------------------------------------------------------
Resource::Resource() {}
//-------------------------------------------------------------------
Resource::~Resource() { this->Finalize(); }
}  // namespace Player