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
	//��active_camera����Ƃ������ΓI�ȃI�t�Z�b�g
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

	//���r���[�C���O�ϊ����J�����I�u�W�F�N�g�̈ʒu�E�p���̋t�ϊ�
	//gluLookAt�Ńr���[�C���O�ϊ��l��player�Ɠ����ړ��ʂ����Z
	//���������牺�́A����ύX����K�v�͂���܂���
	

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
