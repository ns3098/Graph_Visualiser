
#include "abstractgraph.h"

AbstractGraph::AbstractGraph():
	Oriented(false)
{
	DEBUG_LOG("AbstractGraph::AbstractGraph")

	CHANGES_INIT
}

AbstractGraph::~AbstractGraph(){
	DEBUG_LOG("AbstractGraph::~AbstractGraph")

	while(Vers.count())
		{delete Vers.takeAt(0);}
}

bool AbstractGraph::AddVertex(int px, int py, const TvertexInfo &vi){
	DEBUG_LOG("AbstractGraph::AddVertex", DL_PARAM(px) DL_PARAM(py))

	REGISTER_ERROR_BEGIN(vi.radius >= 0, "AbstractGraph::AddVertex", "Radius < 0")
		return false;
	REGISTER_ERROR_END

	REGISTER_ERROR_BEGIN((px-vi.radius >=0 && py-vi.radius >= 0),
						 "AbstractGraph::AddVertex", "Bourder cross")
		return false;
	REGISTER_ERROR_END

	CHANGES_BEGIN
	emit ATVertexAdd(Vers.count()+1);

	Tvertex *NewVer = new Tvertex(vi);
	NewVer->x = px; NewVer->y = py;
	Vers << NewVer;

	emit VertexAdded(Vers.count());
	CHANGES_END

	return true;
}

bool AbstractGraph::UpdateVertex(int v, const TvertexInfo &vi){
	DEBUG_LOG("AbstractGraph::UpdateVertex", DL_PARAM(v))

	REGISTER_ERROR_BEGIN((v > 0 && v <= VertexCount()),
						 "AbstractGraph::UpdateVertex", "Out of range")
		return false;
	REGISTER_ERROR_END

	REGISTER_ERROR_BEGIN(vi.radius >= 0, "AbstractGraph::UpdateVertex", "Radius < 0")
		return false;
	REGISTER_ERROR_END

	Tvertex &ver = *Vers[v-1];		// Get vertex to update

	REGISTER_ERROR_BEGIN((ver.x-vi.radius >=0 && ver.y-vi.radius >= 0),
						 "AbstractGraph::UpdateVertex", "Bourder cross")
		return false;
	REGISTER_ERROR_END

	CHANGES_BEGIN
	emit ATVertexUpdate(v);

	ver.SetProperties(vi);

	emit VertexUpdated(v);
	CHANGES_END

			return true;
}

bool AbstractGraph::MoveVertex(int v, int px, int py){
	DEBUG_LOG("AbstractGraph::MoveVertex", DL_PARAM(px) DL_PARAM(py))

	REGISTER_ERROR_BEGIN((v > 0 && v <= VertexCount()),
						 "AbstractGraph::MoveVertex", "Out of range")
		return false;
	REGISTER_ERROR_END

	Tvertex &ver = *Vers[v-1];

	REGISTER_ERROR_BEGIN((px-ver.Radius >=0 && py-ver.Radius >= 0),
						 "AbstractGraph::MoveVertex", "Bourder cross")
		return false;
	REGISTER_ERROR_END

	CHANGES_BEGIN
	emit ATVertexMove(v);

	ver.x = px;
	ver.y = py;

	emit VertexMoved(v);
	CHANGES_END

	return true;
}

bool AbstractGraph::RemoveVertex(int v){
	DEBUG_LOG("AbstractGraph::RemoveVertex", DL_PARAM(v))

	REGISTER_ERROR_BEGIN((v > 0 && v <= VertexCount()),
						 "AbstractGraph::RemoveVertex", "Out of range")
		return false;
	REGISTER_ERROR_END

	CHANGES_BEGIN
	emit ATVertexRemove(v);

				// Delete all ribs to v
	for(int i=1; i <= VertexCount(); ++i){
		if(IsRibExist(ABrib(i,v))){
			RemoveRib(ABrib(i,v));
		}
	}
				// Delete all ribs from v
	if(Oriented){
		Tvertex &ver = *Vers[v-1];
		for(int i=0; i<ver.Ribs.count(); ++i){
			RemoveRib(ABrib(v, Vers.indexOf(ver[i].To)+1));
		}
	}

	delete Vers.takeAt(v-1);	// Delete

	emit VertexRemoved(v);
	CHANGES_END

	return true;
}

bool AbstractGraph::AddRib(const ABrib r, const TribInfo &ri){
	DEBUG_LOG("AbstractGraph::AddRib", DL_PARAM(r.from) DL_PARAM(r.to))

	REGISTER_ERROR_BEGIN((r.from > 0 && r.from <= VertexCount() &&
						  r.to   > 0 && r.to   <= VertexCount()),
						 "AbstractGraph::AddRib", "Out of range")
		return false;
	REGISTER_ERROR_END

	REGISTER_ERROR_BEGIN(r.from != r.to, "AbstractGraph::AddRib", "Add loop")
		return false;
	REGISTER_ERROR_END

	REGISTER_ERROR_BEGIN(FindABrib(r) < 0,
						 "AbstractGraph::AddRib", "Rib have already exist")
		UpdateRib(r, ri);
		return false;
	REGISTER_ERROR_END

	CHANGES_BEGIN
	emit ATRibAdd(r);

	Tvertex &Vfrom = *Vers[r.from-1];
	Tvertex &Vto = *Vers[r.to-1];

				// Add a -> b rib
	Trib rib(ri);
	rib.To = &Vto;
	Vfrom.Ribs << rib;

				// Add a <- b rib
	if(!Oriented){
		rib.To = &Vfrom;
		Vto.Ribs << rib;
	}

	emit RibAdded(r);
	CHANGES_END

	return true;
}

bool AbstractGraph::UpdateRib(const ABrib r, const TribInfo &ri){
	DEBUG_LOG("AbstractGraph::UpdateRib", DL_PARAM(r.from) DL_PARAM(r.to))

				// a -> b rib
	int rab = FindABrib(r);
	REGISTER_ERROR_BEGIN(rab >= 0, "AbstractGraph::UpdateRib", "Rib does not exist")
		return false;
	REGISTER_ERROR_END

	CHANGES_BEGIN
	emit ATRibUpdate(r);

	(*Vers[r.from-1])[rab].SetProperties(ri);

				// a <- b rib
	if(!Oriented){
		rab = FindABrib(ABrib(r.to, r.from));
		(*Vers[r.to-1])[rab].SetProperties(ri);
	}

	emit RibUpdated(r);
	CHANGES_END

	return true;
}

bool AbstractGraph::RemoveRib(const ABrib r){
	DEBUG_LOG("AbstractGraph::RemoveRib", DL_PARAM(r.from) DL_PARAM(r.to))

				// Remove rib a -> b
	int rab = FindABrib(r);
	REGISTER_ERROR_BEGIN(rab >= 0, "AbstractGraph::RemoveRib", "Rib does not exist")
		return false;
	REGISTER_ERROR_END

	CHANGES_BEGIN
	emit ATRibRemove(r);

	Vers[r.from-1]->Ribs.remove(rab);

				// Remove rib a <- b
	if(!Oriented){
		rab = FindABrib(ABrib(r.to, r.from));
		Vers[r.to-1]->Ribs.remove(rab);
	}

	emit RibRemoved(r);
	CHANGES_END

	return true;
}

void AbstractGraph::SetOriented(bool io){
	DEBUG_LOG("AbstractGraph::SetOriented", DL_PARAM(io))

	if(io == Oriented){return;}

	CHANGES_BEGIN

	if(io){
		Oriented = io;
		CHANGES_END
		return;
	}
				// if switch to not oriented - add ribs
	for(int i=1; i<=VertexCount(); ++i){
		const Tvertex &ver = *Vers[i-1];

		for(int j=0; j<ver.Ribs.count(); ++j){
			int to = Vers.indexOf(ver[j].To)+1;

			if(!IsRibExist(ABrib(to, i))){		// Add rib
				AddRib(ABrib(to, i), TribInfo(ver[j]));
			}else{								// Update rib
				UpdateRib(ABrib(to, i), TribInfo(ver[j]));
			}
		}
	}
	Oriented = io;

	CHANGES_END
}

void AbstractGraph::Clear(){
	DEBUG_LOG("AbstractGraph::Clear")

	CHANGES_BEGIN
	while(VertexCount()){RemoveVertex(1);}
	CHANGES_END
}

int AbstractGraph::VertexCount() const{
	return Vers.count();
}

int AbstractGraph::RibCount() const{
	int c = 0;
	for(int i=0; i<VertexCount(); ++i){
		c += Vers[i]->Ribs.count();
	}

	if(!Oriented){
		return c/2;
	}else{
		return c;
	}
}

bool AbstractGraph::IsOriented(){
	return Oriented;
}

QRect AbstractGraph::VertexToRound(int v) const{
	REGISTER_ERROR_BEGIN((v > 0 && v <= VertexCount()) ,
						 "AbstractGraph::VertexToRound", "Out of range")
		return QRect();
	REGISTER_ERROR_END

	const Tvertex &ver = *Vers[v-1];
	return QRect(QPoint(ver.x-ver.Radius, ver.y-ver.Radius),
				 QPoint(ver.x+ver.Radius, ver.y+ver.Radius));
}

QLine AbstractGraph::RibToLine(const ABrib &r) const{
	int rab = FindABrib(r);
	REGISTER_ERROR_BEGIN(rab >= 0, "AbstractGraph::RibToLine", "Rib does not exist")
		return QLine();
	REGISTER_ERROR_END

	const Tvertex &Vfrom = *Vers[r.from-1];
	const Tvertex &Vto = *Vers[r.to-1];

	int vx = Vto.x - Vfrom.x;
	int vy = Vto.y - Vfrom.y;
	double l = sqrt(sqr(vx) + sqr(vy));

	QPoint p1(Vfrom.x + vx*Vfrom.Radius/l, Vfrom.y + vy*Vfrom.Radius/l);
	QPoint p2(Vto.x   - vx*Vto.Radius/l,   Vto.y   - vy*Vto.Radius/l);

	return QLine(p1, p2);
}

TvertexInfo AbstractGraph::GetVertexInfo(int v) const{
	REGISTER_ERROR_BEGIN((v > 0 && v <= VertexCount()) ,
						 "AbstractGraph::GetVertexInfo", "Out of range")
		return TvertexInfo();
	REGISTER_ERROR_END

	return TvertexInfo(*Vers[v-1], v);
}

TribInfo AbstractGraph::GetRibInfo(const ABrib &r) const{
	int rab = FindABrib(r);
	REGISTER_ERROR_BEGIN(rab >= 0, "AbstractGraph::GetRibInfo", "Rib does not exist")
		return false;
	REGISTER_ERROR_END

	return TribInfo((*Vers[r.from-1])[rab], r.from, r.to);
}

GraphModel AbstractGraph::GetGraphModel() const{
	GraphModel gr;

	gr.oriented = Oriented;

	for(int i=0; i<VertexCount(); ++i){
		gr.vertexes << TvertexInfo(*Vers[i]);
		for(int j=0; j<Vers[i]->Ribs.count(); ++j){
			gr.ribs << TribInfo((*Vers[i])[j],  i+1,
				   Vers.indexOf((*Vers[i])[j].To)+1);
		}
	}

	return gr;
}

void AbstractGraph::SetGraphModel(const GraphModel &gr){
	DEBUG_LOG("AbstractGraph::SetGraphModel")

	CHANGES_BEGIN
	Clear();
	SetOriented(gr.oriented);

	for(int i=0; i<gr.vertexes.count(); ++i){
		AddVertex(gr.vertexes[i].x, gr.vertexes[i].y, gr.vertexes[i]);
	}

	for(int i=0; i<gr.ribs.count(); ++i){
		AddRib(ABrib(gr.ribs[i].from, gr.ribs[i].to), gr.ribs[i]);
	}
	CHANGES_END
}

int AbstractGraph::FindVertex(int px, int py, int r, int exept) const{
	for(int i=1; i <= VertexCount(); ++i){
		const Tvertex &ver = *Vers[i-1];

					// Miss hidden or exept vertexes
		if(ver.isHidden || (i == exept)){continue;}

					// If rounds cross
		if(sqrt(sqr(px-ver.x) + sqr(py-ver.y)) <= ver.Radius + r){
			return i;
		}
	}

	return 0;
}

ABrib AbstractGraph::FindRib(int px, int py, int r) const{
	QPoint m(px,py);
	QLine rLine; ABrib rib;
	double l;

	GraphModel gr = GetGraphModel();
	for(int i=0; i<gr.ribs.count(); ++i){
		rib = ABrib(gr.ribs[i].from, gr.ribs[i].to);

					// Miss hidden ribs
		if(gr.ribs[i].hidden || Vers[rib.from-1]->isHidden
							 || Vers[rib.to-1]->isHidden)
			{continue;}

		rLine = RibToLine(rib);
		l = PointToLine(m, rLine);
		if(l <= r){return rib;}
	}

	return ABrib(0,0);
}

bool AbstractGraph::IsRibExist(const ABrib &r) const{
	return (FindABrib(r) >= 0);
}

int AbstractGraph::FindABrib(const ABrib &r) const{
	REGISTER_ERROR_BEGIN((r.from > 0 && r.from <= VertexCount() &&
						  r.to   > 0 && r.to   <= VertexCount()),
						 "AbstractGraph::FindABrib", "Out of range")
		return -1;
	REGISTER_ERROR_END

	return FindABrib(*Vers[r.from-1], *Vers[r.to-1]);
}

int AbstractGraph::FindABrib(const Tvertex &va, const Tvertex &vb) const{
				// Find a -> b rib
	for(int i=0; i < va.Ribs.count(); ++i){
		if(va[i].To == &vb) return i;
	}
	return -1;
}
