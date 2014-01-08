// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#include "AsiftKeypoints.h"


AsiftKeypoints::AsiftKeypoints(int tilts,std::string ini_name)
{
	path = pro::Dir();
	this->tilts = tilts;
	inireadSiftParameters(ini_name);
	num = 0;
	w = 0,h = 0;
	//tiltsCalc();
}

AsiftKeypoints::~AsiftKeypoints(void)
{

}

int AsiftKeypoints::computeAsiftKeyPoints(int verb)
{
	keys.clear();
	num = compute_asift_keypoints(imgPixels,w,h,tilts,verb,keys,siftparams);
	setOnceKeys();
	return num;
}

int AsiftKeypoints::keypointsTotal(){
	num=0;
	for (int tt = 0; tt < (int) keys.size(); tt++){
		for (int rr = 0; rr < (int) keys[tt].size(); rr++)
		{
			num += (int) keys[tt][rr].size();
		}
	}
	return num;
}

int AsiftKeypoints::getNum() const{
	return num;
}

int AsiftKeypoints::getTilts() const{
	return tilts;
}

void AsiftKeypoints::setImage(pro::Image &src,int resizeFlag,int width,int height){
	imgGray.grayeScale(src);
	resize(imgGray,imgGrayZoom,resizeFlag,width,height);
	unsigned char * iarr = imgGrayZoom.getU8Data();
	imgPixels = std::vector<float>(iarr,iarr + w * h);
	delete[] iarr;
}

void AsiftKeypoints::setImage(pro::Image &src,int resizeFlag,int width,int height,float &zoom){
	setImage(src,resizeFlag,width,height);
	zoom = this->zoom;
}


void AsiftKeypoints::inireadSiftParameters(ptree &pt){
	
	siftparams.OctaveMax	  = pt.get_optional<int>("SIFTParams.OctaveMax"		  ).get();
	siftparams.DoubleImSize	  = pt.get_optional<int>("SIFTParams.DoubleImSize"    ).get();
	siftparams.order		  = pt.get_optional<int>("SIFTParams.order"			  ).get();
	siftparams.InitSigma	  = pt.get_optional<float>("SIFTParams.InitSigma"	  ).get();
	siftparams.BorderDist	  = pt.get_optional<int>("SIFTParams.BorderDist"	  ).get();
	siftparams.Scales		  = pt.get_optional<int>("SIFTParams.Scales"		  ).get();
	siftparams.PeakThresh	  = pt.get_optional<float>("SIFTParams.PeakThresh"	  ).get();
	siftparams.EdgeThresh	  = pt.get_optional<float>("SIFTParams.EdgeThresh"	  ).get();
	siftparams.EdgeThresh1	  = pt.get_optional<float>("SIFTParams.EdgeThresh1"	  ).get();
	siftparams.OriBins		  = pt.get_optional<int>("SIFTParams.OriBins"		  ).get();
	siftparams.OriSigma		  = pt.get_optional<float>("SIFTParams.OriSigma"	  ).get();
	siftparams.OriHistThresh  = pt.get_optional<float>("SIFTParams.OriHistThresh" ).get();
	siftparams.MaxIndexVal	  = pt.get_optional<float>("SIFTParams.MaxIndexVal"	  ).get();
	siftparams.MagFactor	  = pt.get_optional<int>("SIFTParams.MagFactor"		  ).get();
	siftparams.IndexSigma	  = pt.get_optional<float>("SIFTParams.IndexSigma"	  ).get();
	siftparams.IgnoreGradSign = pt.get_optional<int>("SIFTParams.IgnoreGradSign"  ).get();
	siftparams.MatchRatio	  = pt.get_optional<float>("SIFTParams.MatchRatio"	  ).get();
	siftparams.MatchXradius	  = pt.get_optional<float>("SIFTParams.MatchXradius"  ).get();
	siftparams.MatchYradius	  = pt.get_optional<float>("SIFTParams.MatchYradius"  ).get();
	
}

void AsiftKeypoints::iniwriteSiftParameters(ptree &pt){
	iniwriteSiftParameters(pt,siftparams);
}

void AsiftKeypoints::iniwriteSiftParameters(ptree &pt,siftPar par){

	pt.put("SIFTParams.OctaveMax"	  ,par.OctaveMax	 );
	pt.put("SIFTParams.DoubleImSize"  ,par.DoubleImSize	 );
	pt.put("SIFTParams.order"		  ,par.order		 );
	pt.put("SIFTParams.InitSigma"	  ,par.InitSigma	 );
	pt.put("SIFTParams.BorderDist"	  ,par.BorderDist	 );
	pt.put("SIFTParams.Scales"		  ,par.Scales		 );
	pt.put("SIFTParams.PeakThresh"	  ,par.PeakThresh	 );
	pt.put("SIFTParams.EdgeThresh"	  ,par.EdgeThresh	 );
	pt.put("SIFTParams.EdgeThresh1"	  ,par.EdgeThresh1	 );
	pt.put("SIFTParams.OriBins"		  ,par.OriBins		 );
	pt.put("SIFTParams.OriSigma"	  ,par.OriSigma		 );
	pt.put("SIFTParams.OriHistThresh" ,par.OriHistThresh );
	pt.put("SIFTParams.MaxIndexVal"	  ,par.MaxIndexVal	 );
	pt.put("SIFTParams.MagFactor"	  ,par.MagFactor	 );
	pt.put("SIFTParams.IndexSigma"	  ,par.IndexSigma	 );
	pt.put("SIFTParams.IgnoreGradSign",par.IgnoreGradSign);
	pt.put("SIFTParams.MatchRatio"	  ,par.MatchRatio	 );
	pt.put("SIFTParams.MatchXradius"  ,par.MatchXradius	 );
	pt.put("SIFTParams.MatchYradius"  ,par.MatchYradius	 );

}

void AsiftKeypoints::inireadSiftParameters(std::string name){

	ptree pt;
	
	if(this->path.isExist(name)){

		read_ini(name, pt);

		inireadSiftParameters(pt);

	}else{
		
		std::cout << "not exist " << name << endl;
		
		default_sift_parameters(siftparams);

		iniwriteSiftParameters(name);


	}

}

void AsiftKeypoints::iniwriteSiftParameters(std::string name){

	ptree pt;
	
	iniwriteSiftParameters(pt);

	write_ini(name,pt);

}

void AsiftKeypoints::iniwriteSiftParameters(std::string name,siftPar par){

	ptree pt;
	
	iniwriteSiftParameters(pt,par);

	write_ini(name,pt);

}

//
//void AsiftKeypoints::output(string name){
//	std::ofstream of;
//	of.open(path.pwd(name));
//
//	if (of.is_open())
//	{
//		// Follow the same convention of David Lowe: 
//		// the first line contains the number of keypoints and the length of the desciptors (128)
//		of << tilts << " " << w << " " << h << " " << zoom << std::endl;  
//		of << getNum()<< "  " << VecLength << "  " << std::endl;
//		of << (int) keys.size() << std::endl;
//		for (int tt = 0; tt < (int) keys.size(); tt++)
//		{
//			of << (int) keys[tt].size() << std::endl;
//			for (int rr = 0; rr < (int) keys[tt].size(); rr++)
//			{
//				of << (int) keys[tt][rr].size() << std::endl;
//				keypointslist::iterator ptr = keys[tt][rr].begin();
//				for(int i=0; i < (int) keys[tt][rr].size(); i++, ptr++)	
//				{
//					of << zoom*ptr->x << "  " << zoom*ptr->y << "  " << zoom*ptr->scale << "  " << ptr->angle;
//					
//					for (int ii = 0; ii < (int) VecLength; ii++)
//					{
//						of << "  " << ptr->vec[ii];
//					}
//					
//					of << std::endl;
//				}
//			}	
//		}
//	}
//	else 
//	{
//		std::cerr << "Unable to open the file keys."; 
//	}
//
//	of.close();
//}
//
//void AsiftKeypoints::input(string name){
//	std::ifstream inf;
//	keys.clear();
//	
//	int detection_num = 0;
//	int tt_num=0,rr_num=0,key_num=0;
//
//	inf.open(path.pwd(name));
//
//	inf >> tilts;
//	inf >> w;
//	inf >> h;
//	inf >> zoom;
//
//	inf >> num;
//	inf >> detection_num;
//	inf >> tt_num;
//	for(int i=0;i<tt_num;i++){
//		inf >> rr_num;
//		vector<keypointslist> keyslists;
//		for(int j=0;j<rr_num;j++){
//			inf >> key_num;
//			keypointslist keyslist;
//			for(int k=0;k<key_num;k++){
//				keypoint key;
//				inf >> key.x; key.x /= zoom;
//				inf >> key.y; key.y /= zoom;
//				inf >> key.scale; key.scale /= zoom;
//				inf >> key.angle;
//				for(int l=0;l<detection_num;l++){
//					inf >> key.vec[l];
//				}
//				keyslist.push_back(key);
//			}
//			keyslists.push_back(keyslist);
//		}
//		keys.push_back(keyslists);
//	}
//
//	inf.close();
//}

void AsiftKeypoints::output(string name){
	std::ofstream of;
	of.open(path.pwd(name));

	if (of.is_open())
	{
		// Follow the same convention of David Lowe: 
		// the first line contains the number of keypoints and the length of the desciptors (128)
		of << tilts << " " << w << " " << h << " " << zoom << std::endl;  
		of << getNum()<< "  " << VecLength << "  " << std::endl;
		of << (int) keys.size() << std::endl;
		for (int tt = 0; tt < (int) keys.size(); tt++)
		{
			of << (int) keys[tt].size() << std::endl;
			for (int rr = 0; rr < (int) keys[tt].size(); rr++)
			{
				of << (int) keys[tt][rr].size() << std::endl;
				keypointslist::iterator ptr = keys[tt][rr].begin();
				for(int i=0; i < (int) keys[tt][rr].size(); i++, ptr++)	
				{
					of << zoom*ptr->x << "  " << zoom*ptr->y << "  " << zoom*ptr->scale << "  " << ptr->angle;
					
					for (int ii = 0; ii < (int) VecLength; ii++)
					{
						of << "  " << ptr->vec[ii];
					}
					
					of << std::endl;
				}
			}	
		}
	}
	else 
	{
		std::cerr << "Unable to open the file keys."; 
	}

	of.close();
}

void AsiftKeypoints::outputOnce(string name){
	std::ofstream of;
	of.open(path.pwd(name));

	if (of.is_open())
	{
		// Follow the same convention of David Lowe: 
		// the first line contains the number of keypoints and the length of the desciptors (128)
		of << tilts << " " << w << " " << h << " " << zoom << std::endl;  
		of << getNum()<< "  " << VecLength << "  " << std::endl;
		//of << (int) keys.size() << std::endl;
		//for (int tt = 0; tt < (int) keys.size(); tt++)
		//{
		//	//of << (int) keys[tt].size() << std::endl;
		//	for (int rr = 0; rr < (int) keys[tt].size(); rr++)
		//	{
				//of << (int) keys[tt][rr].size() << std::endl;
				keypointslist::iterator ptr = onceKeys.begin();
				//for(int i=0; i < (int) keys[tt][rr].size(); i++, ptr++)	
				for(int i=0; i < (int) onceKeys.size(); i++, ptr++)	
				{
					of << zoom*ptr->x << "  " << zoom*ptr->y << "  " << zoom*ptr->scale << "  " << ptr->angle;
					
					for (int ii = 0; ii < (int) VecLength; ii++)
					{
						of << "  " << ptr->vec[ii];
					}
					
					of << std::endl;
				}
		//	}	
		//}
	}
	else 
	{
		std::cerr << "Unable to open the file keys."; 
	}

	of.close();
}

void AsiftKeypoints::input(string name){
	std::ifstream inf;
	keys.clear();
	
	int detection_num = 0;
	int tt_num=0,rr_num=0,key_num=0;

	inf.open(path.pwd(name));

	inf >> tilts;
	inf >> w;
	inf >> h;
	inf >> zoom;

	inf >> num;
	inf >> detection_num;
	inf >> tt_num;

	for(int i=0;i<tt_num;i++){
		inf >> rr_num;
		vector<keypointslist> keyslists;
		for(int j=0;j<rr_num;j++){
			inf >> key_num;
			keypointslist keyslist;
			for(int k=0;k<key_num;k++){
				keypoint key;
				inf >> key.x; key.x /= zoom;
				inf >> key.y; key.y /= zoom;
				inf >> key.scale; key.scale /= zoom;
				inf >> key.angle;
				for(int l=0;l<detection_num;l++){
					inf >> key.vec[l];
				}
				keyslist.push_back(key);
			}
			keyslists.push_back(keyslist);
		}
		keys.push_back(keyslists);
	}

	inf.close();
}

void AsiftKeypoints::inputOnce(string name){
	std::ifstream inf;
	keys.clear();
	
	int detection_num = 0;
	int tt_num=0,rr_num=0,key_num=0;

	inf.open(path.pwd(name));

	inf >> tilts;
	inf >> w;
	inf >> h;
	inf >> zoom;

	inf >> num;
	inf >> detection_num;

	//inf >> tt_num;
	//for(int i=0;i<tt_num;i++){
		//inf >> rr_num;
		//vector<keypointslist> keyslists;
		//for(int j=0;j<rr_num;j++){
			//inf >> key_num;
			//keypointslist keyslist;
			for(int k=0;k<num;k++){
				keypoint key;
				inf >> key.x; key.x /= zoom;
				inf >> key.y; key.y /= zoom;
				inf >> key.scale; key.scale /= zoom;
				inf >> key.angle;
				for(int l=0;l<detection_num;l++){
					inf >> key.vec[l];
				}
				onceKeys.push_back(key);
				//keyslist.push_back(key);
			}
			//keyslists.push_back(keyslist);
		//}
		//keys.push_back(keyslists);
	//}

	inf.close();
}


void AsiftKeypoints::filterRectangle(cv::Point2f pt1,cv::Point2f pt2){

	// 最大最小の点を取得
	int maxX,maxY,minX,minY;
	if(pt1.x>pt2.x){
		maxX=pt1.x;
		minX=pt2.x;
	}else{
		maxX=pt2.x;
		minX=pt1.x;
	}
	if(pt1.y>pt2.y){
		maxY=pt1.y;
		minY=pt2.y;
	}else{
		maxY=pt2.y;
		minY=pt1.y;
	}

	int count=0;

	for (int tt = 0; tt < (int) keys.size(); tt++)
	{
		for (int rr = 0; rr < (int) keys[tt].size(); rr++)
		{
			keypointslist::iterator ptr = keys[tt][rr].begin();
			for(int i=0; i < (int) keys[tt][rr].size(); i++)	
			{
				// 矩形範囲条件
				if(zoom*keys[tt][rr][i].x > maxX  || zoom*keys[tt][rr][i].x < minX ||
					zoom*keys[tt][rr][i].y > maxY || zoom*keys[tt][rr][i].y < minY){
						// 範囲外のキーポイントを削除
					keys[tt][rr].erase(keys[tt][rr].begin()+i);
					//ptr--; 
					i--;
				}
			}
		}	
	}

	keypointsTotal();
}

void AsiftKeypoints::filterRectangleDel(cv::Rect rects[],int n){
	
	int maxX,maxY,minX,minY;


	for(int j=0;j<n;j++){
		if(rects[j].x>rects[j].x+rects[j].width){
			maxX = rects[j].x;
			minX = rects[j].x+rects[j].width;
		}else{
			maxX = rects[j].x+rects[j].width;
			minX = rects[j].x;
		}
		if(rects[j].y>rects[j].y+rects[j].height){
			maxY = rects[j].y;
			minY = rects[j].y+rects[j].height;
		}else{
			maxY = rects[j].y+rects[j].height;
			minY = rects[j].y;
		}

		for (int tt = 0; tt < (int) keys.size(); tt++)
		{
			for (int rr = 0; rr < (int) keys[tt].size(); rr++)
			{
				keypointslist::iterator ptr = keys[tt][rr].begin();
				for(int i=0; i < (int) keys[tt][rr].size(); i++)	
				{
					// 矩形範囲条件
					if(zoom*keys[tt][rr][i].x <= maxX  || zoom*keys[tt][rr][i].x >= minX ||
						zoom*keys[tt][rr][i].y <= maxY || zoom*keys[tt][rr][i].y >= minY){
							// 範囲外のキーポイントを削除
						keys[tt][rr].erase(keys[tt][rr].begin()+i);
						//ptr--; 
						i--;
					}
				}
			}	
		}
	
	}

}

void AsiftKeypoints::filterCenterLine(cv::Point2f centerPt,float distance){
	
	AsiftKeypoints xAxis = AsiftKeypoints(*this);
	AsiftKeypoints yAxis = AsiftKeypoints(*this);

	for (int tt = 0; tt < (int) xAxis.keys.size(); tt++)
	{
		for (int rr = 0; rr < (int) xAxis.keys[tt].size(); rr++)
		{
			keypointslist::iterator ptr = xAxis.keys[tt][rr].begin();
			for(int i=0; i < (int) xAxis.keys[tt][rr].size(); i++)
			{
				if(distance < abs(zoom*xAxis.keys[tt][rr][i].y-centerPt.y)){
					xAxis.keys[tt][rr].erase(xAxis.keys[tt][rr].begin()+i);
					i--;
				}
			}
		}
	}

	for (int tt = 0; tt < (int) yAxis.keys.size(); tt++)
	{
		for (int rr = 0; rr < (int) yAxis.keys[tt].size(); rr++)
		{
			keypointslist::iterator ptr = yAxis.keys[tt][rr].begin();
			for(int i=0; i < (int) yAxis.keys[tt][rr].size(); i++)	
			{
				if(distance < abs(zoom*yAxis.keys[tt][rr][i].x-centerPt.x)){
					yAxis.keys[tt][rr].erase(yAxis.keys[tt][rr].begin()+i);
					i--;
				}
			}
		}
	}

	if(xAxis.keypointsTotal() > yAxis.keypointsTotal()){
		*this = AsiftKeypoints(xAxis);
	}else{
		*this = AsiftKeypoints(yAxis);
	}

	keypointsTotal();
}

void AsiftKeypoints::setOnceKeys(){
	
	for (int tt = 0; tt < (int) keys.size(); tt++)
	{
		//of << (int) keys[tt].size() << std::endl;
		for (int rr = 0; rr < (int) keys[tt].size(); rr++)
		{
			//of << (int) keys[tt][rr].size() << std::endl;
			keypointslist::iterator ptr = keys[tt][rr].begin();
			for(int i=0; i < (int) keys[tt][rr].size(); i++, ptr++)	
			{
				keypoint key;
				key.x = keys[tt][rr][i].x;
				key.y = keys[tt][rr][i].y;
				key.scale = keys[tt][rr][i].scale;
				key.angle = keys[tt][rr][i].angle;
				for(int l=0;l<VecLength;l++){
					key.vec[l] = keys[tt][rr][i].vec[l];
				}
				onceKeys.push_back(key);
			}

		}

	}

}

void AsiftKeypoints::tiltsCalc(){
	
	int t_min = 1;
	float t_k = sqrt(2.);
	int num_rot_t2 = 10;
	
	rots = new int[tilts];
	keys.resize(tilts);

	for(int tt=1;tt<=tilts;tt++){
		
		float t = t_min * pow(t_k, tt-1);

		if ( t == 1 )
		{					
			// copy the image from vector to array as compute_sift_keypoints uses only array.				
			rots[0] = 1;

			keys[0].resize(1);
		}
		else
		{
		  int num_rot1;

		  if(num_rot_t2*t/2 >= 0){
			  num_rot1 = num_rot_t2*t/2+0.5;
		  }else{			
			  num_rot1 = num_rot_t2*t/2-0.5;
		  }

		  if ( num_rot1%2 == 1 )
		  {
			num_rot1 = num_rot1 + 1;
		  }
		  num_rot1 = num_rot1 / 2;

		  rots[tt] = num_rot1;

		  keys.resize(num_rot1);
		}
	}
}

void AsiftKeypoints::draw(pro::Image &src,cv::Scalar siftcol,cv::Scalar asiftcol){

	for (int tt = 0; tt < (int) keys.size(); tt++)
	{
		for (int rr = 0; rr < (int) keys[tt].size(); rr++)
		{
			keypointslist::iterator ptr = keys[tt][rr].begin();
			for(int i=0; i < (int) keys[tt][rr].size(); i++, ptr++)	
			{
				// 色の選択
				cv::Scalar color;
				if(tt == 0){
					color = siftcol;
				}else{
					color = asiftcol;
				}

				// 中心点の描写
				src.circle(cv::Point2f(zoom*ptr->x,zoom*ptr->y),1,color);
				// スケールの描写
				src.circle(cv::Point2f(zoom*ptr->x,zoom*ptr->y),zoom*ptr->scale,color,1);
				// 特徴点のオリエンテーション方向を描写
				if(ptr->angle>=0){
					cv::Point pt2(zoom*ptr->x + zoom*cos(ptr->angle)*ptr->scale,
						zoom*ptr->y + zoom*sin(ptr->angle)*ptr->scale);
					src.line(cv::Point2f(zoom*ptr->x,zoom*ptr->y),pt2,color,1);
				}
			}
		}	
	}
}

void AsiftKeypoints::drawOnce(pro::Image &src,cv::Scalar col){

	keypointslist::iterator ptr = onceKeys.begin();
	for(int i=0; i < (int) onceKeys.size(); i++, ptr++)	
	{
		// 中心点の描写
		src.circle(cv::Point2f(zoom*ptr->x,zoom*ptr->y),1,col);
		// スケールの描写
		src.circle(cv::Point2f(zoom*ptr->x,zoom*ptr->y),zoom*ptr->scale,col,1);
		// 特徴点のオリエンテーション方向を描写
		if(ptr->angle>=0){
			cv::Point pt2(zoom*ptr->x + zoom*cos(ptr->angle)*ptr->scale,
				zoom*ptr->y + zoom*sin(ptr->angle)*ptr->scale);
			src.line(cv::Point2f(zoom*ptr->x,zoom*ptr->y),pt2,col,1);
		}
	}

}

void AsiftKeypoints::resize(pro::Image &src,pro::Image &dst,int resizeFlag,int width,int height){
	if(!resizeFlag){
		dst.clone(src);
		zoom = 1;
	}else{
		//std::cout << "WARNING: The input images are resized to " << IMAGE_X << "x" << IMAGE_Y << " for ASIFT. " << endl 
		//<< "         But the results will be normalized to the original image size." << endl << endl;
		
		float InitSigma_aa = 1.6;
		
		int wS,hS,w,h;
				
		float areaS = width * height;

		w = src.size().width;
		h = src.size().height;

		float area1 = w * h;
		zoom = sqrt(area1/areaS);
		
		wS = (int) (w / zoom);
		hS = (int) (h / zoom);
		
		/* Anti-aliasing filtering along vertical direction */
		if ( zoom > 1 )
		{
			int ksize;
			float sigma_aa = InitSigma_aa * zoom / 2;
			const float GaussTruncate1 = 4.0;
			ksize = (int)(2.0 * GaussTruncate1 * sigma_aa + 1.0);
			ksize = MAX(3, ksize);    /* Kernel must be at least 3. */
			if (ksize % 2 == 0)       /* Make kernel size odd. */
				ksize++;
			assert(ksize < 100);
			cv::GaussianBlur((const cv::Mat&)src, (cv::Mat&)dst, cv::Size(ksize,ksize), sigma_aa, sigma_aa);
		}
			
		dst.resize(src,cv::Size(wS,hS));

		this->w = wS;
		this->h = hS;
		
	}
}
