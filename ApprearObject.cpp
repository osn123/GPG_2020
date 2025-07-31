//-----------------------------------------------------------------------------
//オブジェクトを生成する窓口
//-----------------------------------------------------------------------------
#include  "ApprearObject.h"
//生成対象にするものは全てinclude
#include  "Task_Player.h"
#include  "Task_Sprite.h"
#include  "Task_Enemy00.h"
#include  "Task_Item00.h"
#include  "Task_Item01.h"
#include  "Task_Item02.h"
//-----------------------------------------------------------------------------
//BCaraを継承しているタスク専用
//名称指定のみで、対応するタスクを生成し、エンジンに登録する
BChara::SP AppearObject_BChara(const string& name_) {
	//
	BChara::SP w = nullptr;
	if ("Player" == name_) { w = Player::Object::Create(true); }
	else if ("Enemy00" == name_) { w = Enemy00::Object::Create(true); }
	else if ("Item00" == name_) { w = Item00::Object::Create(true); }
	else if ("Item01" == name_) { w = Item01::Object::Create(true); }
	else if ("Item02" == name_) { w = Item02::Object::Create(true); }
	else if ("Sprite" == name_) { w = Sprite::Object::Create(true); }
	return w;
}