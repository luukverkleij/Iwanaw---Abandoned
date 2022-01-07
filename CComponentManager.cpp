#include "CComponentManager.h"
#include "CComponent.h"

CComponentManager::CComponentManager() {
		this->poolList.resize(CComponentBase::total_components, std::numeric_limits<unsigned int>::max());
		this->destroyList.resize(CComponentBase::total_components);
}


CComponentManager::~CComponentManager() {
	for(unsigned int index = 0; index < destroyList.size(); ++index) {
		if (destroyList[index]) {
			destroyList[index](poolList[index]);
		}
	}
}