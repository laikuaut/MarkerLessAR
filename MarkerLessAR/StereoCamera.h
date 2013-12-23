// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#pragma once

#include "../MyLibs/Core/type_c.h"
#include "../MyLibs/OpenCVLibs/OpenCVLibs.h"

class StereoCamera
{
private:

public:

	cv::Mat cameraParamL;
	cv::Mat cameraParamR;

	float camBetween;

public:
	StereoCamera(void);
	~StereoCamera(void);

	void init(std::string xmlName,float camBetween);

	// ワールド座標系を取得する。(カメラL側がワールド座標となる)
	cv::Point3f getWorldPoint(cv::Point2f Lpt,cv::Point2f Rpt);

private:

	// 内部パラメータ読み込み関数
	void getCameraParam(cv::Mat &param,std::string paramName,std::string xmlName);

};

