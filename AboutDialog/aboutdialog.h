

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QDesktopServices>
#include <QUrl>
#include "ui_aboutdialog.h"

class AboutDialog : public QDialog{
	Q_OBJECT
public:
	explicit AboutDialog(QWidget *parent = 0):
		QDialog(parent),
		ui(new Ui::AboutDialog)
	{
		ui->setupUi(this);
		resize(0, 0);
		setFixedSize(size());
	}

	~AboutDialog(){delete ui;}

private slots:
	void on_label_linkActivated(const QString &link){
		QDesktopServices::openUrl(link);
	}

private:
	Ui::AboutDialog *ui;

};

#endif // ABOUTDIALOG_H
