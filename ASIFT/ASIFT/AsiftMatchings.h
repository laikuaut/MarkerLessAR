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
	AsiftKeypoints *asiftKeys1;
	AsiftKeypoints *asiftKeys2;
	siftPar siftparams;

public:

	matchingslist matchings;

	pro::Dir path;

public:

	AsiftMatchings(void);
	AsiftMatchings(AsiftKeypoints keys1,AsiftKeypoints keys2);
	~AsiftMatchings(void);

	void setKeypoints(AsiftKeypoints keys1,AsiftKeypoints keys2);

	// �}�b�`���O�G�Z
	int computeAsiftMatches(int verb);
	// �}�b�`���O���擾
	int getNum() const;

	void output(string name);
	void input(string name);

};

