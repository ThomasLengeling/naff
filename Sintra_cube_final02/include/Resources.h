#pragma once
#include "cinder/CinderResources.h"

//#define RES_MY_RES			CINDER_RESOURCE( ../resources/, image_name.png, 128, IMAGE )



#define SATIE               CINDER_RESOURCE( ../resources/, satie_01.mp3, 127, MP3 )
#define DRUMS               CINDER_RESOURCE( ../resources/, Satie_Batacas.mp3, 128, MP3 )

#define RES_VERT_GLSL		CINDER_RESOURCE( ../resources/, blur_vert.glsl, 129, GLSL )
#define RES_FRAG_GLSL		CINDER_RESOURCE( ../resources/, blur_frag.glsl, 130, GLSL )

#define PHONG_FRAG_GLSL		CINDER_RESOURCE( ../resources/, phong_frag.glsl, 131, GLSL )
#define PHONG_VERT_GLSL		CINDER_RESOURCE( ../resources/, phong_vert.glsl, 132, GLSL )


#define MASK                CINDER_RESOURCE( ../resources/, mask.jpg, 133, IMG )


#define RES_CUBE1_ID		CINDER_RESOURCE( ../resources/, cubePosX.jpg,		138, IMAGE )
#define RES_CUBE2_ID		CINDER_RESOURCE( ../resources/, cubePosY.jpg,		139, IMAGE )
#define RES_CUBE3_ID		CINDER_RESOURCE( ../resources/, cubePosZ.jpg,		140, IMAGE )
#define RES_CUBE4_ID		CINDER_RESOURCE( ../resources/, cubeNegX.jpg,		141, IMAGE )
#define RES_CUBE5_ID		CINDER_RESOURCE( ../resources/, cubeNegY.jpg,		142, IMAGE )
#define RES_CUBE6_ID		CINDER_RESOURCE( ../resources/, cubeNegZ.jpg,		143, IMAGE )


#define ROOM_VERT		CINDER_RESOURCE( ../resources/, room.vert, 144, GLSL )
#define ROOM_FRAG		CINDER_RESOURCE( ../resources/, room.frag, 145, GLSL )

#define SPHERE_VERT		CINDER_RESOURCE( ../resources/, sphere.vert, 146, GLSL )
#define SPHERE_FRAG		CINDER_RESOURCE( ../resources/, sphere.frag, 147, GLSL )