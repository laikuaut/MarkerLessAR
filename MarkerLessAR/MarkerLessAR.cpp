// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#include "MarkerLessAR.h"
#include "../MyLibs/Core/Calc.h"

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
	asiftLR.init(0,iniAsiftFileName);
	
	cameraBetween = 95;
	calibrateXmlName = "CameraCalibrate.xml";

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
	asiftLeft.setNames(imgMarkerName,imgLeftName,videoLeftName);

	/********************************
	 * Right
	 */
	asiftRight.init(1,iniAsiftFileName);
	asiftRight.setNames(imgMarkerName,imgRightName,videoRightName);
	
	/********************************
	 * Left & Right
	 */
	asiftLR.init(1,iniAsiftFileName);
	asiftLR.setNames(imgLeftName,imgRightName,"");

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
	calibrateXmlName = pt.get_optional<std::string>("Camera.CalibrateXml").get();

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
	pt.put("Camera.CalibrateXml",calibrateXmlName);

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

void MarkerLessAR::outputPoint3s(vector<cv::Point3f> point3s,std::string name){

	std::ofstream of;
	of.open(path.pwd(name));

	if(!of.is_open()){
		cout << name << "not open." << endl;
		return;
	}

	of << point3s.size() << endl;

	for(int i=0;i<point3s.size();i++){
		of << point3s[i].x << " "
		   << point3s[i].y << " "
		   << point3s[i].z << endl;
	}

}

vector<cv::Point3f> MarkerLessAR::inputPoint3s(std::string name){

	std::ifstream inf;
	vector<cv::Point3f> pt3s;

	int num;

	inf.open(path.pwd(name));

	inf >> num;

	for(int i=0;i<num;i++){
		cv::Point3f pt3;
		inf >> pt3.x;
		inf >> pt3.y;
		inf >> pt3.z;
		pt3s.push_back(pt3);
	}

	return pt3s;

}

matchingslist MarkerLessAR::matchingLR(matchingslist matchingsL,matchingslist matchingsR){
	double radius = 0.1;
	matchingslist matchingsLR;
	for(int i=0;i<matchingsL.size();i++){
		for(int j=0;j<matchingsR.size();j++){
			if(pro::Calc::getDistance2(
				cv::Point2f(matchingsL[i].first.x,matchingsL[i].first.y),
				cv::Point2f(matchingsR[j].first.x,matchingsR[j].first.y)) < radius*radius){
					matchingsLR.push_back(matching(matchingsL[i].second,matchingsR[j].second));
			}
		}
	}
	return matchingsLR;
}

vector<cv::Point3f> MarkerLessAR::getWorldPoints(AsiftMatchings matchings){
	stereo.init(calibrateXmlName,cameraBetween);
	vector<cv::Point3f> wpts;
	for(int i=0;i<matchings.matchings.size();i++){
		wpts.push_back(stereo.getWorldPoint(
			cv::Point2f(matchings.matchings[i].first.x,matchings.matchings[i].first.y),
			cv::Point2f(matchings.matchings[i].second.x,matchings.matchings[i].second.y)));
	}
	return wpts;
}

void MarkerLessAR::run(){

	pro::Timer timer;
	
	/************************************************
	 * 画像のセット
	 */
	cout << "loading marker image..." << endl; 

	asiftLeft.initImages();
	cout << "left image ok !" << endl;
	asiftRight.initImages();
	cout << "right image ok !" << endl;
	asiftLR.initKeys(Asift::IMAGE_ID_BASE,1);
	asiftLR.initKeys(Asift::IMAGE_ID_INPUT,1);
	asiftLR.initImages();
	cout << "L&R image ok !" << endl;
	
	/************************************************
	 * マーカーの読み込み
	 */
	cout << "loading marker keys......" << endl;

	asiftLeft.input(Asift::INPUT_ID_KEYS_BASE);
	cout << "left keys ok !" << endl;
	asiftRight.input(Asift::INPUT_ID_KEYS_BASE);
	cout << "left xAxis ok !" << endl;
	asiftLeft.input(Asift::INPUT_ID_KEYS_XAXIS);
	cout << "right keys ok !" << endl;
	asiftRight.input(Asift::INPUT_ID_KEYS_XAXIS);
	cout << "right xAxis ok !" << endl;
	
	/************************************************
	 * キーポイントの演算
	 */
	std::cout << "Computing Left on the image..." << endl;
	asiftLeft.computeKeyPoints(Asift::IMAGE_ID_INPUT);
	std::cout << asiftLeft.inputKeys.getNum() <<  " ASIFT keypoints are detected." << endl;
	std::cout << "Computing Right on the image..." << endl;
	asiftRight.computeKeyPoints(Asift::IMAGE_ID_INPUT);
	std::cout << asiftRight.inputKeys.getNum() <<  " ASIFT keypoints are detected." << endl;

	/************************************************
	 * キーポイントマッチング処理
	 */
	// 左右それぞれのマッチング処理
	std::cout << "Matching Left on the keypoints..." << endl;
	asiftLeft.computeMatching(asiftLeft.baseKeys,asiftLeft.inputKeys);
	std::cout << "Matching Right on the keypoints..." << endl;
	asiftRight.computeMatching(asiftRight.baseKeys,asiftRight.inputKeys);

	// マッチング点だけにキーポイントを補正
	std::cout << "only Left Matching Keyspoints..." << endl;
	asiftLeft.matchings.filterMatching();
	std::cout << "only Right Matching Keyspoints..." << endl;
	asiftRight.matchings.filterMatching();

	// 左右同士のマッチング
	matchingsLR.setKeypoints(*asiftLeft.matchings.asiftKeys2,*asiftRight.matchings.asiftKeys2);
	matchingsLR.matchings=matchingLR(asiftLeft.matchings.matchings,asiftRight.matchings.matchings);
	matchingsLR.output(pro::Dir::getStem(imgMarkerName)+"_LR.txt");
	//asiftLR.createHoriImage(asiftLR.imgInput,matchingsLR,"LRH.png");
	cout << matchingsLR.getNum() << " matching." << endl;

	// ワールド座標へ変換
	worldPoints = getWorldPoints(matchingsLR);
	outputPoint3s(worldPoints,"worldPoints.txt");

	/************************************************
	 * X軸マッチング
	 */
	// 左右それぞれのX軸のマッチング処理
	std::cout << "Matching Left on the xAxis keypoints..." << endl;
	asiftLeft.computeMatching(asiftLeft.xAxis,asiftLeft.inputKeys);
	std::cout << "Matching Right on the xAxis keypoints..." << endl;
	asiftRight.computeMatching(asiftRight.xAxis,asiftRight.inputKeys);
	
	// X軸のマッチング点だけにキーポイントを補正
	std::cout << "only Left Matching xAxis Keyspoints..." << endl;
	asiftLeft.matchings.filterMatching();
	std::cout << "only Right Matching xAxis Keyspoints..." << endl;
	asiftRight.matchings.filterMatching();
	
	// 左右同士のマッチング xAxis
	matchingsLR.setKeypoints(*asiftLeft.matchings.asiftKeys2,*asiftRight.matchings.asiftKeys2);
	matchingsLR.matchings=matchingLR(asiftLeft.matchings.matchings,asiftRight.matchings.matchings);
	matchingsLR.output(pro::Dir::getStem(imgMarkerName)+"_xAxisLR.txt");
	asiftLR.createVertImage(asiftLR.imgInput,matchingsLR,"xAxisLRH.png");
	cout << matchingsLR.getNum() << " matching." << endl;
	
	// ワールド座標へ変換
	worldPoints = getWorldPoints(matchingsLR);
	outputPoint3s(worldPoints,"worldPoints_xAxis.txt");

	/************************************************
	 * 結果出力
	 */
	// テスト表示
	asiftLeft.output(Asift::OUTPUT_ID_HORI,asiftLeft.imgInput);
	asiftRight.output(Asift::OUTPUT_ID_HORI,asiftRight.imgInput);
	asiftLR.output(Asift::OUTPUT_ID_HORI,asiftLR.imgInput);
	//cv::waitKey(0);

}