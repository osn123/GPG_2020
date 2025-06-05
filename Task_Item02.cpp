//-------------------------------------------------------------------
// アイテム０2（ポーション）
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
	// リソースの初期化
	bool Resource::Initialize() {
		this->img = DG::Image::Create("./data/image/Item00.png");
		return true;
	}
	//-------------------------------------------------------------------
	// リソースの解放
	bool Resource::Finalize() {
		this->img.reset();
		return true;
	}
	//-------------------------------------------------------------------
	// 「初期化」タスク生成時に１回だけ行う処理
	bool Object::Initialize() {
		// スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		// リソースクラス生成orリソース共有
		this->res = Resource::Create();

		// ★データ初期化
		this->render2D_Priority[1] = 0.7f;
		this->hitBase = ML::Box2D(-16, -16, 32, 32);
		// ★タスクの生成
		return true;
	}
	//-------------------------------------------------------------------
	// 「終了」タスク消滅時に１回だけ行う処理
	bool Object::Finalize() {
		// ★データ＆タスク解放

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			// ★引き継ぎタスクの生成
		}

		return true;
	}
	//-------------------------------------------------------------------
	// 「更新」１フレーム毎に行う処理
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
	// 「２Ｄ描画」１フレーム毎に行う処理
	void Object::Render2D_AF() {
		BChara::DrawInfo di = this->Anim();
		di.draw.Offset(this->pos);
		// スクロール対応
		di.draw.Offset(-ge->camera2D.x, -ge->camera2D.y);

		this->res->img->Draw(di.draw, di.src,di.color);

	}
	//-----------------------------------------------------------------------------
	// アニメーション制御
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

	// ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
// 以下は基本的に変更不要なメソッド
// ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
//-------------------------------------------------------------------
// タスク生成窓口
	Object::SP Object::Create(bool flagGameEnginePushBack_) {
		Object::SP ob = Object::SP(new Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);  // ゲームエンジンに登録
				// （メソッド名が変なのは旧バージョンのコピーによるバグを回避するため
			}
			if (!ob->B_Initialize()) {
				ob->Kill();  // イニシャライズに失敗したらKill
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
	// リソースクラスの生成
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
