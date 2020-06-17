

#ifndef DIJKSTRADIALOG_H
#define DIJKSTRADIALOG_H

#include <QDialog>
#include <QTreeWidget>

#include "../../AbstractGraph/abstractgraph.h"
#include "../reset_graph.h"

#define RIB_INF_WEIGHT 1000000

namespace Ui{class DijkstraDialog;}

struct DijkstraPath{
	DijkstraPath(unsigned dist=0, int fr=0):
		distance(dist), from(fr){}

	unsigned distance;
	int from;
};

typedef QVector<DijkstraPath> DijkstraResult;

class DijkstraDialog: public QDialog{
	Q_OBJECT
public:
	explicit DijkstraDialog(AbstractGraph *gr,
							const DijkstraResult &res,
							QWidget *parent = 0);
	~DijkstraDialog();

private slots:
	void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *prev);

private:
	void ResetVerPath(int ver, QColor Cver, QColor Crib);

private:
	Ui::DijkstraDialog *ui;

	DijkstraResult Result;
	AbstractGraph *Graph;
};

#endif // DIJKSTRADIALOG_H
