#include "AsiftMatchings.h"


AsiftMatchings::AsiftMatchings(void)
{
	path = pro::Dir();
	default_sift_parameters(siftparams);
}


AsiftMatchings::AsiftMatchings(AsiftKeypoints keys1,AsiftKeypoints keys2)
{
	path = pro::Dir();
	setKeypoints(keys1,keys2);
}


AsiftMatchings::~AsiftMatchings(void)
{
}

void AsiftMatchings::setKeypoints(AsiftKeypoints keys1,AsiftKeypoints keys2){
	this->asiftKeys1 = new AsiftKeypoints(keys1);
	this->asiftKeys2 = new AsiftKeypoints(keys2);
	siftparams = keys2.siftparams;
}


int AsiftMatchings::computeAsiftMatches(int verb){
	matchings.clear();
	num = compute_asift_matches(asiftKeys1->tilts,asiftKeys2->tilts,asiftKeys1->w,asiftKeys1->h,asiftKeys2->w,asiftKeys2->h
			,verb,asiftKeys1->keys,asiftKeys2->keys,matchings,siftparams);
	return num;
}

int AsiftMatchings::getNum() const{
	return num;
}

void AsiftMatchings::output(string name){
	
	std::ofstream of;

	of.open(path.pwd(name));

	if (of.is_open())
	{		
		// Write the number of matchings in the first line
		of << getNum() << std::endl;
		
		matchingslist::iterator ptr = matchings.begin();
		for(int i=0; i < (int) matchings.size(); i++, ptr++)		
		{
			// std::pair‚Åˆ—
			of << asiftKeys1->zoom*ptr->first.x << "  " << asiftKeys1->zoom*ptr->first.y << "  " <<  asiftKeys2->zoom*ptr->second.x << 
			"  " <<  asiftKeys2->zoom*ptr->second.y << std::endl;
		}		
	}
	else 
	{
		std::cerr << "Unable to open the file matchings."; 
	}

	of.close();
}