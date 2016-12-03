#include<iostream>

#ifndef POSITION_H
#define POSITION_H
class Position
{
public:
	Position():x(0.),y(0.){}  // initialization.
	Position(float _x, float _y):x(_x),y(_y){} 
	Position(const Position& v){ x= v.getx(); y= v.gety(); }
	
	float sqrlen(){ return x*x+y*y;}     // return squared length.
	float len(){ return sqrt(x*x+y*y); } // return length.

	const Position& operator =(const Position& v){ x=v.getx();y= v.gety(); return v;}

	// it's equal to _p, return true, otherwise return false.
	bool operator == (const Position& _p ) { return x== _p.getx() && y == _p.gety(); }
	// if it's colinear with _p and its direction is the same with _p's, return true,
	// otherwise, return false.
	bool codirection (const Position& _p ) { return (x*_p.gety()-y*_p.getx() == (float)0.0) && (x*_p.getx()+y*_p.gety()>0.); }

    // if it's colinear with _p and has opposite direction to with _p's, return true,
	// otherwise, return false.
	bool opdirection (const Position& _p ) { return (x*_p.gety()-y*_p.getx() == (float)0.0) && (x*_p.getx()+y*_p.gety()<0.); }
	
	// if point _p is on its left hand side, return true.
	// otherwise return false.
	bool onleft(const Position& _p) { return x*_p.gety()-y*_p.getx()> 0.0; }

	// if point _p is on its right hand side, return true.
	// otherwise return false.
	bool onright(const Position& _p) { return x*_p.gety()-y*_p.getx()<0.0; }
	float getx()const {return x;}
	float gety()const {return y;}

	// define - + * / operations for Position class.
	friend Position operator - (const Position& a, const Position& b );
    friend Position operator + (const Position& a, const Position& b );
	friend float operator * (const Position& a, const Position& b ); // dot product.
	friend Position operator * (float f, const Position& p);
	friend Position operator * (const Position& p,float f );
	friend Position operator / (const Position& p,float f );
	Position inver(){ return Position(-x,-y);}
	Position xinver(){ return Position(-x,y); }
	Position yinver() { return Position(x,-y); }
	float maxx(){ return 1.0/2.0*(x + y +abs(x-y)); }
	// output Position class.
	friend std::ostream& operator <<(std::ostream& out,const Position& p);
private:
	float x,y;  // x and y coordinates.
};

Position operator - (const Position& a, const Position& b )
{
	return Position(a.getx()-b.getx(), a.gety()-b.gety());
}

Position operator + (const Position& a, const Position& b )
{
	return Position(a.getx()+b.getx(), a.gety()+b.gety());
}

float operator * (const Position& a, const Position& b )
{
	return a.getx()*b.getx()+ a.gety()*b.gety();
}

Position operator * (float f, const Position& p)
{
	return Position(f* p.getx(),f*p.gety());
}

Position operator * (const Position& p,float f )
{
	return Position(f*p.getx(),f*p.gety());
}


Position operator / (const Position& p,float f )
{
	return Position(p.getx()/f, p.gety()/f);
}

std::ostream& operator <<(std::ostream& out,const Position& p)
{
	out<<"("<<p.getx()<<","<<p.gety()<<")";
	return out;
}

#endif
