#include "Dir.h"

#include<iostream>
#include<cstring>
#include<fstream>
#include<sstream>
#include<cstdio>
#include<boost/regex.hpp>

using namespace std;
namespace fs = boost::filesystem;

namespace pro{

Dir::Dir(void){
	path = fs::system_complete(".");
	ErrorShow = false;
}

Dir::Dir(bool flag)
{
	path = fs::system_complete(".");
	ErrorShow = flag;
}

Dir::Dir(const fs::path& path,bool flag){
	if(!isPath(path))
		throw DirException(DirException::PATH_ERROR,path.string(),"Dir.cpp","Dir::Dir(boost::filesystem::path&)",__LINE__);
	this->path = fs::system_complete(path);
	ErrorShow = flag;
}

Dir::~Dir(void)
{

}

bool Dir::isPath(const fs::path& path){
	fs::path pth = path.string();
	string str_path;
	if(pth.is_absolute()){
		str_path = pth.relative_path().string();
	}
	return !(boost::regex_search(str_path.begin(), str_path.end(), boost::regex(DIR_REGEX_STR)));
}

const std::string Dir::pwd() const{
	return path.string();
}

const std::string Dir::pwd(const std::string& name) const{
	return path.string() + "/" + name;
}

bool Dir::isExist() const{
	return fs::exists(path);
}

bool Dir::isExist(const string& name) const{
	return fs::exists(pwd(name));
}

bool Dir::isDirectory() const{
	return fs::is_directory(path);
}

bool Dir::isDirectory(const string& name) const{
	return fs::is_directory(pwd(name));
}

const std::string Dir::getFileName() const{
	return path.filename().string();
}

const std::string Dir::getRemoveFileName() const{
	fs::path p(pwd());
	return p.remove_filename().string();
}

const std::string Dir::getDrive() const{
	return path.root_name().string();
}

const vector<string> Dir::getDirectoryFilePaths(bool allDir,vector<string> &paths){
	//vector<string> paths;
	// カレントディレクトリのファイル一覧してみよう
	fs::directory_iterator end;
	for( fs::directory_iterator it(path); it!=end; ++it )
	{
		paths.push_back(it->path().string());
		if(allDir && isDirectory(Dir::getFileName(it->path().string()))){
			cd(Dir::getFileName(it->path().string()));
			getDirectoryFilePaths(allDir,paths);
			cd("..");
		}
	}
	return paths;
}

const vector<string> Dir::getDirectoryFileNames(){
	vector<string> names;
	vector<string> paths;
	paths = getDirectoryFilePaths();
	vector<string>::iterator it = paths.begin();
	for(it=paths.begin();it!=paths.end();it++){
		names.push_back(Dir::getFileName(*it));
	}
	return names;
}

const string Dir::getExtention(const string path){
	fs::path p(path);
	return p.extension().string();
}

const string Dir::getFileName(const string path){
	fs::path p(path);
	return p.filename().string();
}

const string Dir::getRemoveFilename(const string path){
	fs::path p(path);
	return p.remove_filename().string();
}

void Dir::move(std::string src,std::string dst){
	string path = Dir::getRemoveFilename(dst);
	string name = Dir::getFileName(dst);
	Dir dir(this->path,false);
	dir.cd(path);
	fs::rename(pwd(src),dir.pwd(name));
}

void Dir::copy(std::string src,std::string dst){
	string path,name;
	Dir dir(this->path,false);
	name=Dir::getFileName(src);
	try{
		dir.cd(dst);
	}catch(const DirException& e){
		if(e.getErrorCode()==DirException::NOT_DIRECTORY ||
		   e.getErrorCode()==DirException::NOT_EXIST){
			path = Dir::getRemoveFilename(dst);
			name = Dir::getFileName(dst);
			dir.cd(path);
		}
	}

	fs::copy(pwd(src),dir.pwd(name));
}

boost::uintmax_t Dir::getSize(std::string src){
	boost::uintmax_t size=0;
	if(isDirectory(src)){
		vector<string> names;
		cd(src);
		names=getDirectoryFileNames();
		for(vector<string>::iterator it=names.begin();it!=names.end();it++){
			size+=getSize(*it);
		}
		cd("..");
	}else{
		size = fs::file_size(pwd(src));
	}
	return size;
}

boost::posix_time::ptime Dir::getLastWriteTime(std::string src){
	const std::time_t last_update = fs::last_write_time(src);
    boost::posix_time::ptime time = boost::posix_time::from_time_t(last_update);
	return time;
}

bool Dir::create(int flag){
	try{
		if(fs::exists(path)){
			if(flag == 0)
				throw DirException(DirException::EXIST,pwd(),"Dir.cpp","Dir::create()",__LINE__);
			else if(flag%2==1){
				string default_path = pwd();
				int num=1;
				stringstream ss;
				do{
					ss.str("");
					ss.clear(stringstream::goodbit);
					ss << default_path << "_" << num++;
					cd(ss.str());
				}while(!fs::create_directory(pwd()));
				return true;
			}else if((flag>>1)%2==1){
				remove_all();
				// -----------------------------------
				// アクセス拒否対策　コード検討・・・
				while(fs::exists(path,boost::system::error_code()));
				for(int i=0;i<100;i++){
					if(fs::create_directory(path,boost::system::error_code()))
						return true;
				}
				// -----------------------------------
				return fs::create_directory(path);
			}else if((flag>>2)%2==1){
				remove();
				return fs::create_directory(path);
			}
		}else{
			if((flag>>3)%2==0){
				if(fs::create_directory(path))
					return true;
				throw DirException(DirException::PATH_ERROR,pwd(),"Dir.cpp","Dir::create()",__LINE__);
			}
			else
				return fs::create_directories(path);
		}
	}catch(const DirException& e){
		if(ErrorShow) e.showError();
		return false;
	}catch(const fs::filesystem_error& e){
		cout << e.what() << endl;
		cout << "予期せぬエラー：[Dir.cpp,Dir::create()]" << endl;
		throw e;
	}
	return false;
}

bool Dir::create(const fs::path& path, int flag){
	Dir dir(this->pwd(),ErrorShow);
	try{
		dir.cd(path,DirException(DirException::PATH_ERROR,path.string(),"Dir.cpp","Dir::create(string,int)",__LINE__));
	}catch(const DirException& e){
		if(ErrorShow) e.showError();
		return false;
	}
	return dir.create(flag);
}

bool Dir::remove() const{
	try{
		if(!fs::exists(path))
			throw DirException(DirException::NOT_EXIST,path.string(),"Dir.cpp","Dir::remove()",__LINE__);
		else if(!fs::is_empty(path))
			throw DirException(DirException::NOT_EMPTY,path.string(),"Dir.cpp","Dir::remove()",__LINE__);
		return fs::remove(path);
	}catch(const DirException& e){
		if(ErrorShow) e.showError();
		return false;
	}catch(const fs::filesystem_error& e){
		cout << e.what() << endl;
		cout << "予期せぬエラー：[Dir.cpp,Dir::remove()]" << endl;
		throw e;
	}
}

bool Dir::remove(const fs::path& path) const{
	Dir dir(this->pwd(),ErrorShow);
	try{
		dir.cd(path,DirException(DirException::PATH_ERROR,path.string(),"Dir.cpp","Dir::remove(string)",__LINE__));
		return dir.remove();
	}catch(const DirException& e){
		if(ErrorShow) e.showError();
		return false;
	}catch(const fs::filesystem_error& e){
		cout << e.what() << endl;
		cout << "予期せぬエラー：[Dir.cpp,Dir::remove(string)]" << endl;
		throw e;
	}
}

boost::uintmax_t Dir::remove_all() const{
	try{
		return fs::remove_all(path);
	}catch(const fs::filesystem_error& e){
		cout << e.what() << endl;
		cout << "予期せぬエラー：[Dir.cpp,Dir::remove_all()]" << endl;
		throw e;
	}
}

boost::uintmax_t Dir::remove_all(const fs::path& path) const{
	Dir dir(this->pwd(),ErrorShow);
	try{
		dir.cd(path,DirException(DirException::PATH_ERROR,path.string(),"Dir.cpp","Dir::remove_all(string)",__LINE__));
		return dir.remove_all();
	}catch(const DirException& e){
		if(ErrorShow) e.showError();
		return 0;
	}catch(const fs::filesystem_error& e){
		cout << e.what() << endl;
		cout << "予期せぬエラー：[Dir.cpp,Dir::remove_all(string)]" << endl;
		throw e;
	}
}

void Dir::cd(const fs::path& path){
	fs::path pth(path.string());
	if(!isPath(path))
		throw DirException(DirException::PATH_ERROR,path.string(),"Dir.cpp","Dir::cd(string)",__LINE__);
	else if(!isExist(path.string())){
	//	throw DirException(DirException::NOT_EXIST,path.string(),"Dir.cpp","Dir::cd(string)",__LINE__);
	}else if(!isDirectory(path.string()))
		throw DirException(DirException::NOT_DIRECTORY,path.string(),"Dir.cpp","Dir::cd(string)",__LINE__);
	if(pth.is_absolute()){
		this->path = fs::system_complete(pth);
	}else if(pth.is_relative()){
		this->path = fs::system_complete(this->path/pth);
	}
}

void Dir::cd(const fs::path& path,const DirException& e){
	fs::path pth(path.string());
	try{
		if(!isPath(path))
			throw DirException(DirException::PATH_ERROR,path.string(),"Dir.cpp","Dir::cd(string,DirException)",__LINE__);
		else if(!isExist(path.string())){
		//	throw DirException(DirException::NOT_EXIST,path.string(),"Dir.cpp","Dir::cd(string)",__LINE__);
		}else if(!isDirectory(path.string()))
			throw DirException(DirException::NOT_DIRECTORY,path.string(),"Dir.cpp","Dir::cd(string)",__LINE__);
	}catch(const DirException& ex){
		if(ErrorShow) ex.showError();
		throw e;
	}
	if(pth.is_absolute()){
		this->path = fs::system_complete(pth);
	}else if(pth.is_relative()){
		this->path = fs::system_complete(this->path/pth);
	}
}

void Dir::setErrorShow(bool errorShow){
	ErrorShow = errorShow;
}

}

