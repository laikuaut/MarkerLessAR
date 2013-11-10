#pragma once

#include"type_c.h"
#include"Timer.h"
#include"Dir.h"
#include"Calc.h"
#include"StringCalc.h"
#include"../Exceptions/Exceptions.h"

#include<string>

namespace pro{

std::string PRO_EXPORTS charToString(const char* str_c,int n=512);
PRO_C_EXTERN const char* stringToChar(const string& str);

}

namespace test{

void PRO_EXPORTS writeMessage(const std::string& message,int tab=0);

}