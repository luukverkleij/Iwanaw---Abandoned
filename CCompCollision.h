#ifndef _CCOMPCOLLISION_H_
#define _CCOMPCOLLISION_H_

#include "CComponent.h"
#include "CShape.h"
#include "CRectangle.h"

// I am ashamed. Can't get unique_ptr to work.

class CCompCollision : public CComponent<CCompCollision> {
public:
	CCompCollision() 
		: CComponent<CCompCollision>(){ 
	}

	CCompCollision(const CCompCollision& comp)
	: CComponent<CCompCollision>(comp){
		if (comp.shape)
			this->shape.reset(comp.shape->clone());
	}

	CCompCollision(CShape* shape)
		: CComponent<CCompCollision>(){
		this->shape.reset(shape); 
	}

	~CCompCollision() {
	}


	void setShape(CShape* pShape){
		this->shape.reset(pShape);
	}

	CShape* getShape(){
		return this->shape.get();
	}
	
	CCompCollision& operator= (const CCompCollision& comp) {
		CComponent<CCompCollision>::operator=(comp);
		this->shape.reset(comp.shape->clone());
		return (*this);
	}

private:
	std::unique_ptr<CShape> shape;

};

#endif //_CCOMPCOLLISION_H_