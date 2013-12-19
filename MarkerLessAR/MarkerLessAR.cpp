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

	initAsift();
}

void MarkerLessAR::defaultParam(){
	
	iniAsiftFileName = "Asift.ini";
	//iniAsiftLeftName = "AsiftLeft.ini";
	//iniAsiftRightName = "AsiftRight.ini";
	
	asiftLeft.init(0,iniAsiftFileName);
	asiftRight.init(0,iniAsiftFileName);
	
	cameraBetween = 100;

	imgMarkerName = "marker.png";

	cameraLeftNum = 0;
	cameraLeftName = "Left";
	imgLeftName = "left.png";
	videoLeftName = "left.avi";

	cameraRightNum = 1;
	cameraRightName = "Right";
	imgRightName = "right.png";
	videoRightName = "right.avi";

}

void MarkerLessAR::initAsift(){
	/********************************
	 * Left
	 */
	asiftLeft.init(1,iniAsiftFileName);
	asiftLeft.setNames(imgMarkerName,imgLeftName);

	/********************************
	 * Right
	 */
	asiftRight.init(1,iniAsiftFileName);
	asiftRight.setNames(imgMarkerName,imgRightName);

}

void MarkerLessAR::readIni(ptree &pt){

	// read ini file name
	iniAsiftFileName = pt.get_optional<std::string>("Ini.Asift").get();
	//iniAsiftLeftName = pt.get_optional<std::string>("Ini.AsiftLeft").get();
	//iniAsiftRightName = pt.get_optional<std::string>("Ini.AsiftRight").get();

	//asiftLeft.init(1,iniAsiftFileName);
	//asiftRight.init(1,iniAsiftFileName);

	//asiftLeft.init(1,iniAsiftLeftName);
	//asiftRight.init(1,iniAsiftRightName);

	// read camera param
	cameraBetween = pt.get_optional<int>("Camera.BetweenDistance").get();

	// read Marker
	imgMarkerName = pt.get_optional<std::string>("Marker.Image").get();

	// read Left
	cameraLeftNum = pt.get_optional<int>("Left.CameraNum").get();
	cameraLeftName = pt.get_optional<std::string>("Left.CameraName").get();
	imgLeftName = pt.get_optional<std::string>("Left.Image").get();
	videoLeftName = pt.get_optional<std::string>("Left.Video").get();

	// read right
	cameraRightNum = pt.get_optional<int>("Right.CameraNum").get();
	cameraRightName = pt.get_optional<std::string>("Right.CameraName").get();
	imgRightName = pt.get_optional<std::string>("Right.Image").get();
	videoRightName = pt.get_optional<std::string>("Right.Video").get();

}

void MarkerLessAR::writeIni(ptree &pt){

	// write ini file
	pt.put("Ini.Asift",iniAsiftFileName);

	// write camera 
	pt.put("Camera.BetweenDistance",cameraBetween);

	// write Marker
	pt.put("Marker.Image",imgMarkerName);

	// write Left
	pt.put("Left.CameraNum",cameraLeftNum);
	pt.put("Left.CameraName",cameraLeftName);
	pt.put("Left.Image",imgLeftName);
	pt.put("Left.Video",videoLeftName);

	// write Right
	pt.put("Right.CameraNum",cameraRightNum);
	pt.put("Right.CameraName",cameraRightName);
	pt.put("Right.Image",imgRightName);
	pt.put("Right.Video",videoRightName);


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