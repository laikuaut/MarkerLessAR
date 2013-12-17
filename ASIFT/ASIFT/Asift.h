// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "demo_lib_sift.h"
#include "io_png.h"

#include "library.h"
#include "frot.h"
#include "fproj.h"
#include "compute_asift_keypoints.h"
#include "compute_asift_matches.h"

#include "../../MyLibs/OpenCVLibs/Image.h"
#include "../../MyLibs/OpenCVLibs/MouseEvent.h"
#include "../../MyLibs/Core/Timer.h"

#include "AsiftKeypoints.h"
#include "AsiftMatchings.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>


class PRO_EXPORTS Asift : public pro::MouseEvent
{
public:

	// �J�����g�p�X
	pro::Dir path;
	
public:

	// ���T�C�Y�œK���T�C�Y
	static const int IMAGE_X = 800;
	static const int IMAGE_Y = 600;

	// �摜ID
	static const int IMAGE_ID_BASE = 1;
	static const int IMAGE_ID_INPUT = 2;
	static const int IMAGE_ID_ELSE = 3;

	// �}�E�X�C�x���gID
	static const int ON_MOUSE_ID_FILTER_RECT = 1;

	// �t�B���^�[Flag
	static const int FILTER_RECT_FLAG = 1;
	static const int FILTER_CENTER_LINE_FLAG = 2;

private:
	/********************************************
	 * �ݒ�t�@�C����
	 */
	// Asift�̐ݒ�t�@�C��
	//std::string iniFileName;
	// sift-paramerter�̐ݒ�t�@�C��
	std::string iniSiftParamName;

	/********************************************
	 * �t�@�C����
	 */
	// ���摜�t�@�C����
	std::string imgBaseName,imgInputName;
	// �o�͉摜�t�@�C����(V:���������摜,���������摜)
	std::string imgVName,imgHName;
	// ���͓��於,�o�͓��於
	std::string capInName,capOutName;
	// �L�[�|�C���g�ۑ��t�@�C����
	std::string baseKeysName,inputKeysName;
	// �}�b�`���O�ۑ��t�@�C����
	std::string matchingsName;
	// X���t�@�C����
	std::string xAxisKeysName;

	/********************************************
	 * �f�B���N�g����
	 */
	// �r�f�I�t���[�����Ƃ̃A�E�g�v�b�g�f�B���N�g��
	pro::Dir videoOutputDir;

	/********************************************
	 * �摜�f�[�^
	 */
	//// ���̃O���[�X�P�[���摜
	//pro::Image imgBaseGray,imgInputGray;
	//// ���T�C�Y��̉摜
	//pro::Image imgBaseZoom,imgInputZoom;
	//// ���T�C�Y��̃O���C�X�P�[���摜
	//pro::Image imgBaseGrayZoom,imgInputGrayZoom;
	//// ���摜�̃s�N�Z���f�[�^
	//std::vector<float> ipixels1,ipixels2;
	//// ���T�C�Y��̃s�N�Z���f�[�^
	//std::vector<float> ipixelsBase,ipixelsInput;

	/********************************************
	 * ���T�C�Y�֘A
	 */
	// ���T�C�Y��
	float baseZoom,inputZoom;
	// ���T�C�Y���邩�̃t���O
	int resizeFlag;
	// ���T�C�Y�T�C�Y
	int resizeWidth,resizeHeight;

	/********************************************
	 * �f�[�^�\���֘A
	 */
	// ���b�Z�[�W�\���H
	int verb;
	// �摜�����̋󔒕�
	int bandWidth;

	/********************************************
	 * �������ʃI�v�V����
	 */
	// baseKeys ���t�@�C������ǂݍ��ނ��ǂ���
	int baseKeysInputFlag;
	// ����ǂݍ��ݏ����̃t���O
	int videoInputFlag;
	// ���搶������InputKeypoints�����ׂăt�@�C���o�͂��邩�ǂ����̃t���O
	int videoKeypointsOutputFlag;
	// SIFT�Ō��o�����L�[�|�C���g���h�b�g�ŉ摜�ɏo��
	int siftDotPrintFlag;
	// �}�E�X����̋�`�t�B���^�[�����邩������
	int filterRectOnMouseFlag;

	/********************************************
	 * �}�E�X�C�x���g�֘A
	 */
	// �}�E�X�C�x���g�𔻒肷��ID
	int onMouseId;
	// �}�E�X�C�x���g�̏����J�E���g
	int onMouseCount;

	/********************************************
	 * �t�B���^�֘A
	 */
	// base�摜�̃L�[�|�C���g��`�t�B���^�[�p
	 cv::Rect filterRect;
	 // �t�B���^�[Flag
	 int filterFlag;

	/********************************************
	 * ���֘A
	 */
	 // X�������߂邽�߂̃L�[�|�C���g���_
	 AsiftKeypoints xAxis;
	 // �Z���^�[�|�C���g
	 cv::Point2f centerPt;
	 // �Z���^�[����̋���
	 float centerLineDistance;

public:
	
	// ���摜
	pro::Image imgBase,imgInput;
	// �L�[�|�C���g�f�[�^
	AsiftKeypoints baseKeys,inputKeys;
	// �}�b�`���O�f�[�^
	AsiftMatchings matchings;

public:

	// �R���X�g���N�^
	Asift(void);
	Asift(string ini_name,string sift_param_ini_name="SiftParam.ini");
	// �f�X�g���N�^
	~Asift(void);

	/********************************************
	 * �������֐�
	 */
	// �������֐�
	void init(int readini,string ini_name="Asift.ini",
				string sift_param_ini_name="SiftParam.ini");
				/*string imgBase_name="imgIn1.png",
				string imgInput_name="imgIn2.png",
				string imgV_name="imgOutVert.png",
				string imgH_name="imgOutHori.png",
				string baseKeys_name="baseKeys.txt",
				string inputKeys_name="inputKeys.txt",
				string matchings_name="matchings.txt",
				string capIn_name="capin.avi",
				string capOut_name="capout.avi",
				int tilt1=7,int tilt2=7,int resize_flag=1);*/
	// �f�t�H���g�l�ݒ�
	void defaultParam();
	// �摜�ǂݍ��ݏ�����
	void initImages();

	/********************************************
	 * Asift.ini�̍쐬�֐�
	 */
	void writeIni(ptree &pt);
	void readIni(ptree &pt);
	void writeIni(std::string ini_file_name);
	void readIni(std::string ini_file_name);

	/********************************************
	 * ���s����
	 */
	// ��{���s�֐�
	void run();
	// �}�[�J�[�쐬
	void markerCreate(std::string markerName,int tilts,int rectFilterFlag=1,int imageShow=1);

	/********************************************
	 * �v���p�e�B
	 */
	// �摜�̐ݒu
	void setImage(pro::Image &src,int id,AsiftKeypoints &keys=AsiftKeypoints());

	/********************************************
	 * �v�Z����
	 */
	// �L�[�|�C���g�̎Z�o
	void computeKeyPoints(int id,AsiftKeypoints &keys=AsiftKeypoints());
	// �}�b�`���O����
	void computeMatching(AsiftKeypoints keys1,AsiftKeypoints keys2);

	/********************************************
	 * �t�B���^����
	 */
	// ��`�t�B���^�[����
	void setFilterRectanglePoints(pro::Image &src);
	// ���S���W���擾����B
	cv::Point2f getCenterPoint(cv::Point2f pt1,cv::Point2f pt2);
	// �Z���^�[���C���t�߂ɂ�������_�̒��o
	void setCenterLinePoints(AsiftKeypoints &keys);
	// �t�B���^�[���s
	void fileterRun(AsiftKeypoints &keys);

private:
	

	// �}�E�X�C�x���gID��ݒ�
	void setMouseEventId(int id);
	// �}�E�X�C�x���g�x�[�X
	void onMouse_impl(int event,int x,int y,int flag);
	// ��`�t�B���^�[�̃}�E�X�C�x���g
	void onMouse_filterRect(int event,int x,int y,int flag);

	// asift�̃��T�C�Y����
	//void resize(pro::Image &src,pro::Image &dst,float &zoom,int resizeFlag);

	// �����摜�����ƃ}�b�`���O���C���`��
	pro::Image createVertImage(pro::Image img);
	// �����摜�����ƃ}�b�`���O���C���`��
	pro::Image createHoriImage(pro::Image img);

};
