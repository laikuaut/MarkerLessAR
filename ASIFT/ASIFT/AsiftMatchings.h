#pragma once

#include "AsiftKeypoints.h"
#include "demo_lib_sift.h"
#include "compute_asift_matches.h"

class AsiftKeypoints;

class AsiftMatchings
{
private:

	int num;
	AsiftKeypoints *asiftKeys1;
	AsiftKeypoints *asiftKeys2;
	siftPar siftparams;

public:

	matchingslist matchings;

public:

	AsiftMatchings(void);
	AsiftMatchings(AsiftKeypoints keys1,AsiftKeypoints keys2);
	~AsiftMatchings(void);

	int computeAsiftMatches(int verb);
	int getNum() const;

};

