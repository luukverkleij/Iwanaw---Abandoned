#ifndef _CCOMPONENTMANAGER_H_
#define _CCOMPONENTMANAGER_H_

#include <vector>
#include <limits>
#include <functional>

class CComponentBase;

template <typename T>
class CComponent;

class CComponentManager {
public:
	CComponentManager();
	~CComponentManager();

	template <typename T>
	std::vector<T>* getPool() {
		unsigned int index = poolList[T::componentID];

		if(index == std::numeric_limits<unsigned int>::max()) {
			index = createPool<T>();
		}

		return &T::containers[index];
	}

	template <typename T>
	void reserve(CComponent<T>* comp, unsigned int cap) {
		this->poolIncreas<T>(cap);
	}

	template <typename T>
	void reserve(unsigned int cap) {
		unsigned int index = poolList[T::componentID];

		if(index == std::numeric_limits<unsigned int>::max()) { 
			index = createPool<T>();
		}

		T::increasePool(index, cap);
	}

	template <typename T>
	unsigned int getIndex() {
		return this->poolList[T::componentID];
	}

private:
	template <typename T>
	unsigned int createPool() {
		unsigned int index = T::createPool();
		poolList[T::componentID] = index;
		destroyList[T::componentID] = T::destroyPool;
		return index;
	}


private:
	std::vector<unsigned int> poolList;
	std::vector<std::function<void(unsigned int)>> destroyList;
};

#endif