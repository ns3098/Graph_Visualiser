

#include "graphview.h"

GraphView::GraphView(QWidget *parent):
	QWidget(parent)
{
	GV_MAIN_DATA_INIT

	setMouseTracking(true);
	setFocusPolicy(Qt::StrongFocus);
	MoveTimer.setInterval(GV_DEF_TIMER_INTERVAL);
	MoveTimer.setSingleShot(true);
	connect(&MoveTimer, SIGNAL(timeout()), this, SLOT(MoveStart()));

	GV_GRAPH_INIT

	ResetMinimumSize();
}

GraphView::~GraphView(){}

void GraphView::SetEditable(bool ie){
	if(ie == isEditable){return;}
	isEditable = ie;

	if(!isEditable){
		StopEditing();
	}
}

void GraphView::SetStdVertex(const TvertexInfo &vi){
	REGISTER_ERROR_BEGIN(vi.radius >= 0, "GraphView::SetStdVertex", "Radius < 0")
		return;
	REGISTER_ERROR_END

	StdVertex = vi;
}

void GraphView::SetStdRib(const TribInfo &ri){
	StdRib = ri;
}

int GraphView::GetSelectedVertex(){
	return VSelected;
}

ABrib GraphView::GetSelectedRib(){
	return RSelected;
}

void GraphView::SetGraph(AbstractGraph *gr){
	StopEditing();
	SetVertexSelected(0);
	SetRibSelected(ABrib());

	if(Graph){
		disconnect(Graph, SIGNAL(destroyed()), this, SLOT(GraphDestroyed()));
		disconnect(Graph, SIGNAL(Changed()),   this, SLOT(repaint()));

		disconnect(Graph, SIGNAL(VertexAdded()),       this, SLOT(ResetMinimumSize()));
		disconnect(Graph, SIGNAL(VertexUpdated(int)),  this, SLOT(GraphVertexUpdate(int)));
		disconnect(Graph, SIGNAL(VertexMoved()),       this, SLOT(ResetMinimumSize()));
		disconnect(Graph, SIGNAL(ATVertexRemove(int)), this, SLOT(GraphVertexRemove(int)));
		disconnect(Graph, SIGNAL(VertexRemoved()),     this, SLOT(ResetMinimumSize()));

		disconnect(Graph, SIGNAL(RibUpdated(ABrib)),   this, SLOT(GraphRibUpdate(ABrib)));
		disconnect(Graph, SIGNAL(ATRibRemove(ABrib)),  this, SLOT(GraphRibRemove(ABrib)));
	}

	Graph = gr;

	if(Graph){
		connect(Graph, SIGNAL(destroyed()), this, SLOT(GraphDestroyed()));
		connect(Graph, SIGNAL(Changed()),   this, SLOT(repaint()));

		connect(Graph, SIGNAL(VertexAdded()),       this, SLOT(ResetMinimumSize()));
		connect(Graph, SIGNAL(VertexUpdated(int)),  this, SLOT(GraphVertexUpdate(int)));
		connect(Graph, SIGNAL(VertexMoved()),       this, SLOT(ResetMinimumSize()));
		connect(Graph, SIGNAL(ATVertexRemove(int)), this, SLOT(GraphVertexRemove(int)));
		connect(Graph, SIGNAL(VertexRemoved()),     this, SLOT(ResetMinimumSize()));

		connect(Graph, SIGNAL(RibUpdated(ABrib)),   this, SLOT(GraphRibUpdate(ABrib)));
		connect(Graph, SIGNAL(ATRibRemove(ABrib)),  this, SLOT(GraphRibRemove(ABrib)));
	}

	repaint();
}

AbstractGraph *GraphView::GetGraph(){
	return Graph;
}

void GraphView::GraphVertexUpdate(int ver){
	ResetMinimumSize();
	if(ver == VSelected && Graph->Vers[VSelected-1]->isHidden){
		StopEditing();			// If hidden
	}
}

void GraphView::GraphVertexRemove(int ver){
	if(ver == VSelected)
		{StopEditing();}
	else{SetVertexSelected(0);}
}

void GraphView::GraphRibUpdate(ABrib r){
	if((r == RSelected) || (!Graph->IsOriented() &&
							r.EqualNOrient(RSelected)))
	{
		TribInfo ri = Graph->GetRibInfo(r);		// If hidden
		if(ri.hidden){SetRibSelected(ABrib());}
	}
}

void GraphView::GraphRibRemove(ABrib r){
	if((r == RSelected) || (!Graph->IsOriented() &&
							r.EqualNOrient(RSelected)))
		{SetRibSelected(ABrib());}
}

void GraphView::GraphDestroyed(){
	Graph = NULL;
	repaint();
}

bool GraphView::BorderCross(int x, int y, int r){
	return (x-r < 0 || x+r >= width() ||
			y-r < 0 || y+r >= height());
}

void GraphView::SetRVSelected(int px, int py){
	int ver = Graph->FindVertex(px, py);
	SetVertexSelected(ver);

	if(!ver){
		ABrib rib = Graph->FindRib(px, py, 7);
		SetRibSelected(rib);
	}else
		{SetRibSelected(ABrib());}
}

void GraphView::SetVertexSelected(int v){
	if(VSelected == v){return;}
	VSelected = v;
	emit VertexSelected(v);
}

void GraphView::SetRibSelected(const ABrib &r){
	if(RSelected == r){return;}
	RSelected = r;
	emit RibSelected(r);
}

void GraphView::ResetMinimumSize(){
	int mx = 100, my = 100;

	int to;
	if(Graph){to = Graph->VertexCount();}
	else{to = 0;}
				// Find minimum x & y
	for(int i=0; i < to; ++i){
		const Tvertex &v = *Graph->Vers[i];
		if(v.isHidden){continue;}

		if(v.x+v.Radius+1 > mx){mx = v.x+v.Radius+1;}
		if(v.y+v.Radius+1 > my){my = v.y+v.Radius+1;}
	}
				// Reset minimum size
	setMinimumSize(mx, my);
}

void GraphView::StopEditing(){
	MoveTimer.stop();
	if(CurrentState == VertexMoving){
		MoveEnd();
	}
	CurrentState = Normal;

	SetVertexSelected(0);
	SetRibSelected(ABrib());

	repaint();
}

void GraphView::paintEvent(QPaintEvent*){
	QPainter paint(this);
	paint.setRenderHints(QPainter::Antialiasing);

				// Draw widget background
	paint.setPen(Qt::black);
	paint.setBrush(Qt::white);
	paint.drawRect(rect());

	if(Graph == NULL){return;}

				// Draw ribs
	for(int i=1; i <= Graph->VertexCount(); ++i){
		for(int j=0; j < Graph->Vers[i-1]->Ribs.count(); ++j){
			drawRib(paint, i, j);
		}
	}
				// Draw vertexes
	for(int i=1; i <= Graph->VertexCount(); ++i){
		drawVertex(paint, i);
	}
				// Draw adding rib
	paint.setPen(Qt::black);
	if(CurrentState == RibAdd){
		drawArrow(paint, QLine(Graph->Vers[RibFromVer-1]->x,
				  Graph->Vers[RibFromVer-1]->y, CurX, CurY));
	}
				// Draw selected vertex
	if(VSelected){
		drawVertex(paint, VSelected);
	}
}

void GraphView::drawVertex(QPainter &painter, int ver){
	const Tvertex &Vdr = *Graph->Vers[ver-1];

				// If vertex is hidden
	if(Vdr.isHidden) return;

				// Set pen & brush
	if(ver == VSelected){
		painter.setPen(Qt::black);
		if(CurrentState == VertexMoving || CurrentState == RibRemove){
			QColor col(Vdr.Color);
			col.setAlpha(0x2F);
			painter.setBrush(col);
		}else{
			painter.setBrush(Vdr.Color);
		}
	}else{
		painter.setBrush(Vdr.Color);
		painter.setPen(Vdr.Color);
	}

				// Draw round
	painter.drawEllipse(Graph->VertexToRound(ver));

				// Draw number
	painter.setPen(Qt::black);
	painter.drawText(Graph->VertexToRound(ver), 0x84,
					 QString::number(ver));
}

void GraphView::drawRib(QPainter &painter, int ver, int rib){
	Tvertex &Vfrom = *Graph->Vers[ver-1];
	Tvertex &Vto = *((*Graph->Vers[ver-1])[rib].To);

				// If hidden
	if(Vfrom[rib].isHidden || Vfrom.isHidden || Vto.isHidden)
		{return;}

	ABrib r(ver, Graph->Vers.indexOf(&Vto)+1);
	QLine ribL(Graph->RibToLine(r));

				// Set pen
	QPen pen(Vfrom[rib].Color);
	if(r == RSelected){pen.setWidth(3);}
	else{pen.setWidth(1);}

	painter.setPen(pen);

				// Draw line
	if(Graph->IsOriented()){drawArrow(painter, ribL);}
	else{painter.drawLine(ribL);}

				// Draw weight
	int x = (ribL.x1() + ribL.x2())/2;
	int y = (ribL.y1() + ribL.y2())/2;

	painter.setPen(Qt::darkRed);
	painter.drawText(x, y, QString::number(Vfrom[rib].Weight));
}

void GraphView::drawArrow(QPainter &painter, const QLine &line, int aLen, double alpha){
	int x, y, x1, y1;
	double l;

	x = -line.dx(); y = -line.dy();
	l = sqrt(sqr(x)+sqr(y));
	if(!l){l = 0.1;}

	x1 = (x*cos(alpha) - y*sin(alpha)) *aLen/l;
	y1 = (x*sin(alpha) + y*cos(alpha)) *aLen/l;
	painter.drawLine(line.x2(),    line.y2(),
					 line.x2()+x1, line.y2()+y1);

	x1 = ( x*cos(alpha) + y*sin(alpha)) *aLen/l;
	y1 = (-x*sin(alpha) + y*cos(alpha)) *aLen/l;
	painter.drawLine(line.x2(),    line.y2(),
					 line.x2()+x1, line.y2()+y1);

	painter.drawLine(line);
}

void GraphView::mousePressEvent(QMouseEvent *ev){
	if(!(isEditable && Graph)){return;}

	CurX = ev->x();				// Set selected
	CurY = ev->y();

	if(ev->button() != Qt::LeftButton){
		if(CurrentState == Normal)
			{SetRVSelected(ev->x(), ev->y()); repaint();}
		return;
	}

	SetRVSelected(ev->x(), ev->y());
	if(CurrentState == RibAdd){CMAddRibEnd();}				// Add new rib
	else if(CurrentState == RibRemove){CMRemoveRibEnd();}	// Remove rib

	if(VSelected > 0){MoveTimer.start();}					// Move timer start

	repaint();
}

void GraphView::mouseReleaseEvent(QMouseEvent *ev){
	if(ev->button() != Qt::LeftButton)
		{return;}

	MoveTimer.stop();
	if(CurrentState == VertexMoving)		// Move end
		{MoveEnd();}
}

void GraphView::mouseDoubleClickEvent(QMouseEvent *ev){
	if(!(isEditable && Graph) || ev->button() != Qt::LeftButton)
		{return;}

	if(VSelected == 0){CMAddVertex();}	// Add vertex
	else{CMAddRibBegin();}				// Rib add state
}

void GraphView::keyPressEvent(QKeyEvent *ev){
	if(!(isEditable && Graph) || CurrentState != Normal)
		{return;}

	if(ev->key() == Qt::Key_Delete){
		if(VSelected){CMRemoveVertex();}
		else if(RSelected.from){CMRemoveRib();}
	}
}

void GraphView::mouseMoveEvent(QMouseEvent *ev){
	if(MoveTimer.isActive()){			// Stop moving
		if(Graph->FindVertex(ev->x(), ev->y()) != VSelected)
			{MoveTimer.stop();}
	}

	if(CurrentState == RibAdd){						// Set current position
		CurX = ev->x(); CurY = ev->y();
		repaint();
	}else if(CurrentState == VertexMoving){			// Vertex move
		if(!VSelected){return;}

		Tvertex &ver = *Graph->Vers[VSelected-1];

		if(ev->x()-ver.Radius < 0){ver.x = ver.Radius;}
		else if(ev->x()+ver.Radius >= width())
			{ver.x = width()-ver.Radius-1;}
		else{ver.x = ev->x();}

		if(ev->y()-ver.Radius < 0){ver.y = ver.Radius;}
		else if(ev->y()+ver.Radius >= height())
			{ver.y = height()-ver.Radius-1;}
		else{ver.y = ev->y();}

		repaint();
	}else if(Graph){
		if(Graph->FindVertex(ev->x(), ev->y()) ||
				Graph->FindRib(ev->x(), ev->y(), 7).from)
			{setCursor(Qt::CrossCursor);}
		else
			{setCursor(Qt::ArrowCursor);}
	}
}

void GraphView::MoveStart(){
	CurrentState = VertexMoving;
	repaint();
}

void GraphView::MoveEnd(){
	CurrentState = Normal;
	if(VSelected){
		Graph->MoveVertex(VSelected, Graph->Vers[VSelected-1]->x,
						  Graph->Vers[VSelected-1]->y);
	}else{
		ResetMinimumSize();
	}
}

void GraphView::contextMenuEvent(QContextMenuEvent *ev){
	if(!(isEditable && Graph) || CurrentState != Normal)
		{return;}

	QMenu menu(this);

				// Add actions
	if(VSelected > 0){
		menu.addAction(QIcon(":/functional/Add.png"),		// Add rib
                       tr("Add edge"), this, SLOT(CMAddRibBegin()));
		menu.addAction(QIcon(":/functional/Delete.png"),	// Remove rib
                       tr("Remove edge"), this, SLOT(CMRemoveRibBegin()));
		menu.addAction(QIcon(":/functional/Delete.png"),	// Remove vertex
					   tr("Remove vertex"),  this, SLOT(CMRemoveVertex()));
	}else if(RSelected.from){
		menu.addAction(QIcon(":/functional/Delete.png"),	// Remove rib
                       tr("Remove edge"),  this, SLOT(CMRemoveRib()));
	}else{
		menu.addAction(QIcon(":/functional/Add.png"),		// Add vertex
					   tr("Add vertex"), this, SLOT(CMAddVertex()));
	}

	menu.popup(ev->globalPos());
	menu.exec();
}

void GraphView::CMAddVertex(){
	if(BorderCross(CurX, CurY, StdVertex.radius))
		{return;}

	Graph->AddVertex(CurX, CurY, StdVertex);
	SetVertexSelected(Graph->VertexCount());
	repaint();
}

void GraphView::CMRemoveVertex(){
	Graph->RemoveVertex(VSelected);
}

void GraphView::CMAddRibBegin(){
	CurrentState = RibAdd;
	RibFromVer = VSelected;
	repaint();
}

void GraphView::CMAddRibEnd(){
	CurrentState = Normal;

	if(VSelected > 0){
		Graph->AddRib(ABrib(RibFromVer, VSelected), StdRib);
	}
}

void GraphView::CMRemoveRibBegin(){
	CurrentState = RibRemove;
	RibFromVer = VSelected;
	repaint();
}

void GraphView::CMRemoveRibEnd(){
	CurrentState = Normal;

	if(VSelected > 0){
		Graph->RemoveRib(ABrib(RibFromVer, VSelected));
	}
}

void GraphView::CMRemoveRib(){
	Graph->RemoveRib(RSelected);
}
