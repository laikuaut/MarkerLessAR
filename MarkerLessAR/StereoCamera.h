// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#pragma once

/**
 * 平行ステレオ法クラス 論文P45
 */

#include "../MyLibs/Core/type_c.h"
#include "../MyLibs/OpenCVLibs/OpenCVLibs.h"

class StereoCamera
{
private:

public:

	/**
	 * カメラの内部パラメータ行列 論文 P42
	 */
	cv::Mat cameraParamL; // 左カメラのパラメータ
	cv::Mat cameraParamR; // 右カメラのパラメータ

	// カメラ間間隔[mm]
	float camBetween;

	// 画像縦横幅
	int width; // 横幅
	int height; // 縦幅

public:
	StereoCamera(void);
	~StereoCamera(void);

	/**
	 * ステレオ法クラスの初期化
	 * @param xmlName 内部パラメータを保存しているxmlファイル名
	 * @param camBetween カメラ間の距離[mm]
	 * @param width 画像横幅 
	 * @param height 画像縦幅 
	 */
	void init(std::string xmlName,float camBetween,int width,int height);

	/**
	 * ワールド座標系を取得する。(カメラL側がワールド座標となる)
	 * @param Lpt 左画像の対応点
	 * @param Rpt 右画像の対応点
	 * @return 3次元座標
	 */
	cv::Point3f getWorldPoint(cv::Point2f Lpt,cv::Point2f Rpt);

private:

	/**
	 * 内部パラメータ読み込み関数
	 * @param param カメラの内部パラメータ行列
	 * @param paramName カメラ名
	 * @param xmlName 内部パラメータを保存しているxmlファイル名
	 */
	void getCameraParam(cv::Mat &param,std::string paramName,std::string xmlName);

};

