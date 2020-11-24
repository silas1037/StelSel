#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QGuiApplication>
#include <QTimer>
#include <QSettings>
#include <QFileInfo>
#include <QDir>

#include <QSettings>
#include <QString>
#include <QVariant>
#include <QStringList>
#include <QIODevice>
#include <QRegExp>
#include <QDebug>

#include <cstdlib>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include <QString>
#include <QDebug>
#include <QStandardPaths>
#include <QtAndroid>
#include <qmap.h>

#include <QJsonDocument>

#include <QJsonParseError>

#include <QFile>

#include <QJsonObject>

#include <QDebug>

#include <QJsonArray>

#ifdef Q_OS_WIN
	static const QString stelEndl="\r\n";
#else
	static const QString stelEndl="\n";
#endif

QMap<QString, QString> map;
QStringList fileLocations;
QString cuserDir = QString::fromLocal8Bit(qgetenv("EXTERNAL_STORAGE")) + "/stellarium";
QSettings *conf;
void makeSureDirExistsAndIsWritable(const QString& dirFullPath)
{

	// Check that the dirFullPath directory exists
	QFileInfo uDir(dirFullPath);
	if (!uDir.exists())
	{
		// The modules directory doesn't exist, lets create it.
		qDebug() << "Creating directory " << QDir::toNativeSeparators(uDir.filePath());
		if (!QDir("/").mkpath(uDir.filePath()))
		{
			throw std::runtime_error(QString("Could not create directory: " +uDir.filePath()).toStdString());
		}
		QFileInfo uDir2(dirFullPath);
		if (!uDir2.isWritable())
		{
			throw std::runtime_error(QString("Directory is not writable: " +uDir2.filePath()).toStdString());
		}
	}
	else if (!uDir.isWritable())
	{
		throw std::runtime_error(QString("Directory is not writable: " +uDir.filePath()).toStdString());
	}
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
#ifdef Q_OS_WIN
	QString winApiPath = getWin32SpecialDirPath(CSIDL_APPDATA);
	if (!winApiPath.isEmpty())
	{
		userDir = winApiPath + "\\Stellarium";
	}
#elif defined(Q_OS_MAC)
	userDir = QDir::homePath() + "/Library/Application Support/Stellarium";
#else
	QStringList perms;
	    perms << "android.permission.WRITE_EXTERNAL_STORAGE" << "android.permission.ACCESS_FINE_LOCATION" << "android.permission.READ_EXTERNAL_STORAGE" << "android.permission.ACCESS_COARSE_LOCATION";
	    QtAndroid::PermissionResultMap checkPerms = QtAndroid::requestPermissionsSync(perms);
	    QHash<QString, QtAndroid::PermissionResult>::iterator i;
	    for (i = checkPerms.begin(); i != checkPerms.end(); i++)
		qDebug() << i.key() << ": " << (i.value() == QtAndroid::PermissionResult::Granted);
#endif

    ui->setupUi(this);
    QStringList fileLocations;
    QString cuserDir = QString::fromLocal8Bit(qgetenv("EXTERNAL_STORAGE")) + "/stellarium";
    if (!QFile(cuserDir).exists())
	    {
	    qWarning() << "User config directory does not exist: " << QDir::toNativeSeparators(cuserDir);
	    ui->textBrowser->append("User config directory does not exist\r\n");
	    }
	    try
	    {
	    makeSureDirExistsAndIsWritable(cuserDir);
	    fileLocations.append(cuserDir);  // Higher priority than default dir
	    qDebug() << "User config directory " << QDir::toNativeSeparators(cuserDir) << " added to list.";
	    }
	    catch (std::runtime_error &e)
	    {
	    ui->textBrowser->append("Cannot write to SD card, will not add custom user directory\r\n");
	    //ui->textBrowser->append("\r\nCannot write to SD card, will not add custom user directory\r\n");
	    qWarning() << "Cannot write to SD card, will not add custom user directory";
	    }
    conf = new QSettings(cuserDir+"/config.ini", QSettings::IniFormat);

    ui->comboBox_2->addItem(conf->value("astro/labels_amount").toString());
    ui->comboBox_3->addItem(conf->value("astro/nebula_hints_amount").toString());

    ui->comboBox_2->addItem("0.5");
    ui->comboBox_2->addItem("1");
    ui->comboBox_2->addItem("1.5");
    ui->comboBox_2->addItem("2");
    ui->comboBox_2->addItem("2.5");
    ui->comboBox_2->addItem("3");
    ui->comboBox_2->addItem("3.5");
    ui->comboBox_2->addItem("4");
    ui->comboBox_2->addItem("4.5");
    ui->comboBox_2->addItem("5");
    ui->comboBox_2->addItem("5.5");
    ui->comboBox_2->addItem("6");
    ui->comboBox_2->addItem("6.5");
    ui->comboBox_2->addItem("7");
    ui->comboBox_2->addItem("7.5");
    ui->comboBox_2->addItem("8");
    ui->comboBox_2->addItem("8.5");
    ui->comboBox_2->addItem("9");
    ui->comboBox_2->addItem("9.5");
    ui->comboBox_2->addItem("10");

    ui->comboBox_3->addItem("0.5");
    ui->comboBox_3->addItem("1");
    ui->comboBox_3->addItem("1.5");
    ui->comboBox_3->addItem("2");
    ui->comboBox_3->addItem("2.5");
    ui->comboBox_3->addItem("3");
    ui->comboBox_3->addItem("3.5");
    ui->comboBox_3->addItem("4");
    ui->comboBox_3->addItem("4.5");
    ui->comboBox_3->addItem("5");
    ui->comboBox_3->addItem("5.5");
    ui->comboBox_3->addItem("6");
    ui->comboBox_3->addItem("6.5");
    ui->comboBox_3->addItem("7");
    ui->comboBox_3->addItem("7.5");
    ui->comboBox_3->addItem("8");
    ui->comboBox_3->addItem("8.5");
    ui->comboBox_3->addItem("9");
    ui->comboBox_3->addItem("9.5");
    ui->comboBox_3->addItem("10");


//    QFile loadFile(cuserDir+"/data.json");
    QFile file(cuserDir+"/data.json");
    qDebug()<<"data.json"<<endl;
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
	qDebug()<<"Can't open the file!"<<endl;
    }
    ui->comboBox->clear();
    while(!file.atEnd())
    {
        QByteArray line = file.readLine().replace("\n","");
        QString str(line);
        QString Name1 = str.split("\t")[0];
        QString Name2 = str.split("\t")[1];
        qDebug()<< Name1<<":"<<Name2;
        ui->comboBox->addItem(Name1);
        map[Name1] = Name2;

        if (Name2 +".dat" == conf->value("astro/nebula_data_file").toString()){
            ui->textBrowser->append("Now the data file: "+Name1+"\n");
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::selector(QString filename,QString namefile){

	conf->setValue("astro/nebula_data_file", filename);
	conf->setValue("astro/nebula_name_file", namefile);
	ui->textBrowser->append("set to "+filename+"\r\n");

	if (ui->comboBox_2->currentText()!=conf->value("astro/labels_amount").toString()){
		conf->setValue("astro/labels_amount",ui->comboBox_2->currentText());
	}
	if (ui->comboBox_3->currentText()!=conf->value("astro/nebula_hints_amount").toString()){
		conf->setValue("astro/nebula_hints_amount",ui->comboBox_3->currentText());
	}

//	QFile file1(cuserDir+"/nebulae/default/"+filename);
//	QFile filex1(cuserDir+"/nebulae/default/ngc2000.dat");

//	QFile file2(cuserDir+"/nebulae/default/"+namefile);
//	QFile filex2(cuserDir+"/nebulae/default/ngc2000names.dat");

	QFile::remove(cuserDir+"/nebulae/default/ngc2000.dat");
	QFile::remove(cuserDir+"/nebulae/default/ngc2000names.dat");
	if(QFile::copy(cuserDir+"/nebulae/default/"+filename, cuserDir+"/nebulae/default/ngc2000.dat")){

		qDebug()<<"Copyed filename! "<< endl;
	}
	else{
		qDebug()<<"Can't open the file1! "<< endl;
	}
	if(QFile::copy(cuserDir+"/nebulae/default/"+namefile, cuserDir+"/nebulae/default/ngc2000names.dat")){
		qDebug()<<"Copyed namefile! "<< endl;
	}
	else{
		qDebug()<<"Can't open the file2! "<< endl;
	}


//	if(!file1.open(QIODevice::ReadOnly))
//	{
//	    qDebug()<<"Can't open the file1! "<<file1<< endl;
//	}
//	else if(!filex1.open(QIODevice::WriteOnly)){
//	    qDebug()<<"Can't open the file1x! "<<filex1<< endl;
//	}
//	else{
//		filex1.write(file1.readAll());
//		filex1.close();
//		file1.close();
//		qDebug()<<"Fin file2! "<<file1<< endl;
//	}

//	if(!file2.open(QIODevice::ReadOnly))
//	{
//	    qDebug()<<"Can't open the file2! "<<file1<<endl;
//	}
//	else if(!filex2.open(QIODevice::WriteOnly)){
//	    qDebug()<<"Can't open the file2x! "<<filex2<< endl;
//	}
//	else{
//		filex2.write(file2.readAll());
//		filex2.close();
//		file2.close();
//		qDebug()<<"Fin file2!"<<endl;
//	}
}

void MainWindow::on_pushButton_clicked()
{
	QString filename = ui->comboBox->currentText();
	filename = map[filename];
	if(!ui->radioButton->isChecked())
		filename += "_";
	QString namefile = filename;
	filename += ".dat";
	namefile += "names.dat";
	selector(filename,namefile);


}


void MainWindow::on_pushButton_2_clicked()
{

}
