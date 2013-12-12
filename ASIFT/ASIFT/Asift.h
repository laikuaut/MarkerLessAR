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

	// 元画像ファイル名
	std::string img1Name,img2Name;
	// 出力画像ファイル名(V:垂直結合画像,水平結合画像)
	std::string imgVName,imgHName;
	// キーポイント保存ファイル名
	std::string keys1Name,keys2Name;
	// マッチング保存ファイル名
	std::string matchingsName;

	// 元画像
	pro::Image img1,img2;
	// 元のグレースケール画像
	pro::Image img1Gray,img2Gray;
	// リサイズ後の画像
	pro::Image img1Zoom,img2Zoom;
	// リサイズ後のグレイスケール画像
	pro::Image img1GrayZoom,img2GrayZoom;
	// 元画像のピクセルデータ
	std::vector<float> ipixels1,ipixels2;
	// リサイズ後のピクセルデータ
	std::vector<float> ipixels1Zoom,ipixels2Zoom;

	// リサイズ比
	float zoom1,zoom2;
	// リサイズするかのフラグ
	int resizeFlag;

	// キーポイントデータ
	AsiftKeypoints keys1,keys2;
	// マッチングデータ
	AsiftMatchings matchings;

	// メッセージ表示？
	int verb;
	// 画像結合の空白幅
	int bandWidth;

	// keys1 をファイルから読み込むかどうか
	int keys1InputFlag;
	// 動画読み込み処理のフラグ
	int videoInputFlag;

public:


	// コンストラクタ
	Asift(void);
	// デストラクタ
	~Asift(void);

	// 初期化関数
	void init(int readini,string img1_name="imgIn1.png",string img2_name="imgIn2.png",
				string imgV_name="imgOutVert.png",string imgH_name="imgOutHori.png",
				string keys1_name="keys1.txt",string keys2_name="keys2.txt",
				string matchings_name="matchings.txt",
				int tilt1=7,int tilt2=7,int resize_flag=1);

	// Asift.iniの作成関数
	void writeIni();
	void readIni();

	// 実行関数
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