// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#pragma once

#include "../ASIFT/ASIFT/Asift.h"

class MarkerLessAR
{
public:

	pro::Dir path;

private:
	
	/********************************************
	 * �ݒ�t�@�C����
	 */
	// MarkerLessAR ini File Name
	//std::string iniFileName;
	// Asift Left side ini File Name
	std::string iniAsiftLeftName;
	// Asift Right side ini File Name
	std::string iniAsiftRightName;

	/********************************************
	 * �X�e���I�J����
	 */
	// camera between distance (mm)
	int cameraBetween;

public:

	/********************************************
	 * Asift
	 */
	// ������Asift
	Asift asiftLeft;
	// �E����Left
	Asift asiftRight;

public:

	// �R���X�g���N�^
	MarkerLessAR(void);
	// �f�X�g���N�^
	~MarkerLessAR(void);

	/********************************************
	 * �������֘A
	 */
	// ������
	void init(int readini,string ini_name="MarkerLessAR.ini");
	// �f�t�H���g�l�ݒ�
	void defaultParam();

	/********************************************
	 * ini�t�@�C���֘A
	 */
	// 
	void readIni(ptree &pt);
	void writeIni(ptree &pt);
	void readIni(std::string name);
	void writeIni(std::string name);

	/********************************************
	 * ���s�֘A
	 */
	// ����s
	void run();

};

