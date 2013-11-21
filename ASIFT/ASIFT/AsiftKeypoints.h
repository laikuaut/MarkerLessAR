#pragma once


#include "demo_lib_sift.h"
#include "compute_asift_keypoints.h"
#include "AsiftMatchings.h"

#include "../../MyLibs/Core/Dir.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>
using namespace boost::property_tree;

class AsiftMatchings;

class AsiftKeypoints
{

	friend class AsiftMatchings;

private:

	int tilts;
	siftPar siftparams;
	int num;
	int w,h;
	float zoom;

public:

	vector< vector< keypointslist > > keys;

	pro::Dir path;

public:
	AsiftKeypoints(int tilts = 1);
	~AsiftKeypoints(void);

	int computeAsiftKeyPoints(vector<float>& image, int width, int height, int verb,float zoom);
	int getNum() const;
	
	void inireadSiftParameters();
	void iniwriteSiftParameters();
	void iniwriteSiftParameters(siftPar par);

	void output(string name);
	void input(string name);

};

