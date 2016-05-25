#ifndef VEC4_CLASS_H
#define VEC4_CLASS_H

#include "vec3.h"

// This macro is used by the vec4 class to generate
// the code for overloaded operators. You will not be
// calling this directly yourself.
#define VEC4_BINARY_COMPONENT_OPERATOR(_oper_) \
   vec4<element_type> operator _oper_ (const_reference v) const\
   { \
      vec4<element_type> a; \
      a.x = x _oper_ v.x; \
      a.y = y _oper_ v.y; \
      a.z = z _oper_ v.z; \
	  a.w = w _oper_ v.w; \
      return a; \
   }

// This macro is used by the vec4 class to generate
// the code for overloaded operators. You will not be
// calling this directly yourself.
#define VEC4_BINARY_BROADCAST_OPERATOR(_oper_) \
   template <typename FT> \
   vec4<element_type> operator _oper_ (FT fValue) const \
   { \
      vec4<element_type> a; \
      a.x = x _oper_ fValue; \
      a.y = y _oper_ fValue; \
      a.z = z _oper_ fValue; \
	  a.w = w _oper_ fValue; \
      return a; \
   }

// This macro is used by the vec4 class to generate
// the code for overloaded operators. You will not be
// calling this directly yourself.
#define VEC4_COMPONENT_OPERATOR(_oper_) \
   reference operator _oper_ (const_reference v) \
   { \
      if (static_cast<void *>(this) != static_cast<const void *>(&v)) \
      { \
         x _oper_ v.x; \
         y _oper_ v.y; \
         z _oper_ v.z; \
		 w _oper_ v.w; \
      } \
      return *this; \
   }

// This macro is used by the vec4 class to generate
// the code for overloaded operators. You will not be
// calling this directly yourself.
#define VEC4_BROADCAST_OPERATOR(_oper_) \
   template <typename FT> \
   reference operator _oper_ (FT fValue) \
   { \
      x _oper_ fValue; \
      y _oper_ fValue; \
      z _oper_ fValue; \
	  w _oper_ fValue; \
      return *this; \
   }

// vec4
//
// Templated class for 3-Dimensional vectors.
//
// Use vec4<float> or vec4f for floats.
// Use vec4<double> or vec4d for doubles.
template <typename T>
struct vec4
{
   //typedef to create an alias for vec4& named reference
   typedef vec4 &reference;
  
   //typedef to create an alias for const vec4& named const_reference
   typedef vec4 const &const_reference;

   // typedef to create an alias for T named element_type
   typedef T element_type;

   // This union allows the data of the vec4 to be accessed
   // in different ways with no additional overhead.
   //
   // Given the vector,
   // 
   //	vec4f myVector;
   //
   // You can access it's data the following ways...
   union
   {
	  // Access the vector as an array of floating point values by using myVector.v
      element_type v[4];

	  // Access the individuals components of the vector with myVector.x, myVector.y, or myVector.z
      struct
      {
		element_type x;
		element_type y;
		element_type z;
		element_type w;
      };

	  struct
	  {
		element_type r;
		element_type g;
		element_type b;
		element_type a;
	  };

	  struct
	  {
		vec3< element_type > xyz;
		element_type w_pad;
	  };
   };

   // Constructor
   vec4(void) { }
   // Destructor
   ~vec4(void) { }
   // Constructor given X,Y,Z values
   vec4(element_type vx, element_type vy, element_type vz, element_type vw) : x(vx), y(vy), z(vz), w(vw) { }

   template< typename VT>
   vec4( const vec3<VT> &v, element_type vw ) : x(v.x), y(v.y), z(v.z), w(vw) { }

   // Copy Constructor
   template <typename VT>
   vec4(const vec4<VT> &v) : x(v.x), y(v.y), z(v.z), w(v.w) { }

   // These are simply macros that generate the code for the
   // various overloaded operators. Simply use +,-,*,/,=,+=,
   // -=, *=, /= operators with vec4 objects to use them.
   // 
   // Ex:
   //
   //	vec4f vecA( 1.0f, 0.0f, 0.0f);
   //	vec4f vecB( 0.0f, 1.0f, 0.0f);
   //
   //	vecA += vecB;
   //
   VEC4_BINARY_COMPONENT_OPERATOR(+);
   VEC4_BINARY_COMPONENT_OPERATOR(-);
   VEC4_BINARY_COMPONENT_OPERATOR(*);
   VEC4_BINARY_COMPONENT_OPERATOR(/);

   VEC4_BINARY_BROADCAST_OPERATOR(+);
   VEC4_BINARY_BROADCAST_OPERATOR(-);
   VEC4_BINARY_BROADCAST_OPERATOR(*);
   VEC4_BINARY_BROADCAST_OPERATOR(/);

   VEC4_COMPONENT_OPERATOR(=);
   VEC4_COMPONENT_OPERATOR(+=);
   VEC4_COMPONENT_OPERATOR(-=);
   VEC4_COMPONENT_OPERATOR(*=);
   VEC4_COMPONENT_OPERATOR(/=);

   VEC4_BROADCAST_OPERATOR(=);
   VEC4_BROADCAST_OPERATOR(+=);
   VEC4_BROADCAST_OPERATOR(-=);
   VEC4_BROADCAST_OPERATOR(*=);
   VEC4_BROADCAST_OPERATOR(/=);

   // Zero out the vector
   reference make_zero(void)
   {
      x = y = z = w = 0;
      return *this;
   }

   // Normalize the vector
   reference normalize(void)
   {
      element_type rsqrt = 1 / magnitude();
      x *= rsqrt;
      y *= rsqrt;
      z *= rsqrt;
	  w *= rsqrt;
      return *this;
   }

   // Negate the vector
   reference negate(void)
   {
      x = -x;
      y = -y;
      z = -z;
	  w = -w;
      return *this;
   }

   // Calculate and return the magnitude of the vector
   element_type magnitude(void)
   {
      element_type fMagnitude;
      fMagnitude = (T)sqrt(x*x + y*y + z*z + w*w);
      return fMagnitude;
   }

   // dot_product
   //
   // Calculat the dot product of two vectors
   //
   // In:
   //	const_reference vectorA - constant reference to the first vector
   //	const_reference vectorB - constant reference to the second vector
   //
   // Return:
   //	element_type - the result of the dotproduct
   inline friend element_type dot_product(const_reference vectorA, const_reference vectorB)
   {
		return vectorA.x*vectorB.x+vectorA.y*vectorB.y+vectorA.z*vectorB.z+vectorA.w*vectorB.w;
   }

  // // cross_product
  // //
  // // Calculate the cross product of two vectors
  // //
  // // In:
  // //	const_reference vectorA - constant reference to the first vector
  // //	const_reference vectorB - constant reference to the second vector
  // //
  // // Out:
  // //	reference vectorO - Where to store the result of the cross product
  // //	
  // // Remarks:
  // //	
  // //	vectorO must not be a reference to the same object as vectorA or vectorB
  // inline friend void cross_product(reference vectorO, const_reference vectorA, const_reference vectorB )
  // {
		//vectorO.x = vectorA.y*vectorB.z - vectorA.z*vectorB.y;
		//vectorO.y = vectorA.z*vectorB.x - vectorA.x*vectorB.z;
		//vectorO.z = vectorA.x*vectorB.y - vectorA.y*vectorB.x;
  // }

   // interpolate
   //
   // Linear interpolate two vectors based on a lamda value and return the resulting vector.
   //
   // In:
   //	const_reference vectorA - constant reference to the first vector
   //	const_reference vectorB - constant reference to the second vector
   //
   // Return:
   //
   //	vec4<T> - The resulting vector from the interpolation	
   inline friend vec4<T> interpolate(const_reference vectorA, const_reference vectorB, float lamda)
   {
       return vectorA + ((vectorB - vectorA) * lamda);
   }

   element_type *array(void) { return this->v; }
   element_type const *array(void) const { return this->v; }
};

// typedef to create an alias for vec4<float> named vec4f
typedef vec4<float> vec4f;
// typedef to create an alias for vec4<double> named vec4d
typedef vec4<double> vec4d;
// typedef to create an alias for vec4f named float3
typedef vec4f float4;
// typedef to create an alias for vec4d named double3
typedef vec4d double4;

#endif
