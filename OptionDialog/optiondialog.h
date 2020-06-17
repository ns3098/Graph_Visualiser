

#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

///				<Includes>
#include <QCoreApplication>
#include <QDialog>
#include <QColorDialog>

#include <QLocale>
#include <QTranslator>
#include <QDir>
#include <QFile>
#include <QDataStream>
///				</Includes>

namespace Ui{class OptionDialog;}

class OptionDialog: public QDialog{
	Q_OBJECT
public:
	struct PROptions;

	explicit OptionDialog(PROptions *opt, QWidget *parent = 0);
	~OptionDialog();

///				<OptionStruct>
	struct PROptions{
		PROptions();

		void Load();			// Load options
		void Save();			// Save options

		void SetLanguage(QLocale lang);
		QLocale Language;
		QTranslator Translator;

		int VertexRadius;		// Standard vertex radius
		QColor VertexColor;		// Standard vertex color
		int RibWeight;			// Standard rib weight
		bool Editable;			// Graph editable flag

		bool VDWVisible;		// Vertex dock widget visible flag
		bool RDWVisible;		// Rib dock widget visible flag
		int GrAlgoSelected;		// Selected algorithm
		int GrAlgoDelay;		// Algorithm sleep delay

		QByteArray Geometry;	// Geometry

		QString LastFile;		// Last opened file
	};
///				</OptionStruct>

///				<MainData>
private:
	Ui::OptionDialog *ui;

	PROptions *Options;			// Options
	QColor TmpColor;			// Temp color
///				</MainData>

///				<MainSlots>
private slots:
	void ColorSet();					// Set temp color
	void on_OptionDialog_accepted();	// Apply all changes
///				</MainSlots>

};

#endif // OPTIONDIALOG_H
