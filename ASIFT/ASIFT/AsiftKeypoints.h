#pragma once


#include "demo_lib_sift.h"
#include "compute_asift_keypoints.h"
#include "AsiftMatchings.h"

class AsiftMatchings;

class AsiftKeypoints
{

	friend class AsiftMatchings;

private:

	int tilts;
	siftPar siftparams;
	int num;
	int w,h;

public:

	vector< vector< keypointslist > > keys;

public:
	AsiftKeypoints(int tilts = 1);
	~AsiftKeypoints(void);

	int computeAsiftKeyPoints(vector<float>& image, int width, int height, int verb);
	int getNum() const;

};

