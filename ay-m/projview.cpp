#include <stdlib.h>
#include <stdio.h>

#include "platform.h"

#include "common.h"

#include "sim.h"

extern SimDataT simdata;
extern WindowDataT window;

/*---------------------------------------------------------------- Viewing
 * Viewing:
 *--------*/
void Viewing( void )
{

	if(simdata.active_camera == &simdata.player )
	{
	//★active_cameraを基準とした相対的なオフセット
    gluLookAt( -2.0, 3.5, 8.0, -1.8, 2.0, -10.0, 0.0, 1.0, 0.0 );
	}

	else
	{
		if(simdata.active_camera == &simdata.objectB )
	{
		gluLookAt( 0.0, 2.5, -8.0, 0, 0.0, 0.0, 0.0, 1.0, 0.0 );
		}
		else{
	gluLookAt( 2.0, 3.5, 8.0, 1.8, 2.0, -10.0, 0.0, 1.0, 0.0 );
    //gluLookAt( 0.0, 8.0, 8.0, 0.0, 2.0, 0.0, 0.0, 1.0, 0.0 );
		}
	}

	//★ビューイング変換＝カメラオブジェクトの位置・姿勢の逆変換
	//gluLookAtでビューイング変換値にplayerと同じ移動量を加算
	//▼ここから下は、今後変更する必要はありません
	

	glRotatef( - simdata.active_camera->roll,  0.0, 0.0, 1.0);
	glRotatef( - simdata.active_camera->pitch, 1.0, 0.0, 0.0);
	glRotatef( - simdata.active_camera->yaw,   0.0, 1.0, 0.0);
	glTranslatef(
		- simdata.active_camera->x,
		- simdata.active_camera->y,
		- simdata.active_camera->z);

}
/*---------------------------------------------------------------- Projection
 * Projection:
 *--------*/
void Projection( void )
{
	//---- begin projection transformation
    gluPerspective( 45.0, window.aspect, simdata.clip_near, simdata.clip_far );
}
