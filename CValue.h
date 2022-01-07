#ifndef _CVALUE_H_
#define _CVALUE_H_

#include <string>
#include <typeindex>

class CValueBase {
public:
};


template <typename T>
class CValue {
public:
	CValue(T value) {
		this->value = value;
	}
	~CValue();

	T value;
};


template <typename T>
class CValueList {
public:
	CValueList(std::vector<T> list) {
		this->list = list;
	}
	~CValueList();

	std::vector<T> list;
};

#endif