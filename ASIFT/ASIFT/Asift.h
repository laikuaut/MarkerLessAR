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

	// カレントパス
	pro::Dir path;
	
public:

	// リサイズ最適化サイズ
	static const int IMAGE_X = 800;
	static const int IMAGE_Y = 600;

	// 画像ID
	static const int IMAGE_ID_BASE = 1;
	static const int IMAGE_ID_INPUT = 2;
	static const int IMAGE_ID_ELSE = 3;

	// 読み込み処理ID
	static const int INPUT_ID_KEYS_BASE = 1;
	static const int INPUT_ID_KEYS_XAXIS = 2;

	// 書き込み処理ID
	static const int OUTPUT_ID_KEYS_BASE = 1;
	static const int OUTPUT_ID_KEYS_INPUT = 2;
	static const int OUTPUT_ID_MATCHING = 3;
	static const int OUTPUT_ID_VERT = 4;
	static const int OUTPUT_ID_HORI = 5;
	static const int OUTPUT_ID_KEYS_XAXIS = 6;

	// マウスイベントID
	static const int ON_MOUSE_ID_FILTER_RECT = 1;
	static const int ON_MOUSE_ID_FILTER_DEL_RECT = 2;

	// フィルターFlag
	static const int FILTER_RECT_FLAG = 1;
	static const int FILTER_CENTER_LINE_FLAG = 2;
	static const int FILTER_RECT_DEL_FLAG = 4;

private:
	/********************************************
	 * 設定ファイル名
	 */
	// Asiftの設定ファイル
	//std::string iniFileName;
	// sift-paramerterの設定ファイル
	std::string iniSiftParamName;

	/********************************************
	 * ファイル名
	 */
	// 元画像ファイル名
	std::string imgBaseName,imgInputName;
	// 出力画像ファイル名(V:垂直結合画像,水平結合画像)
	std::string imgVName,imgHName;
	// 入力動画名,出力動画名
	std::string capInName;
	std::string capVName,capHName;
	// キーポイント保存ファイル名
	std::string baseKeysName,inputKeysName;
	// マッチング保存ファイル名
	std::string matchingsName;
	// X軸ファイル名
	std::string xAxisKeysName;
	
	/********************************************
	 * 名前
	 */
	// ベース名
	std::string baseName;
	// インプット名
	std::string inputName;
	// ベース拡張子
	std::string baseExtention;
	// インプット拡張子
	std::string inputExtention;
	/********************************************
	 * ディレクトリ名
	 */
	// ビデオフレームごとのアウトプットディレクトリ
	pro::Dir videoOutputDir;

	/********************************************
	 * 画像データ
	 */
	//// 元のグレースケール画像
	//pro::Image imgBaseGray,imgInputGray;
	//// リサイズ後の画像
	//pro::Image imgBaseZoom,imgInputZoom;
	//// リサイズ後のグレイスケール画像
	//pro::Image imgBaseGrayZoom,imgInputGrayZoom;
	//// 元画像のピクセルデータ
	//std::vector<float> ipixels1,ipixels2;
	//// リサイズ後のピクセルデータ
	//std::vector<float> ipixelsBase,ipixelsInput;

	/********************************************
	 * リサイズ関連
	 */
	// リサイズ比
	float baseZoom,inputZoom;
	// リサイズするかのフラグ
	int resizeFlag;
	// リサイズサイズ
	int resizeWidth,resizeHeight;

	/********************************************
	 * データ表示関連
	 */
	// メッセージ表示？
	int verb;
	// 画像結合の空白幅
	int bandWidth;

	/********************************************
	 * 処理判別オプション
	 */
	// baseKeys をファイルから読み込むかどうか
	int baseKeysInputFlag;
	// 動画読み込み処理のフラ
	int videoInputFlag;
	// 動画生成時のInputKeypointsをすべてファイル出力するかどうかのフラグ
	int videoKeypointsOutputFlag;
	// SIFTで検出したキーポイントをドットで画像に出力
	int siftDotPrintFlag;
	// マウス操作の矩形フィルターをするかそうか
	int filterRectOnMouseFlag;

	/********************************************
	 * マウスイベント関連
	 */
	// マウスイベントを判定するID
	int onMouseId;
	// マウスイベントの処理カウント
	int onMouseCount;

	/********************************************
	 * フィルタ関連
	 */
	// base画像のキーポイント矩形フィルター用
	 cv::Rect filterRect;
	 // 削除矩形フィルター用
	 cv::Rect *filterDelRect;
	 int filterDelRectNum;
	 // フィルターFlag
	 int filterFlag;

	/********************************************
	 * 軸関連
	 */
	 // センターポイント
	 cv::Point2f centerPt;
	 // センターからの距離
	 float centerLineDistance;

public:
	
	// 元画像
	pro::Image imgBase,imgInput;
	// キーポイントデータ
	AsiftKeypoints baseKeys,inputKeys;
	// マッチングデータ
	AsiftMatchings matchings;
	// X軸を求めるためのキーポイント候補点
	AsiftKeypoints xAxisKeys;

public:

	// コンストラクタ
	Asift(void);
	Asift(string ini_name,string sift_param_ini_name="SiftParam.ini");
	// デストラクタ
	~Asift(void);

	/********************************************
	 * 初期化関数
	 */
	// 初期化関数
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

	// デフォルト値設定
	void defaultParam();
	// 画像読み込み初期化
	void initImages();
	// 出力名を設定する
	void initNames();
	// キーポイント初期化
	void initKeys(int id,int tilts);

	/********************************************
	 * Asift.iniの作成関数
	 */
	void writeIni(ptree &pt);
	void readIni(ptree &pt);
	void writeIni(std::string ini_file_name);
	void readIni(std::string ini_file_name);

	/********************************************
	 * 実行処理
	 */
	// 基本実行関数
	void run();
	// 引数から画像名とパラメータを設定し実行
	void run(std::string baseImg,std::string inputImg,int tilt1,int tilt2);
	// マーカー作成
	void markerCreate(std::string markerName,int tilts,int rectFilterFlag=1,int imageShow=1);


	/********************************************
	 * プロパティ
	 */
	// 画像の設置
	void setImage(pro::Image &src,int id,AsiftKeypoints &keys=AsiftKeypoints());
	// 名前を設置
	void setNames(std::string imgBaseName,std::string imgInputName,std::string videoInputName);

	/********************************************
	 * 計算処理
	 */
	// キーポイントの算出
	void computeKeyPoints(int id,AsiftKeypoints &keys=AsiftKeypoints());
	// マッチング処理
	void computeMatching(AsiftKeypoints keys1,AsiftKeypoints keys2);

	/********************************************
	 * フィルタ処理
	 */
	// 矩形フィルター処理
	void setFilterRect(pro::Image &src);
	// 矩形削除フィルター処理
	void setFilterDelRect(pro::Image &src);
	// 中心座標を取得する。
	cv::Point2f getCenterPoint(cv::Point2f pt1,cv::Point2f pt2);
	// センターライン付近にある特徴点の抽出
	void setCenterLinePoints(AsiftKeypoints &keys);
	// フィルター実行
	void fileterRun(AsiftKeypoints &keys);

	/********************************************
	 * 入出力処理
	 */
	void input(int id);
	void output(int id,pro::Image src=pro::Image());
	
	// 垂直画像結合とマッチングライン描写
	pro::Image createVertImage(pro::Image img,AsiftMatchings matchings,std::string name);
	// 水平画像結合とマッチングライン描写
	pro::Image createHoriImage(pro::Image img,AsiftMatchings matchings,std::string name);

private:

	// マウスイベントIDを設定
	void setMouseEventId(int id);
	// マウスイベントベース
	void onMouse_impl(int event,int x,int y,int flag);
	// 矩形フィルターのマウスイベント
	void onMouse_filterRect(int event,int x,int y,int flag);
	// 削除矩形フィルターのマウスイベント
	void onMouse_filterDelRect(int event,int x,int y,int flag);

	// asiftのリサイズ処理
	//void resize(pro::Image &src,pro::Image &dst,float &zoom,int resizeFlag);

	// 垂直画像結合とマッチングライン描写
	pro::Image createVertImage(pro::Image img);
	// 水平画像結合とマッチングライン描写
	pro::Image createHoriImage(pro::Image img);

};
