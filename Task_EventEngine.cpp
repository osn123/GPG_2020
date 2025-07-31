//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_EventEngine.h"
#include  "Task_Ev_Message.h"
#include  "Task_Ev_Image.h"
#include  "Task_Ev_FadeInOut.h"

namespace  EventEngine
{
	Object::WP Object::instance; //
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
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
		
		//★タスクの生成

		//
		//

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//
		//

		//★データ＆タスク解放
		if (this->evFile.is_open())
		{
			this->evFile.close();//
		}

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		if (!this->evFile)
		{
			this->Kill();
		}
		//
		string lineText;
		string headerText;
		string dataText;
		while (this->ReadLine(lineText))
		{
			//
			string::size_type t = lineText.find(">");
			headerText = lineText.substr(0, t);
			dataText = lineText.substr(t + 1);
			//
			bool rtv = this->Execute(headerText, dataText);
			//
			if (false==rtv || BTask::State::Active!=this->CheckState())
			{
				break;
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
	}

	//
	//
	Object::SP Object::Create_Mutex() {
		//
		if (auto p = instance.lock())
		{
			return nullptr;//
		}
		else
		{
			p = Object::Create(true);
			instance = p;
			return p;
		}
	}

	bool Object::Set(const string& fPath_) {
		//
		if (this->evFile.is_open()) {
			this->evFile.close();
		}
		//
		this->evFile.open(fPath_);
		//
		if (!this->evFile)
		{
			return false;
		}
		return true;
	}

	bool Object::ReadLine(string& lineT_) {
		//
		bool rtv = false;
		while (getline(this->evFile,lineT_))
		{
			//
			//
			if (string::npos == lineT_.find_first_not_of(" 　")) {
				continue;
			}
			//
			if ('/'==lineT_.at(0))
			{
				continue;
			}
			//
			if (string::npos==lineT_.find(">"))
			{
				continue;
			}
			//
			rtv=true;
				break;
		}
		return rtv;
	}

	bool Object::Execute(string& hs_, string& ds_) {
		//
		string::size_type t;
		while ((t = ds_.find_first_of("(,);")) != string::npos) {
			ds_[t] = ' ';
		}
		//
		stringstream ss;
		ss << ds_;
		//
		if (hs_ == "end") {
			this->Kill();
		}
		//
		else if (hs_ =="msg")
		{
			Ev_Message::Object::CreateOrReset(ss);
		}
		else if (hs_ == "evimg"){ Ev_Image::Object::CreateOrReset(ss); }
		else if (hs_ == "if") { this->If(ss); }
		else if (hs_ == "flag") { this->EventFlag(ss); }
		else if (hs_ == "fade_io") { Ev_FadeInOut::Object::CreateOrFadeIn(ss); }
		else if (hs_ == "label") {}
		else if (hs_ == "img") { this->Image(ss); }
		else if (hs_ == "fileset") { this->FileSet(ss); }
		else  {
			return false;
		}
		return true;
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