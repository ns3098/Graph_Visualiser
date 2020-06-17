
#include <QDataStream>
#include "abstractgraph.h"

QDataStream &operator<<(QDataStream &io, const TribInfo &r){
	io << r.from  << r.to << r.weight
	   << r.color << r.hidden;
	return io;
}

QDataStream &operator>>(QDataStream &io, TribInfo &r){
	io >> r.from  >> r.to >> r.weight
	   >> r.color >> r.hidden;
	return io;
}

QDataStream &operator<<(QDataStream &io, const TvertexInfo &v){
	io << v.number << v.x << v.y << v.radius
	   << v.degree << v.color << v.hidden;
	return io;
}

QDataStream &operator>>(QDataStream &io, TvertexInfo &v){
	io >> v.number >> v.x >> v.y >> v.radius
	   >> v.degree >> v.color >> v.hidden;
	return io;
}

QDataStream &operator<<(QDataStream &io, const GraphModel &gr){
	io << gr.oriented;

	io << gr.vertexes.count();
	for(int i=0; i<gr.vertexes.count(); ++i){
		io << gr.vertexes[i];
	}

	io << gr.ribs.count();
	for(int i=0; i<gr.ribs.count(); ++i){
		io << gr.ribs[i];
	}

	return io;
}

QDataStream &operator>>(QDataStream &io, GraphModel &gr){
	int n;
	io >> gr.oriented;

	io >> n;
	gr.vertexes.resize(n);
	for(int i=0; i<n; ++i){
		io >> gr.vertexes[i];
	}

	io >> n;
	gr.ribs.resize(n);
	for(int i=0; i<n; ++i){
		io >> gr.ribs[i];
	}

	return io;
}
