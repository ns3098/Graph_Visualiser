QT += core gui
ICON = App.icns
RC_FILE = app.rc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = "Graph Builder"
VERSION = 1.0
TEMPLATE = app

SOURCES += main.cpp\
		mainwindow.cpp \
	OptionDialog/optiondialog.cpp \
	AbstractGraph/abstractgraph.cpp \
	AbstractGraph/graph_struct_decl.cpp \
	AbstractGraph/graphio.cpp \
	GraphView/graphview.cpp \
	AbstractGraph/graph_save_struct.cpp \
	GraphAlgorithms/DijkstraDialog/dijkstradialog.cpp

HEADERS  += mainwindow.h \
	OptionDialog/optiondialog.h \
	AboutDialog/aboutdialog.h \
	project_global.h \
	AbstractGraph/abstractgraph.h \
	AbstractGraph/abstractgraph_header.h \
	AbstractGraph/graph_struct_decl.h \
	GraphView/graphview.h \
	GraphView/graphview_header.h \
	AbstractGraph/graph_save_struct.h \
	Global/point_to_line.h \
	Global/debug_log.h \
	Global/changes_api.h \
	Global/sleep.h \
	mainwindow_header.h \
	GraphAlgorithms/graph_algorithms.h \
	GraphAlgorithms/bfs.h \
	GraphAlgorithms/dfs.h \
	GraphAlgorithms/dijkstra.h \
	GraphAlgorithms/GetVertex/getvertex.h \
	GraphAlgorithms/reset_graph.h \
	GraphAlgorithms/DijkstraDialog/dijkstradialog.h \
	GraphAlgorithms/minost_kruskal.h \
	GraphAlgorithms/minost_prim.h

FORMS    += mainwindow.ui \
	OptionDialog/optiondialog.ui \
	AboutDialog/aboutdialog.ui \
	GraphAlgorithms/GetVertex/getvertex.ui \
	GraphAlgorithms/DijkstraDialog/dijkstradialog.ui

RESOURCES += Resourses/resourses.qrc \
             qdarkstyle/style.qrc

