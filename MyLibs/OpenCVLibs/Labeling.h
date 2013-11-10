#pragma once

#include"..\Core\Core.h"
#include"opencv2\core\core.hpp"
#include<vector>

namespace pro{

class PRO_EXPORTS Labeling
{
public:

	int w,h;
	int num;

private:
	
	std::vector<int> labels;

public:

	Labeling();

	void init(cv::Size size);
	void labeling(const unsigned char* bins);

	int& operator() (const int x, const int y); 
	const int& operator() (const int x, const int y) const;

};

}