/******************************************************************************
 * keybdmouse.cpp: keyboard and mouse callback functions 
 */
#include <stdlib.h>
#include <stdio.h>

#include "platform.h"

#include "common.h"

#include "sim.h"

extern SimDataT simdata;
extern WindowDataT window; //�E�B���h�E�̑傫���Ȃǂ��i�[����\����

//-------- modifier key status
bool isShiftKeyDown( void );
bool isCtrlKeyDown( void );
bool isAltKeyDown( void );

/*-------------------------------------------------- modifier key status
 * Shift, Ctrl, Alt key status
 */
bool isShiftKeyDown( void )
{ return (bool)(glutGetModifiers() & GLUT_ACTIVE_SHIFT); }
bool isCtrlKeyDown( void )
{ return (bool)(glutGetModifiers() & GLUT_ACTIVE_CTRL); }
bool isAltKeyDown( void )
{ return (bool)(glutGetModifiers() & GLUT_ACTIVE_ALT); }

/*------------------------------------------------------------- charKeyDown/Up
 * charKeyDown/Up - GLUT keyboard callback function
 * key: ASCII code
 * x, y: mouse position
 * �p�����L�[�ɂ���
 *--------*/
void charKeyDown( unsigned char key, int x, int y )
{
    switch( key ){
      case 'h': // help
        printf( "\nInstruction\n" );
        printf( "[H]:Help\n" );
		printf( "[R]:Reset\n" );
        printf( "[Q]:Quit\n" );
		printf( "[F1]:Camera1\n" );
		printf( "[F2]:Camera2\n" );
		printf( "[1]:Switch day time\n" );
		printf( "[LeftMouseDrag]:Move\n" );
		printf( "[RightMouseDrag]:Shot\n\n" );
        break;
      case 'q': // quit
        exit(0);
		break;
	  case 'p':
		printf("�v���C���[��%f, %f\n", simdata.player.x, simdata.player.y );
        break;
	  case 'b':
		printf("�o���b�g��%f, %f\n", simdata.bulletE[1].x, simdata.bulletE[1].y );
        break;
	  case 'r':
		  simdata.kbdR = true;
		  break;
	  case 'f':
		  if( simdata.kbdF == false){
		  simdata.kbdF = true;
		  }

		  if( simdata.kbdF){
		  simdata.kbdF = false;
		  }

		  break;
	  case '1':
		  if(simdata.bossFlag == false){
			  switch(simdata.dayTime)
			  {
			  case(0):

				  simdata.dayTime = 1;
				  for(int i = 0; i < 3; i++){
					  simdata.dayColor[i] = 0.0;
					  simdata.dayColor2[i] = 0.0;
				  }
				  printf("��\n");

				  break;
			  case(1):
				  simdata.dayTime = 0;
				  for(int i = 0; i < 3; i++){
					  simdata.dayColor[i] = 1.0;
					  simdata.dayColor2[i] = 1.0;
				  }
				  printf("��\n");
				  break;
			  default:
				  break;
			  }
			  break;
		  }
	  default:
		  break;
    }
    return;
}
void charKeyUp( unsigned char key, int x, int y )
{
    switch( key ){
	  default:
		break;
	}
}
/*--------------------------------------------------------------- funcKeyDown/Up
 * funcKeyDown/Up - GLUT special key callback
 * key: GLUT key code
 * x, y: mouse position
 * ����L�[�ɂ���
 */
void funcKeyDown( int key, int x, int y )
{
	//�t�@���N�V�����L�[
	//[F*]: GLUT_KEY_F*(*��1�`12)
	//[F1]: GLUT_KEY_F1
	//[F12]:GLUT_KEY_F12
	//���̑��̋@�\�L�[
    //[PageUp]  : GLUT_KEY_PAGE_UP:
    //[PageDown]: GLUT_KEY_PAGE_DOWN:
    //[Home]    : GLUT_KEY_HOME:
    //[End]     : GLUT_KEY_END:
    //[Insert]  : GLUT_KEY_INSERT:

	//�J�[�\���L�[
	switch( key ){
	  case GLUT_KEY_LEFT: //[��]
		  //�v���C��������
		  simdata.turn_player = 5.0;

		break;
  	  case GLUT_KEY_RIGHT://[��]
		  //�v���C���E����
		  simdata.turn_player = -5.0;

		break;
	  case GLUT_KEY_UP: //[��]
		  //�v���C���O�i
		  simdata.move_player = -0.1;

		break;
	  case GLUT_KEY_DOWN://[��]
		  //�v���C�����
		  simdata.move_player = 0.1;

		break;

	  case GLUT_KEY_F1 : //

		  //��player�I�u�W�F�N�g�̃A�h���X��active_camera�ɃZ�b�g����
	      simdata.active_camera = &simdata.player;

		break;

	  case GLUT_KEY_F2 : //

		  //��objectA�̃A�h���X��active_camera�ɃZ�b�g����
	      simdata.active_camera = &simdata.objectA;

		break;

	  case GLUT_KEY_F3 : //

		  //��objectB�̃A�h���X��active_camera�ɃZ�b�g����
	      //simdata.active_camera = &simdata.objectB;

		break;


	}	
}		
void funcKeyUp( int key, int x, int y )
{
	switch( key ){
	  case GLUT_KEY_LEFT: //[��]

		break;
  	  case GLUT_KEY_RIGHT://[��]

		break;
	  case GLUT_KEY_UP: //[��]
		  
		  simdata.player.pitch += 10;

		break;
	  case GLUT_KEY_DOWN://[��]

		  simdata.player.pitch -= 10;

		break;
	}
}
/*------------------------------------------------------------- mouseClick
 * mouseClick - GLUT mouse callback function
 * button: mouse button
 * state�Fpress or release
 * x, y�Fmouse position
 *--------*/
void mouseClick( int button , int state, int x, int y )
{
	/////���}�E�X���W�𐳋K������11/15

	//�E�B���h�E�̕����}�E�X���W-1.0 ~ 0.0�Ɋ��蓖��
	//��X���W��Y���W�Ƃŕ����̎�舵�����ӁI�I
	simdata.mouseX = simdata.mouseX0 = (float)x / window.width * 2.0 - 1.0;
	simdata.mouseY = simdata.mouseY0 = - (float)y / window.height * 2.0 + 1.0;
	//printf( "�}�E�X�N���b�N%f, %f\n", simdata.mouseX, simdata.mouseY );

	simdata.mousedX = simdata.mousedY = 0.0;

    /////


    switch( button ){
      case GLUT_LEFT_BUTTON:
        switch( state ){
          case GLUT_DOWN: // press
            simdata.mousebutton[0] = true;
            break;
          case GLUT_UP: // release
			simdata.mousebutton[0] = false;
            break;
          default:
            break;
        }
        break;
      case GLUT_MIDDLE_BUTTON:
        switch( state ){
          case GLUT_DOWN: // press
			simdata.mousebutton[1] = true;
			simdata.mousebuttonB[1] = true;
            break;
          case GLUT_UP: // release
			simdata.mousebutton[1] = false;
            break;
          default:
            break;
        }
        break;
      case GLUT_RIGHT_BUTTON:
        switch( state ){
          case GLUT_DOWN: // press

			  //180101�ǋL
			  simdata.playerBullet[1].Count++;

			  //180110�ǋL
			  simdata.bulletPitch = 10;

			 //1220�ǋL
			 //simdata.player.throwPower = 0.1;
			 //171227�ǋL
			 simdata.playerBullet[1].throwPower = 0.1;

			 simdata.bullet[1].yaw = simdata.player.yaw + 180.0;
			     simdata.routepower = 0;
			//
			simdata.mousebutton[2] = true;

            simdata.mousebuttonB[2] = true;

            break;
		  case GLUT_UP: // release
			  simdata.bullet[1].x =simdata.player.x;
			  simdata.bullet[1].y =1.0;
			  simdata.bullet[1].z =simdata.player.z;

			  simdata.bullet[1].pastz = simdata.bullet[1].z;


			simdata.pastbullet = simdata.player;
			simdata.gravity_bullet = 0;
			//171227�ǋL
			simdata.playerBullet[1].gravity = 0;

			//171227
			simdata.waitBullet = 0;
			//simdata.playerBurret[1].Count ++;

			simdata.mousebutton[2] = false;
			simdata.mousebuttonDown[2] = true;
			simdata.bullet[1].status = 0;
            break;
          default:
            break;
        }
        break;
      default:
        break;
    }
    return;
}
/*---------------------------------------------------------------- mouseDrag 
 * mouseDrag - mouse drag callback function
 */
void mouseDrag( int x, int y )
{
	////////��1118
	if( simdata.mousebutton[0] )
	{
		//1�O�̒l��ۑ�
		simdata.mouseX0 = simdata.mouseX;
		simdata.mouseX0 = simdata.mouseY;

		//�V�������W���擾
		simdata.mouseX =   (float)x / window.width * 2.0 - 1.0;
		simdata.mouseY = - (float)y / window.height * 2.0 + 1.0;

		//�ړ��ʂ��v�Z
		simdata.mousedX = simdata.mouseX - simdata.mouseX0;
		simdata.mousedY = simdata.mouseY - simdata.mouseY0;

	   // printf( "%f, %f\n", simdata.mousedX, simdata.mousedY );



	}
	////////

	//171227
	if( simdata.mousebutton[2] )
	{
		//1�O�̒l��ۑ�
		simdata.mouseX0 = simdata.mouseX;
		simdata.mouseX0 = simdata.mouseY;

		//�V�������W���擾
		simdata.mouseX =   (float)x / window.width * 2.0 - 1.0;
		simdata.mouseY = - (float)y / window.height * 2.0 + 1.0;

		//�ړ��ʂ��v�Z
		simdata.mousedX = simdata.mouseX - simdata.mouseX0;
		simdata.mousedY = simdata.mouseY - simdata.mouseY0;

	    //printf( "%f, %f\n", simdata.mousedX, simdata.mousedY );
	}
    return;
}
/*-------------------------------------------------------------- mouseMotion
 * mouseMotion - passive mouse motion callback function
 */
void mouseMotion( int x, int y )
{
	////////


	////////
    return;
}