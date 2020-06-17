
#include "graph_struct_decl.h"

Trib::Trib(int w, const QColor &col):
	To(NULL), Weight(w),
	Color(col), isHidden(false){}

Trib::Trib(const TribInfo &ri):
	To(NULL)
{
	SetProperties(ri);
}

void Trib::SetProperties(const TribInfo &ri){
	Weight = ri.weight;
	Color = ri.color;
	isHidden = ri.hidden;
}


Tvertex::Tvertex(int r, const QColor &col):
	x(0), y(0), Radius(r),
	Color(col), isHidden(false){}

Tvertex::Tvertex(const TvertexInfo &vi){
	SetProperties(vi);
}

void Tvertex::SetProperties(const TvertexInfo &vi){
	Radius = vi.radius;
	Color = vi.color;
	isHidden = vi.hidden;
}


TribInfo::TribInfo(int w, const QColor &col):
	from(0), to(0), weight(w),
	color(col), hidden(false){}

TribInfo::TribInfo(const Trib &rib, int From, int To):
	from(From), to(To)
{
	GetProperties(rib);
}

void TribInfo::GetProperties(const Trib &rib){
	weight = rib.Weight;
	color = rib.Color;
	hidden = rib.isHidden;
}


TvertexInfo::TvertexInfo(int r, const QColor &col):
	x(0), y(0), radius(r), color(col),
	number(0), degree(0), hidden(false){}

TvertexInfo::TvertexInfo(const Tvertex &ver, int n):
	number(n)
{
	GetProperties(ver);
}

void TvertexInfo::GetProperties(const Tvertex &ver){
	x = ver.x;
	y = ver.y;
	radius = ver.Radius;
	color = ver.Color;
	degree = ver.Ribs.count();
	hidden = ver.isHidden;
}
