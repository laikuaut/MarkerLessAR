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
	 * ���g�p
	 */
	void filterKeysMatchings(keypointslist &keys,int first);
	// �}�b�`���O�����L�[�|�C���g�݂̂��c���i���s�j
	// void filterKeysMatchings(vector< vector< keypointslist > > &keys,int first);

public:

	AsiftMatchings(void);
	AsiftMatchings(AsiftKeypoints keys1,AsiftKeypoints keys2);
	~AsiftMatchings(void);

	// �L�[�|�C���g�̃Z�b�g
	void setKeypoints(AsiftKeypoints keys1,AsiftKeypoints keys2);

	// �}�b�`���O�����L�[�|�C���g�݂̂ɂ���
	void filterMatching();

	// �}�b�`���O�G�Z
	int computeAsiftMatches(int verb);
	// �}�b�`���O���擾
	int getNum() const;

	// ���o�͏���
	void output(string name);
	void input(string name);

	// �����摜�̍쐬
	pro::Image drawHoriImage(pro::Image first,pro::Image second,int bandWidth,std::string name);

	// �����摜�̍쐬
	pro::Image drawVertImage(pro::Image first,pro::Image second,int bandWidth,std::string name);

};

