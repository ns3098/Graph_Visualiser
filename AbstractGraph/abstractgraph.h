

#ifndef ABSTRACTGRAPH_H
#define ABSTRACTGRAPH_H

#include "../project_global.h"
#include "graph_save_struct.h"
#include "graph_struct_decl.h"
#include "abstractgraph_header.h"

///			<Class>
class AbstractGraph: public QObject{
	Q_OBJECT
	friend class GraphView;

public:
	explicit AbstractGraph();
	~AbstractGraph();

///				<MainData>
private:
	QVector <Tvertex*> Vers;		// Vertexes
	bool Oriented;					// Oriented flag
	CHANGES_VARS
///				</MainData>

///				<Controls>
public:								// Vertex
	bool AddVertex(int px, int py, const TvertexInfo &vi = TvertexInfo());
	bool UpdateVertex(int v, const TvertexInfo &vi);
	bool MoveVertex(int v, int px, int py);
	bool RemoveVertex(int v);
									// Rib
	bool AddRib(const ABrib r, const TribInfo &ri = TribInfo());
	bool UpdateRib(const ABrib r, const TribInfo &ri);
	bool RemoveRib(const ABrib r);
									// Other
	void SetOriented(bool io);
	void Clear();
///				</Controls>

///				<Info>
public:
	int VertexCount() const;					// Vertex count
	int RibCount() const;						// Rib count

	bool IsOriented();							// Is oriented

	QRect VertexToRound(int v) const;			// Vertex to round
	QLine RibToLine(const ABrib &r) const;		// Rib to line

	TvertexInfo GetVertexInfo(int v) const;		// Vertex info
	TribInfo GetRibInfo(const ABrib &r) const;	// Rib info
///				</Info>

///				<GraphModel>
public:
	GraphModel GetGraphModel() const;			// Get graph model
	void SetGraphModel(const GraphModel &gr);	// Set graph model
///				</GraphModel>

///				<Search>
public:
	int FindVertex(int px, int py, int r = 0, int exept = -1) const;
	ABrib FindRib(int px, int py, int r = 0) const;
	bool IsRibExist(const ABrib &r) const;
///				</Search>

///				<HelperFunctions>
private:
	int FindABrib(const ABrib &r) const;
	int FindABrib(const Tvertex &va, const Tvertex &vb) const;
///				<HelperFunctions>

///				<Signals>
signals:
				// Vertex changes
	void ATVertexAdd(int=0);    void VertexAdded(int=0);
	void ATVertexUpdate(int=0); void VertexUpdated(int=0);
	void ATVertexMove(int=0);   void VertexMoved(int=0);
	void ATVertexRemove(int=0); void VertexRemoved(int=0);

				// Rib changes
	void ATRibAdd(ABrib=ABrib());    void RibAdded(ABrib=ABrib());
	void ATRibUpdate(ABrib=ABrib()); void RibUpdated(ABrib=ABrib());
	void ATRibRemove(ABrib=ABrib()); void RibRemoved(ABrib=ABrib());

				// Other changes
	void AboutToChange(); void Changed();
///				</Signals>

};
///			</Class>

#endif // ABSTRACTGRAPH_H
