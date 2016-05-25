#ifndef VEC2_CLASS_H
#define VEC2_CLASS_H

// This macro is used by the vec2 class to generate
// the code for overloaded operators. You will not be
// calling this directly yourself.
#define VEC2_BINARY_COMPONENT_OPERATOR(_oper_) \
   vec2<element_type> operator _oper_ (const_reference v) const\
   { \
      vec2<element_type> a; \
      a.x = x _oper_ v.x; \
      a.y = y _oper_ v.y; \
      return a; \
   }

// This macro is used by the vec2 class to generate
// the code for overloaded operators. You will not be
// calling this directly yourself.
#define VEC2_BINARY_BROADCAST_OPERATOR(_oper_) \
   template <typename FT> \
   vec2<element_type> operator _oper_ (FT fValue) const \
   { \
      vec2<element_type> a; \
      a.x = x _oper_ fValue; \
      a.y = y _oper_ fValue; \
      return a; \
   }

// This macro is used by the vec2 class to generate
// the code for overloaded operators. You will not be
// calling this directly yourself.
#define VEC2_COMPONENT_OPERATOR(_oper_) \
   reference operator _oper_ (const_reference v) \
   { \
      if (static_cast<void *>(this) != static_cast<const void *>(&v)) \
      { \
         x _oper_ v.x; \
         y _oper_ v.y; \
      } \
      return *this; \
   }

// This macro is used by the vec2 class to generate
// the code for overloaded operators. You will not be
// calling this directly yourself.
#define VEC2_BROADCAST_OPERATOR(_oper_) \
   template <typename FT> \
   reference operator _oper_ (FT fValue) \
   { \
      x _oper_ fValue; \
      y _oper_ fValue; \
      return *this; \
   }

// vec2
//
// Templated class for 2-Dimensional vectors.
//
// Use vec2<float> or vec2f for floats.
// Use vec2<double> or vec2d for doubles.
template <typename T>
struct vec2
{
   //typedef to create an alias for vec2& named reference
   typedef vec2 &reference;
  
   //typedef to create an alias for const vec2& named const_reference
   typedef vec2 const &const_reference;

   // typedef to create an alias for T named element_type
   typedef T element_type;

   // This union allows the data of the vec2 to be accessed
   // in different ways with no additional overhead.
   //
   // Given the vector,
   // 
   //	vec2f myVector;
   //
   // You can access it's data the following ways...
   union
   {
	  // Access the vector as an array of floating point values by using myVector.v
      element_type v[2];

	  // Access the individuals components of the vector with myVector.x, myVector.y, or myVector.z
      struct
      {
         element_type x;
         element_type y;
      };
   };

   // Constructor
   vec2(void) { }
   // Destructor
   ~vec2(void) { }
   // Constructor given X,Y,Z values
   vec2(element_type vx, element_type vy) : x(vx), y(vy) { }

   // Copy Constructor
   template <typename VT>
   vec2(const vec2<VT> &v) : x(v.x), y(v.y) { }

   // These are simply macros that generate the code for the
   // various overloaded operators. Simply use +,-,*,/,=,+=,
   // -=, *=, /= operators with vec2 objects to use them.
   // 
   // Ex:
   //
   //	vec2f vecA( 1.0f, 0.0f, 0.0f);
   //	vec2f vecB( 0.0f, 1.0f, 0.0f);
   //
   //	vecA += vecB;
   //
   VEC2_BINARY_COMPONENT_OPERATOR(+);
   VEC2_BINARY_COMPONENT_OPERATOR(-);
   VEC2_BINARY_COMPONENT_OPERATOR(*);
   VEC2_BINARY_COMPONENT_OPERATOR(/);

   VEC2_BINARY_BROADCAST_OPERATOR(+);
   VEC2_BINARY_BROADCAST_OPERATOR(-);
   VEC2_BINARY_BROADCAST_OPERATOR(*);
   VEC2_BINARY_BROADCAST_OPERATOR(/);

   VEC2_COMPONENT_OPERATOR(=);
   VEC2_COMPONENT_OPERATOR(+=);
   VEC2_COMPONENT_OPERATOR(-=);
   VEC2_COMPONENT_OPERATOR(*=);
   VEC2_COMPONENT_OPERATOR(/=);

   VEC2_BROADCAST_OPERATOR(=);
   VEC2_BROADCAST_OPERATOR(+=);
   VEC2_BROADCAST_OPERATOR(-=);
   VEC2_BROADCAST_OPERATOR(*=);
   VEC2_BROADCAST_OPERATOR(/=);

   // Zero out the vector
   reference make_zero(void)
   {
      x = y = 0;
      return *this;
   }

   // normalize the vector
   reference normalize(void)
   {
      element_type rsqrt = 1 / magnitude();
      x *= rsqrt;
      y *= rsqrt;
      return *this;
   }

   // Negate the vector
   reference negate(void)
   {
      x = -x;
      y = -y;
      return *this;
   }

   // Calculate and return the magnitude of the vector
   element_type magnitude(void)
   {
      element_type fMagnitude;
      fMagnitude = (T)sqrt(x*x + y*y);
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
		return vectorA.x*vectorB.x+vectorA.y*vectorB.y;
   }

   // cross_product
   //
   // Calculate the cross product of two vectors
   //
   // In:
   //	const_reference vectorA - constant reference to the first vector
   //	const_reference vectorB - constant reference to the second vector
   //
   // Out:
   //	reference vectorO - Where to store the result of the cross product
   //	
   // Remarks:
   //	
   //	vectorO must not be a reference to the same object as vectorA or vectorB
   inline friend void Cross(reference vectorO, const_reference vectorA, float s )
   {
	   vectorO.x = -s * vectorA.x;
	   vectorO.y = s * vectorA.y;
   }

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
   //	vec2<T> - The resulting vector from the interpolation	
   inline friend vec2<T> interpolate(const_reference vectorA, const_reference vectorB, float lamda)
   {
       return vectorA + ((vectorB - vectorA) * lamda);
   }

   element_type *array(void) { return this->v; }
   element_type const *array(void) const { return this->v; }
};

// typedef to create an alias for vec2<float> named vec2f
typedef vec2<float> vec2f;
// typedef to create an alias for vec2<double> named vec2d
typedef vec2<double> vec2d;
// typedef to create an alias for vec2f named float2
typedef vec2f float2;
// typedef to create an alias for vec2d named double2
typedef vec2d double2;

#endif
