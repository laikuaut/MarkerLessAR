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

	// �X��(Asfit�p�����[�^)
	int tilts;
	// Sift Parameter
	siftPar siftparams;
	// �L�[�|�C���g��
	int num;
	// �c����
	int w,h;
	// �g�嗦
	float zoom;

	// ���̃O���[�X�P�[���摜
	pro::Image imgGray;
	// ���T�C�Y��̃O���C�X�P�[���摜
	pro::Image imgGrayZoom;
	// ���T�C�Y��̃s�N�Z���f�[�^
	std::vector<float> imgPixels;

public:

	// �L�[�|�C���g <tilt<rot<keylist<key>>>>
	vector< vector< keypointslist > > keys;

	// �L�[�|�C���g�p�X
	pro::Dir path;

public:
	AsiftKeypoints(int tilts = 1,std::string ini_name="SiftParam.ini");
	~AsiftKeypoints(void);

	// Asift�L�[�|�C���g�v�Z
	int computeAsiftKeyPoints(int verb);

	// �A�N�Z�T���֘A
	// �L�[�|�C���g���v�Z
	int keypointsTotal();
	// �L�[�|�C���g�����擾
	int getNum() const;
	// �`���g�擾
	int getTilts() const;
	// �摜�̐ݒu
	void setImage(pro::Image &src,int resizeFlag,int width,int height,float &zoom);
	void setImage(pro::Image &src,int resizeFlag,int width,int height);

	//�@.ini�t�@�C���֘A -> ptree �փf�[�^��ǉ��@�ǂݏ��������Ȃ�
	void inireadSiftParameters (ptree &pt);
	void iniwriteSiftParameters(ptree &pt);
	void iniwriteSiftParameters(ptree &pt,siftPar par);
	//�@.ini�t�@�C���֘A -> ptree �����錾�@�ǂݏ�����������
	void inireadSiftParameters (std::string name);
	void iniwriteSiftParameters(std::string name);
	void iniwriteSiftParameters(std::string name,siftPar par);

	// �o�͏���
	void output(string name);
	// ���͏���
	void input(string name);

	// ��`�t�B���^�[����
	void filterRectangle(cv::Point2f pt1,cv::Point2f pt2);
	// �Z���^�[���C���t�B���^�[����
	void filterCenterLine(cv::Point2f centerPt,float distance);


	// �L�[�|�C���g�̕`��
	void draw(pro::Image& src,cv::Scalar siftcol=cv::Scalar(0,0,255),cv::Scalar asiftcol=cv::Scalar(255,0,0));

private:

	// ���T�C�Y����
	void resize(pro::Image &src,pro::Image &dst,int resizeFlag,int width,int height);


};

