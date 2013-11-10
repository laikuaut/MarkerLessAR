// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#pragma once

#ifdef NSTATIC
#	define PRO_EXPORTS __declspec(dllexport)
#else
#	define PRO_EXPORTS
#endif

#ifdef NSTATIC
#	define PRO_C_EXTERN extern "C" __declspec(dllexport)
#else
#	define PRO_C_EXTERN extern "C"
#endif

#if defined(NSTATIC)
#define BOOST_ALL_DYN_LINK
#endif

// testプロジェクトにてLoggerを表示するための宣言
#define TEST_MESSAGE

#include<string>
#include<ctime>
#include<vector>
#include<opencv2\core\core.hpp>

using std::vector;
using std::string;

/*****************************
 *  warning
 */

// warning C4251 class 'std::basic_string<_Elem,_Traits,_Alloc>' は __export キーワードを使って class 'pro::Dir' にエクスポートしてください。
template class PRO_EXPORTS std::basic_string<char, std::char_traits<char>, std::allocator<char> >;

// warning C4251 class 'std::vector<_Ty>' は __export キーワードを使って class 'pro::Timer' にエクスポートしてください。
// http://support.microsoft.com/default.aspx?scid=kb;ja-jp;168958
// Timer.h
template class PRO_EXPORTS std::vector<clock_t>;
template class PRO_EXPORTS std::vector<std::string>; 
template class PRO_EXPORTS std::vector<int>; 
template class PRO_EXPORTS std::vector<short>; 
template class PRO_EXPORTS std::vector<cv::Point>;
template class PRO_EXPORTS cv::Point_<float>;
template class PRO_EXPORTS std::vector<cv::Point_<float>>;

