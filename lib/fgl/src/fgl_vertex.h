#ifndef __fgl_vertex_h__
#define __fgl_vertex_h__

#include <stdbool.h>
#include <stdint.h>

#include <libfixmath/fixmath.h>

typedef struct {
	fix16_t x, y, z;
} fgl_vec3_t;

// TODO: make many of these inline?
fix16_t fix16_add3(fix16_t a, fix16_t b, fix16_t c);

fix16_t fgl_vec3_magnitude(fgl_vec3_t in);

fgl_vec3_t fgl_vec3_scale(fgl_vec3_t in, fix16_t s);

void fgl_vec3_print(fgl_vec3_t inVertex);

fgl_vec3_t fgl_vec3_add(fgl_vec3_t a, fgl_vec3_t b);

fgl_vec3_t fgl_vec3_sub(fgl_vec3_t a, fgl_vec3_t b);

fgl_vec3_t fgl_vec3_normalize(fgl_vec3_t in);

fix16_t fgl_vec3_dot(fgl_vec3_t a, fgl_vec3_t b);

fgl_vec3_t fgl_vec3_cross(fgl_vec3_t a, fgl_vec3_t b);

#endif // __fgl_vertex_h__
