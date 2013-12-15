#include "Asift.h"


Asift::Asift(void)
{
}


Asift::~Asift(void)
{
}

void Asift::init(int readini,string imgBase_name,string imgInput_name,
			string imgV_name,string imgH_name,
			string baseKeys_name,string inputKeys_name,
			string matchings_name,string capIn_name,string capOut_name,
			int tilt1,int tilt2,int resize_flag){

	// 設定初期化

	imgBaseName=imgBase_name;
	imgInputName=imgInput_name;
	imgVName=imgV_name;
	imgHName=imgH_name;
	baseKeysName = baseKeys_name;
	inputKeysName = inputKeys_name;
	matchingsName = matchings_name;
	capInName = capIn_name;
	capOutName = capOut_name;
	resizeFlag = resize_flag;

	baseKeys = AsiftKeypoints(tilt1);
	inputKeys = AsiftKeypoints(tilt2);

	verb = 0;
	bandWidth = 20;

	baseKeysInputFlag = 0;
	videoInputFlag = 0;
	siftDotPrintFlag = 0;

	filterFlag = 0;

	// iniファイルから読み込み
	if(readini){
	
		readIni();
	
	}

	// 画像読み込み
	imgBase.load(imgBaseName);
	setImage(imgBase,IMAGE_ID_BASE);

	if(!videoInputFlag){
		imgInput.load(imgInputName);
		setImage(imgInput,IMAGE_ID_INPUT);
	}


}

void Asift::writeIni(){
	ptree pt;

	// write path
	pt.put("File.BaseImage",imgBaseName);
	pt.put("File.InputImage",imgInputName);
	pt.put("File.OutPutVertImage",imgVName);
	pt.put("File.OutputHoriImage",imgHName);
	pt.put("File.OutputKeyPointsBase",baseKeysName);
	pt.put("File.OutputKeyPointsInput",inputKeysName);
	pt.put("File.OutputMatching",matchingsName);
	pt.put("File.CapInput",capInName);
	pt.put("File.CapOutput",capOutName);
	
	// write Asift parameter
	pt.put("Asift.BaseTilt",baseKeys.getTilts());
	pt.put("Asift.InputTilt",inputKeys.getTilts());
	pt.put("Asift.ResizeFlag",resizeFlag);
	pt.put("Asift.Verb",verb);
	pt.put("Asift.BandWidth",bandWidth);

	// write Options 
	pt.put("Option.BaseFileInput",baseKeysInputFlag);
	pt.put("Option.VideoInput",videoInputFlag);
	pt.put("Option.SiftDotPrint",siftDotPrintFlag);

	// write Sift Parameter
	baseKeys.iniwriteSiftParameters(pt);

	// writing
	write_ini("Asift.ini",pt);
}

void Asift::readIni(){


	if(path.isExist("Asift.ini")){

		ptree pt;
		read_ini("Asift.ini", pt);

		// read path
		imgBaseName = pt.get_optional<std::string>("File.BaseImage").get();
		imgInputName = pt.get_optional<std::string>("File.InputImage").get();
		imgVName = pt.get_optional<std::string>("File.OutPutVertImage").get();
		imgHName = pt.get_optional<std::string>("File.OutputHoriImage").get();
		baseKeysName = pt.get_optional<std::string>("File.OutputKeyPointsBase").get();
		inputKeysName = pt.get_optional<std::string>("File.OutputKeyPointsInput").get();
		matchingsName = pt.get_optional<std::string>("File.OutputMatching").get();
		capInName = pt.get_optional<std::string>("File.CapInput").get();
		capOutName = pt.get_optional<std::string>("File.CapOutput").get();

		// read Asift Parameter
		int tilts1 = pt.get_optional<int>("Asift.BaseTilt").get();
		int tilts2 = pt.get_optional<int>("Asift.InputTilt").get();

		baseKeys = AsiftKeypoints(tilts1);
		inputKeys = AsiftKeypoints(tilts2);
		
		// read Asift Parameter options
		resizeFlag = pt.get_optional<int>("Asift.ResizeFlag").get();
		verb = pt.get_optional<int>("Asift.Verb").get();
		bandWidth = pt.get_optional<int>("Asift.BandWidth").get();

		// read Options
		baseKeysInputFlag = pt.get_optional<int>("Option.BaseFileInput").get();
		videoInputFlag = pt.get_optional<int>("Option.VideoInput").get();
		siftDotPrintFlag = pt.get_optional<int>("Option.SiftDotPrint").get();

		// Read Sift Parameter 
		baseKeys.inireadSiftParameters(pt);
		inputKeys.inireadSiftParameters(pt);

	}else{

		std::cout << "not exist Asift.ini" << endl;
		writeIni();

	}

}

void Asift::run(){
	time_t tstart, tend;
	pro::Timer timer;
	
	/***********************************************************
	 *  Keypoints1 
	 */
	std::cout << "Computing keypoints1 on the two images..." << endl;
	timer.start();

		// キーポイントの計算
		if(!baseKeysInputFlag){
			computeKeyPoints(IMAGE_ID_BASE);
		}else{
			baseKeys.input(baseKeysName);
		}

		// Sift キーポイントの描写
		if(siftDotPrintFlag){
			baseKeys.draw(imgBase);
		}
		
	std::cout << baseKeys.getNum() <<  " ASIFT keypoints are detected." << endl;
	std::cout << "Keypoints1 computation accomplished in " << (double)timer.getDiff()/pro::Timer::PER_SEC << " seconds." << endl << endl;
	//timer.lap();

	
	/***************************************************************
	 * 矩形フィルター処理
	 */
	std::cout << "keypoints1 Rect Filter set..." << endl;
		// 矩形フィルター処理
		setFilterRectanglePoints();
	timer.lap();
		fileterRun();
	std::cout << "pt1(" << baseFilterRect.x << "," << baseFilterRect.y << ") " << flush;
	std::cout << "pt2(" << baseFilterRect.x+baseFilterRect.width << "," 
		<< baseFilterRect.y + baseFilterRect.height << ")" << endl;
	std::cout << baseKeys.getNum() <<  " ASIFT keypoints are detected." << endl;
	std::cout << "Keypoints1 computation accomplished in " << (double)timer.getDiff()/pro::Timer::PER_SEC << " seconds." << endl << endl;
	timer.lap();


	/***************************************************************
	 * 動画読み込み処理
	 */
	if(videoInputFlag){

		// 読み込み動画
		cv::VideoCapture cap(capInName);
		// ファイルがオープンできたかの確認
		if(!cap.isOpened()) return;

		// ビデオライタ
		int fps = 15;
		cv::VideoWriter writerH("H"+capOutName, CV_FOURCC('X','V','I','D'), fps, cv::Size(imgBase.size().width*2+bandWidth,imgBase.size().height));
		cv::VideoWriter writerV("V"+capOutName, CV_FOURCC('X','V','I','D'), fps, cv::Size(imgBase.size().width,imgBase.size().height*2+bandWidth));

		pro::Image frame;

		// 進行状況
		int count = 0;
		int framenum = cap.get(CV_CAP_PROP_FRAME_COUNT);

		// 表示
		std::cout << "video Input is " << capInName << "." << endl;
		std::cout << "video Output is " << capOutName << "." << endl;
		cout << "start" << endl;

		while(1) {
			cap.read((cv::Mat&)frame);  // キャプチャ

			if(frame.empty()){
				break;
			}
			
			setImage(frame,IMAGE_ID_INPUT);

			// フレーム数の進行状況を表示
			std::cout << ++count << "/" << framenum << endl;

			computeKeyPoints(IMAGE_ID_INPUT);	
			std::cout << inputKeys.getNum() <<  " ASIFT keypoints are detected." << endl;

			computeMatching();

			// 水平画像作成
			//frame.imshow("viwe",1);
			if(cv::waitKey(30) >= 0) break;

			writerH << createHoriImage(frame).imshow("view");
			writerV << createVertImage(frame);

		}

	/***************************************************************
	 * 画像読み込み処理
	 */
	}else{

		/***********************************************************
		 *  Keypoints2
		 */
		std::cout << "Computing keypoints2 on the two images..." << endl;

			computeKeyPoints(IMAGE_ID_INPUT);	
			std::cout << inputKeys.getNum() <<  " ASIFT keypoints are detected." << endl;

			//if(siftDotPrintFlag){
			//	inputKeys.draw(imgInput);
			//}

		std::cout << "Keypoints2 computation accomplished in " << (double)timer.getDiff()/pro::Timer::PER_SEC << " seconds." << endl << endl;
		timer.lap();
	
		/***********************************************************
		 *  Keypoints matching
		 */
		std::cout << "Matching the keypoints..." << endl;

			computeMatching();
		
		//std::cout << "The two images match! " << matchings.getNum() << " matchings are identified." << endl;
		std::cout << "Keypoints matching accomplished in " << (double)timer.getDiff()/pro::Timer::PER_SEC << " seconds." << endl << endl;
		timer.lap();

		std::cout << "All computation accomplished in " << (double)timer.getNow()/pro::Timer::PER_SEC << "seconds." << endl << endl;
		
		/***********************************************************
		 *  outputs
		 */
		// 水平画像作成
		createHoriImage(imgInput);
		// 水平画像作成
		createVertImage(imgInput);
		// キーポイントファイル作成
		baseKeys.output(baseKeysName);
		inputKeys.output(inputKeysName);
		// マッチングファイル作成
		matchings.output(matchingsName);
	
	}

}

void Asift::setImage(pro::Image &src,int id){
	if(id==IMAGE_ID_BASE){
		imgBaseGray.grayeScale(src);
		resize(imgBaseGray,imgBaseGrayZoom,baseZoom,resizeFlag);
		unsigned char * iarr1 = imgBaseGrayZoom.getU8Data();
		ipixelsBase = std::vector<float>(iarr1,iarr1 + imgBaseGrayZoom.size().width * imgBaseGrayZoom.size().height);
		delete[] iarr1;
	}else if(IMAGE_ID_INPUT){
		imgInputGray.grayeScale(src);
		resize(imgInputGray,imgInputGrayZoom,inputZoom,resizeFlag);
		unsigned char * iarr2 = imgInputGrayZoom.getU8Data();
		ipixelsInput = std::vector<float>(iarr2,iarr2 + imgInputGrayZoom.size().width * imgInputGrayZoom.size().height);
		delete[] iarr2;
	}
}

void Asift::computeKeyPoints(int id){
	if(id==IMAGE_ID_BASE){
		baseKeys.computeAsiftKeyPoints(ipixelsBase,imgBaseGrayZoom.size().width,imgBaseGrayZoom.size().height,verb,baseZoom);
	}else if(id==IMAGE_ID_INPUT){
		inputKeys.computeAsiftKeyPoints(ipixelsInput,imgInputGrayZoom.size().width,imgInputGrayZoom.size().height,verb,inputZoom);
	}
}

void Asift::computeMatching(){
	matchings = AsiftMatchings(baseKeys,inputKeys);
	matchings.computeAsiftMatches(verb);
}

void Asift::setFilterRectanglePoints(){

	pro::Image base;
	string winName="FilterRectangle";
	base.clone(imgBase);
	setMouseEventId(ON_MOUSE_ID_FILTER_RECT);
	onMouseCount = 0;
	while(1){
		base.imshow(winName);
		imgBase.clone(base);
		if(onMouseCount==0){
			imgBase.circle(cv::Point2f(baseFilterRect.x,baseFilterRect.y),1,cv::Scalar(0,255,0));
		}else if(onMouseCount==1){
			imgBase.circle(cv::Point2f(baseFilterRect.x+baseFilterRect.width
							,baseFilterRect.y+baseFilterRect.height)
							,2,cv::Scalar(255,255,0));
			imgBase.rectangle(cv::Point2f(baseFilterRect.x,baseFilterRect.y)
							,cv::Point2f(baseFilterRect.x+baseFilterRect.width
							,baseFilterRect.y+baseFilterRect.height),cv::Scalar(0,255,0));
		}else{
			break;
		}

		imgBase.imshow(winName);

		cv::waitKey(30);

		cv::setMouseCallback(winName,onMouse,this);
	}

	filterFlag = filterFlag | FILTER_RECT_FLAG;
}

void Asift::fileterRun(){
	if((filterFlag & FILTER_RECT_FLAG) == 1){
		baseKeys.filterRectangle(cv::Point2f(baseFilterRect.x,baseFilterRect.y)
								,cv::Point2f(baseFilterRect.x+baseFilterRect.width
								,baseFilterRect.y+baseFilterRect.height));
	}
}


void Asift::setMouseEventId(int id){
	onMouseId = id;
}

void Asift::onMouse_impl(int event,int x,int y,int flag){
	if(onMouseId==ON_MOUSE_ID_FILTER_RECT)
		onMouse_filterRect(event,x,y,flag);
}

void Asift::onMouse_filterRect(int event,int x,int y,int flag){
	switch(event) {
		case cv::EVENT_MOUSEMOVE:
			if(onMouseCount==0){
				baseFilterRect.x = x;
				baseFilterRect.y = y;
			}else{
				baseFilterRect.width = x-baseFilterRect.x;
				baseFilterRect.height = y-baseFilterRect.y;
			}
			break;
		case cv::EVENT_LBUTTONDOWN:
			if(onMouseCount==0){
				baseFilterRect.x = x;
				baseFilterRect.y = y;
				onMouseCount++;
			}else{
				baseFilterRect.width = x-baseFilterRect.x;
				baseFilterRect.height = y-baseFilterRect.y;
				onMouseCount++;
			}
			break;
		case cv::EVENT_RBUTTONDOWN:
			if(onMouseCount>0){
				onMouseCount--;
			}
			break;
		default:
			break;
	}
}

void Asift::resize(pro::Image &src,pro::Image &dst,float &zoom,int resizeFlag){
	if(!resizeFlag){
		dst.clone(src);
		zoom = 1;
	}else{
		//std::cout << "WARNING: The input images are resized to " << IMAGE_X << "x" << IMAGE_Y << " for ASIFT. " << endl 
		//<< "         But the results will be normalized to the original image size." << endl << endl;
		
		float InitSigma_aa = 1.6;
		
		int wS,hS,w,h;
				
		float areaS = IMAGE_X * IMAGE_Y;

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
		
	}
}

pro::Image Asift::createVertImage(pro::Image img){
	pro::Image vertImg;
	vertImg.vertconcat(imgBase,img,bandWidth);
	matchingslist::iterator ptr = matchings.matchings.begin();
	for(int i=0; i < (int) matchings.matchings.size(); i++, ptr++)
	{		
		vertImg.line(cv::Point2f((baseZoom*ptr->first.x),(baseZoom*ptr->first.y)), 
			cv::Point2f((inputZoom*ptr->second.x),(inputZoom*ptr->second.y) + imgBase.size().height + bandWidth),cv::Scalar::all(255));
	}
	if(!videoInputFlag)
		vertImg.save(imgVName);
	return vertImg;
}

pro::Image Asift::createHoriImage(pro::Image img){
	pro::Image horiImg;
	horiImg.horiconcat(imgBase,img,bandWidth);
	matchingslist::iterator ptr = matchings.matchings.begin();
	for(int i=0; i < (int) matchings.matchings.size(); i++, ptr++)
	{		
		horiImg.line(cv::Point2f((baseZoom*ptr->first.x),(baseZoom*ptr->first.y)), 
			cv::Point2f((inputZoom*ptr->second.x) + imgBase.size().width + bandWidth,(inputZoom*ptr->second.y)),cv::Scalar::all(255));
	}
	if(!videoInputFlag)
		horiImg.save(imgHName);
	return horiImg;
}