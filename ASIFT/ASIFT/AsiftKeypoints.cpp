#include "AsiftKeypoints.h"


AsiftKeypoints::AsiftKeypoints(int tilts)
{

	this->tilts = tilts;
	default_sift_parameters(siftparams);
	num = 0;
	w = h = 0;
}


AsiftKeypoints::~AsiftKeypoints(void)
{
}

int AsiftKeypoints::computeAsiftKeyPoints(vector<float>& image, int width, int height, int verb)
{
	w = width,h = height;
	num = compute_asift_keypoints(image,width,height,tilts,verb,keys,siftparams);
	return num;
}

int AsiftKeypoints::getNum() const{
	return num;
}