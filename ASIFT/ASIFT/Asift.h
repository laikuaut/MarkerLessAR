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
#include "../../MyLibs/Core/Timer.h"

#include "AsiftKeypoints.h"
#include "AsiftMatchings.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>


class PRO_EXPORTS Asift
{
public:

	pro::Dir path;
	static const int IMAGE_X = 800;
	static const int IMAGE_Y = 600;

private:

	// ���摜�t�@�C����
	std::string img1Name,img2Name;
	// �o�͉摜�t�@�C����(V:���������摜,���������摜)
	std::string imgVName,imgHName;
	// �L�[�|�C���g�ۑ��t�@�C����
	std::string keys1Name,keys2Name;
	// �}�b�`���O�ۑ��t�@�C����
	std::string matchingsName;

	// ���摜
	pro::Image img1,img2;
	// ���̃O���[�X�P�[���摜
	pro::Image img1Gray,img2Gray;
	// ���T�C�Y��̉摜
	pro::Image img1Zoom,img2Zoom;
	// ���T�C�Y��̃O���C�X�P�[���摜
	pro::Image img1GrayZoom,img2GrayZoom;
	// ���摜�̃s�N�Z���f�[�^
	std::vector<float> ipixels1,ipixels2;
	// ���T�C�Y��̃s�N�Z���f�[�^
	std::vector<float> ipixels1Zoom,ipixels2Zoom;

	// ���T�C�Y��
	float zoom1,zoom2;
	// ���T�C�Y���邩�̃t���O
	int resizeFlag;

	// �L�[�|�C���g�f�[�^
	AsiftKeypoints keys1,keys2;
	// �}�b�`���O�f�[�^
	AsiftMatchings matchings;

	// ���b�Z�[�W�\���H
	int verb;
	// �摜�����̋󔒕�
	int bandWidth;

	// keys1 ���t�@�C������ǂݍ��ނ��ǂ���
	int keys1InputFlag;
	// ����ǂݍ��ݏ����̃t���O
	int videoInputFlag;

public:


	// �R���X�g���N�^
	Asift(void);
	// �f�X�g���N�^
	~Asift(void);

	// �������֐�
	void init(int readini,string img1_name="imgIn1.png",string img2_name="imgIn2.png",
				string imgV_name="imgOutVert.png",string imgH_name="imgOutHori.png",
				string keys1_name="keys1.txt",string keys2_name="keys2.txt",
				string matchings_name="matchings.txt",
				int tilt1=7,int tilt2=7,int resize_flag=1);

	// Asift.ini�̍쐬�֐�
	void writeIni();
	void readIni();

	// ���s�֐�
	void run();

private:

	void resize(pro::Image &src,pro::Image &dst,float &zoom,int resizeFlag);

	void createVertImage(pro::Image img);
	pro::Image createHoriImage(pro::Image img);

};
	//pro::Image frame;

	//while(1){
	//	cap.read((cv::Mat&)frame);

	//	img2Gray.grayeScale(frame);
	//	resize(img2Gray,img2GrayZoom,zoom2,0);
	//	unsigned char * iarr2 = img2Gray.getU8Data();
	//	ipixels2Zoom = std::vector<float>(iarr2,iarr2 + img2GrayZoom.size().width * img2GrayZoom.size().height);
	
	//	frame.imshow("Capture",1);
	//	if(cv::waitKey(30) >= 0) break;
	//	delete[] iarr2;
	//}