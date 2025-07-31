#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

namespace  EventEngine
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("イベント");	//グループ名
	const  string  defName("実行エンジン");	//タスク名
	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()	override;
		bool  Finalize()	override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//共有する変数はここに追加する
		
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		Resource::SP	res;
	private:
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate()			override;//「実行」１フレーム毎に行う処理
		void  Render2D_AF()		override;//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
		static WP instance;	//
		ifstream evFile;//

		//
		//
		static Object::SP Create_Mutex();
		//
		bool Set(const string& fPath_);
		//
		bool ReadLine(string& lineT_);
		//
		bool Execute(string& hs_, string& ds_);
		//
		//
		bool EventFlag(stringstream& ss_);
		//
		bool If(stringstream& ss_);
		//
		bool Image(stringstream& ss_);

		bool FileSet(stringstream& ss_);
	};
}