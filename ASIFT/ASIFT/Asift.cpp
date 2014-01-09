// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#include "Asift.h"


Asift::Asift(void)
{
	init(1);
}

Asift::Asift(string ini_name,string sift_param_ini_name){
	init(1,ini_name,sift_param_ini_name);
}

Asift::~Asift(void)
{
}

void Asift::init(int readini,string ini_name,string sift_param_ini_name){
			//string imgBase_name,string imgInput_name,
			//string imgV_name,string imgH_name,
			//string baseKeys_name,string inputKeys_name,
			//string matchings_name,string capIn_name,string capOut_name,
			//int tilt1,int tilt2,int resize_flag){

	/**********************************************
	 * 設定初期化
	 */
	path = pro::Dir();

	/**********************************************
	 * iniファイルから読み込み
	 */
	if(readini){
		readIni(ini_name);
	}else{
		defaultParam();
	}
	
	initNames();
}

void Asift::defaultParam(){
		// 設定ファイル名
	//iniFileName = "Asift.ini";
	iniSiftParamName = "SiftParam.ini";

	// ファイル名初期化
	imgBaseName="imgBase.png";
	imgInputName="imgInput.png";
	//imgVName="imgOutVert.png";
	//imgHName="imgOutHori.png";
	//baseKeysName = "baseKeys.txt";
	//inputKeysName = "inputKeys.txt";
	//matchingsName = "matchings.txt";
	capInName = "capin.avi";
	//capOutName = "capout.avi";
	//xAxisKeysName = "xAxisKeys.txt";

	// ディレクトリ初期化
	videoOutputDir = pro::Dir("videoOutput",false);

	// リサイズフラグ初期化
	resizeFlag = 1;

	// キーポイント初期化
	initKeys(IMAGE_ID_BASE,7);
	initKeys(IMAGE_ID_INPUT,7);
	//baseKeys = AsiftKeypoints(7,iniSiftParamName);
	//inputKeys = AsiftKeypoints(7,iniSiftParamName);

	// メッセージ表示フラグ初期化
	verb = 0;
	// 連結空白幅初期化
	bandWidth = 20;

	// リサイズサイズ
	resizeWidth = IMAGE_X;
	resizeHeight = IMAGE_Y;

	// オプション初期化
	baseKeysInputFlag = 0;
	videoInputFlag = 0;
	siftDotPrintFlag = 0;
	filterRectOnMouseFlag = 0;

	// フィルターフラグ初期化
	filterFlag = 0;

	// マウスイベント系初期化
	onMouseId = 0;
	onMouseCount = 0;

	// X軸キーポイントの初期化
	//xAxisKeys = AsiftKeypoints(7,iniSiftParamName);
	centerLineDistance = 20;

}

void Asift::initImages(){

	/**********************************************
	 * 画像読み込み
	 */
	// ベース画像読み込み
	imgBase.load(imgBaseName);
	setImage(imgBase,IMAGE_ID_BASE);

	// インプット読み込み
	if(!videoInputFlag){
		imgInput.load(imgInputName);
		setImage(imgInput,IMAGE_ID_INPUT);
	}

	/**********************************************
	 * そのほかの処理の初期化
	 */
	// 矩形フィルターを画像全体に初期化
	filterRect = cv::Rect(0,0,imgBase.size().width,imgBase.size().height);
	centerPt.x = (filterRect.width - filterRect.x)/2;
	centerPt.y = (filterRect.height- filterRect.y)/2;
}

void Asift::initNames(){
	
	// ベース名取得
	baseName = pro::Dir::getStem(imgBaseName);
	// ベース拡張子取得
	baseExtention = pro::Dir::getExtention(imgBaseName);

	// キーポイント名設定
	baseKeysName = baseName + "_Keys.txt";
	xAxisKeysName = baseName + "_xAxisKeys.txt";

	// インプットビデオ系設定
	if(videoInputFlag){
		inputName = pro::Dir::getStem(capInName);
		inputExtention = pro::Dir::getExtention(capInName);
		capVName = inputName + "_Vert" + inputExtention;
		capHName = inputName + "_Heri" + inputExtention;
	// インプット画像系設定
	}else{
		inputName = pro::Dir::getStem(imgInputName);
		inputExtention = pro::Dir::getExtention(imgInputName);
	}
	
	// 水平・垂直画像名設定
	imgVName = inputName + "_Vert" + baseExtention;
	imgHName = inputName + "_Heri" + baseExtention;
	
	// インプットキーポイント名設定
	inputKeysName = inputName + "_Keys.txt";

	// マッチング名設定
	matchingsName = inputName + "_Matching.txt";
}

void Asift::initKeys(int id,int tilts){
	if(id == IMAGE_ID_BASE){
		baseKeys = AsiftKeypoints(tilts,iniSiftParamName);
	}else if(id == IMAGE_ID_INPUT){
		inputKeys = AsiftKeypoints(tilts,iniSiftParamName);
	}
}

void Asift::writeIni(ptree &pt){
	// write Path
	pt.put("Path.VideoOutput",videoOutputDir.getFileName());

	// write ini file
	pt.put("Ini.SiftParam",iniSiftParamName);

	// write File
	pt.put("File.BaseImage",imgBaseName);
	pt.put("File.InputImage",imgInputName);
	//pt.put("File.OutPutVertImage",imgVName);
	//pt.put("File.OutputHoriImage",imgHName);
	//pt.put("File.OutputKeyPointsBase",baseKeysName);
	//pt.put("File.OutputKeyPointsInput",inputKeysName);
	//pt.put("File.OutputMatching",matchingsName);
	pt.put("File.CapInput",capInName);
	//pt.put("File.CapOutput",capOutName);
	//pt.put("File.XAxisKeys",xAxisKeysName);
	
	// write Asift parameter
	pt.put("Asift.BaseTilt",baseKeys.getTilts());
	pt.put("Asift.InputTilt",inputKeys.getTilts());
	pt.put("Asift.ResizeFlag",resizeFlag);
	pt.put("Asift.Verb",verb);
	pt.put("Asift.BandWidth",bandWidth);

	// write OptionFlag
	pt.put("OptionFlag.BaseFileInput",baseKeysInputFlag);
	pt.put("OptionFlag.VideoInput",videoInputFlag);
	pt.put("OptionFlag.VideoKeypointsOutput",videoKeypointsOutputFlag);
	pt.put("OptionFlag.SiftDotPrint",siftDotPrintFlag);
	pt.put("OptionFlag.FilterRectOnMouse",filterRectOnMouseFlag);

	// write OptionValue
	pt.put("OptionValue.ResizeWidth",resizeWidth);
	pt.put("OptionValue.ResizeHeight",resizeHeight);
	pt.put("OptionValue.CenterLineDistance",centerLineDistance);
}

void Asift::readIni(ptree &pt){

	// read path
	videoOutputDir = pro::Dir(pt.get_optional<std::string>("Path.VideoOutput").get(),false);

	// read ini file
	iniSiftParamName = pt.get_optional<std::string>("Ini.SiftParam").get();

	// read file
	imgBaseName = pt.get_optional<std::string>("File.BaseImage").get();
	imgInputName = pt.get_optional<std::string>("File.InputImage").get();
	//imgVName = pt.get_optional<std::string>("File.OutPutVertImage").get();
	//imgHName = pt.get_optional<std::string>("File.OutputHoriImage").get();
	//baseKeysName = pt.get_optional<std::string>("File.OutputKeyPointsBase").get();
	//inputKeysName = pt.get_optional<std::string>("File.OutputKeyPointsInput").get();
	//matchingsName = pt.get_optional<std::string>("File.OutputMatching").get();
	capInName = pt.get_optional<std::string>("File.CapInput").get();
	//capOutName = pt.get_optional<std::string>("File.CapOutput").get();
	//xAxisKeysName = pt.get_optional<std::string>("File.XAxisKeys").get();

	// read Asift Parameter
	int tilts1 = pt.get_optional<int>("Asift.BaseTilt").get();
	int tilts2 = pt.get_optional<int>("Asift.InputTilt").get();
		
	// read Asift Parameter options
	resizeFlag = pt.get_optional<int>("Asift.ResizeFlag").get();
	verb = pt.get_optional<int>("Asift.Verb").get();
	bandWidth = pt.get_optional<int>("Asift.BandWidth").get();

	// read OptionFlag
	baseKeysInputFlag = pt.get_optional<int>("OptionFlag.BaseFileInput").get();
	videoInputFlag = pt.get_optional<int>("OptionFlag.VideoInput").get();
	videoKeypointsOutputFlag = pt.get_optional<int>("OptionFlag.VideoKeypointsOutput").get();
	siftDotPrintFlag = pt.get_optional<int>("OptionFlag.SiftDotPrint").get();
	filterRectOnMouseFlag = pt.get_optional<int>("OptionFlag.FilterRectOnMouse").get();

	// read OptionValue
	resizeWidth = pt.get_optional<int>("OptionValue.ResizeWidth").get();
	resizeHeight = pt.get_optional<int>("OptionValue.ResizeHeight").get();
	centerLineDistance = pt.get_optional<int>("OptionValue.CenterLineDistance").get();
			
	initKeys(IMAGE_ID_BASE,tilts1);
	initKeys(IMAGE_ID_INPUT,tilts2);
	//baseKeys = AsiftKeypoints(tilts1,iniSiftParamName);
	//inputKeys = AsiftKeypoints(tilts2,iniSiftParamName);
}

void Asift::writeIni(std::string ini_file_name){

	/**********************************************
	 * asift ptree
	 */
	ptree pt;

	writeIni(pt);

	// writing
	write_ini(ini_file_name,pt);

}

void Asift::readIni(std::string ini_file_name){

	
	/**********************************************
	 * asift ini read
	 */
	ptree pt;
	if(path.isExist(ini_file_name)){

		read_ini(ini_file_name, pt);

		readIni(pt);

	}else{

		std::cout << "not exist " << ini_file_name << endl;
		
		defaultParam();
		writeIni(ini_file_name);

	}

}

void Asift::run(){
	time_t tstart, tend;
	pro::Timer timer;
	
	/***********************************************************
	 *  画像初期化処理
	 */
	initImages();
	
	/***********************************************************
	 *  Keypoints BaseImage
	 */
	std::cout << "Computing keypoints1 on the two images..." << endl;
	timer.start();

		// キーポイントの計算
		if(!baseKeysInputFlag){
			// キーポイント算出
			computeKeyPoints(IMAGE_ID_BASE);
		}else{
			// ファイル入力
			input(INPUT_ID_KEYS_BASE);
			input(INPUT_ID_KEYS_XAXIS);
			//baseKeys.input(baseKeysName);
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
	if(filterRectOnMouseFlag){
		std::cout << "keypoints1 Rect Filter set..." << endl;
			// 矩形フィルターセット
			setFilterRect(imgBase);
		timer.lap();
			// フィルタ実行
			fileterRun(baseKeys);
		std::cout << "pt1(" << filterRect.x << "," << filterRect.y << ") " << flush;
		std::cout << "pt2(" << filterRect.x+filterRect.width << "," 
			<< filterRect.y + filterRect.height << ")" << endl;
		std::cout << baseKeys.getNum() <<  " ASIFT keypoints are detected." << endl;
		std::cout << "Keypoints1 computation accomplished in " << (double)timer.getDiff()/pro::Timer::PER_SEC << " seconds." << endl << endl;
		timer.lap();
	}

	/***************************************************************
	 * X軸付近の特徴点取得
	 */
	setCenterLinePoints(baseKeys);
	//xAxisKeys.draw(imgBase);
	//xAxisKeys.output(xAxisKeysName);
	output(OUTPUT_ID_KEYS_XAXIS);
	
	/***************************************************************
	 * Base Keypoints ファイル出力
	 */
	//baseKeys.output(baseKeysName);
	output(OUTPUT_ID_KEYS_BASE);

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
		cv::VideoWriter writerH(capHName, CV_FOURCC('X','V','I','D'), fps, cv::Size(imgBase.size().width*2+bandWidth,imgBase.size().height));
		cv::VideoWriter writerV(capVName, CV_FOURCC('X','V','I','D'), fps, cv::Size(imgBase.size().width,imgBase.size().height*2+bandWidth));

		pro::Image frame;

		// 進行状況
		int count = 0;
		int framenum = cap.get(CV_CAP_PROP_FRAME_COUNT);

		// 表示
		std::cout << "video Input is " << capInName << "." << endl;
		std::cout << "video Output is " << capHName << " " << capVName << "." << endl;
		cout << "start" << endl;

		// ファイル出力用のディレクトリ作成
		if(videoKeypointsOutputFlag){
			videoOutputDir.create(pro::Dir::ANOTHER_CREATE|pro::Dir::CREATE_DIRS);
			inputKeys.path.cd(videoOutputDir.pwd());
			matchings.path.cd(videoOutputDir.pwd());
			cout << endl;
		}

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

			computeMatching(baseKeys,inputKeys);
			
			// ファイル出力処理
			if(videoKeypointsOutputFlag){
				stringstream ssInput,ssMatching;
				ssInput << pro::Dir::getStem(inputKeysName) << "_" <<
					count << pro::Dir::getExtention(inputKeysName);
				ssMatching << pro::Dir::getStem(matchingsName) << "_" <<
					count << pro::Dir::getExtention(matchingsName);
				inputKeys.output(ssInput.str());
				matchings.output(ssMatching.str());
			}

			// 水平画像作成
			//frame.imshow("viwe",1);
			if(cv::waitKey(30) >= 0) break;

			writerH << createHoriImage(frame).imshow("view",1);
			writerV << createVertImage(frame);

		}

	/***************************************************************
	 * 画像読み込み処理
	 */
	}else{

		/***********************************************************
		 *  Keypoints Input Image
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

			computeMatching(baseKeys,inputKeys);
			//matchings.filterMatching();
			//matchings.asiftKeys1->draw(imgBase);
			//matchings.asiftKeys2->draw(imgInput);
			//std::cout << "keys1:" << matchings.asiftKeys1->keypointsTotal() << std::endl;
			//std::cout << "keys2:" << matchings.asiftKeys2->keypointsTotal() << std::endl;
			//std::cout << "matching:" << matchings.matchings.size() << std::endl;

		//std::cout << "The two images match! " << matchings.getNum() << " matchings are identified." << endl;
		std::cout << "Keypoints matching accomplished in " << (double)timer.getDiff()/pro::Timer::PER_SEC << " seconds." << endl << endl;
		timer.lap();

		std::cout << "All computation accomplished in " << (double)timer.getNow()/pro::Timer::PER_SEC << "seconds." << endl << endl;
		
		/***********************************************************
		 *  outputs
		 */
		// 水平画像作成
		//createHoriImage(imgInput);
		output(OUTPUT_ID_HORI,imgInput);
		// 水平画像作成
		//createVertImage(imgInput);
		output(OUTPUT_ID_VERT,imgInput);
		// キーポイントファイル作成
		output(OUTPUT_ID_KEYS_INPUT);
		//inputKeys.output(inputKeysName);
		// マッチングファイル作成
		output(OUTPUT_ID_MATCHING);
		//matchings.output(matchingsName);
	
	}

}

void Asift::markerCreate(std::string markerName,int tilts,int rectFilterFlag,int imageShow){
	
	pro::Timer timer;

	pro::Image imgMarker;
	AsiftKeypoints markerKeys = AsiftKeypoints(tilts);
	markerKeys.path = path;

	/***************************************************************
	 * 画像読み込み
	 */
	imgMarker.load(path,markerName);
	setImage(imgMarker,IMAGE_ID_ELSE,markerKeys);
	
	/***************************************************************
	 * キーポイントの計算
	 */
	std::cout << "Computing Marker on the image..." << endl;
	timer.start();

		// キーポイント算出
		computeKeyPoints(IMAGE_ID_ELSE,markerKeys);
		markerKeys.draw(imgMarker);
		
	std::cout << markerKeys.getNum() <<  " ASIFT keypoints are detected." << endl;
	std::cout << "markerKeys computation accomplished in " << (double)timer.getDiff()/pro::Timer::PER_SEC << " seconds." << endl << endl;
	
	/***************************************************************
	 * 矩形フィルター処理
	 */
	if(rectFilterFlag){
		std::cout << "markerKeys Rect Filter set..." << endl;
			// 矩形フィルター処理
			setFilterRect(imgMarker);
		timer.lap();
			fileterRun(markerKeys);
		std::cout << "pt1(" << filterRect.x << "," << filterRect.y << ") " << flush;
		std::cout << "pt2(" << filterRect.x+filterRect.width << "," 
			<< filterRect.y + filterRect.height << ")" << endl;
		std::cout << markerKeys.getNum() <<  " ASIFT keypoints are detected." << endl;
		std::cout << "markerKeys computation accomplished in " << (double)timer.getDiff()/pro::Timer::PER_SEC << " seconds." << endl << endl;
		timer.lap();
	}

	/***************************************************************
	 * X軸付近の特徴点取得
	 */
	setCenterLinePoints(markerKeys);
	xAxisKeys.output(pro::Dir::getStem(markerName)+"_xAxisKeys.txt");
	// 一次連続ベクター
	//xAxisKeys.setOnceKeys();
	xAxisKeys.outputOnce(pro::Dir::getStem(markerName)+"_xAxisOnceKeys.txt");
	
	/***************************************************************
	 * Base Keypoints ファイル出力
	 */
	markerKeys.output(pro::Dir::getStem(markerName)+"_Keys.txt");
	// 一次連続ベクター
	//markerKeys.setOnceKeys();
	markerKeys.outputOnce(pro::Dir::getStem(markerName)+"_OnceKeys.txt");
	
	/***************************************************************
	 * 選択されたキーポイントを描写
	 */
	if(imageShow){
		imgMarker.load(path,markerName);
		markerKeys.draw(imgMarker);
		xAxisKeys.draw(imgMarker,cv::Scalar(255,255,0),cv::Scalar(255,255,0));
		imgMarker.imshow(pro::Dir::getStem(markerName),1);
		cv::waitKey(0);
	}

}

void Asift::setImage(pro::Image &src,int id,AsiftKeypoints &keys){
	if(id==IMAGE_ID_BASE){
		baseKeys.setImage(src,resizeFlag,resizeWidth,resizeHeight,baseZoom);
	}else if(id==IMAGE_ID_INPUT){
		inputKeys.setImage(src,resizeFlag,resizeWidth,resizeHeight,inputZoom);
	}else if(id==IMAGE_ID_ELSE){
		keys.setImage(src,resizeFlag,resizeWidth,resizeHeight);
	}
}

void Asift::setNames(std::string imgBaseName,std::string imgInputName,std::string videoInputName){
	this->imgBaseName = imgBaseName;
	this->imgInputName = imgInputName;
	this->capInName = videoInputName;

	initNames();
}

void Asift::computeKeyPoints(int id,AsiftKeypoints &keys){
	if(id==IMAGE_ID_BASE){
		baseKeys.computeAsiftKeyPoints(verb);
	}else if(id==IMAGE_ID_INPUT){
		inputKeys.computeAsiftKeyPoints(verb);
	}else if(id==IMAGE_ID_ELSE){
		keys.computeAsiftKeyPoints(verb);
	}
}

void Asift::computeMatching(AsiftKeypoints keys1,AsiftKeypoints keys2){
	matchings.setKeypoints(keys1,keys2);
	matchings.computeAsiftMatches(verb);
}

void Asift::setFilterRect(pro::Image &src){

	pro::Image base;
	string winName="FilterRectangle";
	base.clone(src);

	setMouseEventId(ON_MOUSE_ID_FILTER_RECT);
	onMouseCount = 0;

	while(1){

		base.imshow(winName);
		src.clone(base);

		if(onMouseCount==0){
			src.circle(cv::Point2f(filterRect.x,filterRect.y),1,cv::Scalar(0,255,0));
		}else if(onMouseCount==1){
			src.circle(cv::Point2f(filterRect.x+filterRect.width
							,filterRect.y+filterRect.height)
							,2,cv::Scalar(255,255,0));
			src.rectangle(cv::Point2f(filterRect.x,filterRect.y)
							,cv::Point2f(filterRect.x+filterRect.width
							,filterRect.y+filterRect.height),cv::Scalar(0,255,0));
		}else{
			break;
		}

		src.imshow(winName);

		cv::waitKey(30);

		cv::setMouseCallback(winName,onMouse,this);
	}
	
	centerPt = getCenterPoint(cv::Point2f(filterRect.x,filterRect.y),
		cv::Point2f(filterRect.x+filterRect.width,filterRect.y+filterRect.height));

	filterFlag = filterFlag | FILTER_RECT_FLAG;

}


/**************
 * 作成中
 */
void Asift::setFilterDelRect(pro::Image &src){

}

cv::Point2f Asift::getCenterPoint(cv::Point2f pt1,cv::Point2f pt2){
	
	int maxX,maxY,minX,minY;
	cv::Point2f center;

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

	center.x = (maxX-minX)/2 + minX;
	center.y = (maxY-minY)/2 + minY;

	return center;
}

void Asift::setCenterLinePoints(AsiftKeypoints &keys){
	xAxisKeys = AsiftKeypoints(keys);
	xAxisKeys.filterCenterLine(centerPt,centerLineDistance);
}

void Asift::fileterRun(AsiftKeypoints &keys){
	if((filterFlag & FILTER_RECT_FLAG) == 1){
		keys.filterRectangle(cv::Point2f(filterRect.x,filterRect.y)
								,cv::Point2f(filterRect.x+filterRect.width
								,filterRect.y+filterRect.height));
	}
	if((filterFlag & FILTER_RECT_DEL_FLAG) == 1){
		keys.filterRectangleDel(filterDelRect,filterDelRectNum);
	}
	filterFlag = 0;
}

void Asift::input(int id){
	if(id == INPUT_ID_KEYS_BASE){
		baseKeys.input(baseKeysName);
	}else if(id == INPUT_ID_KEYS_XAXIS){
		xAxisKeys.input(xAxisKeysName);
	}
}

void Asift::output(int id,pro::Image src){
	if(id == OUTPUT_ID_KEYS_BASE){
		baseKeys.output(baseKeysName);
	}else if(id == OUTPUT_ID_KEYS_INPUT){
		inputKeys.output(inputKeysName);
	}else if(id == OUTPUT_ID_MATCHING){
		matchings.output(matchingsName);
	}else if(id == OUTPUT_ID_VERT){
		createVertImage(src);
	}else if(id == OUTPUT_ID_HORI){
		createHoriImage(src);
	}else if(id == OUTPUT_ID_KEYS_XAXIS){
		xAxisKeys.output(xAxisKeysName);
	}
}

void Asift::setMouseEventId(int id){
	onMouseId = id;
}

void Asift::onMouse_impl(int event,int x,int y,int flag){
	if(onMouseId==ON_MOUSE_ID_FILTER_RECT)
		onMouse_filterRect(event,x,y,flag);
	else if(onMouseId==ON_MOUSE_ID_FILTER_DEL_RECT){
		onMouse_filterDelRect(event,x,y,flag);
	}
}

void Asift::onMouse_filterRect(int event,int x,int y,int flag){
	switch(event) {
		case cv::EVENT_MOUSEMOVE:
			if(onMouseCount==0){
				filterRect.x = x;
				filterRect.y = y;
			}else{
				filterRect.width = x-filterRect.x;
				filterRect.height = y-filterRect.y;
			}
			break;
		case cv::EVENT_LBUTTONDOWN:
			if(onMouseCount==0){
				filterRect.x = x;
				filterRect.y = y;
				onMouseCount++;
			}else{
				filterRect.width = x-filterRect.x;
				filterRect.height = y-filterRect.y;
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


/**************
 * 作成中
 */
void Asift::onMouse_filterDelRect(int event,int x,int y,int flag){

}


//void Asift::resize(pro::Image &src,pro::Image &dst,float &zoom,int resizeFlag){
//	if(!resizeFlag){
//		dst.clone(src);
//		zoom = 1;
//	}else{
//		//std::cout << "WARNING: The input images are resized to " << IMAGE_X << "x" << IMAGE_Y << " for ASIFT. " << endl 
//		//<< "         But the results will be normalized to the original image size." << endl << endl;
//		
//		float InitSigma_aa = 1.6;
//		
//		int wS,hS,w,h;
//				
//		float areaS = IMAGE_X * IMAGE_Y;
//
//		w = src.size().width;
//		h = src.size().height;
//
//		float area1 = w * h;
//		zoom = sqrt(area1/areaS);
//		
//		wS = (int) (w / zoom);
//		hS = (int) (h / zoom);
//		
//		/* Anti-aliasing filtering along vertical direction */
//		if ( zoom > 1 )
//		{
//			int ksize;
//			float sigma_aa = InitSigma_aa * zoom / 2;
//			const float GaussTruncate1 = 4.0;
//			ksize = (int)(2.0 * GaussTruncate1 * sigma_aa + 1.0);
//			ksize = MAX(3, ksize);    /* Kernel must be at least 3. */
//			if (ksize % 2 == 0)       /* Make kernel size odd. */
//				ksize++;
//			assert(ksize < 100);
//			cv::GaussianBlur((const cv::Mat&)src, (cv::Mat&)dst, cv::Size(ksize,ksize), sigma_aa, sigma_aa);
//		}
//			
//		dst.resize(src,cv::Size(wS,hS));
//		
//	}
//}

pro::Image Asift::createVertImage(pro::Image img,AsiftMatchings matchings,std::string name){
	pro::Image vertImg;
	//vertImg.vertconcat(imgBase,img,bandWidth);
	//matchingslist::iterator ptr = matchings.matchings.begin();
	//for(int i=0; i < (int) matchings.matchings.size(); i++, ptr++)
	//{		
	//	vertImg.line(cv::Point2f((baseZoom*ptr->first.x),(baseZoom*ptr->first.y)), 
	//		cv::Point2f((inputZoom*ptr->second.x),(inputZoom*ptr->second.y) + imgBase.size().height + bandWidth),cv::Scalar::all(255));
	//}
	vertImg = matchings.drawVertImage(imgBase,img,bandWidth,name);
	if(!videoInputFlag)
		vertImg.save(name);
	return vertImg;
}

pro::Image Asift::createVertImage(pro::Image img){
	return createVertImage(img,matchings,imgVName);
}

pro::Image Asift::createHoriImage(pro::Image img,AsiftMatchings matchings,std::string name){
	pro::Image horiImg;
	//horiImg.horiconcat(imgBase,img,bandWidth);
	//matchingslist::iterator ptr = matchings.matchings.begin();
	//for(int i=0; i < (int) matchings.matchings.size(); i++, ptr++)
	//{		
	//	horiImg.line(cv::Point2f((baseZoom*ptr->first.x),(baseZoom*ptr->first.y)), 
	//		cv::Point2f((inputZoom*ptr->second.x) + imgBase.size().width + bandWidth,(inputZoom*ptr->second.y)),cv::Scalar::all(255));
	//}
	horiImg = matchings.drawHoriImage(imgBase,img,bandWidth,name);
	if(!videoInputFlag)
		horiImg.save(name);
	return horiImg;
}

pro::Image Asift::createHoriImage(pro::Image img){
	return createHoriImage(img,matchings,imgHName);
}
