#include  "MyPG.h"
#include  "Task_EventEngine.h"

namespace EventEngine {

	bool Object::FileSet(stringstream& ss_) {
		string filePath;
		ss_ >> filePath;

		return this->Set(filePath);
	}

	bool Object::Image(stringstream& ss_) {
		string OffScreenName;
		string filePath;
		ss_ >> OffScreenName >> filePath;
		DG::Image::SP img = DG::ImageManager::Get(OffScreenName);
		if (nullptr != img) {
			img->ReLoad(filePath);//
		}
		else
		{
			img = DG::Image::Create(filePath);
			DG::ImageManager::Set(OffScreenName, img);
		}
		return true;
	}


	//
	bool Object::EventFlag(stringstream& ss_) {
		string flagName;
		string sign;
		float value;
		ss_ >> flagName >> sign >> value;
		if (sign == "=") { ge->evFlags[flagName] = value; }
		else if (sign == "+") { ge->evFlags[flagName] += value; }
		else if (sign == "-") { ge->evFlags[flagName] -= value; }
		else { return false; }
		return true;
	}

	bool Object::If(stringstream& ss_) {
		//
		string flagKind;
		ss_ >> flagKind;
		bool flag = false;
		string labelName;
		//
		if (flagKind == "ev_flag")
		{
			string flagName;
			string sign;
			float value;
			ss_ >> flagName >> sign >> value >> labelName;
			//
			if (sign == "==") { flag = ge->evFlags[flagName] == value; }
			else if (sign == ">") { flag = ge->evFlags[flagName] > value; }
			else if (sign == "<") { flag = ge->evFlags[flagName] < value; }
			else { return false; }
		}
		//
		else if (flagKind == "charaParam") {
			//
		}
		//
		else if (flagKind == "System") {
			//
		}
		//
		if (true == flag) {
			//
			this->evFile.seekg(0, ios_base::beg);
			//
			string lt;
			while (this->ReadLine(lt)) {
				//
				string::size_type t = lt.find(">");
				string headerStr = lt.substr(0, t);
				string dataStr = lt.substr(t + 1);
				if ("label" == headerStr) {
					//
					stringstream ss;
					ss << dataStr;
					string labelName2;
					ss >> labelName2;
					if (labelName == labelName2) {
						break;
					}
				}
			}
		}
		return true;
	}
}//