// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#include "MarkerLessAR.h"


MarkerLessAR::MarkerLessAR(void)
{
}


MarkerLessAR::~MarkerLessAR(void)
{
}

void MarkerLessAR::init(int readini,string ini_name){

	path = pro::Dir();
	
	if(readini){
		readIni(ini_name);
	}else{
		defaultParam();
	}
	
}

void MarkerLessAR::defaultParam(){
	
	iniAsiftLeftName = "AsiftLeft.ini";
	iniAsiftRightName = "AsiftRight.ini";
	
	asiftLeft.init(0,iniAsiftLeftName);
	asiftRight.init(0,iniAsiftRightName);
	
	cameraBetween = 100;

}

void MarkerLessAR::readIni(ptree &pt){

	// read ini file name
	iniAsiftLeftName = pt.get_optional<std::string>("Ini.AsiftLeft").get();
	iniAsiftRightName = pt.get_optional<std::string>("Ini.AsiftRight").get();

	asiftLeft.init(1,iniAsiftLeftName);
	asiftRight.init(1,iniAsiftRightName);

	// read camera param
	cameraBetween = pt.get_optional<int>("Camera.BetweenDistance").get();
}

void MarkerLessAR::writeIni(ptree &pt){

	// write ini file
	pt.put("Ini.AsiftLeft",iniAsiftLeftName);
	pt.put("Ini.AsiftRight",iniAsiftRightName);

	// camera between distance
	pt.put("Camera.BetweenDistance",cameraBetween);

}

void MarkerLessAR::readIni(std::string name){

	ptree pt;
	if(path.isExist(name)){
		read_ini(name,pt);
		readIni(pt);
	}else{
		cout << "not exist " << name << endl;
		defaultParam();
		writeIni(name);
	}

}

void MarkerLessAR::writeIni(std::string name){

	ptree pt;

	writeIni(pt);

	write_ini(name,pt);

}

void MarkerLessAR::run(){
}