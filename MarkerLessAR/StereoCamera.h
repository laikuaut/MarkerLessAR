// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#pragma once

/**
 * ���s�X�e���I�@�N���X �_��P45
 */

#include "../MyLibs/Core/type_c.h"
#include "../MyLibs/OpenCVLibs/OpenCVLibs.h"

class StereoCamera
{
private:

public:

	/**
	 * �J�����̓����p�����[�^�s�� �_�� P42
	 */
	cv::Mat cameraParamL; // ���J�����̃p�����[�^
	cv::Mat cameraParamR; // �E�J�����̃p�����[�^

	// �J�����ԊԊu[mm]
	float camBetween;

	// �摜�c����
	int width; // ����
	int height; // �c��

public:
	StereoCamera(void);
	~StereoCamera(void);

	/**
	 * �X�e���I�@�N���X�̏�����
	 * @param xmlName �����p�����[�^��ۑ����Ă���xml�t�@�C����
	 * @param camBetween �J�����Ԃ̋���[mm]
	 * @param width �摜���� 
	 * @param height �摜�c�� 
	 */
	void init(std::string xmlName,float camBetween,int width,int height);

	/**
	 * ���[���h���W�n���擾����B(�J����L�������[���h���W�ƂȂ�)
	 * @param Lpt ���摜�̑Ή��_
	 * @param Rpt �E�摜�̑Ή��_
	 * @return 3�������W
	 */
	cv::Point3f getWorldPoint(cv::Point2f Lpt,cv::Point2f Rpt);

private:

	/**
	 * �����p�����[�^�ǂݍ��݊֐�
	 * @param param �J�����̓����p�����[�^�s��
	 * @param paramName �J������
	 * @param xmlName �����p�����[�^��ۑ����Ă���xml�t�@�C����
	 */
	void getCameraParam(cv::Mat &param,std::string paramName,std::string xmlName);

};

