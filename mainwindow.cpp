

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "GraphAlgorithms/graph_algorithms.h"

MainWindow::MainWindow(QWidget *parent):
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	MW_MAIN_DATA_INIT

				// Setup ui
	ui->setupUi(this);
	MainLayot = new QHBoxLayout;
	ui->centralWidget->setLayout(MainLayot);

	GraphV = new GraphView;
	GraphV->SetGraph(&Graph);
	MainLayot->addWidget(GraphV);

				// Connections
	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(Open()));
	connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(Save()));
	connect(ui->actionSave_As, SIGNAL(triggered()), this, SLOT(SaveAs()));

	connect(ui->actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

				// Graph changes
	connect(GraphV, SIGNAL(VertexSelected()), this, SLOT(UpdateVerInfo()));
	connect(GraphV, SIGNAL(RibSelected()), this, SLOT(UpdateRibInfo()));
	connect(&Graph, SIGNAL(Changed()), this, SLOT(GraphChanged()));

				// Options load
	Options.Load();
	UpdateOptions();
	restoreGeometry(Options.Geometry);

	if(!Options.LastFile.isEmpty()){
		if(!Open(Options.LastFile))
			{SetSaveFile("");}
	}
				// Dock widgets init
	UpdateVerInfo();
	UpdateRibInfo();
}

MainWindow::~MainWindow(){delete ui;}

void MainWindow::closeEvent(QCloseEvent *ev){
	if(GrChanged){
		int s = QMessageBox::information(this, tr("Close"),
					tr("Do you want to save graph?"),
					QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
		if(s == QMessageBox::Yes)
			{Save();}
		else if(s == QMessageBox::Cancel)
			{ev->ignore(); return;}
	}

	Options.Geometry = saveGeometry();
	Options.Save();
	Options.GrAlgoDelay = 0;	// For fast algo stop
}

void MainWindow::changeEvent(QEvent *ev){
	if(ev->type() == QEvent::LanguageChange) {
		ui->retranslateUi(this);
		ResetTitle();
	}else
		{QMainWindow::changeEvent(ev);}
}

void MainWindow::UpdateOptions(){
	GraphV->SetEditable(Options.Editable);

	GraphV->SetStdVertex(TvertexInfo(Options.VertexRadius,
									  Options.VertexColor));

	GraphV->SetStdRib(TribInfo(Options.RibWeight));

	ui->actionDock_visible->setChecked(Options.VDWVisible);
	on_actionDock_visible_toggled(Options.VDWVisible);

	ui->actionRib_info->setChecked(Options.RDWVisible);
	on_actionRib_info_toggled(Options.RDWVisible);

	SetAlgoSelected(Options.GrAlgoSelected);

	ResetTitle();
}

void MainWindow::SetSaveFile(const QString &FileName){
	Options.LastFile = FileName;
	Options.Save();
	ResetTitle();
}

void MainWindow::ResetTitle(){
	QString Title;
	if(Options.LastFile.isEmpty()){
        Title = tr("Graph Algorithms Visualizer");
	}else{
		QFileInfo FI(Options.LastFile);
		Title = FI.fileName();
	}
	if(GrChanged){Title += '*';}

	setWindowTitle(Title);
}

void MainWindow::GraphChanged(){
	GrChanged = true;
	ResetTitle();
}

void MainWindow::UpdateVerInfo(){
	int v = GraphV->GetSelectedVertex();

	if(!v){
		ui->dockWidgetContents->setEnabled(false);
		ui->VNumberLineEdit->setText("");
		ui->VXLineEdit->setText("");
		ui->VYLineEdit->setText("");
		ui->VDegreeLineEdit->setText("");
		ui->VRadiusSpinBox->setValue(0);
		ui->VColorLineEdit->setText("");
	}else{
		ui->dockWidgetContents->setEnabled(true);
		TvertexInfo vi = Graph.GetVertexInfo(v);
		ui->VNumberLineEdit->setText(QString::number(vi.number));
		ui->VXLineEdit->setText(QString::number(vi.x));
		ui->VYLineEdit->setText(QString::number(vi.y));
		ui->VDegreeLineEdit->setText(QString::number(vi.degree));
		ui->VRadiusSpinBox->setValue(vi.radius);
		ui->VColorLineEdit->setText(vi.color.name());
	}
}

void MainWindow::on_actionDock_visible_toggled(bool arg1){
	ui->VertexDockWidget->setVisible(arg1);
	Options.VDWVisible = arg1;
	Options.Save();
}

void MainWindow::on_VRadiusSpinBox_valueChanged(int arg1){
	int v = GraphV->GetSelectedVertex();
	if(!v){return;}

	TvertexInfo vi = Graph.GetVertexInfo(v);
	if(vi.radius == arg1){return;}

	int oldR = vi.radius;
	vi.radius = arg1;
	if(!Graph.UpdateVertex(v, vi)){
		ui->VRadiusSpinBox->setValue(oldR);
	}
}

void MainWindow::on_VColorToolButton_clicked(){
	int v = GraphV->GetSelectedVertex();
	if(!v){return;}

	TvertexInfo vi = Graph.GetVertexInfo(v);
	QColor col = QColorDialog::getColor(vi.color, this, tr("Vertex color"));
	if(!col.isValid()){return;}

	vi.color = col; Graph.UpdateVertex(v, vi);
	ui->VColorLineEdit->setText(vi.color.name());
}

void MainWindow::UpdateRibInfo(){
	ABrib r = GraphV->GetSelectedRib();

	if(!r.from){
		ui->dockWidgetContents_2->setEnabled(false);
		ui->RFromLineEdit->setText("");
		ui->RToLineEdit->setText("");
		ui->RWeightSpinBox->setValue(0);
		ui->RColorLineEdit->setText("");
	}else{
		ui->dockWidgetContents_2->setEnabled(true);
		TribInfo ri = Graph.GetRibInfo(r);
		ui->RFromLineEdit->setText(QString::number(ri.from));
		ui->RToLineEdit->setText(QString::number(ri.to));
		ui->RWeightSpinBox->setValue(ri.weight);
		ui->RColorLineEdit->setText(ri.color.name());
	}
}

void MainWindow::on_actionRib_info_toggled(bool arg1){
	ui->RibDockWidget->setVisible(arg1);
	Options.RDWVisible = arg1;
	Options.Save();
}

void MainWindow::on_RWeightSpinBox_valueChanged(int arg1){
	ABrib r = GraphV->GetSelectedRib();
	if(!r.from){return;}

	TribInfo ri = Graph.GetRibInfo(r);
	if(ri.weight == arg1){return;}
	ri.weight = arg1;
	Graph.UpdateRib(r, ri);
}

void MainWindow::on_RColorToolButton_clicked(){
	ABrib r = GraphV->GetSelectedRib();
	if(!r.from){return;}

	TribInfo ri = Graph.GetRibInfo(r);
	QColor col = QColorDialog::getColor(ri.color, this, tr("Rib color"));
	if(!col.isValid()){return;}

	ri.color = col;
	Graph.UpdateRib(r, ri);
	ui->RColorLineEdit->setText(ri.color.name());
}

void MainWindow::on_actionRun_algorithm_triggered(){
	GraphV->SetEditable(false);			// Set disabled
	ui->menuBar->setEnabled(false);

	AbstractGraph *NG = new AbstractGraph;
	NG->SetGraphModel(Graph.GetGraphModel());
	GraphV->SetGraph(NG);

	RunGraphAlgorithm(Options.GrAlgoSelected, NG, Options.GrAlgoDelay);

	GraphV->SetGraph(&Graph);			// Set enabled
	delete NG;
	GraphV->SetEditable(Options.Editable);
	ui->menuBar->setEnabled(true);
}

#define X(A) \
void MainWindow::on_action##A##_toggled(bool arg1){\
	if(arg1 && Options.GrAlgoSelected != A##GA){\
		SetAlgoSelected(A##GA);\
	}\
}
	GR_ALGO_LIST
#undef X

void MainWindow::SetAlgoSelected(int algo){
	Options.GrAlgoSelected = algo;
	Options.Save();
	ResetSelected();

#define X(A) if(algo == A##GA){ui->action##A->setChecked(true);}
	GR_ALGO_LIST
#undef X
}

void MainWindow::ResetSelected(){
#define X(A) ui->action##A->setChecked(false);
	GR_ALGO_LIST
#undef X
}

bool MainWindow::Open(QString FileName){
				// Get open name
	if(FileName.isEmpty()){
		FileName = QFileDialog::getOpenFileName(this, tr("Open graph"),
							QDir::currentPath(), "Graph file (*.graph)");

		if(FileName.isEmpty()){return false;}
	}

				// If cant open file
	QFile file(FileName);
	if(!file.open(QFile::ReadOnly)){
		QMessageBox::warning(this, tr("Error"), tr("Cant open file %1:\n%2")
								.arg(FileName, file.errorString()));
		return false;
	}

				// Read
	GraphModel gr;
	QDataStream io(&file);
	io >> gr;
	Graph.SetGraphModel(gr);
	file.close();

	GrChanged = false;
	SetSaveFile(FileName);

	UpdateVerInfo(); UpdateRibInfo();
	ui->actionOriented->setChecked(Graph.IsOriented());

	return true;
}

bool MainWindow::Save(){
					// Get save name
	if(Options.LastFile.isEmpty()){
		QString FileName;
		FileName = QFileDialog::getSaveFileName(this, tr("Save graph"),
						QDir::currentPath(), "Graph file (*.graph)");

		if(FileName.isEmpty()){return false;}

					// If type not set
		QFileInfo FI(FileName);
		if(FI.suffix() != "graph"){FileName += ".graph";}

		SetSaveFile(FileName);
	}

				// If cant open file
	QSaveFile file(Options.LastFile);
	if(!file.open(QFile::WriteOnly)){
		QMessageBox::warning(this, tr("Error"), tr("Cant save file %1:\n%2")
								.arg(Options.LastFile, file.errorString()));
		return false;
	}

				// Write
	GraphModel gr = Graph.GetGraphModel();
	QDataStream io(&file);
	io << gr;
	if(!file.commit()){
		QMessageBox::warning(this, tr("Error"), tr("Cant save file %1:\n%2")
								.arg(Options.LastFile, file.errorString()));
		return false;
	}

	GrChanged = false;
	ResetTitle();

	return true;
}

bool MainWindow::SaveAs(QString FileName){
				// Save last file name
	QString TmpFileName = Options.LastFile;
	Options.LastFile = FileName;

				// Try to save
	if(!Save()){
		SetSaveFile(TmpFileName);
		return false;
	}
	return true;
}

void MainWindow::on_actionOriented_toggled(bool arg1){
	Graph.SetOriented(arg1);
}

void MainWindow::on_actionClear_triggered(){
	Graph.Clear();
}

void MainWindow::on_actionOptions_triggered(){
	OptionDialog opts(&Options, this);
	opts.exec();
	UpdateOptions();
}

void MainWindow::on_actionAbout_triggered(){
	AboutDialog about(this);
	about.exec();
}
