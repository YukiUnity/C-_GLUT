#include <math.h>

void DirectionAtoB( ObjDataT *a, ObjDataT *b, vector_t *dir );
float VectorNorm( vector_t *v );
float DistanceAtoB( ObjDataT *a, ObjDataT *b );
float VectorToPolar( vector_t *v,  euler_t *angle );
float EulerAtoB( ObjDataT *a, ObjDataT *b, euler_t *angle );
void MoveObject( ObjDataT *obj );
