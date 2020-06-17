

#ifndef GRAPH_STRUCT_DECL_H
#define GRAPH_STRUCT_DECL_H

/*
 * Graph engine structures declaration:
 *  - Items(Tvertex, Trib)
 *  - Info items(TvertexInfo, TribInfo)
*/

///			<Includes>
#include <QColor>
#include <QVector>
///			</Includes>

///			<Defines>
#define VER_DEFAULT_RADIUS 20
#define VER_DEFAULT_COLOR Qt::green
#define RIB_DEFAULT_WEIGHT 10
#define RIB_DEFAULT_COLOR Qt::black
///			</Defines>

///			<StructDecl>
struct Trib;
struct Tvertex;
struct TribInfo;
struct TvertexInfo;
///			</StructDecl>

///			<GraphItems>
			// Rib
struct Trib{
	Trib(int w = RIB_DEFAULT_WEIGHT,
		 const QColor &col = RIB_DEFAULT_COLOR);

	Trib(const TribInfo &ri);

	void SetProperties(const TribInfo &ri);

	Tvertex *To;		// To vertex
	int Weight;			// Rib weight

	QColor Color;		// Rib color
	bool isHidden;		// Hidden flag
};

			// Vertex
struct Tvertex{
	Tvertex(int r = VER_DEFAULT_RADIUS,
			const QColor &col = VER_DEFAULT_COLOR);

	Tvertex(const TvertexInfo &vi);

	void SetProperties(const TvertexInfo &vi);

	int x,y;			// Center position
	int Radius;			// Vertex radius

	QColor Color;		// Vertex color
	bool isHidden;		// Hidden flag

	QVector <Trib> Ribs;// Ribs vector

					// Returns i rib
	inline const Trib &operator[](int i) const
		{return Ribs.at(i);}
	inline Trib &operator[](int i)
		{return Ribs[i];}
};
///			</GraphItems>

///			<GtaphItemsInfo>
			// Rib item info
struct TribInfo{
	TribInfo(int w = RIB_DEFAULT_WEIGHT,
			 const QColor &col = RIB_DEFAULT_COLOR);

	TribInfo(const Trib &rib, int From = 0, int To = 0);

	void GetProperties(const Trib &rib);

	int from, to;		// Vector
	int weight;			// Rib weight

	QColor color;		// Color
	bool hidden;		// Hidden flag
};

			// Vertex item info
struct TvertexInfo{
	TvertexInfo(int r = VER_DEFAULT_RADIUS,
				const QColor &col = VER_DEFAULT_COLOR);

	TvertexInfo(const Tvertex &ver, int n = 0);

	void GetProperties(const Tvertex &ver);

	int x,y;			// Position
	int radius;			// Radius

	int number;			// Number
	int degree;			// Degree
	QColor color;		// Color
	bool hidden;		// Hidden flag
};
///			</GtaphItemsInfo>

///			<IOdeclaration>
QDataStream &operator<<(QDataStream &, const TribInfo &);
QDataStream &operator>>(QDataStream &, TribInfo &);

QDataStream &operator<<(QDataStream &, const TvertexInfo &);
QDataStream &operator>>(QDataStream &, TvertexInfo &);
///			</IOdeclaration>

///			<Undefs>
#undef VER_DEFAULT_RADIUS
#undef VER_DEFAULT_COLOR
#undef RIB_DEFAULT_WEIGHT
#undef RIB_DEFAULT_COLOR
///			</Undefs>

#endif // GRAPH_STRUCT_DECL_H
