#include "Asift.h"


Asift::Asift(void)
{
}


Asift::~Asift(void)
{
}



void Asift::init(string img1_name,string img2_name,
			string imgV_name,string imgH_name,
			string keys1_name,string keys2_name,
			string matchings_name,
			int tilt1,int tilt2,int resize_flag){
	img1Name=img1_name;
	img2Name=img2_name;
	imgVName=imgV_name;
	imgHName=imgH_name;
	keys1Name = keys1_name;
	keys2Name = keys2_name;
	matchingsName = matchings_name;

	img1.load(img1_name);
	img1Gray.grayeScale(img1);
	img2.load(img2_name);
	img2Gray.grayeScale(img2);

	verb = 0;

	resizeFlag = resize_flag;

	resize(img1Gray,img1GrayZoom,zoom1,resize_flag);
	resize(img2Gray,img2GrayZoom,zoom2,resize_flag);

	unsigned char * iarr1 = img1GrayZoom.getU8Data();
	unsigned char * iarr2 = img2GrayZoom.getU8Data();

	ipixels1Zoom = std::vector<float>(iarr1,iarr1 + img1GrayZoom.size().width * img1GrayZoom.size().height);
	ipixels2Zoom = std::vector<float>(iarr2,iarr2 + img2GrayZoom.size().width * img2GrayZoom.size().height);
	
	keys1 = AsiftKeypoints(tilt1);
	keys2 = AsiftKeypoints(tilt2);

	bandWidth = 20;
}


void Asift::run(){
	time_t tstart, tend;
	pro::Timer timer;
	
	cout << "Computing keypoints1 on the two images..." << endl;
	timer.start();

		keys1.computeAsiftKeyPoints(ipixels1Zoom,img1GrayZoom.size().width,img1GrayZoom.size().height,verb,zoom1);
		//keys1.input(keys1Name);
		//cout << keys1.getNum() << endl;

	cout << "Keypoints1 computation accomplished in " << (double)timer.getDiff()/pro::Timer::PER_SEC << " seconds." << endl << endl;
	timer.lap();

	//cv::VideoCapture cap(0);
	//if(!cap.isOpened()) return;
	//cv::Size cap_size(800,600);
	//cap.set(CV_CAP_PROP_FRAME_WIDTH, cap_size.width);
	//cap.set(CV_CAP_PROP_FRAME_HEIGHT, cap_size.height);
	////int fps = 15;
	////cv::VideoWriter writer("capture.avi", CV_FOURCC('X','V','I','D'), fps, cap_size);
	//img1GrayZoom.imshow("img1",1);	

	//pro::Image frame;

	//while(1){
	//	cap.read((cv::Mat&)frame);

	//	img2Gray.grayeScale(frame);
	//	resize(img2Gray,img2GrayZoom,zoom2,0);
	//	unsigned char * iarr2 = img2Gray.getU8Data();
	//	ipixels2Zoom = std::vector<float>(iarr2,iarr2 + img2GrayZoom.size().width * img2GrayZoom.size().height);

		//cout << "Computing keypoints2 on the two images..." << endl;

			keys2.computeAsiftKeyPoints(ipixels2Zoom,img2GrayZoom.size().width,img2GrayZoom.size().height,verb,zoom2);
	
		//cout << "Keypoints2 computation accomplished in " << (double)timer.getDiff()/pro::Timer::PER_SEC << " seconds." << endl << endl;
		//timer.lap();
	
		//cout << "Matching the keypoints..." << endl;

			AsiftMatchings asiftMatchings(keys1,keys2);	
			asiftMatchings.computeAsiftMatches(verb);

		//cout << "Keypoints matching accomplished in " << (double)timer.getDiff()/pro::Timer::PER_SEC << " seconds." << endl << endl;
		//timer.lap();

		//cout << "All computation accomplished in " << (double)timer.getNow()/pro::Timer::PER_SEC << "seconds." << endl << endl;

	//	frame.imshow("Capture",1);
	//	if(cv::waitKey(30) >= 0) break;
	//	delete[] iarr2;
	//}

	keys1.output(keys1Name);
	keys2.output(keys2Name);
	matchings.output(matchingsName);

}

void Asift::resize(pro::Image &src,pro::Image &dst,float &zoom,int resizeFlag){
	if(!resizeFlag){
		dst.clone(src);
		zoom = 1;
	}else{
		//cout << "WARNING: The input images are resized to " << IMAGE_X << "x" << IMAGE_Y << " for ASIFT. " << endl 
		//<< "         But the results will be normalized to the original image size." << endl << endl;
		
		float InitSigma_aa = 1.6;
		
		int wS,hS,w,h;
				
		float areaS = IMAGE_X * IMAGE_Y;

		w = src.size().width;
		h = src.size().height;

		float area1 = w * h;
		zoom1 = sqrt(area1/areaS);
		
		wS = (int) (w / zoom1);
		hS = (int) (h / zoom1);
		
		/* Anti-aliasing filtering along vertical direction */
		if ( zoom1 > 1 )
		{
			int ksize;
			float sigma_aa = InitSigma_aa * zoom1 / 2;
			const float GaussTruncate1 = 4.0;
			ksize = (int)(2.0 * GaussTruncate1 * sigma_aa + 1.0);
			ksize = MAX(3, ksize);    /* Kernel must be at least 3. */
			if (ksize % 2 == 0)       /* Make kernel size odd. */
				ksize++;
			assert(ksize < 100);
			cv::GaussianBlur((const cv::Mat&)src, (cv::Mat&)dst, cv::Size(ksize,ksize), sigma_aa, sigma_aa);
		}
			
		dst.resize(src,cv::Size(wS,hS));
		
	}
}