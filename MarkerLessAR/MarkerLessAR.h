// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#pragma once

#include "../ASIFT/ASIFT/Asift.h"

class MarkerLessAR
{
public:

	pro::Dir path;

private:
	
	/********************************************
	 * 設定ファイル名
	 */
	// MarkerLessAR ini File Name
	//std::string iniFileName;
	// Asift Left side ini File Name
	std::string iniAsiftLeftName;
	// Asift Right side ini File Name
	std::string iniAsiftRightName;

	/********************************************
	 * ステレオカメラ
	 */
	// camera between distance (mm)
	int cameraBetween;

public:

	/********************************************
	 * Asift
	 */
	// 左側のAsift
	Asift asiftLeft;
	// 右側のLeft
	Asift asiftRight;

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
	// デフォルト値設定
	void defaultParam();

	/********************************************
	 * iniファイル関連
	 */
	// 
	void readIni(ptree &pt);
	void writeIni(ptree &pt);
	void readIni(std::string name);
	void writeIni(std::string name);

	/********************************************
	 * 実行関連
	 */
	// 主実行
	void run();

};

