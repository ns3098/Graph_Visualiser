

#include "dijkstradialog.h"
#include "ui_dijkstradialog.h"

DijkstraDialog::DijkstraDialog(AbstractGraph *gr, const DijkstraResult &res, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DijkstraDialog),
	Result(res), Graph(gr)
{
	ui->setupUi(this);
	ResetGraph(Graph);

	for(int i=1; i<Result.count(); ++i){			// Fill list of distances
		QTreeWidgetItem *it = new QTreeWidgetItem;
		it->setData(0, Qt::DisplayRole, i);
		if(Result[i].distance < RIB_INF_WEIGHT)
			{it->setData(1, Qt::DisplayRole, Result[i].distance);}
		else
			{it->setData(1, Qt::DisplayRole, tr("No path"));}
		ui->treeWidget->addTopLevelItem(it);
	}

	ui->treeWidget->sortByColumn(0, Qt::AscendingOrder);
}

DijkstraDialog::~DijkstraDialog(){delete ui;}

void DijkstraDialog::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *prev){
	if(prev)
		ResetVerPath(prev->data(0, Qt::DisplayRole).toInt(), Qt::green, Qt::black);
	if(current)
		ResetVerPath(current->data(0, Qt::DisplayRole).toInt(), Qt::blue, Qt::red);
}

void DijkstraDialog::ResetVerPath(int ver, QColor Cver, QColor Crib){
	int i = ver;
	AG_CHANGE_VER_PARAM((*Graph), i, color, Cver);

	while(Result[i].from){
		AG_CHANGE_VER_PARAM((*Graph), Result[i].from, color, Cver);
		AG_CHANGE_RIB_PARAM((*Graph), ABrib(Result[i].from, i), color, Crib);
		i = Result[i].from;
	}
}
