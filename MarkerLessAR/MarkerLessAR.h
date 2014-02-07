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

	// �ꎞ�I�Ȏ��s���x�p�ϐ�
	pro::Timer timer;
	
	/********************************************
	 * �ݒ�t�@�C����
	 */
	// MarkerLessAR ini File Name
	std::string iniAsiftFileName;

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
	 * �����ˉe�s��@�_��P51 
	 */
	double persAu;
	double persAv;
	double persU0;
	double persV0;
	double persN;
	double persF;
	double persW;
	double persH;
	double persL;
	double persR;
	double persT;
	double persB;

private:

	/********************************************
	 * �听�����͂ɂ��}�[�J�ʒu�E�p���̌���
	 */
	// X���̐ݒ�
	int setXAxis();
	// Y���̐ݒ�
	void setYAxis();
	// Z���̐ݒ�
	void setZAxis();
	// �d�S�̐ݒ�
	void setCenterAxis();
	// �}�[�J�̈ʒu�p�����t�@�C���ɏo��
	void outputAxis();

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
	void init(int readini,string imgLeftName,string imgRightName,string ini_name="MarkerLessAR.ini");
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
	vector<cv::Point3f> inputWorldPoints();
	vector<cv::Point3f> inputXAxisWorldPoints();
	void outputWorldPoints();

	/********************************************
	 * �听�����͂ɂ��}�[�J�ʒu�E�p���̌���
	 * @see setXAxis
	 * @see setYAxis
	 * @see setZAxis
	 * @see setCenterAxis
	 * @see outputAxis
	 */
	int setAxis();

	/********************************************
	 * �����ˉe�s��v�Z �_��P51 
	 */
	void setPersMat();

	/********************************************
	 * ���s�֘A
	 */
	// ����s
	void run();
	
	/********************************************
	 * �v���p�e�B�֘A
	 */
	// ���摜�̖��O�擾
	string getLeftImgName();
};

