

#ifndef GRAPHALGOBFS_H
#define GRAPHALGOBFS_H

#include <QDialog>
#include "ui_getvertex.h"

class GetVertex: public QDialog{
	Q_OBJECT
public:
	explicit GetVertex(int count, QWidget *parent = 0):
		QDialog(parent),
		ui(new Ui::GetVertex)
	{
		ui->setupUi(this);
		resize(0,0); setFixedSize(size());
		ui->spinBox->setMinimum(1);
		ui->spinBox->setMaximum(count);
	}

	~GetVertex(){delete ui;}

	static int GetStartVertex(int count, QWidget *par = 0){
		GetVertex gv(count, par);
		gv.exec();

		if(gv.result() == QDialog::Rejected){		// Return 0 if cancel
			return 0;
		}
		return gv.ui->spinBox->value();
	}

private:
	Ui::GetVertex *ui;
};

#endif // GRAPHALGOBFS_H
