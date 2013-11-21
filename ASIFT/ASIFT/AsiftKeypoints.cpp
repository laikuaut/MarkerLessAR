#include "AsiftKeypoints.h"


AsiftKeypoints::AsiftKeypoints(int tilts)
{
	path = pro::Dir();
	this->tilts = tilts;
	inireadSiftParameters();
	//default_sift_parameters(siftparams);
	num = 0;
	w = h = 0;
}

AsiftKeypoints::~AsiftKeypoints(void)
{
}

int AsiftKeypoints::computeAsiftKeyPoints(vector<float>& image, int width, int height, int verb,float zoom)
{
	w = width,h = height;
	this->zoom = zoom;
	num = compute_asift_keypoints(image,width,height,tilts,verb,keys,siftparams);
	return num;
}

int AsiftKeypoints::getNum() const{
	return num;
}

void AsiftKeypoints::inireadSiftParameters(){

	if(path.isExist("Asift.ini")){

		ptree pt;
		read_ini("Asift.ini", pt);

		siftparams.OctaveMax	  = pt.get_optional<int>("SIFTParams.OctaveMax").get();
		siftparams.DoubleImSize	  = pt.get_optional<int>("SIFTParams.DoubleImSize"  ).get();
		siftparams.order		  = pt.get_optional<int>("SIFTParams.order"		  ).get();
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
		siftparams.MagFactor	  = pt.get_optional<int>("SIFTParams.MagFactor"	  ).get();
		siftparams.IndexSigma	  = pt.get_optional<float>("SIFTParams.IndexSigma"	  ).get();
		siftparams.IgnoreGradSign = pt.get_optional<int>("SIFTParams.IgnoreGradSign").get();
		siftparams.MatchRatio	  = pt.get_optional<float>("SIFTParams.MatchRatio"	  ).get();
		siftparams.MatchXradius	  = pt.get_optional<float>("SIFTParams.MatchXradius"  ).get();
		siftparams.MatchYradius	  = pt.get_optional<float>("SIFTParams.MatchYradius"  ).get();

	}else{
		default_sift_parameters(siftparams);
		iniwriteSiftParameters();
	}

}

void AsiftKeypoints::iniwriteSiftParameters(){
	iniwriteSiftParameters(siftparams);	
}

void AsiftKeypoints::iniwriteSiftParameters(siftPar par){
	ptree pt;
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
	write_ini("Asift.ini" , pt);
}

void AsiftKeypoints::output(string name){
	std::ofstream of;
	of.open(path.pwd(name));

	if (of.is_open())
	{
		// Follow the same convention of David Lowe: 
		// the first line contains the number of keypoints and the length of the desciptors (128)
		of << tilts << " " << w << " " << h << " " << zoom << std::endl;  
		of << getNum() << "  " << VecLength << "  " << std::endl;
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
