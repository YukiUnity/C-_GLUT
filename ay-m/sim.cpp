#include <stdio.h>
#include <stdlib.h>

//�����w�֐��p�w�b�_�[�ǂݍ���(mathmatic)
#include <math.h>

#include "common.h"
#include "sim.h"
#include "calc.h" //1220��


SimDataT simdata;
extern int time;

	float rmax,rmin;

	bool flagA,flagB,flagC,flagD,flagE,flagF = false;

//�����_���֐�
float Random( float rmin, float rmax ){
    double r = 0.0;
    r = (double)rand()/(double)RAND_MAX;
    r = r * ( rmax - rmin ) + rmin;
    return r;
}


//�����_���֐�����莩�R�ɏo���A���S���Y���i�K�E�V�A���j
double Gaussian( float rmin, float rmax ){
	double r = 0.0;
	for( int i=0; i< 12; i++ ){
		r += Random( rmin, rmax );
	}
	return r/12.0;

}


//�֐��v���g�^�C�v
bool HitTest( ObjDataT *a, ObjDataT *b );
//a�F �Փ˔���̑���
//b�F �Փ˔���̎��
//�߂�l�F �Փ˂����ꍇ��true���Ԃ�

float DistanceAtoB( ObjDataT *a, ObjDataT *b );
//�߂�l�F ������a��b�Ƃ̊Ԃ̋�����Ԃ�


bool HitTest( ObjDataT *a, ObjDataT *b )
{
	if( DistanceAtoB( a, b ) < a->radius + b->radius ) return true;
	else return false;
}


/*---------------------------------------------------------------- InitScene
 * InitScene:
 *--------*/
void InitScene( void )
{
	printf("�Q�[���f�[�^�����[�h���Ă��܂��c�c\n");

	//180112�ǋL
	simdata.dayTime = 0;
	for(int i = 0; i < 3; i++){
		simdata.dayColor[i] = 1.0;
		simdata.dayColor2[i] = 1.0;
		}

	////// �V�[���f�[�^�̏�����
	simdata.clip_far = 500.0; //���t�@�[�N���b�v����
	simdata.clip_near = 0.1; //���j�A�N���b�v����
	simdata.air_color[0] = simdata.dayColor[0];
	simdata.air_color[1] = simdata.dayColor[1];
	simdata.air_color[2] = simdata.dayColor[2];
	simdata.air_color[3] = 0.5; // fog density factor
	simdata.sky_color[0] = 0.2;
	simdata.sky_color[1] = 0.3;
	simdata.sky_color[2] = 0.4;
	simdata.sky_color[3] = 0.5; // sky color factor
	//////


    ///////11/8�ǉ��F�\���̂̃f�[�^�����l�ݒ�iactive_camera�ւ̃Z�b�g�̂�11/15�j
	
	//���v���C���[�̏����ʒu�E�p����ݒ�
	simdata.player.x = 0.0;
	simdata.player.y = 0.0;
	simdata.player.z = 0.0;
	simdata.player.roll = 0.0;
	simdata.player.pitch = 0.0;
	simdata.player.yaw = 0.0;
	simdata.player.scale[0] = 1.0; 
	simdata.player.scale[1] = 1.0; 
	simdata.player.scale[2] = 1.0; 

	//1213�����蔻��
	simdata.player.status = 0.0;
	simdata.player.radius = 1.0; 

	//1220�@�������
	simdata.player.throwPower = 10.0;

	//180112�ǋL
	simdata.playerNextpos.x = 0.0;
	simdata.playerNextpos.y = 0.0;
	simdata.playerNextpos.z = 0.0;
	simdata.playerNextpos.roll = 0.0;
	simdata.playerNextpos.pitch = 0.0;
	simdata.playerNextpos.yaw = 0.0;
	simdata.playerNextpos.scale[0] = 1.0; 
	simdata.playerNextpos.scale[1] = 1.0; 
	simdata.playerNextpos.scale[2] = 1.0; 
				  
	//1213�����蔻Nextpos��
	simdata.playerNextpos.status = 0.0;
	simdata.playerNextpos.radius = 1.0; 

	//���I�u�W�F�N�gA.B�̏����ʒu�E�p��
	simdata.objectA.x = 0.0;
	simdata.objectA.y = 2.5;
	simdata.objectA.z = 0.0;
	simdata.objectA.roll = 0.0;
	simdata.objectA.pitch = 0.0;
	simdata.objectA.yaw = 0.0;
	simdata.objectA.scale[0] = 0.5; 
	simdata.objectA.scale[1] = 0.5; 
	simdata.objectA.scale[2] = 0.5; 

	simdata.objectA.radius = 3.0; 

	simdata.objectB.x = 0.0;
	simdata.objectB.y = 0.0;
	simdata.objectB.z = 0.0;
	simdata.objectB.roll = 0.0;
	simdata.objectB.pitch = 0.0;
	simdata.objectB.yaw = 0.0;
	simdata.objectB.scale[0] = 1.0; 
	simdata.objectB.scale[1] = 1.0; 
	simdata.objectB.scale[2] = 1.0; 

	simdata.boss.x = -50.0;
	simdata.boss.y = 35.0;
	simdata.boss.z = -80.0;
	simdata.boss.roll = 0.0;
	simdata.boss.pitch = 0.0;
	simdata.boss.yaw = 0.0;
	simdata.boss.scale[0] = 5.0; 
	simdata.boss.scale[1] = 5.0; 
	simdata.boss.scale[2] = 5.0; 

	simdata.boss.status = 0;
	simdata.boss.radius = simdata.boss.scale[0];

	for(int i = 0; i<N_BULLET; i++)
	{
		simdata.bullet[i].x = 0.0;
		simdata.bullet[i].y = 0.0;
		simdata.bullet[i].z = 0.0;
		simdata.bullet[i].roll = 0.0;
		simdata.bullet[i].pitch = 0.0;
		simdata.bullet[i].yaw = 0.0;
		simdata.bullet[i].scale[0] = 1.0; 
		simdata.bullet[i].scale[1] = 1.0; 
		simdata.bullet[i].scale[2] = 1.0; 
				  
		simdata.bullet[i].status = 0;
		simdata.bullet[i].radius = 1.0; 
	}

	for(int i = 0; i<N_BULLET; i++)
	{
		simdata.bulletE[i].x = 0.0;
		simdata.bulletE[i].y = 0.0;
		simdata.bulletE[i].z = 0.0;
		simdata.bulletE[i].roll = 0.0;
		simdata.bulletE[i].pitch = 0.0;
		simdata.bulletE[i].yaw = 0.0;
		simdata.bulletE[i].scale[0] = 1.0; 
		simdata.bulletE[i].scale[1] = 1.0; 
		simdata.bulletE[i].scale[2] = 1.0; 
				  	  
		simdata.bulletE[i].status = 0;
		simdata.bulletE[i].radius = 1.0; 
	}

	simdata.bulletE[0].status = 1;

	simdata.pastbullet.x = 0.0;
	simdata.pastbullet.y = 0.0;
	simdata.pastbullet.z = 0.0;
	simdata.pastbullet.roll = 0.0;
	simdata.pastbullet.pitch = 0.0;
	simdata.pastbullet.yaw = 0.0;
	simdata.pastbullet.scale[0] = 1.0; 
	simdata.pastbullet.scale[1] = 1.0; 
	simdata.pastbullet.scale[2] = 1.0; 
				  
	simdata.pastbullet.status = 0.0;
	simdata.pastbullet.radius = 0.1; 

	//1213�������
	simdata.throwpower = 0.3;

	//1227�e�\�z
	for(int i = 0; i<N_BULLET; i++)
	{
		simdata.bulletRoute[i].x = 0.0;
		simdata.bulletRoute[i].y = 0.0;
		simdata.bulletRoute[i].z = 0.0;
		simdata.bulletRoute[i].roll = 0.0;
		simdata.bulletRoute[i].pitch = 0.0;
		simdata.bulletRoute[i].yaw = 0.0;
		simdata.bulletRoute[i].scale[0] = 1.0; 
		simdata.bulletRoute[i].scale[1] = 1.0; 
		simdata.bulletRoute[i].scale[2] = 1.0; 
				  
		simdata.bulletRoute[i].status = 0.0;
		simdata.bulletRoute[i].radius = 0.3; 
				  
		simdata.bulletRoute[i].turn = 0.0;
		simdata.bulletRoute[i].move = 0.0; 
	}


	//180112�ǋL

	for(int i = 0; i<500; i++)
	{
		simdata.wallA[i].x = 1000.0;
		simdata.wallA[i].y = 1000.0;
		simdata.wallA[i].z = 1000.0;
		simdata.wallA[i].roll = 0.0;
		simdata.wallA[i].pitch = 0.0;
		simdata.wallA[i].yaw = 0.0;
		simdata.wallA[i].scale[0] = 1.0; 
		simdata.wallA[i].scale[1] = 1.0; 
		simdata.wallA[i].scale[2] = 1.0; 
				
		simdata.wallA[i].status = 0.0;
		simdata.wallA[i].radius = 1.0; 
	}

	//��player�I�u�W�F�N�g�̃A�h���X��active_camera�ɃZ�b�g����i�������j
	simdata.active_camera = &simdata.player;

	//////


	//////�}�E�X�{�^���l�̏�����

	//����11��18���ǋL
	simdata.mousebutton[0] = false; //���}�E�X�{�^�������ĂȂ�
	simdata.mousebutton[1] = false; //���}�E�X�{�^�������ĂȂ�
	simdata.mousebutton[2] = false; //�E�}�E�X�{�^�������ĂȂ�
	simdata.mouseX = simdata.mouseX0 = simdata.mousedX = 0.0; //�}�E�XX���W�[�����Z�b�g
	simdata.mouseY = simdata.mouseY0 = simdata.mousedY = 0.0; //�}�E�XY���W�[�����Z�b�g

	//////


	//////11/22�ǋLblock�̏����ݒ�

	for( int i=0; i<N_BLOCKS; i++ ){

		float x,z;

		//11/22���Ō�ǋL�@����̈���ł̌��ʂ���蒼��
		do{
			x = Random( -200.0, 50.0 );
			z = Random( -100.0, 50.0 );
		}while( -5.0 < x && x < 5.0 && -5.0 < z && z < 5.0 );


		simdata.block[i].state = 0;
		simdata.block[i].visible = true; //������Ƃ������Ƃɂ���
		simdata.block[i].x = x;
		simdata.block[i].y = 0.0;
		simdata.block[i].z = z;
		simdata.block[i].roll = 0.0;
		simdata.block[i].pitch = 0.0;
		simdata.block[i].yaw = 0.0;
		simdata.block[i].scale[0] = 1.0; 
	    simdata.block[i].scale[1] = 1.0; 
	    simdata.block[i].scale[2] = 1.0; 

		simdata.block[i].status = 0;
		simdata.block[i].radius = 1.0;

		simdata.blockCollision[i].state = 0;
		simdata.blockCollision[i].visible = true; //������Ƃ������Ƃɂ���
		simdata.blockCollision[i].x = simdata.block[i].x;
		simdata.blockCollision[i].y = 2.0;
		simdata.blockCollision[i].z = simdata.block[i].z;
		simdata.blockCollision[i].roll = 0.0;
		simdata.blockCollision[i].pitch = 0.0;
		simdata.blockCollision[i].yaw = 0.0;
		simdata.blockCollision[i].scale[0] = 1.0; 
	    simdata.blockCollision[i].scale[1] = 1.0; 
	    simdata.blockCollision[i].scale[2] = 1.0; 
					 	   
		simdata.blockCollision[i].status = 0;
		simdata.blockCollision[i].radius = 2.0;
	
	}


	for( int i=0; i<N_STARS; i++ ){

		simdata.NPC[i].state = 0;
		simdata.NPC[i].visible = true; //������Ƃ������Ƃɂ���
		simdata.NPC[i].x = Random( -300.0, 150.0 );
		simdata.NPC[i].y = Random ( 15.0, 20.5 );
		simdata.NPC[i].z = Random( -200.0, 150.0 );
		simdata.NPC[i].roll = 0.0;
		simdata.NPC[i].pitch = 0.0;
		simdata.NPC[i].yaw = 0.0;
		simdata.NPC[i].scale[0] = 1.0; 
	    simdata.NPC[i].scale[1] = 1.0; 
	    simdata.NPC[i].scale[2] = 1.0; 

		simdata.NPC[i].status = 0;
		simdata.NPC[i].radius = 5.0;
	}

	for( int i=0; i<N_CLOUDS; i++ ){

		simdata.cloud[i].state = 0;
		simdata.cloud[i].visible = true; //������Ƃ������Ƃɂ���
		simdata.cloud[i].x = Random( -700.0, 750.0 );
		simdata.cloud[i].y = 70.0;
		simdata.cloud[i].z = Random( -700.0, 750.0 );
		simdata.cloud[i].roll = 0.0;
		simdata.cloud[i].pitch = 0.0;
		simdata.cloud[i].yaw = 0.0;
		simdata.cloud[i].scale[0] = 1.0; 
	    simdata.cloud[i].scale[1] = 1.0; 
	    simdata.cloud[i].scale[2] = 1.0; 
			   
		simdata.cloud[i].status = 0;
		simdata.cloud[i].radius = 5.0;
	}

	//////
	//////���[�U�[�p

	//�w�i�I�u�W�F�N�g�����_���l�擾
	simdata.heightA = 0;
	simdata.heightB = 0;
	simdata.heightC = 0;

	for(int i = 0; i<N_HEXS*2; i++)
	{
		for(int j = 0; j<N_HEXS*2; j++)
		{
			simdata.count[i][j] = (int)Random(-0.3,5.3);
		}
	}

	//�X�^�[�����_���l�擾

	for(int k = 0; k<N_STARS; k++)
	{
		simdata.starCount[k] = (int)Random(0.0,5.0);
	}

	for(int k = 0; k<N_STARS; k++)
	{
		switch(simdata.starCount[k]){
		case(0):
			simdata.starCount[k] = 10;
			break;
		case(1):
			simdata.starCount[k] = 12;
			break;
		case(2):
			simdata.starCount[k] = 14;
			break;
		case(3):
			simdata.starCount[k] = 16;
			break;
		case(4):
			simdata.starCount[k] = 10;
			break;
		case(5):
			simdata.starCount[k] = 10;
			break;
		default:
			simdata.starCount[k] = 10;
			break;
		}
	}

	//180115�ǋL
	for(int k = 0; k<N_CLOUDS; k++)
	{
		simdata.cloudCount[k] = (int)Random(0.0,2.0);
	}


	//1220�ǋL
	simdata.gravity_bullet = 0;

	//////

    simdata.routepower = 0;

	simdata.waitBullet = 0;
	for(int i = 0; i<N_BULLET;i ++)
	{
	simdata.playerBullet[i].Count = 0;
    simdata.playerBullet[i].throwPower = 0;
	simdata.playerBullet[i].upPower = 0;
	simdata.playerBullet[i].sidePower = 0;
	simdata.playerBullet[i].gravity = 0;
	}

	//180112�ǋL
	simdata.score = 0;
	simdata.scoreStar = 0;
	simdata.bossFlag = false;
    simdata.bossFlagB = false;
	simdata.bossGrowFlag = false;
	simdata.bossHP = 3;
	simdata.bossScale = 1.0;
	
	simdata.kbdR = false;

	printf("�Q�[���X�^�[�g!\n");

    return;
}

/*-------------------------------------------------------------- UpdateScene
 * UpdateScene:
 *--------*/
void UpdateScene( void )
{

	simdata.objectA = simdata.player;
	simdata.objectB = simdata.player;

	//171227�ǋL�@�e�̃J�E���g
	//int c = 0;
	//c = simdata.playerBurret[1].Count;

	if(simdata.mousebuttonDown[2])
	{
		//simdata.playerBurret[1].Count++;
		simdata.mousebuttonDown[2] = false;
	}


	//////// �f�[�^�X�V ///////
	const float PI = 3.14159;
	float yaw;
	
	
	//�}�E�X�h���b�O����̒l
	float v = 1.0; //�ړ����x�����p�����[�^�i���슴�x�j
	float r = 10.0; //��]���x�����p�����[�^�i���슴�x�j

	simdata.turn_player = - r * simdata.mousedX;
	simdata.move_player = - v * simdata.mousedY;
	//171227
	simdata.turn_player2 = - v * simdata.mousedY;

	//171227�ǋL
	if(simdata.mousebutton[0])
	{
		simdata.player.status = 0;
		if(simdata.player.status == 0)
		{
			//�v���C���[�̈ړ��v�Z
			simdata.player.yaw += simdata.turn_player; //������yaw���X�V
			yaw = simdata.player.yaw / 180.0 * PI; //�i�x�j����i���W�A���j�ɕϊ�

			//180112�ǋL
			simdata.playerNextpos.x = simdata.player.x + simdata.move_player * sinf( yaw ); //������yaw���X�V
			simdata.playerNextpos.z = simdata.player.z + simdata.move_player * cosf( yaw ); //������yaw���X�V
			for( int i = 0; i<500; i++ )
			{
				if( HitTest( &simdata.playerNextpos, &simdata.wallA[i] ))
				{
					simdata.player.status = 1;
				}
				else
				{
					
				}
			}
			if(simdata.player.status == 0)
			{
			//                   ���ł���l�i�ړ��ʁj                   ���W�A���ւ̕ϊ�
					simdata.player.x += simdata.move_player * sinf( yaw ); //������yaw���X�V
					simdata.player.z += simdata.move_player * cosf( yaw ); //������yaw���X�V
			}
			else
			{
			}
			}
	else
	{
	}
}

	//171227�G�C��
	if(simdata.mousebutton[2])
	{
		simdata.bullet[1].yaw = simdata.player.yaw + 180.0;
		simdata.player.yaw += simdata.turn_player; //������yaw���X�V
		simdata.player.pitch -= simdata.turn_player2;
	}
	//������ړ��ʂ����Z�b�g�i�A���I�ɓ����������Ƃ��ɂ̓R�����g�ɂ���j
	/*
	simdata.move_player = 0.0; //����(�A���I�ɂ����ꍇ�͑��x)
	simdata.turn_player = 0.0; //�p�x�i�A���I�ɂ����ꍇ�͊p���x�j
	*/

	//���}�E�X�{�^���̊m�F
	/*if( simdata.mousebutton[0] )
	{
		printf( "LEFT MOUSE BUTTON PRESSED DOWN\n" );
	}
	else
	{
		printf( "RELEASE\n" );
	}*/


	
	/*----------------------------------------------------------------------*/
	//171227�e�ۂ̕����ێ�

		//1213�ǋL
		float pitch2,yaw2;
		float r2 =  DistanceAtoB( &simdata.pastbullet, &simdata.bullet[1] );
		float power;

		float chargepower;

		//1220�ǋL�e�ۂ̃J�E���g
		int buretCount = 1;


		//1220�ǋL ������͂̃x�N�g������
		float upPower = 0;
		float sidePower = 0;

		//171227�ǋL
		if(simdata.mousebutton[2] == true)
		{
			if(simdata.playerBullet[1].throwPower <= 3.5)
			{
			//simdata.player.throwPower += 0.1;
			//171227�ǋL
			simdata.playerBullet[1].throwPower += 0.015;
			simdata.bulletPitch -= 0.045;
			}
		}

		simdata.playerBullet[1].upPower = 0;
		simdata.playerBullet[1].sidePower = 0;

		//180112�ǋL
		simdata.playerBullet[2].upPower = 0;
		simdata.playerBullet[2].sidePower = 0;

		//simdata.playerBurret[c].upPower = simdata.player.throwPower * sinf((simdata.pastburret.pitch +10 )  / 180.0 * PI);
		//simdata.playerBurret[c].sidePower = simdata.player.throwPower * cosf((simdata.pastburret.pitch +10) / 180.0 * PI );
		//171227�ǋL throwPower�̕����ێ��@�i�o���b�g��upPower�ւ̔��f���P���[�v�x��Ă�H�j
		simdata.playerBullet[1].upPower = simdata.playerBullet[1].throwPower * sinf((simdata.player.pitch + simdata.bulletPitch )  / 180.0 * PI);
		simdata.playerBullet[1].sidePower = simdata.playerBullet[1].throwPower * cosf((simdata.player.pitch + simdata. bulletPitch ) / 180.0 * PI );

		//180112�ǋL
		simdata.playerBullet[2].upPower = (simdata.playerBullet[1].throwPower + 0.1) * sinf((simdata.player.pitch + simdata.bulletPitch - 0.12 )  / 180.0 * PI);
		simdata.playerBullet[2].sidePower = (simdata.playerBullet[1].throwPower + 0.1) * cosf((simdata.player.pitch + simdata. bulletPitch - 0.12 ) / 180.0 * PI );


		yaw2 = simdata.bullet[1].yaw / 180.0 * PI;
		/*
		if(simdata.mousebutton[2] == false)
		{
		simdata.player.throwPower = 1.0;
		}
		*/
	

		if(	simdata.waitBullet == 2)
		{

			//������
			//                                     *0.3 = ������͂̋���(�e���͊܂܂Ȃ��B�������x�̂�)
			//y�����̗�

			simdata.playerBullet[1].gravity += 0.01;

			power = ( simdata.playerBullet[1].upPower - simdata.playerBullet[1].gravity );

			//x,z�����̗�
			//simdata.move_burret = simdata.playerBurret[c].sidePower;

			simdata.bullet[1].y += power ;

			simdata.bullet[1].x += simdata.playerBullet[1].sidePower * sinf( yaw2 );
			simdata.bullet[1].z += simdata.playerBullet[1].sidePower * cosf( yaw2 );
		}

		if(	simdata.waitBullet = 1)
		{
			simdata.waitBullet = 2;
		}

	
		//1227�e���\���p
		//�������1���o���b�g�̏����ʒu�Ɠ������o���b�g�͏����ʒu�����ς���Ă��炸�A�e�������̂܂܂ɂȂ��Ă�H�i�o���b�g�̕��̃o�O�j
		float yaw3 = 0;

		//171231�ǋL
		yaw3 = simdata.bullet[1].yaw / 180.0 * PI;

		if(simdata.mousebutton[2] == true)
		{
			for(int i = 0; i<N_BULLET; i++)
			{
				//simdata.routepower = (simdata.playerBurret[c].upPower - 0.03);
				yaw3 = simdata.player.yaw / 180.0 * PI;
				//                     gravity_burret��3�{�i3�t���[����j
				if(i >= 1)
				{
					//                             1227�ǋL
					simdata.bulletRoute[i].x = simdata.bulletRoute[i-1].x + (simdata.playerBullet[2].sidePower * sinf( yaw2 ));
					simdata.bulletRoute[i].y = simdata.bulletRoute[i-1].y + (simdata.playerBullet[2].upPower - (0.01 + 0.01 * i));
					simdata.bulletRoute[i].z = simdata.bulletRoute[i-1].z + (simdata.playerBullet[2].sidePower * cosf( yaw2 ));
				}
				if(i==0)
				{
					simdata.bulletRoute[i].x = simdata.player.x;
					simdata.bulletRoute[i].y = 1.0;
					simdata.bulletRoute[i].z = simdata.player.z;
					simdata.bulletRoute[i].yaw = simdata.player.yaw + 180.0;
				}
			}
		
			//171227�ǋL�@100���ŃJ�E���g���Z�b�g
			if (simdata.playerBullet[1].Count == N_BULLET-1)
			{
				simdata.playerBullet[1].Count = 0;
			}

		/*
		//1220�e���\���p
		float routepower = 0;
		float yaw3 = 0;

		if(simdata.mousebuttonB[2] == true)
		{
		for(int i = 0; i<10; i++)
		{
		simdata.burretRoute[i].x =simdata.player.x;
		simdata.burretRoute[i].y =simdata.player.y;
		simdata.burretRoute[i].z =simdata.player.z;
		simdata.burretRoute[i].yaw = simdata.player.yaw + 180.0;
		}
		for(int i = 0; i<10; i++)
		{
		simdata.burretRoute[i].turn = simdata.player.throwPower * sinf((simdata.player.pitch +20 )  / 180.0 * PI);
		simdata.burretRoute[i].move = simdata.player.throwPower * cosf((simdata.player.pitch +20) / 180.0 * PI );
		}
		for(int i = 0; i<10; i++)
		{
		routepower = (simdata.burretRoute[i].turn - ( 0.03 + 0.03 * (i+1)));
		yaw3 = simdata.player.yaw / 180.0 * PI;
		//                     gravity_burret��3�{�i3�t���[����j
		if(i >= 2)
		{
		//                             1227�ǋL
		simdata.burretRoute[i].x = simdata.burretRoute[i-1].x + (simdata.burretRoute[i].move * sinf( yaw2 ));
		simdata.burretRoute[i].y = simdata.burretRoute[i-1].y + routepower;
		simdata.burretRoute[i].z = simdata.burretRoute[i-1].z + (simdata.burretRoute[i].move * cosf( yaw2 ));
		}
		if(i==0)
		{
		simdata.burretRoute[i].x = simdata.player.x;
		simdata.burretRoute[i].y = 0;
		simdata.burretRoute[i].z = simdata.player.z;
		}
		}
		}
		*/

	}

		//1213�ǋL�Փ˔���
		simdata.player.status = 0; //�v���C�����͂����ň�U���Z�b�g
		for( int i = 0; i<N_BLOCKS; i++ ){
			if(simdata.block[i].status == 0){
				if( HitTest( &simdata.bullet[1], &simdata.block[i] )){
					//simdata.player.status = 1; //�v���C�����q�b�g
					simdata.block[i].status = 1; //���葤�q�b�g
					simdata.score += 100;
					printf("�X�R�A: %i \n",simdata.score);
					//printf("score: %i \n",simdata.score);
				}

				else{
					//simdata.block[i].status = 0; //���葤���Z�b�g
				}
			}
			if(simdata.block[i].status == 0){
	   		    if( HitTest( &simdata.bullet[1], &simdata.blockCollision[i] )){
					simdata.block[i].status = 1; //���葤�q�b�g
					simdata.score += 100;
					printf("�X�R�A: %i \n",simdata.score);
					//printf("score: %i \n",simdata.score);
				}
			}
		}

		for( int i = 0; i<500; i++ )
		{
			if( HitTest( &simdata.player, &simdata.wallA[i] ))
			{
				simdata.player.status = 1; //�v���C�����q�b�g
				printf( "�q�b�g�I\n" );
			}
			else
			{
				simdata.player.status = 0; //�v���C�����q�b�g
			}
		}

		//180112�ǋL
		if(simdata.dayTime == 1){
		for( int i = 0; i<N_STARS; i++ ){
			if(simdata.NPC[i].status == 0){
				if( HitTest( &simdata.bullet[1], &simdata.NPC[i] )){
					//simdata.player.status = 1; //�v���C�����q�b�g
					simdata.NPC[i].status = 1; //���葤�q�b�g
					simdata.score += 5;
					simdata.scoreStar += 5;
					printf("�X�R�A: %i \n",simdata.score);
					//printf("score: %i \n",simdata.score);
				}

				else{
					//simdata.block[i].status = 0; //���葤���Z�b�g
				}
			}
		}
		}

		

	//�{�X
		if(simdata.bossFlagB == false){
		if(simdata.scoreStar >= 300){
		    simdata.bossFlag = true;
		}
		}

		if(simdata.bossFlag){
			simdata.dayColor[0] = 0.01;
			simdata.dayColor[1] = 0.0;
			simdata.dayColor[2] = 0.0;
			if(simdata.boss.scale[0] <= 10.0){
			simdata.boss.scale[0] += 0.015;
			simdata.boss.scale[1] += 0.015;
			simdata.boss.scale[2] += 0.015;
			simdata.boss.y += 0.03;
			}
			if(simdata.boss.scale[0] >= 10.0){
				simdata.bossGrowFlag = true;
			}
			if(simdata.bullet[1].status == 0){
			if( HitTest( &simdata.bullet[1], &simdata.boss )){
					simdata.bossHP -= 1; //���葤�q�b�g
					simdata.score += 500;
					simdata.bullet[1].status = 1;
					//printf("<<�����Ď������>>�Ƀq�b�g!: %i \n",simdata.score);
					printf("�{�X�Ƀq�b�g! \n",simdata.score);
					printf("�X�R�A: %i \n",simdata.score);
					//printf("score: %i \n",simdata.score);
			}
			}
		}

		//180115�ǋL
		if(simdata.bossFlag && simdata.bossGrowFlag){
			euler_t dir_a2p;

			EulerAtoB( &simdata.boss, &simdata.player, &dir_a2p );

			simdata.boss.move = 0.00;

			simdata.boss.yaw = dir_a2p.yaw;
			simdata.boss.pitch = dir_a2p.pitch;
			simdata.boss.roll = dir_a2p.roll;
			MoveObject( &simdata.boss );



			for(int i = 0; i<N_BULLET ;i++)
			{
				if( simdata.bulletE[1].status == 0){

					simdata.bulletE[1].status = 1; //���ˏ�ԂɑJ��

					simdata.bulletE[1].move = 0.01; //�e���w��

					simdata.bulletE[1].x = simdata.boss.x;

					simdata.bulletE[1].y = simdata.boss.y;

					simdata.bulletE[1].z = simdata.boss.z;

					simdata.bulletE[1].yaw = simdata.boss.yaw; //���ˎ�̂̕�����e�ۂɐݒ�		
					simdata.bulletE[1].pitch = simdata.boss.pitch;
					simdata.bulletE[1].roll = simdata.boss.roll;
				}
				//��

				switch( simdata.bulletE[1].status ){
				case 0:
					break;
				case 1:

					//���ʒu�̍X�V
					MoveObject( &simdata.bulletE[1] );
					//���Փ˔���
					if( HitTest( &simdata.bulletE[1], &simdata.player) ){ //�������ځF�U���Ώ�
						simdata.bulletE[1].status = 2; //���e�Ɉڍs
						simdata.player.status = 1; 
						printf("\n");
						printf("���Ȃ��͔s�k���܂���\n\n");
						printf("�ŏI�X�R�A: %i\n\n",simdata.score);
						InitScene();
					}

					if( DistanceAtoB( &simdata.bulletE[1], &simdata.boss ) > 150.0 ){
						simdata.bulletE[1].status = 0; //�e������ĉ����܂Ō������烊�Z�b�g
					}			

					break;
				case 2:
					simdata.bulletE[1].status = 0; //�ȒP�̂��߁A���e�㏉����Ԃɕ��A
					break;
				}
			}
		}


//�t���O�ɂ��x�N�g���̔��]
	switch(flagA){
	case(false):
		simdata.heightA = simdata.heightA + (HEX_REDIUS/50);
		break;
	case(true):
		simdata.heightA = simdata.heightA - (HEX_REDIUS/50);
		break;
	default:
		break;
	}

	switch(flagB){
	case(false):
		simdata.heightB = simdata.heightB + (HEX_REDIUS/25);
		break;
	case(true):
		simdata.heightB = simdata.heightB - (HEX_REDIUS/25);
		break;
	default:
		break;
	}

	switch(flagC){
	case(false):
		simdata.heightC = simdata.heightC + (HEX_REDIUS/17);
		break;
	case(true):
		simdata.heightC = simdata.heightC - (HEX_REDIUS/17);
		break;
	default:
		break;
	}

	switch(flagD){
	case(false):
		simdata.heightD = simdata.heightD + (HEX_REDIUS/12);
		break;
	case(true):
		simdata.heightD = simdata.heightD - (HEX_REDIUS/12);
		break;
	default:
		break;
	}

		switch(flagE){
	case(false):
		simdata.heightE = simdata.heightE + (HEX_REDIUS/10);
		break;
	case(true):
		simdata.heightE = simdata.heightE - (HEX_REDIUS/10);
		break;
	default:
		break;
	}

			switch(flagF){
	case(false):
		simdata.heightF = simdata.heightF + (HEX_REDIUS/11);
		break;
	case(true):
		simdata.heightF = simdata.heightF - (HEX_REDIUS/11);
		break;
	default:
		break;
	}
	


//�t���O�̑J��

	const int moveHex = HEX_REDIUS * 40;

	switch((int)simdata.heightA){
	case(0):
		flagA = false;
		break;
	case(moveHex):
		flagA = true;
		break;
	default:
		break;
	}

	switch((int)simdata.heightB){
	case(0):
		flagB = false;
		break;
	case(moveHex):
		flagB = true;
		break;
	default:
		break;
	}

	switch((int)simdata.heightC){
	case(1):
		flagC = false;
		break;
	case(moveHex):
		flagC = true;
		break;
	default:
		break;
	}

	switch((int)simdata.heightD){
	case(1):
		flagD = false;
		break;
	case(moveHex):
		flagD = true;
		break;
	default:
		break;
	}

		switch((int)simdata.heightE){
	case(1):
		flagE = false;
		break;
	case(moveHex):
		flagE = true;
		break;
	default:
		break;
	}

			switch((int)simdata.heightF){
	case(1):
		flagF = false;
		break;
	case(moveHex):
		flagF = true;
		break;
	default:
		break;
	}

	//180112�ǋL
	simdata.air_color[0] = simdata.dayColor[0];
	simdata.air_color[1] = simdata.dayColor[1];
	simdata.air_color[2] = simdata.dayColor[2];

	//180112�ǋL
	if(simdata.bossHP == 0)
	{
		if(simdata.bossFlag){
		simdata.score += 1000;
		//printf("Congratuations!! You eliminated the moon signature!");
		printf("\n");
	    printf("���߂łƂ��������܂�!!�@�{�X��|���܂���!\n");
	    //printf("���߂łƂ��������܂�!!�@<<�����Ď������>>��|���܂���!");
	    printf("�X�R�A: %i \n\n",simdata.score);
		}
	simdata.bossFlag = false;
	simdata.bossFlagB = true;
	simdata.bossGrowFlag = false;
	
	}

	//
	if(simdata.kbdR){
		simdata.kbdR = false;
		InitScene();
	}

	////////
    return;
}
