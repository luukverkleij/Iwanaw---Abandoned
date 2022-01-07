#ifndef _CCOMPPOSITION_H_
#define _CCOMPPOSITION_H_

#include "CComponent.h"
#include "CShape.h"

class CCompPosition : public CComponent<CCompPosition> {
public:
	CCompPosition();
	CCompPosition(double x, double y);

	void setX(double x);
	void setY(double y);
	void setPos(double x, double y);

	double getX();
	double getY();
	Point_d getPos();
	Point_d getAbsolutePos();

private:
	void updateMapped();
	double x, y;
};

#endif