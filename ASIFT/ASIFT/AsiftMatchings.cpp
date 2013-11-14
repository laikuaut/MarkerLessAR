#include "AsiftMatchings.h"


AsiftMatchings::AsiftMatchings(void)
{
	default_sift_parameters(siftparams);
}


AsiftMatchings::AsiftMatchings(AsiftKeypoints keys1,AsiftKeypoints keys2)
{
	this->asiftKeys1 = new AsiftKeypoints(keys1);
	this->asiftKeys2 = new AsiftKeypoints(keys2);
	default_sift_parameters(siftparams);
}


AsiftMatchings::~AsiftMatchings(void)
{
}

int AsiftMatchings::computeAsiftMatches(int verb){
	num = compute_asift_matches(asiftKeys1->tilts,asiftKeys2->tilts,asiftKeys1->w,asiftKeys1->h,asiftKeys2->w,asiftKeys2->h
			,verb,asiftKeys1->keys,asiftKeys2->keys,matchings,siftparams);
	return num;
}

int AsiftMatchings::getNum() const{
	return num;
}