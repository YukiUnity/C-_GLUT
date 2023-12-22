#include <stdlib.h>
#include <stdio.h>

#include <GL/glut.h>
#include <math.h> //add

#include "platform.h"

#include "common.h"

#include "sim.h"

extern WindowDataT window;
extern SimDataT simdata;

/*----*/

#define WINDOW_XO 100
#define WINDOW_YO 100
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define PI 3.14 //add

float aspect = (float)WINDOW_WIDTH/WINDOW_HEIGHT; /* �A�X�y�N�g�� */

float fov = 45.0; //�J����������p

float frame = 1.0;

float height = 2.0;

float rotation = 0.0;

int flag = 0;
//�������z��́A���̐������i�[���ŁA�E�̐����i�z�񐔁j�������Ȃ��Ƃ����Ȃ��H�H
//�ǂ���ł��ǂ����A�錾���̒l��"0"���J�E���g���Ȃ��̂Œ��Ӂi���񂾂ƂR�Ə����ĂR�z��j
	float pointHexA [3][8];
	float pointHexB [3][8];

	float tx,ty,tz;

/*---*/
	int starCheck = 1; 

	float Hexlenghs = 0.0;


float RandomB( float rmin, float rmax ){
    double r;
    r = (double)rand()/(double)RAND_MAX;
    r = r * ( rmax - rmin ) + rmin;
    return r;
}


//-------- prototypes for user functions

void setMaterialColor( float r, float g, float b );
void setMaterialColorB( float r, float g, float b ,float a = 1.0);
void setMaterialColorMoon( float r, float g, float b ,float a = 1.0);

//�����C�A�E�g�̂��߂֗̕��֐��̃v���g�^�C�v�錾
//������ObjDataT�^�\���̕ϐ��́u�|�C���^�v
void setTransform ( ObjDataT *objdata );

/*--------------------------------------------------------- setTransform
 * setTransform: 11/15���ǋL
 *--------*/
//�����ۂ̊֐�
void setTransform ( ObjDataT *objdata )
{
	glTranslatef( objdata->x, objdata->y, objdata->z ); //���s�ړ�
	glRotatef( objdata->yaw, 0.0, 1.0, 0.0); //���[�A������]
	glRotatef( objdata->pitch, 1.0, 0.0, 0.0 ); //�s�b�`�A������]
	glRotatef( objdata->roll, 0.0, 0.0, 1.0 ); //���[���A������]
	glScalef( objdata->scale[0],  objdata->scale[1], objdata->scale[2]);
}

/*--------------------------------------------------------- setMaterialColor
 * setMaterialColor:
 *--------*/
void setMaterialColor( float r, float g, float b )
{
    float diffuse[4];
    float specular[] = { 0.8, 0.8, 0.8, 1.0 };

	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
	diffuse[3] = 1.0;

    glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
    glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
    glMaterialf( GL_FRONT, GL_SHININESS, 32.0 );

    return;
}

void setMaterialColorB( float r, float g, float b, float a )
{
    float diffuse[4];

	//180112
    float specular[] = { 0.2, 0.2, 0.2, 0.0 };
	//float specular[] = { 0.8, 0.8, 0.8, 1.0 };

	float ambient[] = { 0.5, 0.5, 0.5, 1.0 };

	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
	diffuse[3] = a;

    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, specular );

	//180112
	//glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, ambient );
	//glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSIVE, ambient );
    glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, 6.0 );
	//glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, 32.0 );

    return;
}

void setMaterialColorMoon( float r, float g, float b, float a )
{
	if(simdata.bossFlag){
		float diffuse[4];

		//180112
		float specular[] = { 5.0, 3.0, 0.2, 1.0 };
		float emission[] = { 0.8, 0.8, 0.8, 1.0 };

		float ambient[] = { 0.01, 0.0, 0.0, 0.2 };

		diffuse[0] = r;
		diffuse[1] = g;
		diffuse[2] = b;
		diffuse[3] = a;

		glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse );
		glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, specular );
		//glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, emission );
		//180112
		glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, ambient );
		glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, 1000.0 );
		//glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, 32.0 );
	}
    return;
}
/*----------------------------------------------------------- drawSolidCube
 * drawSolidCube:
 *--------*/
/*------------*/

//��
void drawStar( void )
{
	float x, y, z;
	int n = 10;
	float ra = 0.5;
	float rb = ra * 0.382;
	int i;

	glPushMatrix(); 

	//�V�F�[�_�[�F�ݒ�
    setMaterialColor(1.0, 0.0, 0.0);

	//���_�i���_�j�ʒu�̒���
	glTranslatef(0.0, 2.0, 0.0);

	//���[���p(y�A�X��)����
	glRotatef(-18.0, 0.0, 0.0, 1.0);

	//�|���S���̕`��
    glBegin( GL_TRIANGLE_FAN );

	for( i=0; i<n ; i++ )
	{
		if( i % 2 != 0)
		{
		x = ra * cosf( 2.0 * PI * (float)i / (float)n );
		y = ra * sinf( 2.0 * PI * (float)i / (float)n );
		z = 3,0;
		glVertex3f( x, y, z);
		}

		else
		{
		x = rb * cosf( 2.0 * PI * (float)i / (float)n );
		y = rb * sinf( 2.0 * PI * (float)i / (float)n );
		z = 3,0;
		glVertex3f( x, y, z);
		}
	}

    glEnd();

	glPopMatrix();

	return;
}
void drawStarB( void )
{
	float x, y, z;
	int n = simdata.starCount[starCheck];
	/*int n = 10;*/
	float ra = 0.3;
	float rb = ra * 0.382;
	int i;

	glPushMatrix(); 
	{
	//�V�F�[�_�[�F�ݒ�
    setMaterialColorB(2.0, 1.8, 0.0);

	//���_�i���_�j�ʒu�̒���
	glTranslatef(0.0, 0.0, 0.0);

	//���[���p(y�A�X��)����
	glRotatef(360.0/n, 0.0, 0.0, 1.0);

	//�|���S���̕`��
    glBegin( GL_TRIANGLE_FAN );

	for( i=0; i<n ; i++ )
	{
		if( i % 2 != 0)
		{
		x = ra * sinf( 2.0 * PI * (float)i / (float)n );
		y = ra * cosf( 2.0 * PI * (float)i / (float)n );
		z = 0,0;
		glVertex3f( x, y, z);
		}

		else
		{
		x = rb * sinf( 2.0 * PI * (float)i / (float)n );
		y = rb * cosf( 2.0 * PI * (float)i / (float)n );
		z = 0,0;
		glVertex3f( x, y, z);
		}
	}

    glEnd();
	}
	glPopMatrix();

	return;
}
void drawStarC( void )
{
	glPushMatrix();
	{
		drawStarB();

		glPushMatrix(); 
		{
			//���[���p(y�A�X��)����
			glRotatef(90.0, 0.0, 1.0, 0.0);

			drawStarB();
		}
		glPopMatrix();
	}
	glPopMatrix();

	return;
}

//�_
void drawCube( void )
{
	float number = 0.1;
	int i = 0;

    glPushMatrix(); //���݂̕ϊ��}�g���N�X��ۑ��i�v�b�V���j
	{
	setMaterialColor(1.0, 1.0, 1.0);

	//���_�i���_�j�ʒu�̒���
	glTranslatef(0.0, 0.0, 0.0);

	//���[�p(z�A���ʊp)����
	glRotatef(0.0, 0.0, 1.0, 0.0);

	//�s�b�`�p(x�A�p)����
	glRotatef(0.0, 1.0, 0.0, 0.0);

	//���[���p(y�A�X��)����
	glRotatef(0.0, 0.0, 0.0, 1.0);

	//�����E�����E���s���T�C�Y����
	glScalef(1.0, 1.0 , 1.0);

    glutSolidCube( 1.0 );        /*���C���L���[�u��`�悷��*/
	}
    glPopMatrix(); //�ϊ��}�g���N�X�𕜌��i�|�b�v�j

	/*height = height + number;*/
    return;
}
void drawCloudPA( void )
{
	//1
	glPushMatrix();
	{
		glTranslatef(-10.0,0.0,-20.0);
		glScalef(80.0,2.0,50.0);
		drawCube();
	}
	glPopMatrix();

	//2
	glPushMatrix();
	{
		glTranslatef(-20.0,0.0,-40.0);
		glScalef(50.5,2.0,50.0);
		drawCube();
	}
	glPopMatrix();

	//3
	glPushMatrix();
	{
		glTranslatef(-10.0,0.0,10.0);
		glScalef(30.0,2.0,50.0);
		drawCube(); 
	}
	glPopMatrix();

	//4
	glPushMatrix();
	{
		glTranslatef(-20.0,0.0,5.0);
		glScalef(30.0,2.0,30.0);
		drawCube(); 
	}
	glPopMatrix();
}
void drawCloudPB( void )
{

	//1
	glPushMatrix();
	{
		glTranslatef(-10.0,0.0,-20.0);
		//glScalef(80.0,2.0,50.0);
		glScalef(50.5,2.0,50.0);
		drawCube();
	}
	glPopMatrix();

	//2
	glPushMatrix();
	{
		glTranslatef(-20.0,0.0,-40.0);
		//glScalef(50.5,2.0,50.0);
		glScalef(30.0,2.0,30.0);
		drawCube();
	}
	glPopMatrix();

	//3
	glPushMatrix();
	{
		glTranslatef(-10.0,0.0,10.0);
		//glScalef(30.0,2.0,50.0);
		glScalef(80.0,2.0,50.0);
		drawCube(); 
	}
	glPopMatrix();

	//4
	glPushMatrix();
	{
		glTranslatef(-20.0,0.0,5.0);
		//glScalef(30.0,2.0,30.0);
		glScalef(30.0,2.0,50.0);
		drawCube(); 
	}
	glPopMatrix();

}
void drawCloudPC( void )
{
	//1
	glPushMatrix();
	{
		glTranslatef(-10.0,0.0,-20.0);
		//glScalef(80.0,2.0,50.0);
		glScalef(20.5,2.0,50.0);
		drawCube();
	}
	glPopMatrix();

	//2
	glPushMatrix();
	{
		glTranslatef(-20.0,0.0,-40.0);
		//glScalef(50.5,2.0,50.0);
		glScalef(10.0,2.0,20.0);
		drawCube();
	}
	glPopMatrix();

	//3
	glPushMatrix();
	{
		glTranslatef(-10.0,0.0,10.0);
		//glScalef(30.0,2.0,50.0);
		glScalef(30.0,2.0,50.0);
		drawCube(); 
	}
	glPopMatrix();

	//4
	glPushMatrix();
	{
		glTranslatef(-20.0,0.0,5.0);
		//glScalef(30.0,2.0,30.0);
		glScalef(30.0,2.0,20.0);
		drawCube(); 
	}
	glPopMatrix();
}
void drawCloud( int i )
{
	glPushMatrix();
	{
		switch(simdata.cloudCount[i])
		{
		case(0):
			drawCloudPA();
			break;
		case(1):
			drawCloudPB();
			break;
		case(2):
			drawCloudPC();
			break;
		default:
			break;
		}

	}
	glPopMatrix();
}

//��
void drawMoon( void )
{
	if(simdata.bossFlag == false){
	glPushMatrix();
	{
		setMaterialColorB(2.0, 1.8, 0.5);
		setTransform( &simdata.boss );
		glutSolidSphere( 1, 30 ,30 );
	}
	glPopMatrix();
	}
	if(simdata.bossFlag){
	glPushMatrix();
	{
		setMaterialColorMoon(0.01, 0.0, 0.0);
		setTransform( &simdata.boss );
		glutSolidSphere( 1, 30 ,30 );
	}
	glPopMatrix();
	}
}

/*-----------------------------------------------*/

//�l
void drawHuman(void)
{
	//��
	glPushMatrix();{
	setMaterialColor(1.0, 1.0, 1.0);
	glTranslatef(0.0, 3.0, 0.0);
	glRotatef(0.0, 0.0, 1.0, 0.0);
	glRotatef(0.0, 1.0, 0.0, 0.0);
	glRotatef(0.0, 0.0, 0.0, 1.0);
	glScalef(0.5, 1.0 , 0.5);

    drawCube();
	}
    glPopMatrix();

	//��
	glPushMatrix();{
	setMaterialColor(1.0, 1.0, 1.0);
	glTranslatef(0.0, 1.7, 0.0);
	glRotatef(0.0, 0.0, 1.0, 0.0);
	glRotatef(0.0, 1.0, 0.0, 0.0);
	glRotatef(0.0, 0.0, 0.0, 1.0);
	glScalef(1.0, 1.8 , 0.8);

    drawCube();
	}
    glPopMatrix();

	//���r
	glPushMatrix();{
	setMaterialColor(1.0, 1.0, 1.0);
	glTranslatef(-1.0, 1.8, 0.0);
	glRotatef(0.0, 0.0, 1.0, 0.0);
	glRotatef(0.0, 1.0, 0.0, 0.0);
	glRotatef(-30.0, 0.0, 0.0, 1.0);
	glScalef(0.2, 1.7 , 0.2);

    drawCube();
	}
    glPopMatrix();

	//�E�r
	glPushMatrix();{
	setMaterialColor(1.0, 1.0, 1.0);
	glTranslatef(1.0, 1.8, 0.0);
	glRotatef(0.0, 0.0, 1.0, 0.0);
	glRotatef(0.0, 1.0, 0.0, 0.0);
	glRotatef(30.0, 0.0, 0.0, 1.0);
	glScalef(0.2, -1.7 , 0.2);

    drawCube();
	}
    glPopMatrix();

	//����
	glPushMatrix();{
	setMaterialColor(1.0, 1.0, 1.0);
	glTranslatef(-0.3, 0.0, 0.0);
	glRotatef(0.0, 0.0, 1.0, 0.0);
	glRotatef(0.0, 1.0, 0.0, 0.0);
	glRotatef(0.0, 0.0, 0.0, 1.0);
	glScalef(0.2, 2.0 , 0.2);

    drawCube();
	}
    glPopMatrix();

	//�E��
	glPushMatrix();{
	setMaterialColor(1.0, 1.0, 1.0);
	glTranslatef(0.3, 0.0, 0.0);
	glRotatef(0.0, 0.0, 1.0, 0.0);
	glRotatef(0.0, 1.0, 0.0, 0.0);
	glRotatef(0.0, 0.0, 0.0, 1.0);
	glScalef(0.2, 2.0 , 0.2);

    drawCube();
	}
    glPopMatrix();
}

//�e

void drawBulletB( int i )
{
	float x,y,z;

    glPushMatrix(); //���݂̕ϊ��}�g���N�X��ۑ��i�v�b�V���j
	{
	setMaterialColor(1.0, 1.0, 1.0);

	//���_�i���_�j�ʒu�̒���
	glTranslatef(simdata.bullet[i].x,simdata.bullet[i].y,simdata.bullet[i].z);

	//���[�p(z�A���ʊp)����
	glRotatef(0.0, 0.0, 1.0, 0.0);

	//�s�b�`�p(x�A�p)����
	glRotatef(0.0, 1.0, 0.0, 0.0);

	//���[���p(y�A�X��)����
	glRotatef(0.0, 0.0, 0.0, 1.0);

	//�����E�����E���s���T�C�Y����
	glScalef(1.0, 1.0 , 1.0);

   glutSolidSphere( 0.5 ,10,10);        /*���C���L���[�u��`�悷��*/
	}
    glPopMatrix(); //�ϊ��}�g���N�X�𕜌��i�|�b�v�j

	/*height = height + number;*/

	


    return;
}

void drawBulletC( int i )
{
	float x,y,z;

    glPushMatrix(); //���݂̕ϊ��}�g���N�X��ۑ��i�v�b�V���j
	{
	setMaterialColorB( simdata.playerBullet[1].throwPower / 7 * 10, 5.0 - (simdata.playerBullet[1].throwPower / 10 * 8), 0.0 );

	//���_�i���_�j�ʒu�̒���
	glTranslatef(simdata.bulletRoute[i].x,simdata.bulletRoute[i].y,simdata.bulletRoute[i].z);

	//���[�p(z�A���ʊp)����
	glRotatef(0.0, 0.0, 1.0, 0.0);

	//�s�b�`�p(x�A�p)����
	glRotatef(0.0, 1.0, 0.0, 0.0);

	//���[���p(y�A�X��)����
	glRotatef(0.0, 0.0, 0.0, 1.0);

	//�����E�����E���s���T�C�Y����
	glScalef(1.0, 1.0 , 1.0);

    glutSolidCube( 0.2 );        /*���C���L���[�u��`�悷��*/
	}
    glPopMatrix(); //�ϊ��}�g���N�X�𕜌��i�|�b�v�j

	/*height = height + number;*/

	


    return;
}

void drawBulletD() 
{
	glPushMatrix();
	{
		glTranslatef( 0.0, 0.0, 0.0 );   //�I�u�W�F�N�g��ʒu����
		glRotatef( 0.0, 0.0, 1.0, 0.0 ); //�I�u�W�F�N�g��p�������F���[�p
		glRotatef( 180.0, 1.0, 0.0, 0.0 ); //�I�u�W�F�N�g��p�������F�s�b�`�p
		glRotatef( 0.0, 0.0, 0.0, 1.0 ); //�I�u�W�F�N�g��p�������F���[���p
		glutSolidCone( 0.5, 0.5, 8, 8 );
	}
	glPopMatrix();
}

//�Ⴞ���
void drawSphere( void )
{
    /* glColor3f( 1.0, 1.0, 0.0 ); �`�悷��}�`�̐F�𔒂Ɏw��*/



	glPushMatrix(); 
	{
    setMaterialColor(1.0, 1.0, 1.0); //�V�F�[�_�[�F�ݒ�

	//���_�i���_�j�ʒu�̒���
	glTranslatef(0, 1.25, 0.0);

	//���[�p(z�A���ʊp)����
	glRotatef(0.0, 0.0, 1.0, 0.0);

	//�s�b�`�p(x�A�p)����
	glRotatef(90.0, 1.0, 0.0, 0.0);

	//���[���p(y�A�X��)����
	glRotatef(0.0, 0.0, 0.0, 1.0);

	//�����E�����E���s���T�C�Y����
	glScalef(2.5, 2.5, 2.5);

	//���`��
	glutSolidSphere( 0.5, 10 ,10 );     
	}
	glPopMatrix();

    return;

}

void drawSphereB( void )
{
    glColor3f( 1.0, 0.5, 1.0 ); /*�`�悷��}�`�̐F�𔒂Ɏw��*/



	glPushMatrix(); 

	setMaterialColor(0.4, 0.1, 0.1); //�V�F�[�_�[�F�ݒ�

	//���_�i���_�j�ʒu�̒���
	glTranslatef(0, 0.5, 0.0);

	//���[�p(z�A���ʊp)����
	glRotatef(0.0, 0.0, 1.0, 0.0);

	//�s�b�`�p(x�A�p)����
	glRotatef(90.0, 1.0, 0.0, 0.0);

	//���[���p(y�A�X��)����
	glRotatef(0.0, 0.0, 0.0, 1.0);

	//�����E�����E���s���T�C�Y����
	glScalef(0.01, 0.01, 1.0);

	//���`��
	glutSolidSphere( 1, 10 ,10 );     

	glPopMatrix();

    return;
}

void drawCone( void )
{


	glPushMatrix(); //�`�悷��}�`�̐F�𔒂Ɏw��

	setMaterialColor(1.0, 0.3, 0.1);

	//���_�i���_�j�ʒu�̒���
	glTranslatef(0.0, 0.0, 0.0);

	//���[�p(z�A���ʊp)����
	glRotatef(0.0, 0.0, 1.0, 0.0);

	//�s�b�`�p(x�A�p)����
	glRotatef(0.0, 1.0, 0.0, 0.0);

	//���[���p(y�A�X��)����
	glRotatef(0.0, 0.0, 0.0, 1.0);

	//�����E�����E���s���T�C�Y����
	glScalef(0.5, 0.5, 0.5);

	//���`��
	glutSolidCone( 0.5, 1.0 , 10 , 10);

	//���ϊ����I��
	glPopMatrix();

	return;

}

void drawSnowman( void ){

	//drawCone�p�̒��_�v�Z
	float nx, ny, nz;
	int nn = 16;
	float nr = 0.5;

	nx = nr * cosf(2.0 * PI * (float)12 / (float)nn);
	ny = 3;
	nz = nr * sinf(2.0 * PI * (float)12 / (float)nn);


	glShadeModel( GL_SMOOTH ); //GL_FLAT�Ɣ���

	glPushMatrix();
	{
		glPushMatrix();
		{
			//drawCone�i�@�j���C�A�E�g�ϊ�
			glTranslatef(nx, 3.125, nz);
			glRotatef(180.0, 0.0, 1.0, 0.0);
			glRotatef(0.0, 1.0, 0.0, 0.0);
			glRotatef(0.0, 0.0, 0.0, 1.0);
			glScalef(0.5, 0.5, 0.5);

			drawCone();
		}
		glPopMatrix();



		//drawSphere�i�́j�p�̃��C�A�E�g�ϊ�
		glPushMatrix(); //���݂̕ϊ��}�g���N�X��ۑ��i�v�b�V���j
		{
			//���_�i���_�j�ʒu�̒���
			glTranslatef(0.0, 2.5, 0.0);

			//���[�p(z�A���ʊp)����
			glRotatef(0.0, 0.0, 1.0, 0.0);

			//�s�b�`�p(x�A�p)����
			glRotatef(0.0, 1.0, 0.0, 0.0);

			//���[���p(y�A�X��)����
			glRotatef(0.0, 0.0, 0.0, 1.0);

			//�����E�����E���s���T�C�Y����
			glScalef(0.5, 0.5, 0.5);

			drawSphere();
		}
		glPopMatrix(); //�ϊ��}�g���N�X�𕜌��i�|�b�v�j


		//drawSphere�i�E�r�j�p�̃��C�A�E�g�ϊ�
		glPushMatrix();
		{
			glTranslatef(-0.5, 1.875, -0.);

			glRotatef(0, 0.0, 1.0, 0.0);

			glRotatef(0.0, 1.0, 0.0, 0.0);

			glRotatef(45.0, 0.0, 0.0, 1.0);

			glScalef(1, 1, 1);

			drawSphereB();
		}
		glPopMatrix();


		//drawSphere�i���r�j�p�̃��C�A�E�g�ϊ�
		glPushMatrix();
		{
			glTranslatef(0.5, 1.875, 0.0);

			glRotatef(.0, 0.0, 1.0, 0.0);

			glRotatef(0.0, 1.0, 0.0, 0.0);

			glRotatef(-45.0, 0.0, 0.0, 1.0);

			glScalef(1, 1, 1);

			drawSphereB();
		}
		glPopMatrix();

		drawSphere();

	}
	glPopMatrix();
}

void drawEnemySnowman ( void )
{
	glPushMatrix();
		{
			//drawCone�i�@�j���C�A�E�g�ϊ�
			
			glTranslatef(0.0, -1.1, 0.0);
			/*
			glRotatef(0.0, 0.0, 1.0, 0.0);
			glRotatef(0.0, 1.0, 0.0, 0.0);
			glRotatef(0.0, 0.0, 0.0, 1.0);
			glScalef(0.0, 0.0, 0.0);
			*/
			drawSnowman();
		}
		glPopMatrix();
}

//�Z�p�`
void drawHexA( void )
{
	Hexlenghs = 0.5;

	float nx, ny, nz;
	int nn = 6;
	float nr = HEX_REDIUS;
	int i;
	int k;

	glColor3f(1.0, 1.0, 1.0 );

	glBegin( GL_TRIANGLE_FAN );

	for( i=0; i<nn ; i++ )
	{
		k = i;
	nx = nr * cosf(2.0 * PI * (float)i / (float)nn);
	ny = 1.0;
	nz = nr * sinf(2.0 * PI * (float)i / (float)nn);



	glVertex3f( nx, ny, nz);

		pointHexA[0][k] = nx;
	pointHexA[1][k] = ny;
	pointHexA[2][k] = nz;
	}

    glEnd();

	return;

}

void drawHexB( void )
{
	float nx, ny, nz;
	int nn = 6;
	float nr = HEX_REDIUS;
	int i;
	int k;

	glColor3f(1.0, 1.0, 1.0 );

	glBegin( GL_TRIANGLE_FAN );

	for( i=0; i<nn ; i++ )
	{
		k=i;
	nx = nr * cosf(2.0 * PI * (float)i / (float)nn);
	ny = -20.0;
	nz = nr * sinf(2.0 * PI * (float)i / (float)nn);


	glVertex3f( nx, ny, nz);

	pointHexB[0][k] = nx;
	pointHexB[1][k] = ny;
	pointHexB[2][k] = nz;

	}

    glEnd();

	return;

}

void drawHexC( void )
{
	float nx, ny, nz;
	int nn = 6;
	float nr = HEX_REDIUS;
	int i;
	int k;

	setMaterialColorB( 1.0, 1.0, 1.0, 0.15 );

	glBegin( GL_TRIANGLE_FAN );

	for( i=0; i<nn ; i++ )
	{
		k=i;
	nx = nr * cosf(2.0 * PI * (float)i / (float)nn);
	ny = -1.0;
	nz = nr * sinf(2.0 * PI * (float)i / (float)nn);


	glVertex3f( nx, ny, nz);

	pointHexB[0][k] = nx;
	pointHexB[1][k] = ny;
	pointHexB[2][k] = nz;


	}

    glEnd();

	return;

}

void drawHexPoly3( void )
{
	int i = 1;
	int j = i;

	for(i = 0; i < 6 ; i++)
	{
		j= i+1;
		glBegin(GL_POLYGON);
		{
			glVertex3f( pointHexA[0][i],pointHexA[1][i],pointHexA[2][i]);
			glVertex3f(pointHexB[0][i],pointHexB[1][i],pointHexB[2][i]);
			glVertex3f(pointHexB[0][j],pointHexB[1][j],pointHexB[2][j]);
			glVertex3f(pointHexA[0][j],pointHexA[1][j],pointHexA[2][j]);
		}
		glEnd();
	}
	glBegin(GL_POLYGON);
	{
		glVertex3f( pointHexA[0][5],pointHexA[1][5],pointHexA[2][5]);
		glVertex3f(pointHexB[0][5],pointHexB[1][5],pointHexB[2][5]);
		glVertex3f(pointHexB[0][0],pointHexB[1][0],pointHexB[2][0]);
		glVertex3f(pointHexA[0][0],pointHexA[1][0],pointHexA[2][0]);
	}
	glEnd();
	
}

void drawHexPoll( void )
{
	glPushMatrix();
	{


		glPushMatrix(); //���݂̕ϊ��}�g���N�X��ۑ��i�v�b�V���j
		{
			setMaterialColorB(0.05, 0.05, 1.0);
			drawHexA();
			drawHexB();
		}
		glPopMatrix(); //�ϊ��}�g���N�X�𕜌��i�|�b�v�j

		glPushMatrix(); //���݂̕ϊ��}�g���N�X��ۑ��i�v�b�V���j
		{
			setMaterialColorB(0.05, 0.05, 0.5);
			drawHexPoly3();
		}
		glPopMatrix(); //�ϊ��}�g���N�X�𕜌��i�|�b�v�j

		glScalef(1.0,1.0,1.0);
				glTranslatef(0.0,20.0,0.0);
	}
	glPopMatrix();
}

void drawHexPollB ( void )
{
	glPushMatrix();

	glPushMatrix(); 

	//���_�i���_�j�ʒu�̒���
	glTranslatef(0.0, 1.0 - (simdata.heightA * 0.05), 0.0);

	//���[�p(z�A���ʊp)����
	glRotatef(0.0, 0.0, 1.0, 0.0);

	//�s�b�`�p(x�A�p)����
	glRotatef(0.0, 1.0, 0.0, 0.0);

	//���[���p(y�A�X��)����
	glRotatef(0.0, 0.0, 0.0, 1.0);

	//�����E�����E���s���T�C�Y����
	glScalef(1.0, 1.0, 1.0);

	//���`��
    drawHexPoll();

	//���ϊ����I��
	glPopMatrix();


	glScalef(1.0,rotation,0.0);
	glPopMatrix();

}

void drawHexPollC ( void )
{
	glPushMatrix();

	glPushMatrix(); 

	//���_�i���_�j�ʒu�̒���
	glTranslatef(0.0, 1.0 - (simdata.heightC * 0.05), 0.0);

	//���[�p(z�A���ʊp)����
	glRotatef(0.0, 0.0, 1.0, 0.0);

	//�s�b�`�p(x�A�p)����
	glRotatef(0.0, 1.0, 0.0, 0.0);

	//���[���p(y�A�X��)����
	glRotatef(0.0, 0.0, 0.0, 1.0);

	//�����E�����E���s���T�C�Y����
	glScalef(1.0, 1.0, 1.0);

	//���`��
    drawHexPoll();

	//���ϊ����I��
	glPopMatrix();


	glScalef(1.0,rotation,0.0);
	glPopMatrix();

}

void drawHexPollD ( void )
{
	glPushMatrix();

	glPushMatrix(); 

	//���_�i���_�j�ʒu�̒���
	glTranslatef(0.0, 1.0 - (simdata.heightD * 0.05), 0.0);

	//���[�p(z�A���ʊp)����
	glRotatef(0.0, 0.0, 1.0, 0.0);

	//�s�b�`�p(x�A�p)����
	glRotatef(0.0, 1.0, 0.0, 0.0);

	//���[���p(y�A�X��)����
	glRotatef(0.0, 0.0, 0.0, 1.0);

	//�����E�����E���s���T�C�Y����
	glScalef(1.0, 1.0, 1.0);

	//���`��
    drawHexPoll();

	//���ϊ����I��
	glPopMatrix();


	glScalef(1.0,rotation,0.0);
	glPopMatrix();

}

void drawHexPollE ( void )
{
	glPushMatrix();

	glPushMatrix(); 

	//���_�i���_�j�ʒu�̒���
	glTranslatef(0.0, 1.0 - (simdata.heightE * 0.05), 0.0);

	//���[�p(z�A���ʊp)����
	glRotatef(0.0, 0.0, 1.0, 0.0);

	//�s�b�`�p(x�A�p)����
	glRotatef(0.0, 1.0, 0.0, 0.0);

	//���[���p(y�A�X��)����
	glRotatef(0.0, 0.0, 0.0, 1.0);

	//�����E�����E���s���T�C�Y����
	glScalef(1.0, 1.0, 1.0);

	//���`��
    drawHexPoll();

	//���ϊ����I��
	glPopMatrix();


	glScalef(1.0,rotation,0.0);
	glPopMatrix();

}

void drawHexPollF ( void )
{
	glPushMatrix();

	glPushMatrix(); 

	//���_�i���_�j�ʒu�̒���
	glTranslatef(0.0, 1.0 - (simdata.heightF * 0.05), 0.0);

	//���[�p(z�A���ʊp)����
	glRotatef(0.0, 0.0, 1.0, 0.0);

	//�s�b�`�p(x�A�p)����
	glRotatef(0.0, 1.0, 0.0, 0.0);

	//���[���p(y�A�X��)����
	glRotatef(0.0, 0.0, 0.0, 1.0);

	//�����E�����E���s���T�C�Y����
	glScalef(1.0, 1.0 , 1.0);

	//���`��
    drawHexPoll();

	//���ϊ����I��
	glPopMatrix();


	glScalef(1.0,rotation,0.0);
	glPopMatrix();

}

//�Z�p�`�̏�
/*------------------------------------------------*/
void drawHexTile(void)
{
	//���[�v����ϐ�
	int i; //x��
	int j; //z��
	int k=0;

	//�^�C���̈ʒu���W
	float x, y, z;

	y = 0.0;

/*	x = (Hexlenghs * 2) + (Hexlenghs / 1.2)1.625;  171206�ǋL */
/*	x = (Hexlenghs * 2) + Hexlenghs + (Hexlenghs/4);   */

	x = (HEX_REDIUS * 3) + (HEX_REDIUS/4);

	z = HEX_REDIUS;


	for(j = 0; j<N_HEXS * 2; j++)
	{

		for(i = 0; i<N_HEXS; i ++)
		{	
			//�Q�Ŋ���؂��ꍇ
			if(j % 2 == 0)
			{
				glPushMatrix(); 
				{
/*		171206		glTranslatef((x * i) + 0.8, y, z * j);*/
					
					//             �i�ύX          �Z�p�`�p�΍��ƌ��Ԍv�Z            �i�ύX
					glTranslatef((x * i) + ((HEX_REDIUS * 3) + (HEX_REDIUS/4)) / 2, y, z * j);
					k = i;
					switch( simdata.count[j][k] ){
					case (0): 
						drawHexPollB();    
						break;

					case (1): 
						drawHexPollB();    
						break;
					case(2):
						drawHexPollC();
						break;
					case (3):
						drawHexPollD();    
					case (4):
						drawHexPollE();  
					case (5):
						drawHexPollF();  

					default:
						break;
					}		

				}
				glPopMatrix();

			}

			//�ʏ�
			else
			{
				glPushMatrix(); 
				{
					glTranslatef(x * i, y, z * j);
					k = i;
					switch( simdata.count[j][k] ){
					case (0): 
						drawHexPollB();    
						break;

					case (1): 
						drawHexPollB();    
						break;
					case(2):
						drawHexPollC();
						break;
					case (3):
						drawHexPollD();    
					case (4):
						drawHexPollE();  
					case (5):
						drawHexPollF();  

					default:
						break;
					}		

				}
				glPopMatrix();

			}
			
		}

	}

}

void drawHexTileEdgeA(void)
{
	//���[�v����ϐ�
	int i; //x��
	int j; //z��
	int k=0;

	//�^�C���̈ʒu���W
	float x, y, z;

	y = 0.0;

/*	x = (Hexlenghs * 2) + (Hexlenghs / 1.2)1.625;  171206�ǋL */
/*	x = (Hexlenghs * 2) + Hexlenghs + (Hexlenghs/4);   */

	x = (HEX_REDIUS * 3) + (HEX_REDIUS/4);
	z = HEX_REDIUS;

	i= 0;
	for(j = 0; j<N_HEXS * 2; j++)
	{

		if(j % 2 == 0)
		{
			glPushMatrix(); 
			{					
				//             �i�ύX          �Z�p�`�p�΍��ƌ��Ԍv�Z            �i�ύX
				glTranslatef((x * i) + ((HEX_REDIUS * 3) + (HEX_REDIUS/4)) / 2, y, z * j);
				k = i;
				drawHexPoll();

				simdata.wallA[simdata.wallCount].x = (HEX_REDIUS * 3) + (HEX_REDIUS/4) / 2 - N_MAPMOVEX;
				simdata.wallA[simdata.wallCount].y = 0;
				simdata.wallA[simdata.wallCount].z = z*j - N_MAPMOVEZ;
				simdata.wallCount ++;
			}
			glPopMatrix();

		}

		//�ʏ�
		else
		{
			glPushMatrix(); 
			{
				glTranslatef(x * i, y, z * j);
				k = i;
				drawHexPoll();	

				simdata.wallA[simdata.wallCount].x = x * i - N_MAPMOVEX;
				simdata.wallA[simdata.wallCount].y = 0;
				simdata.wallA[simdata.wallCount].z = z*j - N_MAPMOVEZ;
				simdata.wallCount ++;
			}
			glPopMatrix();
		}
	}
}

void drawHexTileEdgeB(void)
{
	//���[�v����ϐ�
	int i; //x��
	int j; //z��
	int k=0;

	//�^�C���̈ʒu���W
	float x, y, z;

	y = 0.0;

/*	x = (Hexlenghs * 2) + (Hexlenghs / 1.2)1.625;  171206�ǋL */
/*	x = (Hexlenghs * 2) + Hexlenghs + (Hexlenghs/4);   */

	x = (HEX_REDIUS * 3) + (HEX_REDIUS/4);
	z = HEX_REDIUS;

	i= N_HEXS;
	for(j = 0; j<N_HEXS * 2; j++)
	{
				if(j % 2 == 0)
		{
			glPushMatrix(); 
			{					
				//             �i�ύX          �Z�p�`�p�΍��ƌ��Ԍv�Z            �i�ύX
				glTranslatef((x * i) + ((HEX_REDIUS * 3) + (HEX_REDIUS/4)) / 2, y, z * j);
				k = i;
				drawHexPoll();

								simdata.wallA[simdata.wallCount].x = (HEX_REDIUS * 3) + (HEX_REDIUS/4) / 2 - N_MAPMOVEX;
				simdata.wallA[simdata.wallCount].y = 0;
				simdata.wallA[simdata.wallCount].z = z*j - N_MAPMOVEZ;
				simdata.wallCount ++;
			}
			glPopMatrix();

		}

		//�ʏ�
		else
		{
			glPushMatrix(); 
			{
				glTranslatef(x * i, y, z * j);
				k = i;
				drawHexPoll();	

								simdata.wallA[simdata.wallCount].x = x * i - N_MAPMOVEX;
				simdata.wallA[simdata.wallCount].y = 0;
				simdata.wallA[simdata.wallCount].z = z*j - N_MAPMOVEZ;
				simdata.wallCount ++;
			}
			glPopMatrix();
		}
	}
}

void drawHexTileEdgeC(void)
{
	//���[�v����ϐ�
	int i; //x��
	int j; //z��
	int k=0;

	//�^�C���̈ʒu���W
	float x, y, z;

	y = 0.0;

	/*	x = (Hexlenghs * 2) + (Hexlenghs / 1.2)1.625;  171206�ǋL */
	/*	x = (Hexlenghs * 2) + Hexlenghs + (Hexlenghs/4);   */

	x = (HEX_REDIUS * 3) + (HEX_REDIUS/4);
	z = HEX_REDIUS;

	for(j = 0; j<2; j++)
	{
	for(i = 0; i<N_HEXS; i++)
	{

		if(j % 2 == 0)
		{
			glPushMatrix(); 
			{					
				//             �i�ύX          �Z�p�`�p�΍��ƌ��Ԍv�Z            �i�ύX
				glTranslatef((x * i) + ((HEX_REDIUS * 3) + (HEX_REDIUS/4)) / 2, y, z * j);
				k = i;
				drawHexPoll();

				simdata.wallA[simdata.wallCount].x = (HEX_REDIUS * 3) + (HEX_REDIUS/4) / 2 - N_MAPMOVEX;
				simdata.wallA[simdata.wallCount].y = 0;
				simdata.wallA[simdata.wallCount].z = z*j - N_MAPMOVEZ;
				simdata.wallCount ++;
			}
			glPopMatrix();

		}

		//�ʏ�
		else
		{
			glPushMatrix(); 
			{
				glTranslatef(x * i, y, z * j);
				k = i;
				drawHexPoll();

				simdata.wallA[simdata.wallCount].x = x * i - N_MAPMOVEX;
				simdata.wallA[simdata.wallCount].y = 0;
				simdata.wallA[simdata.wallCount].z = z*j - N_MAPMOVEZ;
				simdata.wallCount ++;
			}
			glPopMatrix();

		}

	}
	}
}

void drawHexTileEdgeD(void)
{
	//���[�v����ϐ�
	int i; //x��
	int j; //z��
	int k=0;

	//�^�C���̈ʒu���W
	float x, y, z;

	y = 0.0;

	/*	x = (Hexlenghs * 2) + (Hexlenghs / 1.2)1.625;  171206�ǋL */
	/*	x = (Hexlenghs * 2) + Hexlenghs + (Hexlenghs/4);   */

	x = (HEX_REDIUS * 3) + (HEX_REDIUS/4);
	z = HEX_REDIUS;

	for(j = (N_HEXS * 2)-2; j<N_HEXS * 2; j++)
	{
	for(i = 0; i<N_HEXS; i++)
	{

		if(j % 2 == 0)
		{
			glPushMatrix(); 
			{					
				//             �i�ύX          �Z�p�`�p�΍��ƌ��Ԍv�Z            �i�ύX
				glTranslatef((x * i) + ((HEX_REDIUS * 3) + (HEX_REDIUS/4)) / 2, y, z * j);
				k = i;
				drawHexPoll();

				simdata.wallA[simdata.wallCount].x = (HEX_REDIUS * 3) + (HEX_REDIUS/4) / 2 - N_MAPMOVEX;
				simdata.wallA[simdata.wallCount].y = 0;
				simdata.wallA[simdata.wallCount].z = z*j - N_MAPMOVEZ;
				simdata.wallCount ++;
			}
			glPopMatrix();

		}

		//�ʏ�
		else
		{
			glPushMatrix(); 
			{
				glTranslatef(x * i, y, z * j);
				k = i;
				drawHexPoll();

				simdata.wallA[simdata.wallCount].x = x * i - N_MAPMOVEX;
				simdata.wallA[simdata.wallCount].y = 0;
				simdata.wallA[simdata.wallCount].z = z*j - N_MAPMOVEZ;
				simdata.wallCount ++;
			}
			glPopMatrix();

		}

	}
	}
}

void drawHexTileEdgeAll(void)
{
	drawHexTileEdgeA();
	drawHexTileEdgeB();
	drawHexTileEdgeC();
	drawHexTileEdgeD();
}

//180110�Z�p�`�̏�
void drawHexTileB(void)
{

	//���[�v����ϐ�
	int i; //x��
	int j; //z��
	int k=0;

	//�^�C���̈ʒu���W
	float x, y, z;

	y = 0.0;

/*	x = (Hexlenghs * 2) + (Hexlenghs / 1.2)1.625;  171206�ǋL */
/*	x = (Hexlenghs * 2) + Hexlenghs + (Hexlenghs/4);   */

	x = (HEX_REDIUS * 3) + (HEX_REDIUS/4);

	z = HEX_REDIUS;


	for(j = 0; j<N_HEXS * 2; j++)
	{

		for(i = 0; i<N_HEXS; i ++)
		{	
			//�Q�Ŋ���؂��ꍇ
			if(j % 2 == 0)
			{
				glPushMatrix(); 
				{
/*		171206		glTranslatef((x * i) + 0.8, y, z * j);*/
					
					//             �i�ύX          �Z�p�`�p�΍��ƌ��Ԍv�Z            �i�ύX
					glTranslatef((x * i) + ((HEX_REDIUS * 3) + (HEX_REDIUS/4)) / 2, y, z * j);
					k = i;
						drawHexC();    
				}
				glPopMatrix();
			}

			//�ʏ�
			else
			{
				glPushMatrix(); 
				{
					glTranslatef(x * i, y, z * j);
					k = i;
						drawHexC();    
				}
				glPopMatrix();
			}
		}
	}
}

void drawHexFloorB (void)
{
	drawHexTile();

	glPushMatrix();
	//�ȑO�̒i��x�������炷
	glTranslatef(-0.8,0.0,-0.5);
	drawHexTile();
	glPopMatrix();
}

/*---------------------------------------------------------------- DrawScene
* DrawScene:
*--------*/
void DrawScene( void )
{		
	glEnable( GL_DEPTH_TEST ); // ---- begin: 
    glEnable( GL_LIGHTING ); //Disable��main.cpp�ōs��

	int i = 0;

	//171227�ǋL

		if(			simdata.mousebutton[2] == false)
		{

				drawBulletB(1);
		}	
		

	//1220�ǋL�e���\��
	if(simdata.mousebutton[2] == true)
	{

	for(int i = 0; i < N_BULLET; i++)
	{
		glPushMatrix();
		{
			drawBulletC(i);	
		}
		glPopMatrix();
	}

	}

	//180115�ǋL�G����̒e
	if(simdata.bossFlag){
	for(int i = 0; i < N_BULLET; i++)
	{
	switch( simdata.bulletE[1].status ){
	case 0:
		break;
	case 1://����
		glPushMatrix();
		{
		setTransform( &simdata.bulletE[1] );
		setMaterialColor( 0.7, 0.1, 0.0 );
		drawBulletD();
		}
		glPopMatrix();
		break;
	case 2://���e
		//����e���ė��p����̂Œ��e�\���Ȃ�
		break;
	}
	}
	}


	//�v���C���[
	glPushMatrix();
	{
		setTransform( &simdata.player ); //�������ɂ�&�ŃA�h���X��n��
		if( simdata.player.status == 1 ){
			setMaterialColorB( 0.0, 0.0, 0.0 );
		}
		else{
			setMaterialColorB( 1.0, 0.75, 0.25 );
		}
		glPushMatrix();
		{
		glTranslatef(0.0, -1.0, 0.0);
		drawSnowman();
		}
	    glPopMatrix();
	}
	glPopMatrix();

	//�H�H
	glPushMatrix();
	{
		setTransform( &simdata.objectA ); //�������ɂ�&�ŃA�h���X��n��
		//drawSphere();
	}
	glPopMatrix();

	//��
	glPushMatrix();
	{
		//glTranslatef(0.0,-1.7,0.0);
		glTranslatef(-N_MAPMOVEX,-3.0,-N_MAPMOVEZ);
		drawHexTile();
	}
	glPopMatrix();

			glPushMatrix();
	{
	//glTranslatef(0.0,-1.7,0.0);
	glTranslatef(-N_MAPMOVEX,10.0,-N_MAPMOVEZ);
	drawHexTileEdgeAll();
	}
	glPopMatrix();

	//180110�ǃJ�E���g���Z�b�g
	simdata.wallCount = 0;;

	rotation = rotation + 0.1 * frame;
	height = height - 0.001 * frame;
	flag++;


	//180112�ǋL
	//��
	switch(simdata.dayTime){
	case(0):

	break;
	case(1):
	for( int i = 0; i < N_STARS; i++ )
	{
		starCheck = i;

		glPushMatrix();
		{

			setTransform( &simdata.NPC[i] );
			if( simdata.NPC[i].status == 0 )
			{
			drawStarC();
			}
		}
		glPopMatrix();
	}
	break;
	default:
	break;
	}

	//NPC
	for( int i = 0; i < N_BLOCKS; i++ ){

		glPushMatrix();
		{

			setTransform( &simdata.block[i] );
			/*setMaterialColor( 1.0, 0.75, 0.25 );*/

			if( simdata.block[i].status == 1 )
			{
				drawEnemySnowman();
			}
			else
			{
				drawHuman();
			}

		}
		glPopMatrix();
	}

		//180112�ǋL
	if(simdata.bossFlagB ==false){
	if(simdata.dayTime == 1){
	drawMoon();
	}
	}

	if(simdata.dayTime == 0){
		for(int i = 0; i < N_CLOUDS; i++)
		{
					glPushMatrix();
		{
			setTransform( &simdata.cloud[i] );
			drawCloud(i);
		}
		glPopMatrix();
		}
	}

		//180110�ǋL
	//�u�����h�@�\��L���ɂ���
    glEnable( GL_BLEND );

    //�u�����h���@�̐ݒ�
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); //��ʓI�Ȑݒ�

    //////////////////////////////////////////////////
		glPushMatrix();
	{
		glTranslatef(-N_MAPMOVEX,0.0,-N_MAPMOVEZ);
    	drawHexTileB();
			}
	glPopMatrix();
    //////////////////////////////////////////////////

    //�u�����h�@�\�𖳌��ɂ���

    glDisable( GL_BLEND );



	return;
}
