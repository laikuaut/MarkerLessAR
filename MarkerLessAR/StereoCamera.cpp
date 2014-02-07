// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#include "StereoCamera.h"

#include <opencv2/calib3d/calib3d.hpp>


StereoCamera::StereoCamera(void)
{
	// init���g�p����
}


StereoCamera::~StereoCamera(void)
{
	// �Ȃ�
}

void StereoCamera::init(std::string xmlName,float camBetween,int width,int height){
	
	/*****************************************
	 * �p�����[�^������
	 */
	this->camBetween = camBetween;
	this->width = width;
	this->height = height;

	/*****************************************
	 * ���E�̃J�����̓����p�����[�^�s���ݒ�
	 * �J�������������ł͎蓮�ɂ��Ă��邪�A�{����MarkerLessAR.ini����ǂݍ��ނׂ�
	 */
	getCameraParam(cameraParamL,"Left800_600",xmlName);
	getCameraParam(cameraParamR,"Right800_600",xmlName);
}

cv::Point3f StereoCamera::getWorldPoint(cv::Point2f Lpt,cv::Point2f Rpt){

	// OpenGL�̍��W�n�֕ϊ�
	Lpt.y = height - Lpt.y;
	Rpt.y = height - Rpt.y;

	// ���K���J�����̓����ˉe�s�� (�_�� P46 ��6.16 P'f)
	double normalizations[][4] = {
		{1,0,0,0},
		{0,1,0,0},
		{0,0,1,0}
	};
	cv::Mat normalizationPers(3,4,CV_64FC1,normalizations);
	//std::cout << normalizationPers << std::endl;

	// �ϊ��s��
	cv::Mat ML = cv::Mat::eye(4,4,CV_64FC1);
	cv::Mat MR = (cv::Mat_<double>(4,4) << 
		1,0,0,camBetween,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1);

	//std::cout << ML << std::endl;
	//std::cout << MR << std::endl;
	
	//std::cout << cameraParamL << std::endl;
	//std::cout << cameraParamR << std::endl;
	

	// �����ˉe�s��
	cv::Mat persL = cv::Mat(4,3,CV_64FC1);
	cv::Mat persR = cv::Mat(4,3,CV_64FC1);
	// (�_�� P46 ��6.21)
	persL = cameraParamL * normalizationPers * ML;
	// (�_�� P46 ��6.22)
	persR = cameraParamR * normalizationPers * MR;
	
	//std::cout << cameraParamL << std::endl;
	//std::cout << persL << std::endl;
	//std::cout << persR << std::endl;
	
	//std::cout<<Lpt<<std::endl;
	//std::cout<<Rpt<<std::endl;

	// �W���s�� (�_��P47 ��6.23 ����)
	cv::Mat Bmat = (cv::Mat_<double>(4,3) << 
		persL.at<double>(2,0)*Lpt.x - persL.at<double>(0,0),
		persL.at<double>(2,1)*Lpt.x - persL.at<double>(0,1),
		persL.at<double>(2,2)*Lpt.x - persL.at<double>(0,2),
		
		persL.at<double>(2,0)*Lpt.y - persL.at<double>(1,0),
		persL.at<double>(2,1)*Lpt.y - persL.at<double>(1,1),
		persL.at<double>(2,2)*Lpt.y - persL.at<double>(1,2),
		
		persR.at<double>(2,0)*Rpt.x - persR.at<double>(0,0),
		persR.at<double>(2,1)*Rpt.x - persR.at<double>(0,1),
		persR.at<double>(2,2)*Rpt.x - persR.at<double>(0,2),
		
		persR.at<double>(2,0)*Rpt.y - persR.at<double>(1,0),
		persR.at<double>(2,1)*Rpt.y - persR.at<double>(1,1),
		persR.at<double>(2,2)*Rpt.y - persR.at<double>(1,2)
		);

	//std::cout << Bmat << std::endl;

	// �W���s�� (�_��P47 ��6.23 �E��)
	cv::Mat Cmat = (cv::Mat_<double>(4,1) << 
		persL.at<double>(0,3) - persL.at<double>(2,3)*Lpt.x,
		persL.at<double>(1,3) - persL.at<double>(2,3)*Lpt.y,
		persR.at<double>(0,3) - persR.at<double>(2,3)*Rpt.x,
		persR.at<double>(1,3) - persR.at<double>(2,3)*Rpt.y);


	// �t�s��@(�_��P47 ��6.24)
	cv::Mat Binv = cv::Mat(3,4,CV_64FC1);
	Binv = (Bmat.t()*Bmat).inv()*Bmat.t();
	
	//std::cout << Binv << std::endl;
	//std::cout << Cmat << std::endl;

	// ���[���h���W�n
	cv::Mat Wmat = cv::Mat(3,1,CV_64FC1);
	Wmat = Binv*Cmat;
	
	//std::cout << Wmat << std::endl;
	
	// ���W�̔��]������
	cv::Point3f wpt;
	wpt.x = -Wmat.at<double>(0,0);
	wpt.y = -Wmat.at<double>(1,0);
	wpt.z = Wmat.at<double>(2,0);

	return wpt;
}

void StereoCamera::getCameraParam(cv::Mat &param,std::string paramName,std::string xmlName){
 
	// �ǂݍ��݃t�@�C���̍쐬��
	// main.cpp : main_calibrate�֐�

    cv::FileStorage cvfs(xmlName, CV_STORAGE_READ);
    cv::FileNode node(cvfs.fs, NULL);

    cv::read(node[paramName], param);

}