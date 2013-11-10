#include "Calc.h"

namespace pro{

Calc::Calc(void)
{
}


Calc::~Calc(void)
{
}

/******************************
 *  座標計算用関数群
 */

cv::Point2f Calc::PointRotate(cv::Point2f pt,int angle){
	double s = sin((double)angle*CV_PI/180.),c = cos((double)angle*CV_PI/180.);
	cv::Point2f point;
	if(abs(s)<0.000001) s=0;
	if(abs(c)<0.000001) c=0;
	point.x = (float)(pt.x * c + pt.y * s);
	point.y = (float)(-pt.x * s + pt.y * c);
	return point;
}
cv::Point2f Calc::PointRotate(double dx,double dy,int angle){
	double s = sin((double)angle*CV_PI/180.);
	double c = cos((double)angle*CV_PI/180.);
	cv::Point2f point;
	if(abs(s)<0.000001) s=0;
	if(abs(c)<0.000001) c=0;
	//std::cout << "s=" << s << std::endl;
	//std::cout << "c=" << c << std::endl;
	point.x = (float)( dx * c + dy * s);
	point.y = (float)(-dx * s + dy * c);
	return point;
}

cv::Point2f Calc::UnitVec(cv::Point2f pt1,cv::Point2f pt2){
	cv::Vec2f unitvec(pt2-pt1);
	float distance = (float)getDistance(pt1,pt2);
	unitvec.val[0]  /= distance;
	unitvec.val[1]  /= distance;
	return unitvec;
}

double Calc::Vec2dScalar(cv::Vec2f vec){
	double scal = sqrt(pow(vec.val[0],2)+pow(vec.val[1],2));
	return scal;
}

double Calc::Vec2fCrossScalar(cv::Vec2f v1,cv::Vec2f v2){
	double angle = InnerProductAngle(v1,v2);
	double v1Scal = Vec2dScalar(v1);
	double v2Scal = Vec2dScalar(v2);
	return v1Scal*v2Scal*sin(angle);
}

double Calc::getDistance(cv::Point2f pt1,cv::Point2f pt2){
	return sqrt(pow(pt2.x - pt1.x,2) + pow(pt2.y - pt1.y,2));
}

double Calc::getDistance2(cv::Point2f pt1,cv::Point2f pt2){
	return pow(pt2.x - pt1.x,2) + pow(pt2.y - pt1.y,2);
}

double Calc::InnerProductAngle(cv::Vec2f v1,cv::Vec2f v2){
	double dotv1 = v1.dot(v2);
	double cosine = dotv1;		// 元が単位ベクトルなので分母が１になっている
	//if(dotv1>1)
	//cout << "dot:" <<  dotv1 << endl;
	double angle = acos(cosine);
	if(dotv1>1)
		angle = 0;
	else if(dotv1 <-1)
		angle = CV_PI;
//		cout << angle << endl;
	return angle;
}

double Calc::getAngle(cv::Point2f pt1,cv::Point2f pt2){
	double tan = abs(pt2.y-pt1.y)/abs(pt2.x-pt1.x);
	double angle = atan(tan);
	return angle*180/CV_PI;
}

/*** 2ベクトルが平行にあるかどうか ***/
bool Calc::Parallel2Vec(double angle){
	angle = angle*180/CV_PI;
	return ( angle > 170  );
}

/*** 2ベクトルが垂直であるかどうか ***/
bool Calc::Vertical2Vec(double angle){
	angle = angle*180/CV_PI;
	return (angle >= 60 && angle <=120);
}

}