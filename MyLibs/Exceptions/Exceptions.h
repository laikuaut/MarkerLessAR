// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#include"Exception.h"
#include"FileException.h"
#include"OutOfRangeException.h"
#include"DirException.h"
#include"CameraException.h"



/*
std::bad_alloc : new���Z�q�̎��s

std::out_of_range : �����͈̔͊O
std::invalid_argument : �����̑z��O�Ȑ��l
std::length_error : �f�[�^���̊ԈႢ

std::range_error : ���Z�ɂ�����͈͊O
std::overflow_error : �I�[�o�[�t���[
std::underflow_error : �A���_�[�t���[

*/