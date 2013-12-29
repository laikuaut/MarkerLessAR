// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#pragma once

#include "../ASIFT/ASIFT/Asift.h"
#include "StereoCamera.h"

class Asift;

class PRO_EXPORTS MarkerLessAR
{

public:

	pro::Dir path;

private:
	
	/********************************************
	 * �ݒ�t�@�C����
	 */
	// MarkerLessAR ini File Name
	std::string iniAsiftFileName;
	// Asift Left side ini File Name
	//std::string iniAsiftLeftName;
	// Asift Right side ini File Name
	//std::string iniAsiftRightName;

	/********************************************
	 * �X�e���I�J����
	 */
	// camera between distance (mm)
	int cameraBetween;
	// ���J�����̔ԍ�
	int cameraLeftNum;
	// �E�J�����̔ԍ�
	int cameraRightNum;
	// ���J�����̖��O
	std::string cameraLeftName;
	// �E�J�����̖��O
	std::string cameraRightName;
	// ���J�����̓����p�����[�^
	cv::Mat cameraLeftParam;
	// �E�J�����̓����p�����[�^
	cv::Mat cameraRightParam;

	// �J�����L�����u���[�V����xml�t�@�C����
	std::string calibrateXmlName;
	// �X�e���I�J����
	StereoCamera stereo;

	/*******************************************
	 * �摜��
	 */
	// �}�[�J�[�摜��
	std::string imgMarkerName;
	// ���C���v�b�g�摜��
	std::string imgLeftName;
	// �E�C���v�b�g�摜��
	std::string imgRightName;

	/*******************************************
	 * ���於
	 */
	// ���C���v�b�g�摜��
	std::string videoLeftName;
	// �E�C���v�b�g�摜��
	std::string videoRightName;

public:

	/********************************************
	 * Asift
	 */
	// ������Asift
	Asift asiftLeft;
	// �E����Left
	Asift asiftRight;

	/********************************************
	 * �摜
	 */
	// �}�[�J�[�摜
	pro::Image imgMarker;
	// ���摜
	pro::Image imgLeft;
	// �E�摜
	pro::Image imgRight;

	/********************************************
	 * Asift�}�b�`���O
	 */
	// ���E�摜�̃}�b�`���O
	AsiftMatchings matchingsLR;
	// ���EAsift
	Asift asiftLR;

	/********************************************
	 * ���[���h���W
	 */
	// ���[���h���W�n�L�[�|�C���g
	//AsiftKeypoints worldKeypoints;
	vector<cv::Point3f> worldPoints;

	/********************************************
	 * ���̎擾
	 */
	cv::Vec3f xAxis;
	cv::Vec3f yAxis;
	cv::Vec3f zAxis;
	cv::Vec3f tAxis;

	/********************************************
	 * �����ˉe�s��
	 */
	double persL;
	double persR;
	double persT;
	double persB;
	double persN;
	double persF;

public:

	// �R���X�g���N�^
	MarkerLessAR(void);
	// �f�X�g���N�^
	~MarkerLessAR(void);

	/********************************************
	 * �������֘A
	 */
	// ������
	void init(int readini,string ini_name="MarkerLessAR.ini");
	// �f�t�H���g�l�ݒ�
	void defaultParam();
	// Asift�̉摜����MLAR�p�ɏ�����
	void initAsift();
	// �摜�ݒu
	void setImages();
	// �L�[�|�C���g�ݒu
	void setKeys();
	
	/********************************************
	 * ���Z����
	 */
	void computeKeys();
	void computeMatching();
	void computeXAxisMatching();

	/********************************************
	 * ini�t�@�C���֘A
	 */
	void readIni(ptree &pt);
	void writeIni(ptree &pt);
	void readIni(std::string name);
	void writeIni(std::string name);

	/********************************************
	 * ���o�͏���
	 */
	void outputPoint3s(vector<cv::Point3f> point3s,std::string name);
	vector<cv::Point3f> inputPoint3s(std::string name);
	
	/********************************************
	 * �}�b�`���O����
	 */
	matchingslist matchingLR(matchingslist matchingsL,matchingslist matchingsR);

	/********************************************
	 * ���[���h���W�n�̎擾
	 */
	vector<cv::Point3f> getWorldPoints(AsiftMatchings mathing);

	/********************************************
	 * �听�����͂ɂ�鎲�̌���
	 */
	void setXAxis();
	void setYAxis();
	void setZAxis();
	void setCenterAxis();
	void setAxis();
	void outputAxis();

	/********************************************
	 * �����ˉe�s��v�Z
	 */
	void setPersMat(double znear,double zfar);

	/********************************************
	 * ���s�֘A
	 */
	// ����s
	void run();

};

