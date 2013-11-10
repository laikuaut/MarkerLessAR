#pragma once

#include"type_c.h"
#include<fstream>
#include<iostream>

#include"Dir.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>

using namespace boost::property_tree;

namespace pro{

class PRO_EXPORTS StringCalc
{
public:

	static const int RANDSTRNUM = 86;
	static const int MAXSTRLENGTH = 4000;

public:
	StringCalc(void);
	~StringCalc(void);

	static void randomInit();
	static void randomStringOutput(int length,string filename);
	static int fileStringMatching(string ansfile,string file);

};

}