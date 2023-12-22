

//���̃w�b�_�t�@�C�����C���N���[�h����ƁA
//�ق��̃t�@�C���Œ�`����Ă���֐����g�p�\��
//�C���N���[�h�̕��@ : #include "sim.h"

#define N_HEXS 50
#define N_BLOCKS 10
#define N_STARS 6000
#define N_CLOUDS 300
#define HEX_REDIUS 1.5
#define N_BULLET 100
#define N_MAPMOVEX 200
#define N_MAPMOVEZ 100
/*-----------------------���֐��v���g�^�C�v�錾------------------------*/

// sim.cpp
void InitScene( void );
void UpdateScene( void );

// draw.cpp
void DrawScene( void );


/*--------------------------���f�[�^�\���̒�`-------------------------*/

//-------- object data�@���̂̏�Ԃ�ۑ����邽�߂̍\����
typedef struct {
    float x, y, z; //���ʒu
	float roll, pitch, yaw; //���p���i��]�j
	int state;
	bool visible; //�`��̉�

	float scale[3];

	float pastx,pasty,pastz;

	float radius;

	bool status;

	//1220�ǋL�����鋭��
	float throwPower;

	//1220�ǋL�ǐՂƑޔ�p
	float turn; //��]�̊p���x �@�k�x�^�t���[���l
	float move; //���x�@�km�^�t���[���l

} ObjDataT;

typedef struct {

	int Count;
	float throwPower;
	float upPower;
	float sidePower;
	float gravity;

} Burret_T;

//-------1220�ǋL�x�N�g���\����
typedef struct {

	float x, y, z;

} vector_t;

//------ 1220�ǋL�I�C���[�p�\����
typedef struct {

	float roll, pitch, yaw;

} euler_t;


//-------- simulation data
typedef struct {

	float clip_near;
	float clip_far;
	float air_color[4];
	float sky_color[4];
    //////// ���[�U��`�̃V�[���ɂ��Ẵf�[�^�������ɂ܂Ƃ߂�

	float move_player; //�v���C���̈ړ��ʁi1�t���[�����F�w�ZPC��60Hz�Ȃ̂�1�b��6�����炢(�ݒ�l0.1)�j��
	float turn_player; //�v���C���̐���ʁi1�t���[�����j��
	//171227
	float turn_player2;

	float move_bullet;
	float turn_bullet;
	float gravity_bullet;

	float throwpower;

	ObjDataT player; //�v���C����
	ObjDataT playerNextpos;
    ObjDataT objectA; //�I�u�W�F�N�gA��
	ObjDataT objectB; //�I�u�W�F�N�gB��

	ObjDataT boss;

	ObjDataT bullet[N_BULLET]; //�e

	ObjDataT bulletRoute[N_BULLET];
	ObjDataT pastbullet; //�e

	ObjDataT bulletE[N_BULLET]; //�e

	ObjDataT *active_camera; //�J�����I�u�W�F�N�g�ւ̃|�C���^����

	ObjDataT wallA[500];
	int wallCount;

	/////�}�E�X�ݒ�p�̕ϐ�
	bool mousebutton[3]; //��1115�}�E�X�{�^����������Ă��邩�ǂ���
	bool mousebuttonB[3]; //����������x����
	//0:��, 1:��, 2:�E
	float mouseX, mouseY; //���}�E�X���W -1����+1�܂ł̊Ԃ̒l�ɐ��K��
	float mouseX0, mouseY0; //�O�̃}�E�X���W
	float mousedX, mousedY; //�}�E�X�ړ���
	/////

	/////201711/22�Œ�I�u�W�F�N�g�̃f�[�^
	//N_BLOCKS��p���邱�ƂŁA"sim.cpp"�A"draw.cpp"�̐��l���ꊇ�ŕύX�\�i�n�[�h�R�[�f�B���O�łȂ��j
	ObjDataT block[N_BLOCKS];
	ObjDataT blockCollision[N_BLOCKS];

	ObjDataT NPC[N_STARS];

	ObjDataT cloud[N_CLOUDS];
	int cloudCount[N_CLOUDS];
	/////

	int count[N_HEXS*2][N_HEXS*2];

	int starCount[N_STARS];
	float heightA,heightB,heightC,heightD,heightE,heightF;

	//171227
	Burret_T playerBullet[N_BULLET];
	Burret_T routeBullet[N_BULLET];

	int waitBullet;
	float routepower;
	bool mousebuttonDown[10];

	//180110�ǋL
	float bulletPitch;

	//180112�ǋL
	int dayTime;
	float dayColor[3];
	float dayColor2[3];

	//
	int score;
	int scoreStar;
	bool bossFlag;
	bool bossFlagB;
	bool bossGrowFlag;
	int bossHP;
	float bossScale;

	bool kbdR;
	bool kbdF;

	////////
} SimDataT;
