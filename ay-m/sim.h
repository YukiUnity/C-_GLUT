

//このヘッダファイルをインクルードすると、
//ほかのファイルで定義されている関数が使用可能に
//インクルードの方法 : #include "sim.h"

#define N_HEXS 50
#define N_BLOCKS 10
#define N_STARS 6000
#define N_CLOUDS 300
#define HEX_REDIUS 1.5
#define N_BULLET 100
#define N_MAPMOVEX 200
#define N_MAPMOVEZ 100
/*-----------------------▼関数プロトタイプ宣言------------------------*/

// sim.cpp
void InitScene( void );
void UpdateScene( void );

// draw.cpp
void DrawScene( void );


/*--------------------------▼データ構造の定義-------------------------*/

//-------- object data　物体の状態を保存するための構造体
typedef struct {
    float x, y, z; //★位置
	float roll, pitch, yaw; //★姿勢（回転）
	int state;
	bool visible; //描画の可否

	float scale[3];

	float pastx,pasty,pastz;

	float radius;

	bool status;

	//1220追記投げる強さ
	float throwPower;

	//1220追記追跡と退避用
	float turn; //回転の角速度 　〔度／フレーム〕
	float move; //速度　〔m／フレーム〕

} ObjDataT;

typedef struct {

	int Count;
	float throwPower;
	float upPower;
	float sidePower;
	float gravity;

} Burret_T;

//-------1220追記ベクトル構造体
typedef struct {

	float x, y, z;

} vector_t;

//------ 1220追記オイラー角構造体
typedef struct {

	float roll, pitch, yaw;

} euler_t;


//-------- simulation data
typedef struct {

	float clip_near;
	float clip_far;
	float air_color[4];
	float sky_color[4];
    //////// ユーザ定義のシーンについてのデータをここにまとめる

	float move_player; //プレイヤの移動量（1フレーム分：学校PCは60Hzなので1秒間6ｍくらい(設定値0.1)）★
	float turn_player; //プレイヤの旋回量（1フレーム分）★
	//171227
	float turn_player2;

	float move_bullet;
	float turn_bullet;
	float gravity_bullet;

	float throwpower;

	ObjDataT player; //プレイヤ★
	ObjDataT playerNextpos;
    ObjDataT objectA; //オブジェクトA★
	ObjDataT objectB; //オブジェクトB★

	ObjDataT boss;

	ObjDataT bullet[N_BULLET]; //弾

	ObjDataT bulletRoute[N_BULLET];
	ObjDataT pastbullet; //弾

	ObjDataT bulletE[N_BULLET]; //弾

	ObjDataT *active_camera; //カメラオブジェクトへのポインタ★★

	ObjDataT wallA[500];
	int wallCount;

	/////マウス設定用の変数
	bool mousebutton[3]; //★1115マウスボタンが押されているかどうか
	bool mousebuttonB[3]; //押した時一度だけ
	//0:左, 1:中, 2:右
	float mouseX, mouseY; //★マウス座標 -1から+1までの間の値に正規化
	float mouseX0, mouseY0; //前のマウス座標
	float mousedX, mousedY; //マウス移動量
	/////

	/////201711/22固定オブジェクトのデータ
	//N_BLOCKSを用いることで、"sim.cpp"、"draw.cpp"の数値を一括で変更可能（ハードコーディングでない）
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

	//180110追記
	float bulletPitch;

	//180112追記
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
