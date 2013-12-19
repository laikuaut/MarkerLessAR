// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#include "StereoCamera.h"

#include <opencv2/calib3d/calib3d.hpp>


StereoCamera::StereoCamera(void)
{

}


StereoCamera::~StereoCamera(void)
{

}

void StereoCamera::init(std::string xmlName,float camBetween){
	getCameraParam(cameraParamL,"Left",xmlName);
	getCameraParam(cameraParamR,"Right",xmlName);
	this->camBetween = camBetween;
}

cv::Point3f StereoCamera::getWorldPoint(cv::Point2f Lpt,cv::Point2f Rpt){

	// 正規化カメラの透視射影行列
	double normalizations[][4] = {
		{1,0,0,0},
		{0,1,0,0},
		{0,0,1,0}
	};
	cv::Mat normalizationPers(3,4,CV_64FC1,normalizations);
	std::cout << normalizationPers << std::endl;

	// 変換行列
	cv::Mat ML = cv::Mat::eye(4,4,CV_64FC1);
	cv::Mat MR = (cv::Mat_<double>(4,4) << 
		1,0,0,camBetween,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1);
	std::cout << ML << std::endl;
	std::cout << MR << std::endl;
	
	std::cout << cameraParamL << std::endl;
	std::cout << cameraParamR << std::endl;

	// 透視射影行列
	cv::Mat persL = cv::Mat(4,3,CV_64FC1);
	cv::Mat persR = cv::Mat(4,3,CV_64FC1);
	persL = cameraParamL * normalizationPers * ML;
	persR = cameraParamR * normalizationPers * MR;
	
	std::cout << persL << std::endl;
	std::cout << persR << std::endl;

	// 係数行列
	cv::Mat Bmat = (cv::Mat_<double>(4,3) << 
		persL.data[4*2+0]*Lpt.x - persL.data[4*0+0],
		persL.data[4*2+1]*Lpt.x - persL.data[4*0+1],
		persL.data[4*2+2]*Lpt.x - persL.data[4*0+2],
		
		persL.data[4*2+0]*Lpt.y - persL.data[4*1+0],
		persL.data[4*2+1]*Lpt.y - persL.data[4*1+1],
		persL.data[4*2+2]*Lpt.y - persL.data[4*1+2],
		
		persR.data[4*2+0]*Rpt.x - persR.data[4*0+0],
		persR.data[4*2+1]*Rpt.x - persR.data[4*0+1],
		persR.data[4*2+2]*Rpt.x - persR.data[4*0+2],
		
		persR.data[4*2+0]*Rpt.y - persR.data[4*1+0],
		persR.data[4*2+1]*Rpt.y - persR.data[4*1+1],
		persR.data[4*2+2]*Rpt.y - persR.data[4*1+2]
		);
	std::cout << Bmat << std::endl;

	// 係数行列
	cv::Mat Cmat = (cv::Mat_<double>(4,1) << 
		persL.data[4*0+3] - persL.data[4*2+3]*Lpt.x,
		persL.data[4*1+3] - persL.data[4*2+3]*Lpt.y,
		persR.data[4*0+3] - persR.data[4*2+3]*Rpt.x,
		persR.data[4*1+3] - persR.data[4*2+3]*Rpt.y);
	std::cout << (double)persL.data[4*0+3] << std::endl;
	std::cout << (double)persL.at<double>(0,4) << std::endl;
	std::cout << Cmat << std::endl;

	// 逆行列
	cv::Mat Binv = cv::Mat(3,4,CV_64FC1);
	Binv = (Bmat.t()*Bmat).inv()*Bmat.t();
	
	std::cout << Binv << std::endl;

	// ワールド座標系
	cv::Mat Wmat = cv::Mat(3,1,CV_64FC1);
	Wmat = Binv*Cmat;
	cv::Point3f wpt;
	wpt.x = Wmat.data[0];
	wpt.y = Wmat.data[1];
	wpt.z = Wmat.data[2];

	return wpt;
}

void StereoCamera::getCameraParam(cv::Mat &param,std::string paramName,std::string xmlName){
	// cv::Mat param;
	//param = cv::Mat(3,3,CV_32FC1);
 
    cv::FileStorage cvfs(xmlName, CV_STORAGE_READ);
    cv::FileNode node(cvfs.fs, NULL);

    cv::read(node[paramName], param);

	std::cout << param.type() << std::endl;

    //std::cout << param << std::endl;
}