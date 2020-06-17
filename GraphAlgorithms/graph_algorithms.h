

#ifndef GRAPH_ALGORITHMS_H
#define GRAPH_ALGORITHMS_H

///			<Includes>
#include "AbstractGraph/abstractgraph.h"
#include "reset_graph.h"

#include "bfs.h"
#include "dfs.h"
#include "dijkstra.h"
#include "minost_prim.h"
#include "minost_kruskal.h"
///			</Includes>

///			<AlgoList>
#define GR_ALGO_LIST \
	X(BFS) X(DFS) X(Dijkstra) X(MinOstP) X(MinOstK)
///			</AlgoList>

///			<Template>
enum GraphAlgorithms{
#define X(A) A##GA,
	GR_ALGO_LIST
#undef X
};

inline void RunGraphAlgorithm(int algo, AbstractGraph *gr, const int &Stime){
	ResetGraph(gr);
	switch(algo){
	#define X(A) case A##GA: RunGrAlgo##A(gr, Stime); break;
		GR_ALGO_LIST
	#undef X
	}
}
///			</Template>

#endif // GRAPH_ALGORITHMS_H
