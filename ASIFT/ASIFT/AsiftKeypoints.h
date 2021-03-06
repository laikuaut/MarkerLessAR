// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

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
	// 回転(rot)
	int *rots;
	// Sift Parameter
	siftPar siftparams;
	// キーポイント数
	int num;
	// 縦横幅
	int w,h;
	// 拡大率
	float zoom;

	// 元のグレースケール画像
	pro::Image imgGray;
	// リサイズ後のグレイスケール画像
	pro::Image imgGrayZoom;
	// リサイズ後のピクセルデータ
	std::vector<float> imgPixels;

public:

	// キーポイント <tilt<rot<keylist<key>>>>
	vector< vector< keypointslist > > keys;
	// 一次ベクター用
	keypointslist onceKeys;

	// キーポイントパス
	pro::Dir path;

public:
	AsiftKeypoints(int tilts = 1,std::string ini_name="SiftParam.ini");
	~AsiftKeypoints(void);

	// Asiftキーポイント計算
	int computeAsiftKeyPoints(int verb);

	// アクセサリ関連
	// キーポイント数計算
	int keypointsTotal();
	// キーポイント総数取得
	int getNum() const;
	// チルト取得
	int getTilts() const;
	// 画像の設置
	void setImage(pro::Image &src,int resizeFlag,int width,int height,float &zoom);
	void setImage(pro::Image &src,int resizeFlag,int width,int height);

	//　.iniファイル関連 -> ptree へデータを追加　読み書き処理なし
	void inireadSiftParameters (ptree &pt);
	void iniwriteSiftParameters(ptree &pt);
	void iniwriteSiftParameters(ptree &pt,siftPar par);
	//　.iniファイル関連 -> ptree 内部宣言　読み書き処理あり
	void inireadSiftParameters (std::string name);
	void iniwriteSiftParameters(std::string name);
	void iniwriteSiftParameters(std::string name,siftPar par);

	// 出力処理
	void output(string name);
	void outputOnce(string name);
	// 入力処理
	void input(string name);
	void inputOnce(string name);

	// 矩形範囲外の削除フィルター処理
	void filterRectangle(cv::Point2f pt1,cv::Point2f pt2);
	// 矩形範囲の削除フィルター処理
	void filterRectangleDel(cv::Rect rects[],int n);
	// センターラインフィルター処理
	void filterCenterLine(cv::Point2f centerPt,float distance);

	// キーポイントを一次ベクターに変換
	void setOnceKeys();

	// チルトからサイズ計算
	void tiltsCalc();

	// キーポイントの描写
	void draw(pro::Image& src,cv::Scalar siftcol=cv::Scalar(0,0,255),cv::Scalar asiftcol=cv::Scalar(255,0,0));
	void drawOnce(pro::Image& src,cv::Scalar col=cv::Scalar(255,0,0));

private:

	// リサイズ処理
	void resize(pro::Image &src,pro::Image &dst,int resizeFlag,int width,int height);


};

