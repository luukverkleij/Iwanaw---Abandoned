#ifndef _CPOINT_H_
#define _CPOINT_H_

#include <SDL.h>
#include <utility>

#include "extra_std.h"

/*!
*	\brief Point class is used for a catch-it-all point class that includes conversions from
*			styles, like SDL_Point.
*	
*/
template<typename T>
class CPoint {
public:

// Constructors
	CPoint();
	CPoint(T x, T y);
	CPoint(SDL_Point &point);
	CPoint(std::pair<T, T> &pair);

	template<typename U>
	CPoint(CPoint<U> &point);

// Functions
	void set(T x, T y);
	SDL_Point toSDL();
	std::string toString();

// Operators
	template<typename U>
	bool operator==(const CPoint<U>& right);
	template <typename U>
	bool operator!=(const CPoint<U>& right);
	template<typename U>
	CPoint<T>& operator+=(const CPoint<U>& right);
	template<typename U>
	CPoint<T>& operator-=(const CPoint<U>& right);

// Data
	T x;
	T y;
};
#include "CPoint_imp.h"


typedef CPoint<uint8_t> Point_ui8;
typedef CPoint<uint16_t> Point_ui16;
typedef CPoint<uint32_t> Point_ui32;
typedef CPoint<uint64_t> Point_ui64;

typedef CPoint<int8_t> Point_i8;
typedef CPoint<int16_t> Point_i16;
typedef CPoint<int32_t> Point_i32;
typedef CPoint<int64_t> Point_i64;

typedef CPoint<float> Point_f;
typedef CPoint<double> Point_d;

#endif