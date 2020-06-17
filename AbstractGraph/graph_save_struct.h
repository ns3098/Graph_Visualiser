

#ifndef GRAPH_SAVE_STRUCT_H
#define GRAPH_SAVE_STRUCT_H

/*
 * Graph save structures:
 *  - Graph model(GraphModel)
 *  - Vector list(VectorListGS)
 *  - Matrix(MatrixGS)
*/

///			<Includes>
#include <QVector>
#include "graph_struct_decl.h"
///			</Includes>

///			<VectorListGS>
struct VLGraphRib{
	VLGraphRib(int Vto = 0, int w = 0):
		to(Vto),weight(w){}

	int to;				// To vertex
	int weight;			// Rib weight
};

typedef QVector < QVector <VLGraphRib> > VectorListGS;
///			</VectorListGS>

///			<RibList>
struct RLGraphItem{
	int from, to, weight;
};

typedef QVector <RLGraphItem> RibListGS;
///			</RibList>

///			<MatrixGS>
typedef QVector< QVector<int> > MatrixGS;
///			</MatrixGS>

///			<GraphModel>
struct GraphModel{
	GraphModel(): oriented(false){}

	bool oriented;
	QVector <TvertexInfo> vertexes;
	QVector <TribInfo> ribs;

	VectorListGS ToVectorList();
	RibListGS ToRibList();
	MatrixGS ToMatrix();
};
///			</GraphModel>

///			<IOdeclaration>
QDataStream &operator<<(QDataStream &, const GraphModel &);
QDataStream &operator>>(QDataStream &, GraphModel &);
///			</IOdeclaration>

#endif // GRAPH_SAVE_STRUCT_H
