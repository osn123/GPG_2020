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
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//uϊ»v^XNΆ¬ΙPρΎ―s€
	bool  Object::Initialize()
	{
		//X[p[NXϊ»
		__super::Initialize(defGroupName, defName, true);
		//\[XNXΆ¬or\[X€L
		this->res = Resource::Create();

		//f[^ϊ»
		
		//^XNΜΆ¬
		ge->StopAll_G("vC");
		ge->StopAll_G("G");
		ge->StopAll_G("eivCj");
		ge->StopAll_G("ACe");

		//
		//

		return  true;
	}
	//-------------------------------------------------------------------
	//uIΉv^XNΑΕΙPρΎ―s€
	bool  Object::Finalize()
	{
		//
		//

		//f[^^XNπϊ
		if (this->evFile.is_open())
		{
			this->evFile.close();//
		}

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//ψ«p¬^XNΜΆ¬
		}

		ge->StopAll_G("vC",false);
		ge->StopAll_G("G", false);
		ge->StopAll_G("eivCj", false);
		ge->StopAll_G("ACe", false);

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[Ιs€
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
	//uQc`ζvPt[Ιs€
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
			if (string::npos == lineT_.find_first_not_of(" @")) {
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
		else if (hs_ == "appear") { this->ApprearObject(ss); }
		else  {
			return false;
		}
		return true;
	}

	//
	//ΘΊΝξ{IΙΟXsvΘ\bh
	//
	//-------------------------------------------------------------------
	//^XNΆ¬ϋ
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//Q[GWΙo^
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYΙΈs΅½ηKill
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
	//\[XNXΜΆ¬
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