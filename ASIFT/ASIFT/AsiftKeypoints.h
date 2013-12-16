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

public:

	// �L�[�|�C���g <tilt<rot<keylist<key>>>>
	vector< vector< keypointslist > > keys;

	// �L�[�|�C���g�p�X
	pro::Dir path;

public:
	AsiftKeypoints(int tilts = 1);
	~AsiftKeypoints(void);

	// Asift�L�[�|�C���g�v�Z
	int computeAsiftKeyPoints(vector<float>& image, int width, int height, int verb,float zoom);

	// �A�N�Z�T���֘A
	int getNum() const;
	int getTilts() const;

	//�@.ini�t�@�C���֘A
	void inireadSiftParameters(ptree &pt);
	void iniwriteSiftParameters(ptree &pt);
	void iniwriteSiftParameters(ptree &pt,siftPar par);

	// ���o�͏���
	void output(string name);
	void input(string name);

	// ��`�t�B���^�[����
	void filterRectangle(cv::Point2f pt1,cv::Point2f pt2);
	// �Z���^�[���C���t�B���^�[����
	void filterCenterLine(cv::Point2f centerPt,float distance);


	// �L�[�|�C���g�̕`��
	void draw(pro::Image& src);

	// �L�[�|�C���g���v�Z
	int keypointsTotal() const;

};

