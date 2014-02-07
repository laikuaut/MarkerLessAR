#include"glmain.h"

/**
 * OpenGL�ł̕`��main
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <iostream>

#include <gl/glut.h>
#include <gl\GL.h>

#include "../MarkerLessAR/MarkerLessAR.h"
#include"../MyLibs/OpenCVLibs/Image.h"

using namespace std;

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

/*
 * �������_�[
 */
void render_axes(float);
/*
 * �L���[�u�b�N(�g���̂�)�����_�[
 */
void render_cuboid(double length_x,double length_y,double length_z);

// �J�����摜
cv::Mat camera_image;

// �J�����摜�̃R�s�[
cv::Mat copy_image;

// �}�[�J���XAR
MarkerLessAR glmlar;

// ���f���r���[�s��
GLdouble modelMat[4*4];

// �������t���O
int initFlag=0;


void setModelMat(double x1,double x2,double x3,
				 double y1,double y2,double y3,
				 double z1,double z2,double z3,
				 double t1,double t2,double t3){
	
	modelMat[0]=x1;
	modelMat[1]=x2;
	modelMat[2]=x3;
	modelMat[3]=0;

	modelMat[4]=y1;
	modelMat[5]=y2;
	modelMat[6]=y3;
	modelMat[7]=0;

	modelMat[8]=z1;
	modelMat[9]=z2;
	modelMat[10]=z3;
	modelMat[11]=0;

	modelMat[12]=t1;
	modelMat[13]=t2;
	modelMat[14]=t3;
	modelMat[15]=1;

}

GLdouble* getModelMat(double x1,double x2,double x3,
				 double y1,double y2,double y3,
				 double z1,double z2,double z3,
				 double t1,double t2,double t3){
	
	GLdouble modelMat[16];

	modelMat[0]=x1;
	modelMat[1]=x2;
	modelMat[2]=x3;
	modelMat[3]=0;

	modelMat[4]=y1;
	modelMat[5]=y2;
	modelMat[6]=y3;
	modelMat[7]=0;

	modelMat[8]=z1;
	modelMat[9]=z2;
	modelMat[10]=z3;
	modelMat[11]=0;

	modelMat[12]=t1;
	modelMat[13]=t2;
	modelMat[14]=t3;
	modelMat[15]=1;

	return modelMat;
}

GLdouble* getModelMat(double x1,double x2,double x3,double x4,
				 double y1,double y2,double y3,double y4,
				 double z1,double z2,double z3,double z4,
				 double t1,double t2,double t3,double t4){
	
	GLdouble modelMat[16];

	modelMat[0]=x1;
	modelMat[1]=x2;
	modelMat[2]=x3;
	modelMat[3]=x4;

	modelMat[4]=y1;
	modelMat[5]=y2;
	modelMat[6]=y3;
	modelMat[7]=y4;

	modelMat[8]=z1;
	modelMat[9]=z2;
	modelMat[10]=z3;
	modelMat[11]=z4;

	modelMat[12]=t1;
	modelMat[13]=t2;
	modelMat[14]=t3;
	modelMat[15]=t4;

	return modelMat;
}

void saveImage( const unsigned int imageWidth, const unsigned int imageHeight )
{
  const unsigned int channnelNum = 3; // RGB�Ȃ�3, RGBA�Ȃ�4
  void* dataBuffer = NULL;
  dataBuffer = ( GLubyte* )malloc( imageWidth * imageHeight * channnelNum );

  // �ǂݎ��OpneGL�̃o�b�t�@���w�� GL_FRONT:�t�����g�o�b�t�@�@GL_BACK:�o�b�N�o�b�t�@
  glReadBuffer( GL_BACK );

  // OpenGL�ŉ�ʂɕ`�悳��Ă�����e���o�b�t�@�Ɋi�[
  glReadPixels(
	0,                 //�ǂݎ��̈�̍�������x���W
	0,                 //�ǂݎ��̈�̍�������y���W //0 or getCurrentWidth() - 1
	imageWidth,             //�ǂݎ��̈�̕�
	imageHeight,            //�ǂݎ��̈�̍���
	GL_BGR_EXT, //it means GL_BGR,           //�擾�������F���̌`��
	GL_UNSIGNED_BYTE,  //�ǂݎ�����f�[�^��ۑ�����z��̌^
	dataBuffer      //�r�b�g�}�b�v�̃s�N�Z���f�[�^�i���ۂɂ̓o�C�g�z��j�ւ̃|�C���^
	);

  GLubyte* p = static_cast<GLubyte*>( dataBuffer );
  std::string fname = "W_"+glmlar.getLeftImgName();
  IplImage* outImage = cvCreateImage( cvSize( imageWidth, imageHeight ), IPL_DEPTH_8U, 3 );

  for ( unsigned int j = 0; j < imageHeight; ++ j )
  {
    for ( unsigned int i = 0; i < imageWidth; ++i )
    {
      outImage->imageData[ ( imageHeight - j - 1 ) * outImage->widthStep + i * 3 + 0 ] = *p;
      outImage->imageData[ ( imageHeight - j - 1 ) * outImage->widthStep + i * 3 + 1 ] = *( p + 1 );
      outImage->imageData[ ( imageHeight - j - 1 ) * outImage->widthStep + i * 3 + 2 ] = *( p + 2 );
      p += 3;
    }
  }

  cvSaveImage( fname.c_str(), outImage );
}

/*
 *	main�֐��ł��D
 */
void glmain(int argc,char *argv[],string *strs){

	// MarkerLessAR�̏�����
	if(argc==4){
		if(strs==nullptr)
			glmlar.init(1,argv[2],argv[3]);
		else
			glmlar.init(1,strs[0],strs[1]);
	}else{
		glmlar.init(1);
	}
	glmlar.initAsift();
	//if
	//glmlar.run();
	//else
	glmlar.setImages();

	// OpenGL�\���p�摜
	camera_image = glmlar.asiftLeft.imgInput;

	// ���s��ǂݍ���
	if(!glmlar.setAxis()){
		cout << "not Marker" << endl;
		std::string fname = "W_"+glmlar.getLeftImgName();
		cv::imwrite(fname.c_str(),camera_image);
		return;
		//exit(1);
	}
	if(-glmlar.tAxis[2]<1){
		glmlar.setPersMat(1,-glmlar.tAxis[2]+100);
	}else{
		glmlar.setPersMat(-glmlar.tAxis[2]-100,-glmlar.tAxis[2]+300);
	}

	for(int i=0;i<16;i++){
		modelMat[i]=0;
	}

	// �J�����摜��OpenGL�̌`���ɕϊ����܂�
	cv::cvtColor(camera_image, copy_image, CV_BGR2RGB);
	cv::flip(copy_image, copy_image, 0);

	// OpenGL�����������܂�
	initialize_opengl(argc, argv, copy_image.cols, copy_image.rows);

	// �C�x���g���󂯕t���܂�
	//glutMainLoop();

	// ����I�����܂�
	return ;
}

/*
 *	OpenGL�����������܂��D
 */
void initialize_opengl(int& argc, char* argv[], int width, int height)
{
	// GLUT�����������܂�
	if(initFlag==0){
		glutInit(&argc, argv);
		initFlag++;
	}
	// �E�B���h�E�̈ʒu��ݒ肵�܂�
	glutInitWindowPosition(0, 0);

	// �E�B���h�E�̑傫����ݒ肵�܂�
	glutInitWindowSize(width, height);

	// �\�����[�h��ݒ肵�܂�
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA);

	// �E�B���h�E�����܂�
	int id = glutCreateWindow(argv[0]);
	
	// �E�B���h�E�`��֐���ݒ肵�܂�
	//glutDisplayFunc(display_function);
	
	// �E�B���h�E�T�C�Y�ύX�֐���ݒ肵�܂�
	//glutReshapeFunc(reshape_function);

	// �L�[���͊֐���ݒ肵�܂�
	//glutKeyboardFunc(keyboard_function);

	// �}�E�X���͊֐���ݒ肵�܂�
	//glutMouseFunc(mouse_function);

	// �w�i�F��ݒ肵�܂�
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// �f�v�X�o�b�t�@��L���ɂ��܂�
	glEnable(GL_DEPTH_TEST);

	// ���������������܂�
	initialize_light();

	// �ގ������������܂�
	initialize_material();

	// �P�Ǝ��s
	display_function();

	glutDestroyWindow(id);
}

/*
 *	���������������܂��D
 */
void initialize_light(void)
{
	// ����������L���ɂ��܂�
	//glEnable(GL_LIGHTING);

	// ����0�Ԃ�L���ɂ��܂�
	//glEnable(GL_LIGHT0);
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
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
	//glPushMatrix();

	// ���_�Ǝ�����ݒ肵�܂�
	// �{�Ԃł́C�R�����g�ɂ��܂�
	//gluLookAt(
	//	2.0, 3.0 ,1.5,
	//	0.0, 0.0, 0.0,
	//	0.0, 2.0, 0.0);

	//glPushMatrix();

	////y rotate
	//glMultMatrixd(getModelMat(
	//	 cos(30.0 * PI/180.0), 0.0,  -1 * sin(30.0 * PI/180.0),
	//	                  0.0, 1.0,                        0.0,
	//	 sin(30.0 * PI/180.0), 0.0,       cos(30.0 * PI/180.0),
	//			       	  0.0, 0.0,                        0.0
	//)); 

	//glMultMatrixd(getModelMat(
	//	 0.00245, 0.6006,-0.7994,-0.974336,0.178846,0.1313,0.2218,0.75599,0.61585,-1.0,-1.0,1.0
	//));  

	//glMultMatrixd(getModelMat(
	//	 0.00245, 0.6006,-0.7994,-0.974336,0.178846,0.1313,0.2218,0.75599,0.61585,-11.708,2.953,277.390
	//));

	//glMultMatrixd(modelMat);
	//glTranslated(0.0f,0.0f,-glmlar.tAxis[2]);
	//render_axes(500);

	//// �����̂�`�悵�܂�
	////glutSolidCube(50);
	//render_cuboid(50,50,50);

	//glPopMatrix();

	// ���f���r���[�s��̃R�s�[�𕜌����܂�
	//glPopMatrix();

	

	glPushMatrix();
	glMultMatrixd(getModelMat(
		 glmlar.xAxis[0], glmlar.xAxis[1],glmlar.xAxis[2],
		 glmlar.yAxis[0], glmlar.yAxis[1],glmlar.yAxis[2],
		 glmlar.zAxis[0], glmlar.zAxis[1],glmlar.zAxis[2],
		 glmlar.tAxis[0], glmlar.tAxis[1],glmlar.tAxis[2]
		 //0.0f,0.0f,glmlar.tAxis[2]
		));
	// ���ʑ���ɕ\��
	//render_plane(150);

	glTranslated(0.0f,0.0f,25.0f);
	//glTranslated(0.0f,0.0f,0.0f);
	render_axes(500);
	render_cuboid(50,50,50);
	glPopMatrix();
	
	

	// X���̎O�����_�`��
	vector<cv::Point3f> pt3s_xAxis;
	pt3s_xAxis = glmlar.inputXAxisWorldPoints();
	glColor3f(0.0f,1.0f,1.0f);
	cout << pt3s_xAxis.size() << endl;
	for(int i=0;i<pt3s_xAxis.size();i++){
		glPushMatrix();

		glMultMatrixd(getModelMat(
		 glmlar.xAxis[0], glmlar.xAxis[1],glmlar.xAxis[2],
		 glmlar.yAxis[0], glmlar.yAxis[1],glmlar.yAxis[2],
		 glmlar.zAxis[0], glmlar.zAxis[1],glmlar.zAxis[2],
		 pt3s_xAxis[i].x,pt3s_xAxis[i].y,pt3s_xAxis[i].z
		 //0.0f,0.0f,pt3s[i].z
		));
		//glTranslated(-pt3s[i].x,-pt3s[i].y,0.0f);
		glutSolidCube(2.0);

		//cout << pt3s[i] << endl;

		glPopMatrix();
	}
	
	// 3�����_�`��
	vector<cv::Point3f> pt3s;
	pt3s = glmlar.inputWorldPoints();
	glColor3f(1.0f,0.0f,0.0f);
	for(int i=0;i<pt3s.size();i++){
		glPushMatrix();

		glMultMatrixd(getModelMat(
		 glmlar.xAxis[0], glmlar.xAxis[1],glmlar.xAxis[2],
		 glmlar.yAxis[0], glmlar.yAxis[1],glmlar.yAxis[2],
		 glmlar.zAxis[0], glmlar.zAxis[1],glmlar.zAxis[2],
		 pt3s[i].x,pt3s[i].y,pt3s[i].z
		 //0.0f,0.0f,pt3s[i].z
		));
		//glTranslated(-pt3s[i].x,-pt3s[i].y,0.0f);
		glutSolidCube(2);

		//cout << pt3s[i] << endl;

		glPopMatrix();
	}

	// OpenGL�̖��߂����s���܂�
	glFlush();

	saveImage(copy_image.cols, copy_image.rows);
	//exit(1);
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
	// �_�� P51�u6.2.6 �����p�����[�^���l�����������ˉe�v
	glMultMatrixd(getModelMat(
		2*glmlar.persAu/glmlar.persW, 0, 0, 0,
		0, 2*glmlar.persAv/glmlar.persH, 0, 0,
		(glmlar.persW-2*glmlar.persU0)/glmlar.persW, (glmlar.persH-2*glmlar.persV0)/glmlar.persH, -(glmlar.persF+glmlar.persN)/(glmlar.persF-glmlar.persN), -1,
		0, 0, -(2*glmlar.persF*glmlar.persN)/(glmlar.persF-glmlar.persN), 0
		));

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

void render_axes(float length){
	glColor3f(1.0f,0.0f,0.0f);
	glBegin(GL_LINES);
		glVertex3f(-length,0.0f,0.0f);
		glVertex3f(length,0.0f,0.0f);
	glEnd();

	glColor3f(0.0f,1.0f,0.0f);
	glBegin(GL_LINES);
		glVertex3f(0.0f,-length,0.0f);
		glVertex3f(0.0f,length,0.0f);
	glEnd();

	glColor3f(0.0f,0.0f,1.0f);
	glBegin(GL_LINES);
		glVertex3f(0.0f,0.0f,-length);
		glVertex3f(0.0f,0.0f,length);
	glEnd();
}


void render_cuboid(double length_x,double length_y,double length_z){
	glLineWidth(2.0f);
	glColor3f(1.0f,1.0f,0.0f);
	glBegin(GL_LINE_LOOP);
		glVertex3f((GLfloat)length_x/2,-(GLfloat)length_y/2,(GLfloat)length_z/2);
		glVertex3f((GLfloat)length_x/2,-(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f((GLfloat)length_x/2,(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f((GLfloat)length_x/2,(GLfloat)length_y/2,(GLfloat)length_z/2);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-(GLfloat)length_x/2,(GLfloat)length_y/2,(GLfloat)length_z/2);
		glVertex3f(-(GLfloat)length_x/2,(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f(-(GLfloat)length_x/2,-(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f(-(GLfloat)length_x/2,-(GLfloat)length_y/2,(GLfloat)length_z/2);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f((GLfloat)length_x/2,(GLfloat)length_y/2,(GLfloat)length_z/2);
		glVertex3f((GLfloat)length_x/2,(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f(-(GLfloat)length_x/2,(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f(-(GLfloat)length_x/2,(GLfloat)length_y/2,(GLfloat)length_z/2);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-(GLfloat)length_x/2,-(GLfloat)length_y/2,(GLfloat)length_z/2);
		glVertex3f(-(GLfloat)length_x/2,-(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f((GLfloat)length_x/2,-(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f((GLfloat)length_x/2,-(GLfloat)length_y/2,(GLfloat)length_z/2);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-(GLfloat)length_x/2,(GLfloat)length_y/2,(GLfloat)length_z/2);
		glVertex3f(-(GLfloat)length_x/2,-(GLfloat)length_y/2,(GLfloat)length_z/2);
		glVertex3f((GLfloat)length_x/2,-(GLfloat)length_y/2,(GLfloat)length_z/2);
		glVertex3f((GLfloat)length_x/2,(GLfloat)length_y/2,(GLfloat)length_z/2);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f((GLfloat)length_x/2,(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f((GLfloat)length_x/2,-(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f(-(GLfloat)length_x/2,-(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f(-(GLfloat)length_x/2,(GLfloat)length_y/2,-(GLfloat)length_z/2);
	glEnd();

}