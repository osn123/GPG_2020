//-------------------------------------------------------------------
// �A�C�e���O2�i�|�[�V�����j
//-------------------------------------------------------------------
#include "Task_Item02.h"

#include "MyPG.h"
#include "Task_Effect00.h"
#include "Task_Map2D.h"
#include "Task_Shot00.h"
#include "Task_Shot01.h"

namespace Item02 {
	Resource::WP Resource::instance;
	//-------------------------------------------------------------------
	// ���\�[�X�̏�����
	bool Resource::Initialize() {
		this->img = DG::Image::Create("./data/image/Item00.png");
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
		this->render2D_Priority[1] = 0.7f;
		this->hitBase = ML::Box2D(-16, -16, 32, 32);
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

		if (this->unHitTime > 0)
		{
			this->unHitTime--;
		}
		Motion mn = this->motion;
		switch (this->motion)
		{
		case Motion::Stand:
			break;
		case Motion::Lose:
			this->pos.y -= 3;
			alpha = 0.3f;
			if (this->moveCnt > 20)
			{
				this->Kill();//
			}
			break;
		default:
			break;
		}
	}
	//-------------------------------------------------------------------
	// �u�Q�c�`��v�P�t���[�����ɍs������
	void Object::Render2D_AF() {
		BChara::DrawInfo di = this->Anim();
		di.draw.Offset(this->pos);
		// �X�N���[���Ή�
		di.draw.Offset(-ge->camera2D.x, -ge->camera2D.y);

		this->res->img->Draw(di.draw, di.src,di.color);

	}
	//-----------------------------------------------------------------------------
	// �A�j���[�V��������
	BChara::DrawInfo  Object::Anim()
	{
		BChara::DrawInfo imageTable[] = {
		 {ML::Box2D(-16, -16, 32, 32),ML::Box2D(0, 0, 32, 32),ML::Color(alpha,1,1,0)},// 
		};
		return imageTable[0];
	}

	void Object::Received(BChara* from_, AttackInfo at_) {
		if (this->motion!=Motion::Stand)
		{
			return;
		}
		this->UpdateMotion(Motion::Lose);
		from_->hp -= 5;
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
		}
		else {
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
