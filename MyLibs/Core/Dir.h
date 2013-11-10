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

// �f�B���N�g���ɂ����Ă͂����Ȃ�������T�����鐳�K�\��
#define DIR_REGEX_STR "[:*?\"<>|]"

namespace pro{

// error C2061: �\���G���[ : ���ʎq 'DirException'
class DirException;

/**               ���� 
 * boost::filesystem::path �� std::string ��
 * �I�[�o�[���[�h�������܂��Ȍ^�ƂȂ邽��
 * boost:filesystem::path �Ɉ����𓝈ꂷ��B 
 **/

class PRO_EXPORTS Dir
{
public:

	typedef enum CreateFlag{
		NONE = 0,					// �ݒ�Ȃ�
		ANOTHER_CREATE = 1,			// ���݂����ꍇ�ق��ɍ��
		OVER_WRITE_REMOVE_ALL = 2,	// ���݂����ꍇ���ׂď����č�蒼��
		OVER_WRITE = 4,				// ���݂���̏ꍇ�����č�蒼��
		CREATE_DIRS = 8				// ���ԃf�B���N�g�����쐬����
	}CreateFlag;

private:

	boost::filesystem::path path;

	bool ErrorShow;	// �G���[�\��/��\���ݒ�

//#pragma warning(push)
//#pragma warning(disable:4251) // �����ł��Ȃ��x����}��
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
	
	void move(std::string src,std::string dst);	// �t�@�C���ړ�
	void copy(std::string src,std::string dst);	// �t�@�C���R�s�[
	boost::uintmax_t getSize(std::string src); // �t�@�C���T�C�Y�擾
	boost::posix_time::ptime getLastWriteTime(std::string src);	// �ŏI�X�V�擾

	bool create(int flag = NONE);
	bool create(const boost::filesystem::path& path,int flag = NONE);
	bool remove() const;
	bool remove(const boost::filesystem::path& path) const;
	boost::uintmax_t remove_all() const;
	boost::uintmax_t remove_all(const boost::filesystem::path& path) const;

	void setErrorShow(bool errorShow);

};

}
