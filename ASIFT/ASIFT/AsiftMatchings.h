// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#pragma once

#include "AsiftKeypoints.h"
#include "demo_lib_sift.h"
#include "compute_asift_matches.h"

#include "../../MyLibs/Core/Dir.h"
#include "../../MyLibs/OpenCVLibs/Image.h"

class AsiftKeypoints;

class PRO_EXPORTS AsiftMatchings
{
private:

	int num;
	siftPar siftparams;

public:
	
	AsiftKeypoints *asiftKeys1;
	AsiftKeypoints *asiftKeys2;
	matchingslist matchings;

	pro::Dir path;

private:

	//void filterKeysMatchings();

	/**
	 * 未使用
	 */
	void filterKeysMatchings(keypointslist &keys,int first);
	// マッチングしたキーポイントのみを残す（失敗）
	// void filterKeysMatchings(vector< vector< keypointslist > > &keys,int first);

public:

	AsiftMatchings(void);
	AsiftMatchings(AsiftKeypoints keys1,AsiftKeypoints keys2);
	~AsiftMatchings(void);

	// キーポイントのセット
	void setKeypoints(AsiftKeypoints keys1,AsiftKeypoints keys2);

	// マッチングしたキーポイントのみにする
	void filterMatching();

	// マッチング絵算
	int computeAsiftMatches(int verb);
	// マッチング数取得
	int getNum() const;

	// 入出力処理
	void output(string name);
	void input(string name);

	// 水平画像の作成
	pro::Image drawHoriImage(pro::Image first,pro::Image second,int bandWidth,std::string name);

	// 垂直画像の作成
	pro::Image drawVertImage(pro::Image first,pro::Image second,int bandWidth,std::string name);

};

