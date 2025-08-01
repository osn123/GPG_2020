//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
//#include  "MyLib.h"
#include  "Task_Ev_Image.h"
#include "GameEngine_Ver3_83.h"

namespace  Ev_Image
{
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

		this->render2D_Priority[1] = 0.2f;
		this->pos = ML::Vec2(0, 0);
		this->drawBase = ML::Box2D(0, 0, 0, 0);
		this->src = ML::Box2D(0, 0, 0, 0);

		//^XNΜΆ¬

		return  true;
	}
	//-------------------------------------------------------------------
	//uIΉv^XNΑΕΙPρΎ―s€
	bool  Object::Finalize()
	{
		//f[^^XNπϊ
		this->img.reset();
		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//ψ«p¬^XNΜΆ¬
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[Ιs€
	void  Object::UpDate()
	{

	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->drawBase;
		draw.Offset(this->pos);
		this->img->Draw(draw, this->src);

	}



	void Object::CreateOrReset(stringstream& ss_) {
		//
		string taskName;
		ss_ >> taskName;

		//
		auto p = ge->GetTask<Object>("Cxgζ", taskName);
		//
		if (nullptr == p) {
			p = Object::Create(true);
			p->Set(taskName, ss_);
		}
		//
		else
		{
			p->Set(taskName, ss_);
		}
	}
	void Object::Set(const string& taskName_, stringstream& ss_)
	{
		//
		string filePath;
		ss_ >> filePath;

		//
		if (filePath == "off")
		{
			this->Kill();
			return;
		}

		this->name = taskName_;
		//
		this->img = DG::Image::Create(filePath);
		//
		enum class XPos { Left, Center, Right };
		XPos	xPos = XPos::Left;
		bool	xRevers = false;
		bool	yRevers = false;
		string posAndRev;
		ss_ >> posAndRev;
		//
		if (string::npos != posAndRev.find("L")) { xPos = XPos::Left; }
		if (string::npos != posAndRev.find("R")) { xPos = XPos::Right; }
		if (string::npos != posAndRev.find("C")) { xPos = XPos::Center; }
		if (string::npos != posAndRev.find("X")) { xRevers = true; }
		else { xRevers = false; }
		if (string::npos != posAndRev.find("Y")) { yRevers = true; }
		else { yRevers = false; }
		//
		//ML::Point s = this->img->Size();
		POINT s = this->img->Size();
		this->drawBase = ML::Box2D(0, 0, s.x, s.y);
		this->src = ML::Box2D(0, 0, s.x, s.y);
		//
		if (XPos::Left == xPos)
		{
			this->pos.x = 0;
		}
		else if (XPos::Right == xPos)
		{
			this->pos.x = (float)ge->screen2DWidth - s.x;
		}
		else
		{
			this->pos.x = (ge->screen2DWidth - s.x) / 2.0f;
		}
		//
		if (true == xRevers) {
			this->src.x = s.x; this->src.w = -s.x;
		}
		if (true == yRevers) {
			this->src.y = s.y; this->src.h = -s.y;
		}

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