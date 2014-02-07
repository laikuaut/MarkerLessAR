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

	// 一時的な実行速度用変数
	pro::Timer timer;
	
	/********************************************
	 * 設定ファイル名
	 */
	// MarkerLessAR ini File Name
	std::string iniAsiftFileName;

	/********************************************
	 * ステレオカメラ
	 */
	// camera between distance (mm)
	int cameraBetween;
	// 左カメラの番号
	int cameraLeftNum;
	// 右カメラの番号
	int cameraRightNum;
	// 左カメラの名前
	std::string cameraLeftName;
	// 右カメラの名前
	std::string cameraRightName;
	// 左カメラの内部パラメータ
	cv::Mat cameraLeftParam;
	// 右カメラの内部パラメータ
	cv::Mat cameraRightParam;
	
	// カメラキャリブレーションxmlファイル名
	std::string calibrateXmlName;
	// ステレオカメラ
	StereoCamera stereo;

	/*******************************************
	 * 画像名
	 */
	// マーカー画像名
	std::string imgMarkerName;
	// 左インプット画像名
	std::string imgLeftName;
	// 右インプット画像名
	std::string imgRightName;

	/*******************************************
	 * 動画名
	 */
	// 左インプット画像名
	std::string videoLeftName;
	// 右インプット画像名
	std::string videoRightName;

public:

	/********************************************
	 * Asift
	 */
	// 左側のAsift
	Asift asiftLeft;
	// 右側のLeft
	Asift asiftRight;

	/********************************************
	 * 画像
	 */
	// マーカー画像
	pro::Image imgMarker;
	// 左画像
	pro::Image imgLeft;
	// 右画像
	pro::Image imgRight;

	/********************************************
	 * Asiftマッチング
	 */
	// 左右画像のマッチング
	AsiftMatchings matchingsLR;
	// 左右Asift
	Asift asiftLR;

	/********************************************
	 * ワールド座標
	 */
	// ワールド座標系キーポイント
	//AsiftKeypoints worldKeypoints;
	vector<cv::Point3f> worldPoints;

	/********************************************
	 * 軸の取得 
	 */
	cv::Vec3f xAxis;
	cv::Vec3f yAxis;
	cv::Vec3f zAxis;
	cv::Vec3f tAxis;

	/********************************************
	 * 透視射影行列　論文P51 
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
	 * 主成分分析によるマーカ位置・姿勢の決定
	 */
	// X軸の設定
	int setXAxis();
	// Y軸の設定
	void setYAxis();
	// Z軸の設定
	void setZAxis();
	// 重心の設定
	void setCenterAxis();
	// マーカの位置姿勢をファイルに出力
	void outputAxis();

public:

	// コンストラクタ
	MarkerLessAR(void);
	// デストラクタ
	~MarkerLessAR(void);

	/********************************************
	 * 初期化関連
	 */
	// 初期化
	void init(int readini,string ini_name="MarkerLessAR.ini");
	void init(int readini,string imgLeftName,string imgRightName,string ini_name="MarkerLessAR.ini");
	// デフォルト値設定
	void defaultParam();
	// Asiftの画像名等MLAR用に初期化
	void initAsift();
	// 画像設置
	void setImages();
	// キーポイント設置
	void setKeys();
	
	/********************************************
	 * 演算処理
	 */
	void computeKeys();
	void computeMatching();
	void computeXAxisMatching();

	/********************************************
	 * iniファイル関連
	 */
	void readIni(ptree &pt);
	void writeIni(ptree &pt);
	void readIni(std::string name);
	void writeIni(std::string name);

	/********************************************
	 * 入出力処理
	 */
	void outputPoint3s(vector<cv::Point3f> point3s,std::string name);
	vector<cv::Point3f> inputPoint3s(std::string name);
	
	/********************************************
	 * マッチング処理
	 */
	matchingslist matchingLR(matchingslist matchingsL,matchingslist matchingsR);

	/********************************************
	 * ワールド座標系の取得
	 */
	vector<cv::Point3f> getWorldPoints(AsiftMatchings mathing);
	vector<cv::Point3f> inputWorldPoints();
	vector<cv::Point3f> inputXAxisWorldPoints();
	void outputWorldPoints();

	/********************************************
	 * 主成分分析によるマーカ位置・姿勢の決定
	 * @see setXAxis
	 * @see setYAxis
	 * @see setZAxis
	 * @see setCenterAxis
	 * @see outputAxis
	 */
	int setAxis();

	/********************************************
	 * 透視射影行列計算 論文P51 
	 */
	void setPersMat();

	/********************************************
	 * 実行関連
	 */
	// 主実行
	void run();
	
	/********************************************
	 * プロパティ関連
	 */
	// 左画像の名前取得
	string getLeftImgName();
};

