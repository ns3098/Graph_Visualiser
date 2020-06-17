

#ifndef DEBUG_LOG_H
#define DEBUG_LOG_H

///			<Includes>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <iostream>
///			</Includes>

///			<Defines>
#define D_COLOR "32"
#define D_COLOR_NORMAL "39"
#define D_COLOR_ERROR "41"
#define D_COLOR_E_NORMAL "49"
///			</Defines>

#if __linux__
struct DebugLogHelper{
	DebugLogHelper(){
		QDir dir; dir.mkpath(dir.currentPath()+"/Options");
		dlog.setFileName(dir.currentPath()+"/Options/Debug.log");
		dlog.open(QFile::WriteOnly);
		dlIO.setDevice(&dlog);
	}
	~DebugLogHelper(){dlog.close();}

	QFile dlog;
	QTextStream dlIO;
};

static DebugLogHelper DEBUG_LOG_HELPER;
#endif

///			<Debug>
			// Debug log function parameter
#define DL_PARAM(X) << X << ','

			// Debug log define
#if __linux__
#define DEBUG_LOG(where, ...) \
	std::cout << "\033[" D_COLOR "m"  << where "(" __VA_ARGS__ << ')'\
			  << "\033[" D_COLOR_NORMAL "m\n";\
/*	qDebug(where "()");*/\
	DEBUG_LOG_HELPER.dlIO << where "(" __VA_ARGS__ << ")\n";

			// Debug log register error
#define DEBUG_LOG_ERROR(where, what) \
	std::cout << "\033[" D_COLOR_ERROR "m"     << where "()"\
			  << "\033[" D_COLOR_E_NORMAL "m"  << ": " what "\n";\
/*	qDebug(where "(): " what);*/\
	DEBUG_LOG_HELPER.dlIO << where "(): " what "\n";
#else
#define DEBUG_LOG(...)
#define DEBUG_LOG_ERROR(...)
#endif
///			</Debug>

///			<DebugError>
			// Register error begin
#define REGISTER_ERROR_BEGIN(cound, where, what) \
	if(!(cound)){\
	DEBUG_LOG_ERROR(where, what)

			// Register error end
#define REGISTER_ERROR_END }
///			</DebugError>

#endif // DEBUG_LOG_H
