#pragma once

// OJP utility type headers consolidated into one include

#include "qcommon/ojp_shared.h"

// Vector types
typedef ojp_ivec3_t ojp_vec3pair_t[2];

// Graph types
typedef struct {
	float cost;
	int node;
} ojp_graphEdge_t;

typedef struct {
	int numEdges;
	ojp_graphEdge_t edges[16];
} ojp_graphNode_t;

typedef struct {
	char name[64];
	vec3_t origin;
	int flags;
} ojp_mapPoint_t;

typedef struct {
	int points[16];
	int numPoints;
} ojp_path_t;

typedef struct {
	float data[16];
} ojp_pool_t;

#define OJP_BITS_SET(vec, bit) ((vec)[(bit) >> 5] |= (1 << ((bit) & 31)))
#define OJP_BITS_CLEAR(vec, bit) ((vec)[(bit) >> 5] &= ~(1 << ((bit) & 31)))
#define OJP_BITS_CHECK(vec, bit) ((vec)[(bit) >> 5] & (1 << ((bit) & 31)))

#define OJP_VECTOR_NULL { 0, 0, 0 }
#define OJP_VECTOR_X { 1, 0, 0 }
#define OJP_VECTOR_Y { 0, 1, 0 }
#define OJP_VECTOR_Z { 0, 0, 1 }
