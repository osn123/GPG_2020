//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ev_FadeInOut.h"

namespace  Ev_FadeInOut
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		//this->img[0] = DG::Image::Create("./data/image/MessageF.png");
		//this->img[1] = DG::Image::Create("./data/image/MessageF.png");
		//this->img[2] = DG::Image::Create("./data/image/MessageF.png");
		//this->img[3] = DG::Image::Create("./data/image/MessageF.png");
		//this->img[4] = DG::Image::Create("./data/image/MessageF.png");
		//this->img[5] = DG::Image::Create("./data/image/MessageF.png");
		//this->img[6] = DG::Image::Create("./data/image/MessageF.png");
		//this->img[7] = DG::Image::Create("./data/image/MessageF.png");
		//this->img[8] = DG::Image::Create("./data/image/MessageF.png");
		//this->img[9] = DG::Image::Create("./data/image/MessageF.png");
		//
		//this->font[0] = DG::Font::Create("HG丸ｺﾞｼｯｸM-PRO", 8, 16);
		//this->font[1] = DG::Font::Create("HG丸ｺﾞｼｯｸM-PRO", 12, 24);
		//this->font[2] = DG::Font::Create("HG丸ｺﾞｼｯｸM-PRO", 16, 32);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		//for (int i = 0; i < _countof(this->img); i++) {
		//	this->img[i].reset();
		//}
		//for (int i = 0; i < _countof(this->font); i++) {
		//	this->font[i].reset();
		//}
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		this->render2D_Priority[1] = 0.005f;
		this->cnt = 0;
		this->Stop();//
		
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		this->img.reset();
		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		if (this->mode== Mode::In)
		{
			this->cnt--;//
			if (this->cnt < 0) {
				//
				ge->StopAll_GN("イベント", "実行エンジン", false);
				//
				this->Kill();
			}
		}
		if (this->mode == Mode::Out)
		{
			this->cnt++;//
			if (this->cnt > 60) {
				//
				ge->StopAll_GN("イベント", "実行エンジン", false);
				//
				this->Stop();
			}
		}

		//auto inp = ge->in1->GetState();
		////
		//if ((this->timeLimit!=0 && this->timeCnt>=this->timeLimit) || inp.B2.down)
		//{
		//	//
		//	this->Stop();
		//	//
		//	ge->StopAll_GN("イベント", "実行エンジン", false);
		//}
		//else {
		//	this->timeCnt++;
		//}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{

		ML::Box2D draw(0, 0, ge->screenWidth, ge->screenHeight);
		this->img->Draw(draw, this->src,ML::Color(this->cnt/60.0f,1,1,1));

	}

	void Object::CreateOrFadeIn(stringstream& ss_)
	{
		//
		auto p = ge->GetTask<Object>(defGroupName, defName);

		//
		if (nullptr == p) {
			p = Object::Create(true);
			p->Set(ss_);
		}
		//
		else {
			p->Set(ss_);
		}
	}

	void Object::Set(stringstream& ss_)
	{
		//
		string filePath;
		ss_ >> filePath;
		//
		if (filePath=="in")
		{
			this->mode = Mode::In;
			this->cnt = 60;
		}
		//
		else
		{
			this->mode = Mode::Out;
			this->cnt = 0;
			//
			this->img = DG::Image::Create(filePath);
			POINT s = this->img->Size();
			this->src = ML::Box2D(0, 0, s.x, s.y);
		}


		//
		ge->StopAll_GN("イベント", "実行エンジン");
		//
		this->Stop(false);	

	}




	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//リソースクラスの生成
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
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
}