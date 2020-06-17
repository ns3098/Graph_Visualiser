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

#include "optiondialog.h"
#include "ui_optiondialog.h"

OptionDialog::OptionDialog(PROptions *opt, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::OptionDialog),
	Options(opt)
{
	ui->setupUi(this);
	resize(0,0); setFixedSize(size());

				// Button connect
	connect(ui->toolButton, SIGNAL(clicked()),
			this, SLOT(ColorSet()));

				// Initialize setings
	ui->comboBox->addItem(QString("English"),   QVariant(QLocale(QLocale::English)));
    //ui->comboBox->addItem(QString("Українська"),QVariant(QLocale(QLocale::Ukrainian)));

	switch(Options->Language.language()){
		case QLocale::English:		ui->comboBox->setCurrentIndex(0); break;
		case QLocale::Ukrainian:	ui->comboBox->setCurrentIndex(1); break;
		default:					ui->comboBox->setCurrentIndex(0);
	}

	ui->horizontalSlider->setValue(1000-Options->GrAlgoDelay);

	ui->spinBox->setValue(Options->VertexRadius);

	TmpColor = Options->VertexColor;
	ui->lineEdit->setText(TmpColor.name());

	ui->spinBox_2->setValue(Options->RibWeight);

	if(Options->Editable){
		ui->checkBox->setCheckState(Qt::Checked);
	}else{
		ui->checkBox->setCheckState(Qt::Unchecked);
	}
}

OptionDialog::~OptionDialog(){delete ui;}

OptionDialog::PROptions::PROptions():
	VertexRadius(20), VertexColor(Qt::green), RibWeight(10),
	Editable(true), VDWVisible(true), RDWVisible(true),
	GrAlgoSelected(0), GrAlgoDelay(500), Language(QLocale::English)
{
	QString pluginsPath = QCoreApplication::applicationDirPath() + "/Options/";
	QDir dir(pluginsPath);
	if(!dir.exists()) dir.mkpath(pluginsPath);
}

void OptionDialog::PROptions::Load(){
	QFile Opt(QCoreApplication::applicationDirPath() + "/Options/Options.dat");
	QDataStream io(&Opt);

	if(!Opt.open(QFile::ReadOnly))
		{Save(); return;}

	io >> VertexRadius >> VertexColor >> RibWeight
	   >> Editable >> VDWVisible >> RDWVisible >> GrAlgoSelected
	   >> GrAlgoDelay >> Language >> Geometry >> LastFile;

	Opt.close();
}

void OptionDialog::PROptions::Save(){
	QFile Opt(QCoreApplication::applicationDirPath() + "/Options/Options.dat");
	QDataStream io(&Opt);

	Opt.open(QFile::WriteOnly);

	io << VertexRadius << VertexColor << RibWeight
	   << Editable << VDWVisible << RDWVisible << GrAlgoSelected
	   << GrAlgoDelay << Language << Geometry << LastFile;

	SetLanguage(Language);
	Opt.close();
}

void OptionDialog::PROptions::SetLanguage(QLocale lang){
	Language = lang;

	if(lang != QLocale::English){
		Translator.load(QLocale::languageToString(lang.language()), ":/");
		QApplication::installTranslator(&Translator);
	}else{
		QApplication::removeTranslator(&Translator);
	}
}

void OptionDialog::ColorSet(){
	QColor col = QColorDialog::getColor(TmpColor, this, tr("Vertex color"));
	if(!col.isValid()){return;}

	TmpColor = col;
	ui->lineEdit->setText(TmpColor.name());
}

void OptionDialog::on_OptionDialog_accepted(){
	Options->SetLanguage(ui->comboBox->currentData().toLocale());
	Options->GrAlgoDelay = 1000 - ui->horizontalSlider->value();

	Options->VertexRadius = ui->spinBox->value();
	Options->VertexColor = TmpColor;
	Options->RibWeight = ui->spinBox_2->value();
	Options->Editable = ui->checkBox->isChecked();

	Options->Save();
}
