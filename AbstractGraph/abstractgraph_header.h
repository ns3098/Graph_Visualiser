

#ifndef ABSTRACTGRAPH_HEADER_H
#define ABSTRACTGRAPH_HEADER_H

///			<Includes>
#include <QObject>
#include <QRect>
#include <QLine>
#include <math.h>
///			</Includes>

///			<Defines>
#define ABOUT_TO_CHANGE_DEF AboutToChange
#define CHANGED_DEF Changed

#define AG_CHANGE_VER_PARAM(AG, ver, param, val) {\
	TvertexInfo vi = AG.GetVertexInfo(ver);\
	if(vi.number){\
		vi.param = val;\
		AG.UpdateVertex(ver, vi);\
	}\
}

#define AG_CHANGE_RIB_PARAM(AG, rib, param, val) {\
	TribInfo ri = AG.GetRibInfo(rib);\
	if(ri.from){\
		ri.param = val;\
		AG.UpdateRib(rib, ri);\
	}\
}
///			</Defines>

///			<Structs>
struct ABrib{
	ABrib(): from(0), to(0){}
	ABrib(int a, int b):
		from(a), to(b){}

	inline bool operator==(const ABrib &other){
		return ((from == other.from) && (to == other.to));
	}

	inline bool EqualNOrient(const ABrib &other){		// Equal at not oriented graph
		return ((*this)==other) || ((to == other.from) && (from == other.to));
	}

	inline bool operator!=(const ABrib &other){
		return !((*this)==other);
	}

	int from, to;
};
///			</Structs>

#endif // ABSTRACTGRAPH_HEADER_H
