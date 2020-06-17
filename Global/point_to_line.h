

#ifndef POINT_TO_LINE_H
#define POINT_TO_LINE_H

#include <math.h>
#include <QPoint>
#include <QLine>

#define sqr(A) ((A)*(A))

inline double Dabs(double x){
	if(x >= 0) return x;
	return -x;
}

inline double dist(const QPoint &p1, const QPoint &p2){
	return sqrt(sqr(p1.x()-p2.x()) + sqr(p1.y()-p2.y()));
}

inline double PointToLine(const QPoint &m, const QLine &l){
	double r0, r1, r2, r12;
	double a, b, c, t;

	r1  = dist(m, l.p1());
	r2  = dist(m, l.p2());
	r12 = dist(l.p1(), l.p2());

			// Right to B
	if(r1 >= dist(QPoint(0,0), QPoint(r2,r12))){
		return r2;
	}
			// Left to A
	if(r2 >= dist(QPoint(0,0), QPoint(r1,r12))){
		return r1;
	}
			// Get line (Ax+By+C=0)
	a = l.p2().y()-l.p1().y();
	b = l.p1().x()-l.p2().x();
	c = l.p1().y() * (l.p2().x()-l.p1().x()) -
		l.p1().x() * (l.p2().y()-l.p1().y());

	t = dist(QPoint(0,0), QPoint(a,b));

	if(c > 0){a*=-1; b*=-1; c*=-1;}
	r0 = (a*m.x()+b*m.y()+c)/t;

	return abs(r0);
//	r0 < 0 => under line;
}

#endif // POINT_TO_LINE_H
