

#ifndef RESET_GRAPH_H
#define RESET_GRAPH_H

#include "../AbstractGraph/abstractgraph.h"

inline void ResetGraph(AbstractGraph *gr, QColor Cver = Qt::green,
					   QColor Crib = Qt::black)
{
	GraphModel gm = gr->GetGraphModel();

	for(int i=1; i<=gr->VertexCount(); ++i){
		AG_CHANGE_VER_PARAM((*gr), i, color, Cver)
	}

	for(int i=0; i<gm.ribs.count(); ++i){
		AG_CHANGE_RIB_PARAM((*gr), ABrib(gm.ribs[i].from, gm.ribs[i].to), color, Crib)
	}
}

#endif // RESET_GRAPH_H
