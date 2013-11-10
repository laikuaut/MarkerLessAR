// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#pragma once

#include"type_c.h"
#include<opencv2\core\core.hpp>
#include<iostream>

namespace pro{

class PRO_EXPORTS Calc
{
public:
	Calc(void);
	~Calc(void);

	/******************************
	 *  座標計算用関数群
	 */
	// 点の回転行列式
	static cv::Point2f PointRotate(cv::Point2f pt,int angle);
	static cv::Point2f PointRotate(double dx,double dy,int angle);
	// 2点から求める単位ベクトル
	static cv::Point2f UnitVec(cv::Point2f pt1,cv::Point2f pt2);
	// 2次元ベクトルの大きさを返す
	static double Vec2dScalar(cv::Vec2f vec);
	// 外積の大きさを返す
	static double Vec2fCrossScalar(cv::Vec2f v1,cv::Vec2f v2);
	// 2点間の距離を求める
	static double getDistance(cv::Point2f pt1,cv::Point2f pt2);
	// 2点間の距離の2乗を返す
	static double getDistance2(cv::Point2f pt1,cv::Point2f pt2);
	// 内積を用いてベクトルのナス角を返す
	static double InnerProductAngle(cv::Vec2f v1,cv::Vec2f v2);
	// ２点から角度を求める。（0～90）
	static double getAngle(cv::Point2f pt1,cv::Point2f pt2);
	// 2ベクトルが平行にあるかどうか
	static bool Parallel2Vec(double angle);
	// 2ベクトルが垂直であるかどうか
	static bool Vertical2Vec(double angle);
};

}