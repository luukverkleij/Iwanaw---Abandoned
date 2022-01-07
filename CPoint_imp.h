#ifndef _CPOINT_IMP_H_
#define _CPOINT_IMP_H_

template<typename T>
CPoint<T>::CPoint() {
	//this->x = static_cast<T>(0);
	//this->y = static_cast<T>(0);
};

template<typename T>
CPoint<T>::CPoint(T x, T y) {
	this->x = x;
	this->y = y;
};

template<typename T>
CPoint<T>::CPoint(SDL_Point &point) {
	this->x = static_cast<T>(point.x);
	this->y = static_cast<T>(point.y);
};

template<typename T>
template<typename U>
CPoint<T>::CPoint(CPoint<U> &point) {
	this->x = static_cast<T>(point.x);
	this->y = static_cast<T>(point.y);
};

template<typename T>
CPoint<T>::CPoint(std::pair<T, T> &pair) {
	this->x = pair.first;
	this->y = pair.second;
};

template<typename T>
void CPoint<T>::set(T x, T y) {
	this->x = x;
	this->y = y;
};

template<typename T>
SDL_Point CPoint<T>::toSDL() {
	return SDL_Point{ this->x, this->y };
};

template<typename T>
std::string CPoint<T>::toString() {
	return "(" + estd::toString(this->x) + "," + estd::toString(this->y) + ")";
};

template<typename T>
template<typename U>
bool CPoint<T>::operator==(const CPoint<U>& right) {
	return x == right.x && y == right.y;
}

template<typename T>
template<typename U>
bool CPoint<T>::operator!=(const CPoint<U>& right) {
	return x != right.x || y != right.y;
}

template<typename T>
template<typename U>
CPoint<T>& CPoint<T>::operator+=(const CPoint<U>& right) {
	this->x += right.x;
	this->y += right.y;
	
	return (*this);
}

template<typename T>
template<typename U>
CPoint<T>& CPoint<T>::operator-=(const CPoint<U>& right) {
	this->x -= right.x;
	this->y -= right.y;

	return (*this);
}

#endif //_CPOINT_IMP_H_