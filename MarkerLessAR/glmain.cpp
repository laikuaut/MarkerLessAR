#include"glmain.h"

/*
 *	main.cpp
 */

#include <stdio.h>
#include <stdlib.h>

#include <gl/glut.h>

#include"../MyLibs/OpenCVLibs/Image.h"
//#include "my_opencv.h"

/*
 *	OpenGL�����������܂��D
 */
void initialize_opengl(int&, char* [], int, int);

/*
 *	���������������܂��D
 */
void initialize_light(void);

/*
 *	�ގ������������܂��D
 */
void initialize_material(void);

/*
 *	�E�B���h�E�`��֐��ł��D
 */
void display_function(void);

/*
 *	�E�B���h�E�T�C�Y�ύX�֐��ł��D
 */
void reshape_function(int, int);

/*
 *	�L�[���͊֐��ł��D
 */
void keyboard_function(unsigned char, int, int);

/*
 *	�}�E�X���͊֐��ł��D
 */
void mouse_function(int, int, int, int);

// �J�����摜
cv::Mat camera_image;

// �J�����摜�̃R�s�[
cv::Mat copy_image;

/*
 *	main�֐��ł��D
 */
void glmain(int argc,char *argv[]){
	// �J�����摜��ǂݍ��݂܂�
	camera_image = cv::imread("./cat.jpg");

	// �J�����摜��OpenGL�̌`���ɕϊ����܂�
	cv::cvtColor(camera_image, copy_image, CV_BGR2RGB);
	cv::flip(copy_image, copy_image, 0);

	// OpenGL�����������܂�
	initialize_opengl(argc, argv, copy_image.cols, copy_image.rows);

	// �C�x���g���󂯕t���܂�
	glutMainLoop();

	// ����I�����܂�
	return ;
}

/*
 *	OpenGL�����������܂��D
 */
void initialize_opengl(int& argc, char* argv[], int width, int height)
{
	// GLUT�����������܂�
	glutInit(&argc, argv);

	// �E�B���h�E�̈ʒu��ݒ肵�܂�
	glutInitWindowPosition(0, 0);

	// �E�B���h�E�̑傫����ݒ肵�܂�
	glutInitWindowSize(width, height);

	// �\�����[�h��ݒ肵�܂�
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA);

	// �E�B���h�E�����܂�
	glutCreateWindow(argv[0]);
	
	// �E�B���h�E�`��֐���ݒ肵�܂�
	glutDisplayFunc(display_function);
	
	// �E�B���h�E�T�C�Y�ύX�֐���ݒ肵�܂�
	glutReshapeFunc(reshape_function);

	// �L�[���͊֐���ݒ肵�܂�
	glutKeyboardFunc(keyboard_function);

	// �}�E�X���͊֐���ݒ肵�܂�
	glutMouseFunc(mouse_function);

	// �w�i�F��ݒ肵�܂�
	glClearColor(1.0, 1.0, 0.0, 1.0);

	// �f�v�X�o�b�t�@��L���ɂ��܂�
	glEnable(GL_DEPTH_TEST);

	// ���������������܂�
	initialize_light();

	// �ގ������������܂�
	initialize_material();
}

/*
 *	���������������܂��D
 */
void initialize_light(void)
{
	// ����������L���ɂ��܂�
	glEnable(GL_LIGHTING);

	// ����0�Ԃ�L���ɂ��܂�
	glEnable(GL_LIGHT0);
}

/*
 *	�ގ������������܂��D
 */
void initialize_material(void)
{
	float ambient[] = {0.1, 0.1, 0.1, 1.0};
	float diffuse[] = {0.0, 0.8, 0.5, 1.0};
	float specular[] = {0.8, 0.8, 0.8, 1.0};
	float shininess = 128.0;

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

/*
 *	�E�B���h�E�`��֐��ł��D
 */
void display_function(void)
{
	// �J���[�o�b�t�@�����������܂�
	glClear(GL_COLOR_BUFFER_BIT);

	// �ˉe�s������������܂�
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// ���f���r���[�s������������܂�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// �J�����摜��w�i�ɏ������݂܂�
	glRasterPos2i(-1, -1);
	glDrawPixels(copy_image.cols, copy_image.rows, GL_RGB, GL_UNSIGNED_BYTE, copy_image.data);

	// �ˉe�s��ƃ��f���r���[�s����Đݒ肵�܂�
	reshape_function(copy_image.cols, copy_image.rows);

	// �f�v�X�o�b�t�@�����������܂�
	glClear(GL_DEPTH_BUFFER_BIT);

	// ���f���r���[�s��̃R�s�[���X�^�b�N�ɕۑ����܂�
	glPushMatrix();

	// ���_�Ǝ�����ݒ肵�܂�
	// �{�Ԃł́C�R�����g�ɂ��܂�
	gluLookAt(
		2.0, 2.0, 1.5,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);

	// �����̂�`�悵�܂�
	glutSolidCube(1.0);

	// ���f���r���[�s��̃R�s�[�𕜌����܂�
	glPopMatrix();

	// OpenGL�̖��߂����s���܂�
	glFlush();
}

/*
 *	�E�B���h�E�T�C�Y�ύX�֐��ł��D
 *
 *	@param width �E�B���h�E�̕�
 *	@param height �E�B���h�E�̍���
 */
void reshape_function(int width, int height)
{
	// �r���[�|�[�g��ݒ肵�܂�
	glViewport(0, 0, width, height);

	// �ˉe�s������������܂�
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// �����ˉe�s���ݒ肵�܂�
	// �{�Ԃł́C�J���������p�����[�^��p����glFrustum�֐��֕ύX���܂�
	gluPerspective(
		45.0,
		(double)width / (double)height,
		1.0,
		100.0);

	// ���f���r���[�s������������܂�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*
 *	�L�[���͊֐��ł��D
 *
 *	@param key �L�[�̎��
 *	@param x ���͎���x���W
 *	@param y ���͎���y���W
 */
void keyboard_function(unsigned char key, int x, int y)
{
	switch (key) {
		// ESC�L�[
		case 0x1b:
			// �����I�����܂�
			exit(0);

		default:
			break;
	}
}

/*
 *	�}�E�X���͊֐��ł��D
 *
 *	@param button �{�^���̎��
 *	@param state �{�^���̏��
 *	@param x ���͎���x���W
 *	@param y ���͎���y���W
 */
void mouse_function(int button, int state, int x, int y)
{
	switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_UP) {
				printf("The left button was pressed at (%d, %d).\n", x, y);
			} else {

			}
			break;

		default:
			break;
	}
}