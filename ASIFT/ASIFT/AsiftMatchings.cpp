// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

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

void AsiftMatchings::filterKeysMatchings(vector< vector< keypointslist > > &keys,int first){
	
	vector< vector< keypointslist > > matchekeys;

	for (int tt = 0; tt < (int) keys.size(); tt++)
	{
		vector<keypointslist> keyslists;
		for (int rr = 0; rr < (int) keys[tt].size(); rr++)
		{
			//keypointslist::iterator ptr = keys[tt][rr].begin();
			keypointslist keylist;
			for(int i=0; i < (int) keys[tt][rr].size(); i++)	
			{
				matchingslist::iterator ptr = matchings.begin();
				for(int j=0; j < (int) matchings.size(); j++, ptr++)		
				{
					if(first == 1){
						if(keys[tt][rr][i].x == ptr->first.x &&
							keys[tt][rr][i].y == ptr->first.y){
							keylist.push_back(keys[tt][rr][i]);
							break;
						}
					}else{
						if(keys[tt][rr][i].x == ptr->second.x &&
							keys[tt][rr][i].y == ptr->second.y){
							keylist.push_back(keys[tt][rr][i]);
							break;
						}
					}
					//ptr--; 
					//i--;
				}
			}
			keyslists.push_back(keylist);
		}
		matchekeys.push_back(keyslists);
	}

	keys.clear();
	keys = matchekeys;
}

void AsiftMatchings::filterMatching(){
	filterKeysMatchings(asiftKeys1->keys,1);
	filterKeysMatchings(asiftKeys2->keys,0);
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
			of << asiftKeys1->zoom*ptr->first.x << "  " 
				<< asiftKeys1->zoom*ptr->first.y << "  " <<  asiftKeys2->zoom*ptr->second.x << 
			"  " <<  asiftKeys2->zoom*ptr->second.y << std::endl;
		}		
	}
	else 
	{
		std::cerr << "Unable to open the file matchings."; 
	}

	of.close();
}