#pragma once

#include"..\Core\Core.h"
#include<opencv2\core\core.hpp>
#include"Image.h"
#include"Labeling.h"
#include"MouseEvent.h"

namespace pro{

class PRO_EXPORTS LabelingCenter : public MouseEvent
{
private:

	Labeling labels;
	vector<cv::Point2f> centers;

	// mouseRangeFilter�p�̕ϐ�
	cv::Point2f mousePt[2];
	int ptCount;
	Image dst;

public:
	LabelingCenter(void);
	~LabelingCenter(void);

	// ������
	void init(cv::Size size);
	// �d�S�v�Z
	void setCenter(const unsigned char* bins);

	// �~�`�ȊO�̏d�S���O
	void circleFilter(double per,double radius_per=1);
	// �T�C�Y�ɍ���Ȃ��d�S���O
	void sizeFilter(int min,int max);
	// ��`�͈͊O���O
	void rangeRectangleFilter(cv::Point2f pt1,cv::Point2f pt2);
	// �t�B���^�ɂ���č폜�����_��߂�
	void filterReset();
	
	
	void onMouse_impl(int event,int x,int y,int flag);
	// �}�E�X�Ŏw�肵�ċ�`�t�B���^�[��������
	void mouseRangeFilter(Image& img);

	void draw(Image& img,short flag=1,cv::Scalar scal = cv::Scalar::all(0));

	void outputCenter();

	cv::Point operator() (const int num);

public:
	int num;
	vector<int> sizes;
	vector<short> flags;
};

}