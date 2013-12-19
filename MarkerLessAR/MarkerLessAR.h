// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#pragma once

#include "../ASIFT/ASIFT/Asift.h"

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
	AsiftMatchings matchingLR;

	/********************************************
	 * ���[���h���W
	 */
	// ���[���h���W�n�L�[�|�C���g
	AsiftKeypoints worldKeypoints;

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

	/********************************************
	 * ini�t�@�C���֘A
	 */
	// 
	void readIni(ptree &pt);
	void writeIni(ptree &pt);
	void readIni(std::string name);
	void writeIni(std::string name);

	/********************************************
	 * ���s�֘A
	 */
	// ����s
	void run();

};

