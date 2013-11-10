// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#pragma once

#include"type_c.h"
#include"../Exceptions/Exceptions.h"

#include<string>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

// ディレクトリにあってはいけない文字を探索する正規表現
#define DIR_REGEX_STR "[:*?\"<>|]"

namespace pro{

// error C2061: 構文エラー : 識別子 'DirException'
class DirException;

/**               メモ 
 * boost::filesystem::path と std::string は
 * オーバーロード時あいまいな型となるため
 * boost:filesystem::path に引数を統一する。 
 **/

class PRO_EXPORTS Dir
{
public:

	typedef enum CreateFlag{
		NONE = 0,					// 設定なし
		ANOTHER_CREATE = 1,			// 存在した場合ほかに作る
		OVER_WRITE_REMOVE_ALL = 2,	// 存在した場合すべて消して作り直す
		OVER_WRITE = 4,				// 存在し空の場合消して作り直す
		CREATE_DIRS = 8				// 中間ディレクトリを作成する
	}CreateFlag;

private:

	boost::filesystem::path path;

	bool ErrorShow;	// エラー表示/非表示設定

//#pragma warning(push)
//#pragma warning(disable:4251) // 解決できない警告を抑制
//	boost::regex const regex;
//#pragma warning(pop)

public:
	
	Dir(void);
	Dir(bool flag);
	Dir(const boost::filesystem::path& path,bool flag);

	~Dir(void);
	
	static bool isPath(const boost::filesystem::path& path);
	
	const std::string pwd() const;
	const std::string pwd(const string& name) const;

	bool isExist() const;
	bool isExist(const string& name) const;

	bool isDirectory() const;
	bool isDirectory(const string& name) const;
	
	const std::string getFileName() const;
	const std::string getRemoveFileName() const;
	const std::string getDrive() const;
	const vector<string> getDirectoryFilePaths(bool allDir=false,vector<string> &paths=vector<string>());
	const vector<string> getDirectoryFileNames();
	
	static const string getExtention(const string path);
	static const string getFileName(const string path);
	static const string getRemoveFilename(const string path);

	void cd(const boost::filesystem::path& path);
	void cd(const boost::filesystem::path& path,const DirException& e);
	
	void move(std::string src,std::string dst);	// ファイル移動
	void copy(std::string src,std::string dst);	// ファイルコピー
	boost::uintmax_t getSize(std::string src); // ファイルサイズ取得
	boost::posix_time::ptime getLastWriteTime(std::string src);	// 最終更新取得

	bool create(int flag = NONE);
	bool create(const boost::filesystem::path& path,int flag = NONE);
	bool remove() const;
	bool remove(const boost::filesystem::path& path) const;
	boost::uintmax_t remove_all() const;
	boost::uintmax_t remove_all(const boost::filesystem::path& path) const;

	void setErrorShow(bool errorShow);

};

}
