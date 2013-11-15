#include "AsiftKeypoints.h"


AsiftKeypoints::AsiftKeypoints(int tilts)
{
	path = pro::Dir();
	this->tilts = tilts;
	default_sift_parameters(siftparams);
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
