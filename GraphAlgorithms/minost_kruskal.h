

#ifndef MINOST_KRUSKAL_H
#define MINOST_KRUSKAL_H

#include <algorithm>
using namespace std;

#include <QMessageBox>
#include "AbstractGraph/abstractgraph.h"

bool cmp(RLGraphItem a, RLGraphItem b){
	if(a.weight == b.weight){
		if(a.from == b.from)
			{return a.to < b.to;}
		return a.from < b.from;
	}
	return a.weight < b.weight;
}

void RunGrAlgoMinOstK(AbstractGraph *gr, const int &Stime){
	if(gr->IsOriented()){
		QMessageBox::warning(NULL, QObject::tr("Minimum spanning tree"),
							 QObject::tr("Graph must not be oriented!"));
	}

	RibListGS sgr = gr->GetGraphModel().ToRibList();		// Return if
	if(!sgr.count()){return;}								//  no vertexes

				// Hide ribs
	for(int i=0; i<sgr.count(); ++i){
		AG_CHANGE_RIB_PARAM((*gr), ABrib(sgr[i].from, sgr[i].to), hidden, true)
	}

	int n = sgr.count()-1;			// Variables
	vector<int> tree_id(n+1);

	sort(sgr.begin(), sgr.end(), cmp);
	for(int i=1; i<=n; ++i)
		{tree_id[i] = i;}

	for(int i=0; i<sgr.count(); ++i){
		int a = sgr[i].from,  b = sgr[i].to;
		if(tree_id[a] != tree_id[b]){
			sleep(Stime);
			int old_id = tree_id[b],  new_id = tree_id[a];
			AG_CHANGE_RIB_PARAM((*gr), ABrib(a,b), hidden, false)
            AG_CHANGE_VER_PARAM((*gr), a, color, Qt::green)
            AG_CHANGE_VER_PARAM((*gr), b, color, Qt::green)
			for(int j=1; j<=n; ++j){
				if(tree_id[j] == old_id){
					tree_id[j] = new_id;
				}
			}
		}
	}

	QMessageBox::information(NULL, QObject::tr("Minimum spanning tree"),
							 QObject::tr("Finished."));
}

#endif // MINOST_KRUSKAL_H
