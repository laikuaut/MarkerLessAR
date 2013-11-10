
#include "Labeling.h"

namespace pro{

//Labeling::Labeling(){
//
//}

Labeling::Labeling(){

}

void Labeling::init(cv::Size size){
	this->w = size.width;
	this->h = size.height;
	labels.resize(w*h);
	memset(&labels[0],0,sizeof(int)*w*h);
}

void Labeling::labeling(const unsigned char* bins){

	int nsort[5];
	int map[30000];	

	map[0] = 0;

	for(int y=1;y<h-1;y++){
		for(int x=1;x<w-1;x++){
			int pos = x+w*y;
			labels[pos] = (bins[pos]);
		}
	}

	for(int x=0;x<w;x++){
		labels[x] = labels[x+w*(h-1)] = 0;
	}
	for(int y=0;y<h;y++){
		labels[w*y] = labels[w-1+w*y] = 0;
	}

	int label = 0;
	for(int y=1;y<h-1;y++){
		for(int x=1;x<w-1;x++){

			map[0] = 0;

			int pos = x+w*y;
			if(labels[pos] == 0){
				continue;
			}

			nsort[0] = 0;
			nsort[1] = labels[pos-1-w];
			nsort[2] = labels[pos-w];
			nsort[3] = labels[pos+1-w];
			nsort[4] = labels[pos-1];

			int flag, countk, i;
			for(flag = 1; flag == 1;){
				for(flag = 0, i = 2; i < 5; i++){
					if (nsort[i] < nsort[i-1]){
						countk = nsort[i];
						nsort[i] = nsort[i-1];
						nsort[i-1] = countk;
						flag = 1;
					}
				}
			}
			countk = 0;
			
			for(i = 1; i < 5; i++){
				if(nsort[i] == 0){
					continue;
				}
				if(nsort[i] == nsort[i-1]){
					continue;
				}
				countk++;
				nsort[countk] = nsort[i];
			}

			i = countk;
			if(i == 0){
				label++;
				labels[pos] = label;
				map[label] = label;
			}
			else if(i == 1){
				countk = nsort[1];
				while(map[countk] != countk){
					countk = map[countk];
				}
				labels[pos]=countk;
			}
			else if(i > 1){
				countk = nsort[1];
				while(map[countk] != countk){
					countk = map[countk];
				}
				labels[pos] = countk;
				for (int p = 2; p <= i; p++){
					map[nsort[p]] = countk;
				}
			}		
		}
	}

	num=0;

	for(int labelnumber=1;labelnumber<=label;labelnumber++){
		if(map[labelnumber] == labelnumber){
			num++;                       
			map[labelnumber] = num;
		}
		else{
			map[labelnumber] = map[map[labelnumber]];
		}
	}

	num++;

	for(int x=1;x<w-1;x++){
		for(int y=1;y<h-1;y++){
			int pos = x+w*y;
			if(labels[pos] != 0){
				labels[pos] = map[labels[pos]];
			}
		}
	}

}

int& Labeling::operator() (const int x, const int y)
{
	return labels[x+w*y]; 
}

const int& Labeling::operator() (const int x, const int y) const 
{
	return labels[x+w*y]; 
}

}