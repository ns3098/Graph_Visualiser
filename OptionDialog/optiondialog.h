/*****************************************************************************
 * Copyright © 2015 Oakware - All Rights Reserved
 *
 * Github : <https://github.com/Oakware>
 * Authors: Ivan Kondratuk <i.o.kondratuk@gmail.com>
 *
 * This file is part of Graph Builder.
 *
 * Graph Builder is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Graph Builder is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Graph Builder.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

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
