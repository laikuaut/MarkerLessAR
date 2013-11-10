#include "LabelingCenter.h"

#include<fstream>

namespace pro{

LabelingCenter::LabelingCenter(void)
{

}


LabelingCenter::~LabelingCenter(void)
{
}

void LabelingCenter::init(cv::Size size){
	labels.init(size);
	centers.resize(labels.w*labels.h);
	memset(&centers[0],0,sizeof(cv::Point)*labels.w*labels.h);
}

void LabelingCenter::setCenter(const unsigned char* bins){
	int x,y;
	//vector<int> counts;
	// ラベリング処理
	labels.labeling(bins);
	
	num = labels.num;
	sizes.resize(num);
	flags.resize(num);
	memset(&sizes[0],0,sizeof(int)*num);
	memset(&flags[0],0,sizeof(short)*num);


	// 重心点の初期化
	for(int i=0;i<num;i++){
		centers[i].x = 0;
		centers[i].y = 0;
		sizes[i] = 0;
		flags[i] = 1;
	}
	// 重心計算開始
	for(y = 0;y < labels.h;y++){
		for(x = 0;x < labels.w;x++){
			centers[labels(x,y)].x += x;
			centers[labels(x,y)].y += y;
			sizes[labels(x,y)] += 1;
		}
	}
	for(int i=0;i<num;i++){
		centers[i].x /= sizes[i];
		centers[i].y /= sizes[i];
 	}
	// 重心計算終了

}

void LabelingCenter::sizeFilter(int min,int max){
	// サイズのフィルター
	for(int i=0;i<num;i++){
		// サイコロのサイズ指定ができる場所
		if(sizes[i]<min || sizes[i] > max){
			flags[i] = 0;
		}
	}
}

void LabelingCenter::circleFilter(double per,double radius_per){
	int x,y;
	
	vector<int> counts;
	counts.resize(num);
	memset(&counts[0],0,sizeof(int)*num);
	for(int i=0;i<num;i++){
		counts[i] = 0;
	}
	// 円形のフィルター
	for(y = 0;y < labels.h;y++){
		for(x = 0;x < labels.w;x++){
			//if(pow(centers[labels(x,y)].x-x,2)+pow(centers[labels(x,y)].y-y,2) < sizes[labels(x,y)]/CV_PI*radius_per){
			if(Calc::getDistance2(centers[labels(x,y)],cv::Point2f(x,y)) < sizes[labels(x,y)]/CV_PI*radius_per){
				counts[labels(x,y)]++;
			}else{
				//std::cout << x << "," << y << std::endl;
			}
		}
	}
	for(int i=0;i<num;i++){
		if(sizes[i]*per>counts[i]){
			flags[i] = 0;
		}
	}
}

void LabelingCenter::rangeRectangleFilter(cv::Point2f pt1,cv::Point2f pt2){
	//int x,y;
	//// 矩形フィルター
	//for(y = 0;y < labels.h;y++){
	//	for(x = 0;x < labels.w;x++){
	//		if(centers[labels(x,y)].x < pt1.x  || 
	//			centers[labels(x,y)].y < pt1.y ||
	//			centers[labels(x,y)].x > pt2.x || 
	//			centers[labels(x,y)].y > pt2.y )
	//			flags[labels(x,y)] = 0;
	//	}
	//}
	// 最大最小の座標を取得
	cv::Point2f max,min;
	if(pt1.x > pt2.x){
		max.x = pt1.x;
		min.x = pt2.x;
	}else{
		max.x = pt2.x;
		min.x = pt1.x;
	}
	if(pt1.y > pt2.y){
		max.y = pt1.y;
		min.y = pt2.y;
	}else{
		max.y = pt2.y;
		min.y = pt1.y;
	}

	// 矩形フィルター
	for(int i=0;i<num;i++){
		if( centers[i].x < min.x || 
			centers[i].y < min.y ||
			centers[i].x > max.x || 
			centers[i].y > max.y )
			flags[i] = 0;
	}
}

void LabelingCenter::filterReset(){
	for(int i=0;i<num;i++){
		flags[i] = 1;
	}
}

void LabelingCenter::onMouse_impl(int event,int x,int y,int flag){
	switch(event) {
	case cv::EVENT_MOUSEMOVE:
		this->x=x;
		this->y=y;
		std::cout << x << "," << y << std::endl;
		break;
	case cv::EVENT_LBUTTONDOWN:
		mousePt[ptCount].x = x;
		mousePt[ptCount].y = y;
		ptCount++;
		break;
	case cv::EVENT_RBUTTONDOWN:
		ptCount=0;
		break;
	default:
		break;
	}
}

void LabelingCenter::mouseRangeFilter(Image& src){

	cv::namedWindow("mouseRangefilter",0);
	ptCount = 0;
	dst.clone(src);

	while(cv::waitKey(30)!='q'){

		if(ptCount==2){
			rangeRectangleFilter(mousePt[0],mousePt[1]);
			cv::destroyWindow("mouseRangefilter");
			break;
		}
		
		if(ptCount==1){
			dst.circle(mousePt[0],5,cv::Scalar(255,255,0));
			dst.circle(cv::Point(x,y),5,cv::Scalar(255,255,0));
			dst.rectangle(mousePt[0],cv::Point(x,y),cv::Scalar(255,255,0));
		}
		dst.imshow("mouseRangefilter");
		dst.clone(src);

		cv::setMouseCallback("mouseRangefilter", onMouse, this);
		
	}
}

void LabelingCenter::draw(Image& img,short flag,cv::Scalar scal){
	for(int i=0;i<num;i++){
		if(flag == flags[i]){
			if(sizes[i] < 2000)
				img.circle(centers[i],sqrt(sizes[i]/CV_PI),scal);
		}
	}
}

void LabelingCenter::outputCenter(){
	
	std::ofstream ofs;
	ofs.open("Centers.txt");
	ofs<<num<<std::endl;
	for(int i=0;i<num;i++){
		if(flags[i])
			ofs <<	centers[i].x << " " 
				<<	centers[i].y << " " 
				<<	sizes[i]	 << std::endl;//" "
				//<<	(int)flags[i]<< std::endl;
	}

}

cv::Point LabelingCenter::operator() (const int num){
	return centers[num];
}



}