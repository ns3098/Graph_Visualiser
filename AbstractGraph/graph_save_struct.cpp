
#include "graph_save_struct.h"

VectorListGS GraphModel::ToVectorList(){
	VectorListGS gr(vertexes.count()+1);

	for(int i=0; i<ribs.count(); ++i){
		if(ribs[i].from > 0 && ribs[i].from <= vertexes.count() &&
		   ribs[i].to > 0   && ribs[i].to   <= vertexes.count())
		{
			gr[ribs[i].from] << VLGraphRib(ribs[i].to, ribs[i].weight);
		}
	}

	return gr;
}

RibListGS GraphModel::ToRibList(){
	RibListGS gr;

	for(int i=0; i<ribs.count(); ++i){
		RLGraphItem gi;
		gi.from = ribs[i].from;
		gi.to = ribs[i].to;
		gi.weight = ribs[i].weight;
		gr << gi;
	}

	return gr;
}

MatrixGS GraphModel::ToMatrix(){
	MatrixGS gr(vertexes.count()+1);

	for(int i=0; i<=vertexes.count(); ++i){
		gr[i].resize(vertexes.count()+1);
	}

	for(int i=0; i<ribs.count(); ++i){
		if(ribs[i].from > 0 && ribs[i].from <= vertexes.count() &&
		   ribs[i].to > 0   && ribs[i].to   <= vertexes.count())
		{
			gr[ribs[i].from][ribs[i].to] = ribs[i].weight;
		}
	}

	return gr;
}
