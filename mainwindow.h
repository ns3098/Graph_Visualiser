
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

///				<Includes>
#include "project_global.h"
#include "GraphView/graphview.h"
#include "OptionDialog/optiondialog.h"
#include "AboutDialog/aboutdialog.h"

#include "mainwindow_header.h"
///				</Includes>

namespace Ui{class MainWindow;}

///			<Class>
class MainWindow: public QMainWindow{
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

///				<MainData>
private:
	Ui::MainWindow *ui;						// Ui
	OptionDialog::PROptions Options;		// Options

	QHBoxLayout *MainLayot;					// Main Layot
	GraphView *GraphV;						// Graph builder widget

	AbstractGraph Graph;					// Graph
	bool GrChanged;							// Changed flag

#define MW_MAIN_DATA_INIT \
	GrChanged = false;
///				</MainData>

///				<Other>
private:
	void closeEvent(QCloseEvent *ev);			// On close
	void changeEvent(QEvent *ev);				// Change event

	void UpdateOptions();						// Update options
	void SetSaveFile(const QString &FileName);	// Set save file
	void ResetTitle();							// Reset title

private slots:
	void GraphChanged();						// Graph changed
///				</Other>

///				<VertexInfoDW>
private slots:
	void UpdateVerInfo();			// Update vertex info

	void on_actionDock_visible_toggled(bool arg1);
	void on_VRadiusSpinBox_valueChanged(int arg1);
	void on_VColorToolButton_clicked();
///				</VertexInfoDW>

///				<RibInfoDW>
private slots:
	void UpdateRibInfo();			// Update rib info

	void on_actionRib_info_toggled(bool arg1);
	void on_RWeightSpinBox_valueChanged(int arg1);
	void on_RColorToolButton_clicked();
///				</RibInfoDW>

///				<Algorithms>
private slots:
	void on_actionRun_algorithm_triggered();		// Run

	void on_actionBFS_toggled(bool arg1);			// Select
	void on_actionDFS_toggled(bool arg1);
	void on_actionDijkstra_toggled(bool arg1);
	void on_actionMinOstP_toggled(bool arg1);
	void on_actionMinOstK_toggled(bool arg1);

private:
	void SetAlgoSelected(int algo);
	void ResetSelected();
///				</Algorithms>

///				<UiSlots>
private slots:
	bool Open(QString FileName = QString());		// Open
	bool Save();									// Save
	bool SaveAs(QString FileName = QString());		// Save as

	void on_actionOriented_toggled(bool arg1);		// Oriented
	void on_actionClear_triggered();				// Clear

	void on_actionOptions_triggered();				// Options
	void on_actionAbout_triggered();				// About
///				</UiSlots>

};
///			</Class>

#endif // MAINWINDOW_H
