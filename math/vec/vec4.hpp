#ifndef VECTOR4_INCLUDED_9C1AD544_35E1_456B_B882_DB0C9DAEBF09
#define VECTOR4_INCLUDED_9C1AD544_35E1_456B_B882_DB0C9DAEBF09


/*
  Vector4
  4D vector interface and impl.
*/


#include "vec_types.hpp"
#include "vec3.hpp"
#include "vec2.hpp"
#include "../general/general.hpp"
#include <array>
#include <assert.h>


namespace math {


// ** Interface ** //


// Constants
MATH_VEC4_INLINE vec4                   vec4_zero();
MATH_VEC4_INLINE vec4                   vec4_one();
MATH_VEC4_INLINE vec4                   vec4_zero_zero_zero_one();

// Initialize vector.
MATH_VEC4_INLINE vec4                   vec4_init(const float val);
MATH_VEC4_INLINE vec4                   vec4_init(const float x, const float y, const float z, const float w);
MATH_VEC4_INLINE vec4                   vec4_init(const vec2 vec, const float z ,const float w);
MATH_VEC4_INLINE vec4                   vec4_init(const vec3 vec, const float w);
MATH_VEC4_INLINE vec4                   vec4_init_with_array(const float *arr);
MATH_VEC4_INLINE vec4                   vec4_init_with_array(const std::array<float, 4> &vec);

// Get components.
MATH_VEC4_INLINE float                  vec4_get_x(const vec4 vec);
MATH_VEC4_INLINE float                  vec4_get_y(const vec4 vec);
MATH_VEC4_INLINE float                  vec4_get_z(const vec4 vec);
MATH_VEC4_INLINE float                  vec4_get_w(const vec4 vec);
MATH_VEC4_INLINE void                   vec4_to_array(const vec4 a, float *out_array);
MATH_VEC4_INLINE std::array<float, 4>   vec4_to_std_array(const vec4 a);

// Component wise arithmetic.
MATH_VEC4_INLINE vec4                   vec4_add(const vec4 a, const vec4 b);
MATH_VEC4_INLINE vec4                   vec4_subtract(const vec4 a, const vec4 b);
MATH_VEC4_INLINE vec4                   vec4_multiply(const vec4 a, const vec4 b);
MATH_VEC4_INLINE vec4                   vec4_divide(const vec4 a, const vec4 b);

// Special operations.
MATH_VEC4_INLINE vec4                   vec4_lerp(const vec4 start, const vec4 end, const float dt);
//MATH_VEC4_INLINE vec4                 vec4_slerp(const vec4 start, const vec4 end, const float dt); // not impl
MATH_VEC4_INLINE vec4                   vec4_scale(const vec4 a, const float scale);
MATH_VEC4_INLINE vec4                   vec4_normalize(const vec4 a);
MATH_VEC4_INLINE float                  vec4_length(const vec4 a);
MATH_VEC4_INLINE float                  vec4_dot(const vec4 a, const vec4 b);

// Equality
MATH_VEC4_INLINE bool                   vec4_is_equal(const vec4 a, const vec4 b);
MATH_VEC4_INLINE bool                   vec4_is_not_equal(const vec4 a, const vec4 b);
MATH_VEC4_INLINE bool                   vec4_is_near(const vec4 a, const vec4 b, const float error);
MATH_VEC4_INLINE bool                   vec4_is_not_near(const vec4 a, const vec4 b, const float error);



// ** Implimentation ** //

// This is not part of public interface, keep walking :)
// Using anything in the detail namespace is undefined-behaviour.
namespace detail
{
  struct internal_vec4
  {
    float x, y, z, w;
  };
}


// Constants
vec4
vec4_zero()
{
  return vec4_init(0.f);
}


vec4
vec4_one()
{
  return vec4_init(1.f);
}


vec4
vec4_zero_zero_zero_one()
{
  return vec4_init(0.f, 0.f, 0.f, 1.f);
}


// Initialize

vec4
vec4_init(const float val)
{
  return vec4_init(val, val, val, val);
}


vec4
vec4_init(const float x, const float y, const float z, const float w)
{
  vec4 return_vec;
  detail::internal_vec4 *internal_vec = reinterpret_cast<detail::internal_vec4*>(&return_vec);

  internal_vec->x = x;
  internal_vec->y = y;
  internal_vec->z = z;
  internal_vec->w = w;

  return return_vec;
}


vec4
vec4_init(const vec2 vec, const float z ,const float w)
{
  return vec4_init(vec2_get_x(vec), vec2_get_y(vec), z, w);
}


vec4
vec4_init(const vec3 vec, const float w)
{
  return vec4_init(vec3_get_x(vec), vec3_get_y(vec), vec3_get_z(vec), w);
}


vec4
vec4_init_with_array(const float *arr)
{
  return vec4_init(arr[0], arr[1], arr[2], arr[3]);
}


vec4
vec4_init_with_array(const std::array<float, 4> &arr)
{
  return vec4_init(arr.at(0), arr.at(1), arr.at(2), arr.at(3));
}


// Get components.

float
vec4_get_x(const vec4 vec)
{
  const detail::internal_vec4 *internal_vec = reinterpret_cast<const detail::internal_vec4*>(&vec);
  return internal_vec->x;
}


float
vec4_get_y(const vec4 vec)
{
  const detail::internal_vec4 *internal_vec = reinterpret_cast<const detail::internal_vec4*>(&vec);
  return internal_vec->y;
}


float
vec4_get_z(const vec4 vec)
{
  const detail::internal_vec4 *internal_vec = reinterpret_cast<const detail::internal_vec4*>(&vec);
  return internal_vec->z;
}


float
vec4_get_w(const vec4 vec)
{
  const detail::internal_vec4 *internal_vec = reinterpret_cast<const detail::internal_vec4*>(&vec);
  return internal_vec->w;
}


void
vec4_to_array(const vec4 a, float *out_array)
{
  out_array[0] = vec4_get_x(a);
  out_array[1] = vec4_get_y(a);
  out_array[2] = vec4_get_z(a);
  out_array[3] = vec4_get_w(a);
}


std::array<float, 4>
vec4_to_std_array(const vec4 a)
{
  std::array<float, 4> return_array = {
    vec4_get_x(a),
    vec4_get_y(a),
    vec4_get_z(a),
    vec4_get_w(a),
  };

  return return_array;
}


vec4
vec4_add(const vec4 a, const vec4 b)
{
  const detail::internal_vec4 *vec_a = reinterpret_cast<const detail::internal_vec4*>(&a);
  const detail::internal_vec4 *vec_b = reinterpret_cast<const detail::internal_vec4*>(&b);

  return vec4_init(vec_a->x + vec_b->x, vec_a->y + vec_b->y, vec_a->z + vec_b->z, vec_a->w + vec_b->w);
}


vec4
vec4_subtract(const vec4 a, const vec4 b)
{
  const detail::internal_vec4 *vec_a = reinterpret_cast<const detail::internal_vec4*>(&a);
  const detail::internal_vec4 *vec_b = reinterpret_cast<const detail::internal_vec4*>(&b);

  return vec4_init(vec_a->x - vec_b->x, vec_a->y - vec_b->y, vec_a->z - vec_b->z, vec_a->w - vec_b->w);
}


vec4
vec4_multiply(const vec4 a, const vec4 b)
{
  const detail::internal_vec4 *vec_a = reinterpret_cast<const detail::internal_vec4*>(&a);
  const detail::internal_vec4 *vec_b = reinterpret_cast<const detail::internal_vec4*>(&b);

  return vec4_init(vec_a->x * vec_b->x, vec_a->y * vec_b->y, vec_a->z * vec_b->z, vec_a->w * vec_b->w);
}


vec4
vec4_divide(const vec4 a, const vec4 b)
{
  const detail::internal_vec4 *vec_a = reinterpret_cast<const detail::internal_vec4*>(&a);
  const detail::internal_vec4 *vec_b = reinterpret_cast<const detail::internal_vec4*>(&b);

  // Divide by zero check.
  assert(vec_b->x != 0 && vec_b->y != 0 && vec_b->z != 0 && vec_b->w != 0);

  return vec4_init(vec_a->x / vec_b->x, vec_a->y / vec_b->y, vec_a->z / vec_b->z, vec_a->w / vec_b->w);
}


vec4
vec4_lerp(const vec4 start, const vec4 end, const float dt)
{
  const vec4 difference = vec4_subtract(end, start);
  const vec4 scaled     = vec4_scale(difference, dt);
  const vec4 position   = vec4_add(start, scaled);

  return position;
}


// vec4
// vec4_slerp(const vec4 start, const vec4 end, const float dt)
// {

// }



vec4
vec4_scale(const vec4 a, const float scale)
{
  const vec4 scale_vec = vec4_init(scale);
  return vec4_multiply(a, scale_vec);
}


vec4
vec4_normalize(const vec4 a)
{
  const float length = vec4_length(a);

  assert(length != 0); // Don't pass zero vectors. (0,0,0,0);

  return vec4_scale(a, (1.f / length));
}


float
vec4_length(const vec4 a)
{
  const detail::internal_vec4 *vec_a = reinterpret_cast<const detail::internal_vec4*>(&a);
  const float x = vec_a->x * vec_a->x + vec_a->y * vec_a->y + vec_a->z * vec_a->z + vec_a->w * vec_a->w;

  return sqrt(x);
}


float
vec4_dot(const vec4 a, const vec4 b)
{
   return (vec4_get_x(a) * vec4_get_x(b)) +
          (vec4_get_y(a) * vec4_get_y(b)) +
          (vec4_get_z(a) * vec4_get_z(b)) + 
          (vec4_get_w(a) * vec4_get_w(b));
}


bool
vec4_is_equal(const vec4 a, const vec4 b)
{
	return
  (
    (vec4_get_x(a) == vec4_get_x(b))
    &&
    (vec4_get_y(a) == vec4_get_y(b))
    &&
    (vec4_get_z(a) == vec4_get_z(b))
    &&
    (vec4_get_w(a) == vec4_get_w(b))
  );
}


bool
vec4_is_not_equal(const vec4 a, const vec4 b)
{
	return !vec4_is_equal(a, b);
}


bool
vec4_is_near(const vec4 a, const vec4 b, const float error)
{
  return(
    is_near(vec4_get_x(a), vec4_get_x(b), error)
    &&
    is_near(vec4_get_y(a), vec4_get_y(b), error)
    &&
    is_near(vec4_get_z(a), vec4_get_z(b), error)
    &&
    is_near(vec4_get_w(a), vec4_get_w(b), error)
  );
}


bool
vec4_is_not_near(const vec4 a, const vec4 b, const float error)
{
	return !vec4_is_near(a, b, error);
}


} // namespace


#endif // include guard