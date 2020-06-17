

#ifndef MINOST_H
#define MINOST_H

#include <set>
using namespace std;

#include <QMessageBox>
#include "AbstractGraph/abstractgraph.h"

void RunGrAlgoMinOstP(AbstractGraph *gr, const int &Stime){
	if(gr->IsOriented()){
		QMessageBox::warning(NULL, QObject::tr("Minimum spanning tree"),
							 QObject::tr("Graph must not be oriented!"));
	}

	VectorListGS sgr = gr->GetGraphModel().ToVectorList();	// Return if
	if(sgr.count() <= 1){return;}							//  no vertexes

	int start = GetVertex::GetStartVertex(sgr.count()-1);	// Get start vertex
	if(!start){return;}

	int n = sgr.count()-1;
	const int INF = 1000000000;

				// Hide ribs
	for(int i=1; i<=n; ++i){
		for(int j=0; j<sgr[i].count(); ++j){
			AG_CHANGE_RIB_PARAM((*gr), ABrib(i, sgr[i][j].to), hidden, true)
		}
	}

	vector <int> min_e(n+1, INF), sel_e(n+1, -1);		// Variables
	min_e[start] = 0;
	set < pair<int,int> > q;
	int v = 0;

	q.insert(make_pair (0, start));
	while(!q.empty()){
		if(v){AG_CHANGE_VER_PARAM((*gr), v, color, Qt::gray)}

		v = q.begin()->second;
		q.erase (q.begin());
        AG_CHANGE_VER_PARAM((*gr), v, color, Qt::green)

		if(sel_e[v] != -1){
			AG_CHANGE_RIB_PARAM((*gr), ABrib(v, sel_e[v]), hidden, false)
		}

		for(int j=0; j<sgr[v].size(); ++j){
			int to = sgr[v][j].to,
				cost = sgr[v][j].weight;
			if(cost < min_e[to]){
				AG_CHANGE_VER_PARAM((*gr), to, color, Qt::red)
				q.erase(make_pair(min_e[to], to));
				min_e[to] = cost;
				sel_e[to] = v;
				q.insert(make_pair(min_e[to], to));
			}
		}

		sleep(Stime);
	}
	AG_CHANGE_VER_PARAM((*gr), v, color, Qt::gray)

	QMessageBox::information(NULL, QObject::tr("Minimum spanning tree"),
							 QObject::tr("Finished."));
}

#endif // MINOST_H
