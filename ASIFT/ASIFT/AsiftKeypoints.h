#pragma once


#include "demo_lib_sift.h"
#include "compute_asift_keypoints.h"
#include "AsiftMatchings.h"

#include "../../MyLibs/Core/Dir.h"
#include "../../MyLibs/OpenCVLibs/Image.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>
using namespace boost::property_tree;

class AsiftMatchings;

class PRO_EXPORTS AsiftKeypoints
{

	friend class AsiftMatchings;

private:

	// 傾き(Asfitパラメータ)
	int tilts;
	// Sift Parameter
	siftPar siftparams;
	// キーポイント数
	int num;
	// 縦横幅
	int w,h;
	// 拡大率
	float zoom;

public:

	// キーポイント <tilt<rot<keylist<key>>>>
	vector< vector< keypointslist > > keys;

	// キーポイントパス
	pro::Dir path;

public:
	AsiftKeypoints(int tilts = 1);
	~AsiftKeypoints(void);

	// Asiftキーポイント計算
	int computeAsiftKeyPoints(vector<float>& image, int width, int height, int verb,float zoom);

	// アクセサリ関連
	int getNum() const;
	int getTilts() const;

	//　.iniファイル関連
	void inireadSiftParameters(ptree &pt);
	void iniwriteSiftParameters(ptree &pt);
	void iniwriteSiftParameters(ptree &pt,siftPar par);

	// 入出力処理
	void output(string name);
	void input(string name);

	// 矩形フィルター処理
	void filterRectangle(cv::Point2f pt1,cv::Point2f pt2);
	// センターラインフィルター処理
	void filterCenterLine(cv::Point2f centerPt,float distance);


	// キーポイントの描写
	void draw(pro::Image& src);

	// キーポイント数計算
	int keypointsTotal() const;

};

