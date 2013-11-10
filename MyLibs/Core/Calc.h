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
	 *  ���W�v�Z�p�֐��Q
	 */
	// �_�̉�]�s��
	static cv::Point2f PointRotate(cv::Point2f pt,int angle);
	static cv::Point2f PointRotate(double dx,double dy,int angle);
	// 2�_���狁�߂�P�ʃx�N�g��
	static cv::Point2f UnitVec(cv::Point2f pt1,cv::Point2f pt2);
	// 2�����x�N�g���̑傫����Ԃ�
	static double Vec2dScalar(cv::Vec2f vec);
	// �O�ς̑傫����Ԃ�
	static double Vec2fCrossScalar(cv::Vec2f v1,cv::Vec2f v2);
	// 2�_�Ԃ̋��������߂�
	static double getDistance(cv::Point2f pt1,cv::Point2f pt2);
	// 2�_�Ԃ̋�����2���Ԃ�
	static double getDistance2(cv::Point2f pt1,cv::Point2f pt2);
	// ���ς�p���ăx�N�g���̃i�X�p��Ԃ�
	static double InnerProductAngle(cv::Vec2f v1,cv::Vec2f v2);
	// �Q�_����p�x�����߂�B�i0�`90�j
	static double getAngle(cv::Point2f pt1,cv::Point2f pt2);
	// 2�x�N�g�������s�ɂ��邩�ǂ���
	static bool Parallel2Vec(double angle);
	// 2�x�N�g���������ł��邩�ǂ���
	static bool Vertical2Vec(double angle);
};

}