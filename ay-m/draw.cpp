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

float aspect = (float)WINDOW_WIDTH/WINDOW_HEIGHT; /* アスペクト比 */

float fov = 45.0; //カメラ垂直画角

float frame = 1.0;

float height = 2.0;

float rotation = 0.0;

int flag = 0;
//多次元配列は、左の数字が格納数で、右の数字（配列数）が多くないといけない？？
//どちらでも良いが、宣言時の値は"0"をカウントしないので注意（今回だと３と書いて３つ配列）
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

//★レイアウトのための便利関数のプロトタイプ宣言
//引数はObjDataT型構造体変数の「ポインタ」
void setTransform ( ObjDataT *objdata );

/*--------------------------------------------------------- setTransform
 * setTransform: 11/15日追記
 *--------*/
//★実際の関数
void setTransform ( ObjDataT *objdata )
{
	glTranslatef( objdata->x, objdata->y, objdata->z ); //平行移動
	glRotatef( objdata->yaw, 0.0, 1.0, 0.0); //ヨー、ｙ軸回転
	glRotatef( objdata->pitch, 1.0, 0.0, 0.0 ); //ピッチ、ｘ軸回転
	glRotatef( objdata->roll, 0.0, 0.0, 1.0 ); //ロール、ｚ軸回転
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

//星
void drawStar( void )
{
	float x, y, z;
	int n = 10;
	float ra = 0.5;
	float rb = ra * 0.382;
	int i;

	glPushMatrix(); 

	//シェーダー色設定
    setMaterialColor(1.0, 0.0, 0.0);

	//頂点（原点）位置の調整
	glTranslatef(0.0, 2.0, 0.0);

	//ロール角(y、傾き)調整
	glRotatef(-18.0, 0.0, 0.0, 1.0);

	//ポリゴンの描画
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
	//シェーダー色設定
    setMaterialColorB(2.0, 1.8, 0.0);

	//頂点（原点）位置の調整
	glTranslatef(0.0, 0.0, 0.0);

	//ロール角(y、傾き)調整
	glRotatef(360.0/n, 0.0, 0.0, 1.0);

	//ポリゴンの描画
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
			//ロール角(y、傾き)調整
			glRotatef(90.0, 0.0, 1.0, 0.0);

			drawStarB();
		}
		glPopMatrix();
	}
	glPopMatrix();

	return;
}

//雲
void drawCube( void )
{
	float number = 0.1;
	int i = 0;

    glPushMatrix(); //現在の変換マトリクスを保存（プッシュ）
	{
	setMaterialColor(1.0, 1.0, 1.0);

	//頂点（原点）位置の調整
	glTranslatef(0.0, 0.0, 0.0);

	//ヨー角(z、方位角)調整
	glRotatef(0.0, 0.0, 1.0, 0.0);

	//ピッチ角(x、仰角)調整
	glRotatef(0.0, 1.0, 0.0, 0.0);

	//ロール角(y、傾き)調整
	glRotatef(0.0, 0.0, 0.0, 1.0);

	//◆幅・高さ・奥行きサイズ調整
	glScalef(1.0, 1.0 , 1.0);

    glutSolidCube( 1.0 );        /*ワイヤキューブを描画する*/
	}
    glPopMatrix(); //変換マトリクスを復元（ポップ）

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

//月
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

//人
void drawHuman(void)
{
	//頭
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

	//胴
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

	//左腕
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

	//右腕
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

	//左足
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

	//右足
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

//弾

void drawBulletB( int i )
{
	float x,y,z;

    glPushMatrix(); //現在の変換マトリクスを保存（プッシュ）
	{
	setMaterialColor(1.0, 1.0, 1.0);

	//頂点（原点）位置の調整
	glTranslatef(simdata.bullet[i].x,simdata.bullet[i].y,simdata.bullet[i].z);

	//ヨー角(z、方位角)調整
	glRotatef(0.0, 0.0, 1.0, 0.0);

	//ピッチ角(x、仰角)調整
	glRotatef(0.0, 1.0, 0.0, 0.0);

	//ロール角(y、傾き)調整
	glRotatef(0.0, 0.0, 0.0, 1.0);

	//◆幅・高さ・奥行きサイズ調整
	glScalef(1.0, 1.0 , 1.0);

   glutSolidSphere( 0.5 ,10,10);        /*ワイヤキューブを描画する*/
	}
    glPopMatrix(); //変換マトリクスを復元（ポップ）

	/*height = height + number;*/

	


    return;
}

void drawBulletC( int i )
{
	float x,y,z;

    glPushMatrix(); //現在の変換マトリクスを保存（プッシュ）
	{
	setMaterialColorB( simdata.playerBullet[1].throwPower / 7 * 10, 5.0 - (simdata.playerBullet[1].throwPower / 10 * 8), 0.0 );

	//頂点（原点）位置の調整
	glTranslatef(simdata.bulletRoute[i].x,simdata.bulletRoute[i].y,simdata.bulletRoute[i].z);

	//ヨー角(z、方位角)調整
	glRotatef(0.0, 0.0, 1.0, 0.0);

	//ピッチ角(x、仰角)調整
	glRotatef(0.0, 1.0, 0.0, 0.0);

	//ロール角(y、傾き)調整
	glRotatef(0.0, 0.0, 0.0, 1.0);

	//◆幅・高さ・奥行きサイズ調整
	glScalef(1.0, 1.0 , 1.0);

    glutSolidCube( 0.2 );        /*ワイヤキューブを描画する*/
	}
    glPopMatrix(); //変換マトリクスを復元（ポップ）

	/*height = height + number;*/

	


    return;
}

void drawBulletD() 
{
	glPushMatrix();
	{
		glTranslatef( 0.0, 0.0, 0.0 );   //オブジェクト基準位置調整
		glRotatef( 0.0, 0.0, 1.0, 0.0 ); //オブジェクト基準姿勢調整：ヨー角
		glRotatef( 180.0, 1.0, 0.0, 0.0 ); //オブジェクト基準姿勢調整：ピッチ角
		glRotatef( 0.0, 0.0, 0.0, 1.0 ); //オブジェクト基準姿勢調整：ロール角
		glutSolidCone( 0.5, 0.5, 8, 8 );
	}
	glPopMatrix();
}

//雪だるま
void drawSphere( void )
{
    /* glColor3f( 1.0, 1.0, 0.0 ); 描画する図形の色を白に指定*/



	glPushMatrix(); 
	{
    setMaterialColor(1.0, 1.0, 1.0); //シェーダー色設定

	//頂点（原点）位置の調整
	glTranslatef(0, 1.25, 0.0);

	//ヨー角(z、方位角)調整
	glRotatef(0.0, 0.0, 1.0, 0.0);

	//ピッチ角(x、仰角)調整
	glRotatef(90.0, 1.0, 0.0, 0.0);

	//ロール角(y、傾き)調整
	glRotatef(0.0, 0.0, 0.0, 1.0);

	//◆幅・高さ・奥行きサイズ調整
	glScalef(2.5, 2.5, 2.5);

	//◆描画
	glutSolidSphere( 0.5, 10 ,10 );     
	}
	glPopMatrix();

    return;

}

void drawSphereB( void )
{
    glColor3f( 1.0, 0.5, 1.0 ); /*描画する図形の色を白に指定*/



	glPushMatrix(); 

	setMaterialColor(0.4, 0.1, 0.1); //シェーダー色設定

	//頂点（原点）位置の調整
	glTranslatef(0, 0.5, 0.0);

	//ヨー角(z、方位角)調整
	glRotatef(0.0, 0.0, 1.0, 0.0);

	//ピッチ角(x、仰角)調整
	glRotatef(90.0, 1.0, 0.0, 0.0);

	//ロール角(y、傾き)調整
	glRotatef(0.0, 0.0, 0.0, 1.0);

	//◆幅・高さ・奥行きサイズ調整
	glScalef(0.01, 0.01, 1.0);

	//◆描画
	glutSolidSphere( 1, 10 ,10 );     

	glPopMatrix();

    return;
}

void drawCone( void )
{


	glPushMatrix(); //描画する図形の色を白に指定

	setMaterialColor(1.0, 0.3, 0.1);

	//頂点（原点）位置の調整
	glTranslatef(0.0, 0.0, 0.0);

	//ヨー角(z、方位角)調整
	glRotatef(0.0, 0.0, 1.0, 0.0);

	//ピッチ角(x、仰角)調整
	glRotatef(0.0, 1.0, 0.0, 0.0);

	//ロール角(y、傾き)調整
	glRotatef(0.0, 0.0, 0.0, 1.0);

	//◆幅・高さ・奥行きサイズ調整
	glScalef(0.5, 0.5, 0.5);

	//◆描画
	glutSolidCone( 0.5, 1.0 , 10 , 10);

	//◆変換を終了
	glPopMatrix();

	return;

}

void drawSnowman( void ){

	//drawCone用の頂点計算
	float nx, ny, nz;
	int nn = 16;
	float nr = 0.5;

	nx = nr * cosf(2.0 * PI * (float)12 / (float)nn);
	ny = 3;
	nz = nr * sinf(2.0 * PI * (float)12 / (float)nn);


	glShadeModel( GL_SMOOTH ); //GL_FLATと反対

	glPushMatrix();
	{
		glPushMatrix();
		{
			//drawCone（鼻）レイアウト変換
			glTranslatef(nx, 3.125, nz);
			glRotatef(180.0, 0.0, 1.0, 0.0);
			glRotatef(0.0, 1.0, 0.0, 0.0);
			glRotatef(0.0, 0.0, 0.0, 1.0);
			glScalef(0.5, 0.5, 0.5);

			drawCone();
		}
		glPopMatrix();



		//drawSphere（体）用のレイアウト変換
		glPushMatrix(); //現在の変換マトリクスを保存（プッシュ）
		{
			//頂点（原点）位置の調整
			glTranslatef(0.0, 2.5, 0.0);

			//ヨー角(z、方位角)調整
			glRotatef(0.0, 0.0, 1.0, 0.0);

			//ピッチ角(x、仰角)調整
			glRotatef(0.0, 1.0, 0.0, 0.0);

			//ロール角(y、傾き)調整
			glRotatef(0.0, 0.0, 0.0, 1.0);

			//◆幅・高さ・奥行きサイズ調整
			glScalef(0.5, 0.5, 0.5);

			drawSphere();
		}
		glPopMatrix(); //変換マトリクスを復元（ポップ）


		//drawSphere（右腕）用のレイアウト変換
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


		//drawSphere（左腕）用のレイアウト変換
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
			//drawCone（鼻）レイアウト変換
			
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

//六角形
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


		glPushMatrix(); //現在の変換マトリクスを保存（プッシュ）
		{
			setMaterialColorB(0.05, 0.05, 1.0);
			drawHexA();
			drawHexB();
		}
		glPopMatrix(); //変換マトリクスを復元（ポップ）

		glPushMatrix(); //現在の変換マトリクスを保存（プッシュ）
		{
			setMaterialColorB(0.05, 0.05, 0.5);
			drawHexPoly3();
		}
		glPopMatrix(); //変換マトリクスを復元（ポップ）

		glScalef(1.0,1.0,1.0);
				glTranslatef(0.0,20.0,0.0);
	}
	glPopMatrix();
}

void drawHexPollB ( void )
{
	glPushMatrix();

	glPushMatrix(); 

	//頂点（原点）位置の調整
	glTranslatef(0.0, 1.0 - (simdata.heightA * 0.05), 0.0);

	//ヨー角(z、方位角)調整
	glRotatef(0.0, 0.0, 1.0, 0.0);

	//ピッチ角(x、仰角)調整
	glRotatef(0.0, 1.0, 0.0, 0.0);

	//ロール角(y、傾き)調整
	glRotatef(0.0, 0.0, 0.0, 1.0);

	//◆幅・高さ・奥行きサイズ調整
	glScalef(1.0, 1.0, 1.0);

	//◆描画
    drawHexPoll();

	//◆変換を終了
	glPopMatrix();


	glScalef(1.0,rotation,0.0);
	glPopMatrix();

}

void drawHexPollC ( void )
{
	glPushMatrix();

	glPushMatrix(); 

	//頂点（原点）位置の調整
	glTranslatef(0.0, 1.0 - (simdata.heightC * 0.05), 0.0);

	//ヨー角(z、方位角)調整
	glRotatef(0.0, 0.0, 1.0, 0.0);

	//ピッチ角(x、仰角)調整
	glRotatef(0.0, 1.0, 0.0, 0.0);

	//ロール角(y、傾き)調整
	glRotatef(0.0, 0.0, 0.0, 1.0);

	//◆幅・高さ・奥行きサイズ調整
	glScalef(1.0, 1.0, 1.0);

	//◆描画
    drawHexPoll();

	//◆変換を終了
	glPopMatrix();


	glScalef(1.0,rotation,0.0);
	glPopMatrix();

}

void drawHexPollD ( void )
{
	glPushMatrix();

	glPushMatrix(); 

	//頂点（原点）位置の調整
	glTranslatef(0.0, 1.0 - (simdata.heightD * 0.05), 0.0);

	//ヨー角(z、方位角)調整
	glRotatef(0.0, 0.0, 1.0, 0.0);

	//ピッチ角(x、仰角)調整
	glRotatef(0.0, 1.0, 0.0, 0.0);

	//ロール角(y、傾き)調整
	glRotatef(0.0, 0.0, 0.0, 1.0);

	//◆幅・高さ・奥行きサイズ調整
	glScalef(1.0, 1.0, 1.0);

	//◆描画
    drawHexPoll();

	//◆変換を終了
	glPopMatrix();


	glScalef(1.0,rotation,0.0);
	glPopMatrix();

}

void drawHexPollE ( void )
{
	glPushMatrix();

	glPushMatrix(); 

	//頂点（原点）位置の調整
	glTranslatef(0.0, 1.0 - (simdata.heightE * 0.05), 0.0);

	//ヨー角(z、方位角)調整
	glRotatef(0.0, 0.0, 1.0, 0.0);

	//ピッチ角(x、仰角)調整
	glRotatef(0.0, 1.0, 0.0, 0.0);

	//ロール角(y、傾き)調整
	glRotatef(0.0, 0.0, 0.0, 1.0);

	//◆幅・高さ・奥行きサイズ調整
	glScalef(1.0, 1.0, 1.0);

	//◆描画
    drawHexPoll();

	//◆変換を終了
	glPopMatrix();


	glScalef(1.0,rotation,0.0);
	glPopMatrix();

}

void drawHexPollF ( void )
{
	glPushMatrix();

	glPushMatrix(); 

	//頂点（原点）位置の調整
	glTranslatef(0.0, 1.0 - (simdata.heightF * 0.05), 0.0);

	//ヨー角(z、方位角)調整
	glRotatef(0.0, 0.0, 1.0, 0.0);

	//ピッチ角(x、仰角)調整
	glRotatef(0.0, 1.0, 0.0, 0.0);

	//ロール角(y、傾き)調整
	glRotatef(0.0, 0.0, 0.0, 1.0);

	//◆幅・高さ・奥行きサイズ調整
	glScalef(1.0, 1.0 , 1.0);

	//◆描画
    drawHexPoll();

	//◆変換を終了
	glPopMatrix();


	glScalef(1.0,rotation,0.0);
	glPopMatrix();

}

//六角形の床
/*------------------------------------------------*/
void drawHexTile(void)
{
	//ループ制御変数
	int i; //x軸
	int j; //z軸
	int k=0;

	//タイルの位置座標
	float x, y, z;

	y = 0.0;

/*	x = (Hexlenghs * 2) + (Hexlenghs / 1.2)1.625;  171206追記 */
/*	x = (Hexlenghs * 2) + Hexlenghs + (Hexlenghs/4);   */

	x = (HEX_REDIUS * 3) + (HEX_REDIUS/4);

	z = HEX_REDIUS;


	for(j = 0; j<N_HEXS * 2; j++)
	{

		for(i = 0; i<N_HEXS; i ++)
		{	
			//２で割り切れる場合
			if(j % 2 == 0)
			{
				glPushMatrix(); 
				{
/*		171206		glTranslatef((x * i) + 0.8, y, z * j);*/
					
					//             段変更          六角形用偏差と隙間計算            段変更
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

			//通常
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
	//ループ制御変数
	int i; //x軸
	int j; //z軸
	int k=0;

	//タイルの位置座標
	float x, y, z;

	y = 0.0;

/*	x = (Hexlenghs * 2) + (Hexlenghs / 1.2)1.625;  171206追記 */
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
				//             段変更          六角形用偏差と隙間計算            段変更
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

		//通常
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
	//ループ制御変数
	int i; //x軸
	int j; //z軸
	int k=0;

	//タイルの位置座標
	float x, y, z;

	y = 0.0;

/*	x = (Hexlenghs * 2) + (Hexlenghs / 1.2)1.625;  171206追記 */
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
				//             段変更          六角形用偏差と隙間計算            段変更
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

		//通常
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
	//ループ制御変数
	int i; //x軸
	int j; //z軸
	int k=0;

	//タイルの位置座標
	float x, y, z;

	y = 0.0;

	/*	x = (Hexlenghs * 2) + (Hexlenghs / 1.2)1.625;  171206追記 */
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
				//             段変更          六角形用偏差と隙間計算            段変更
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

		//通常
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
	//ループ制御変数
	int i; //x軸
	int j; //z軸
	int k=0;

	//タイルの位置座標
	float x, y, z;

	y = 0.0;

	/*	x = (Hexlenghs * 2) + (Hexlenghs / 1.2)1.625;  171206追記 */
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
				//             段変更          六角形用偏差と隙間計算            段変更
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

		//通常
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

//180110六角形の床
void drawHexTileB(void)
{

	//ループ制御変数
	int i; //x軸
	int j; //z軸
	int k=0;

	//タイルの位置座標
	float x, y, z;

	y = 0.0;

/*	x = (Hexlenghs * 2) + (Hexlenghs / 1.2)1.625;  171206追記 */
/*	x = (Hexlenghs * 2) + Hexlenghs + (Hexlenghs/4);   */

	x = (HEX_REDIUS * 3) + (HEX_REDIUS/4);

	z = HEX_REDIUS;


	for(j = 0; j<N_HEXS * 2; j++)
	{

		for(i = 0; i<N_HEXS; i ++)
		{	
			//２で割り切れる場合
			if(j % 2 == 0)
			{
				glPushMatrix(); 
				{
/*		171206		glTranslatef((x * i) + 0.8, y, z * j);*/
					
					//             段変更          六角形用偏差と隙間計算            段変更
					glTranslatef((x * i) + ((HEX_REDIUS * 3) + (HEX_REDIUS/4)) / 2, y, z * j);
					k = i;
						drawHexC();    
				}
				glPopMatrix();
			}

			//通常
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
	//以前の段とx軸をずらす
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
    glEnable( GL_LIGHTING ); //Disableはmain.cppで行う

	int i = 0;

	//171227追記

		if(			simdata.mousebutton[2] == false)
		{

				drawBulletB(1);
		}	
		

	//1220追記弾道予測
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

	//180115追記敵からの弾
	if(simdata.bossFlag){
	for(int i = 0; i < N_BULLET; i++)
	{
	switch( simdata.bulletE[1].status ){
	case 0:
		break;
	case 1://飛翔
		glPushMatrix();
		{
		setTransform( &simdata.bulletE[1] );
		setMaterialColor( 0.7, 0.1, 0.0 );
		drawBulletD();
		}
		glPopMatrix();
		break;
	case 2://着弾
		//今回弾を再利用するので着弾表現なし
		break;
	}
	}
	}


	//プレイヤー
	glPushMatrix();
	{
		setTransform( &simdata.player ); //★引数には&でアドレスを渡す
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

	//？？
	glPushMatrix();
	{
		setTransform( &simdata.objectA ); //★引数には&でアドレスを渡す
		//drawSphere();
	}
	glPopMatrix();

	//床
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

	//180110壁カウントリセット
	simdata.wallCount = 0;;

	rotation = rotation + 0.1 * frame;
	height = height - 0.001 * frame;
	flag++;


	//180112追記
	//星
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

		//180112追記
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

		//180110追記
	//ブレンド機能を有効にする
    glEnable( GL_BLEND );

    //ブレンド方法の設定
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); //一般的な設定

    //////////////////////////////////////////////////
		glPushMatrix();
	{
		glTranslatef(-N_MAPMOVEX,0.0,-N_MAPMOVEZ);
    	drawHexTileB();
			}
	glPopMatrix();
    //////////////////////////////////////////////////

    //ブレンド機能を無効にする

    glDisable( GL_BLEND );



	return;
}
