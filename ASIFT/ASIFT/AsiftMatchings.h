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
	// マッチングしたキーポイントのみを残す
	void filterKeysMatchings(vector< vector< keypointslist > > &keys,int first);

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

	void output(string name);
	void input(string name);

};

