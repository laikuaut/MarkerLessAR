// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#include "StereoCamera.h"

#include <opencv2/calib3d/calib3d.hpp>


StereoCamera::StereoCamera(void)
{
	// initを使用する
}


StereoCamera::~StereoCamera(void)
{
	// なし
}

void StereoCamera::init(std::string xmlName,float camBetween,int width,int height){
	
	/*****************************************
	 * パラメータ初期化
	 */
	this->camBetween = camBetween;
	this->width = width;
	this->height = height;

	/*****************************************
	 * 左右のカメラの内部パラメータ行列を設定
	 * カメラ名をここでは手動にしているが、本当はMarkerLessAR.iniから読み込むべき
	 */
	getCameraParam(cameraParamL,"Left800_600",xmlName);
	getCameraParam(cameraParamR,"Right800_600",xmlName);
}

cv::Point3f StereoCamera::getWorldPoint(cv::Point2f Lpt,cv::Point2f Rpt){

	// OpenGLの座標系へ変換
	Lpt.y = height - Lpt.y;
	Rpt.y = height - Rpt.y;

	// 正規化カメラの透視射影行列 (論文 P46 式6.16 P'f)
	double normalizations[][4] = {
		{1,0,0,0},
		{0,1,0,0},
		{0,0,1,0}
	};
	cv::Mat normalizationPers(3,4,CV_64FC1,normalizations);
	//std::cout << normalizationPers << std::endl;

	// 変換行列
	cv::Mat ML = cv::Mat::eye(4,4,CV_64FC1);
	cv::Mat MR = (cv::Mat_<double>(4,4) << 
		1,0,0,camBetween,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1);

	//std::cout << ML << std::endl;
	//std::cout << MR << std::endl;
	
	//std::cout << cameraParamL << std::endl;
	//std::cout << cameraParamR << std::endl;
	

	// 透視射影行列
	cv::Mat persL = cv::Mat(4,3,CV_64FC1);
	cv::Mat persR = cv::Mat(4,3,CV_64FC1);
	// (論文 P46 式6.21)
	persL = cameraParamL * normalizationPers * ML;
	// (論文 P46 式6.22)
	persR = cameraParamR * normalizationPers * MR;
	
	//std::cout << cameraParamL << std::endl;
	//std::cout << persL << std::endl;
	//std::cout << persR << std::endl;
	
	//std::cout<<Lpt<<std::endl;
	//std::cout<<Rpt<<std::endl;

	// 係数行列 (論文P47 式6.23 左辺)
	cv::Mat Bmat = (cv::Mat_<double>(4,3) << 
		persL.at<double>(2,0)*Lpt.x - persL.at<double>(0,0),
		persL.at<double>(2,1)*Lpt.x - persL.at<double>(0,1),
		persL.at<double>(2,2)*Lpt.x - persL.at<double>(0,2),
		
		persL.at<double>(2,0)*Lpt.y - persL.at<double>(1,0),
		persL.at<double>(2,1)*Lpt.y - persL.at<double>(1,1),
		persL.at<double>(2,2)*Lpt.y - persL.at<double>(1,2),
		
		persR.at<double>(2,0)*Rpt.x - persR.at<double>(0,0),
		persR.at<double>(2,1)*Rpt.x - persR.at<double>(0,1),
		persR.at<double>(2,2)*Rpt.x - persR.at<double>(0,2),
		
		persR.at<double>(2,0)*Rpt.y - persR.at<double>(1,0),
		persR.at<double>(2,1)*Rpt.y - persR.at<double>(1,1),
		persR.at<double>(2,2)*Rpt.y - persR.at<double>(1,2)
		);

	//std::cout << Bmat << std::endl;

	// 係数行列 (論文P47 式6.23 右辺)
	cv::Mat Cmat = (cv::Mat_<double>(4,1) << 
		persL.at<double>(0,3) - persL.at<double>(2,3)*Lpt.x,
		persL.at<double>(1,3) - persL.at<double>(2,3)*Lpt.y,
		persR.at<double>(0,3) - persR.at<double>(2,3)*Rpt.x,
		persR.at<double>(1,3) - persR.at<double>(2,3)*Rpt.y);


	// 逆行列　(論文P47 式6.24)
	cv::Mat Binv = cv::Mat(3,4,CV_64FC1);
	Binv = (Bmat.t()*Bmat).inv()*Bmat.t();
	
	//std::cout << Binv << std::endl;
	//std::cout << Cmat << std::endl;

	// ワールド座標系
	cv::Mat Wmat = cv::Mat(3,1,CV_64FC1);
	Wmat = Binv*Cmat;
	
	//std::cout << Wmat << std::endl;
	
	// 座標の反転を解消
	cv::Point3f wpt;
	wpt.x = -Wmat.at<double>(0,0);
	wpt.y = -Wmat.at<double>(1,0);
	wpt.z = Wmat.at<double>(2,0);

	return wpt;
}

void StereoCamera::getCameraParam(cv::Mat &param,std::string paramName,std::string xmlName){
 
	// 読み込みファイルの作成は
	// main.cpp : main_calibrate関数

    cv::FileStorage cvfs(xmlName, CV_STORAGE_READ);
    cv::FileNode node(cvfs.fs, NULL);

    cv::read(node[paramName], param);

}