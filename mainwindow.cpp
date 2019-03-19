#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDir>
#include <QFileDialog>
#include <string>
#include <string.h>
#include <errno.h>
#include <QMessageBox>
#include <QDebug>
#include <QDesktopServices>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariantMap>
#include "workerthread.h"
#include "instrumentwidget.h"
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),pWorker(NULL),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Keysight_DSO254A_IP="";//"10.121.92.193"
    Oscpath="";//"\\10.121.92.193\z00377089"
    Localpath="";//"E:\z00377089\Orlando\Auto_test_data\PI_test_Maxpower"
    PicName="";
    password="";
    inst_manufacturer="";
    inst_model="";
}

bool MainWindow::isRunnig = false;

MainWindow::~MainWindow()
{
    delete ui;
    //reSave
    SaveInI();
    SaveParam();
    if(pWorker)
       pWorker->stop();
}

void MainWindow::SaveInI()
{
     QString file_path = QCoreApplication::applicationDirPath()+"/config.ini";
     auto json_doc = QJsonDocument::fromVariant(m_config);
     QString qString(json_doc.toJson());
     QFile file(file_path);
     if (file.open(QIODevice::WriteOnly)) {
       QTextStream out(&file);
       out << qString;
       file.close();
     }
}

void MainWindow::SaveParam()
{
    QString file_path = QCoreApplication::applicationDirPath()+"/param.xlsx";//QDir::currentPath()
    QXlsx::Document xlsx(file_path);
    //int number_of_row = xlsx.dimension().lastRow();
    //int number_of_column = xlsx.dimension().lastColumn();
    int row = 2;
    for(std::map<QString,InstConfig>::iterator it=m_conf.begin();it!=m_conf.end();++it){
        InstConfig& cf = it->second;
        xlsx.write(row,1,cf.suiteName);
        xlsx.write(row,2,cf.Slect_Model);
        xlsx.write(row,3,cf.Vec_time);
        xlsx.write(row,4,cf.HorizontalPosition);
        xlsx.write(row,5,cf.HorizontalScale);
        xlsx.write(row,6,cf.Trigger_CHANnel);
        xlsx.write(row,7,cf.Trigger_Model);
        xlsx.write(row,8,cf.Trigger_Edge);
        xlsx.write(row,9,cf.Trigger_Level);
        xlsx.write(row,10,cf.CHANnel1_Status);
        xlsx.write(row,11,cf.CHANnel1_Name);
        xlsx.write(row,12,cf.Channel1_Scale);
        xlsx.write(row,13,cf.CHANnel1_Coupling);
        xlsx.write(row,14,cf.channel1_bandwidth);
        xlsx.write(row,15,cf.channel1_Offset);
        xlsx.write(row,16,cf.ch1_measure_type1);
        xlsx.write(row,17,cf.ch1_measure_type2);
        xlsx.write(row,18,cf.ch1_measure_type3);
        xlsx.write(row,19,cf.ch1_measure_type4);
        xlsx.write(row,20,cf.ch1_measure_hold);
        xlsx.write(row,21,cf.CHANnel2_Status);
        xlsx.write(row,22,cf.CHANnel2_Name);
        xlsx.write(row,23,cf.Channel2_Scale);
        xlsx.write(row,24,cf.CHANnel2_Coupling);
        xlsx.write(row,25,cf.channel2_bandwidth);
        xlsx.write(row,26,cf.channel2_Offset);
        xlsx.write(row,27,cf.ch2_measure_type1);
        xlsx.write(row,28,cf.ch2_measure_type2);
        xlsx.write(row,29,cf.ch2_measure_type3);
        xlsx.write(row,30,cf.ch2_measure_type4);
        xlsx.write(row,31,cf.ch2_measure_hold);
        xlsx.write(row,32,cf.CHANnel3_Status);
        xlsx.write(row,33,cf.CHANnel3_Name);
        xlsx.write(row,34,cf.Channel3_Scale);
        xlsx.write(row,35,cf.CHANnel3_Coupling);
        xlsx.write(row,36,cf.channel3_bandwidth);
        xlsx.write(row,37,cf.channel3_Offset);
        xlsx.write(row,38,cf.ch3_measure_type1);
        xlsx.write(row,39,cf.ch3_measure_type2);
        xlsx.write(row,40,cf.ch3_measure_type3);
        xlsx.write(row,41,cf.ch3_measure_type4);
        xlsx.write(row,42,cf.ch3_measure_hold);
        xlsx.write(row,43,cf.CHANnel4_Status);
        xlsx.write(row,44,cf.CHANnel4_Name);
        xlsx.write(row,45,cf.Channel4_Scale);
        xlsx.write(row,46,cf.CHANnel4_Coupling);
        xlsx.write(row,47,cf.channel4_bandwidth);
        xlsx.write(row,48,cf.channel4_Offset);
        xlsx.write(row,49,cf.ch4_measure_type1);
        xlsx.write(row,50,cf.ch4_measure_type2);
        xlsx.write(row,51,cf.ch4_measure_type3);
        xlsx.write(row,52,cf.ch4_measure_type4);
        xlsx.write(row,53,cf.ch4_measure_hold);
        xlsx.write(row,54,cf.Profile);
        row++;
    }
    bool flag = xlsx.save();
    while(!flag){
        QMessageBox::question(this, "提示", "请确认已经关闭param.xlxs",QMessageBox::Ok);
        flag = xlsx.save();
    }
}

void MainWindow::loadInI()
{
    QString file_path = QCoreApplication::applicationDirPath()+"/config.ini";//QDir::currentPath()
    QFile file_obj(file_path);
    if(!file_obj.open(QIODevice::ReadOnly)){
        qDebug()<<"Failed to open "<<file_path;
    }

    QTextStream file_text(&file_obj);
    QString json_string;
    json_string = file_text.readAll();
    file_obj.close();
    QByteArray json_bytes = json_string.toLocal8Bit();
    auto json_doc=QJsonDocument::fromJson(json_bytes);

    if(json_doc.isNull()){
        qDebug()<<"Failed to create JSON doc.";
    }
    if(!json_doc.isObject()){
        qDebug()<<"JSON is not an object.";
    }

    QJsonObject json_obj=json_doc.object();

    if(json_obj.isEmpty()){
        qDebug()<<"JSON object is empty.";
    }

    m_config = json_obj.toVariantMap();
}

void MainWindow::loadParam()
{
    QString dicPath = QCoreApplication::applicationDirPath()+"/Orlando_AutoTest/vector";
    QDir vecDir(dicPath);
    m_vecs = vecDir.entryList(QDir::Files, QDir::Name);//QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | //QDir::SortFlag::NoSort
}


void MainWindow::init()
{
    ui->widget_vnc->createContainer(this,"vncviewer.exe");
    connect(this, SIGNAL(append(QString)),ui->textEdit, SLOT(append(QString)));
    //connect(ui->widget,SIGNAL(),this,SLOT(onVecChange(QString)));

     QString file_path = QCoreApplication::applicationDirPath()+"/param.xlsx";//QDir::currentPath()
     QXlsx::Document xlsx(file_path);
     QString A0 = xlsx.read(1,1).toString();
     if(0!=QString::compare(A0,"vector_suite",Qt::CaseInsensitive))
     {
         //格式异常 新建文件
     }
     //std::cout<<A0.toLocal8Bit().data()<<std::endl;
     //vector_suite
     int number_of_row = xlsx.dimension().lastRow();
     //int number_of_column = xlsx.dimension().lastColumn();
     for (int row = 2; row <= number_of_row; ++row) {
         InstConfig c;
         //for(int column = 1; column < number_of_column ; ++column) {
         c.suiteName = xlsx.cellAt(row, 1)->value().toString();
         c.Slect_Model = xlsx.cellAt(row,2)->value().toString();
         c.Vec_time = xlsx.cellAt(row,3)->value().toUInt();

         c.HorizontalPosition = xlsx.cellAt(row,4)->value().toDouble();
         c.HorizontalScale = xlsx.cellAt(row,5)->value().toDouble();
         c.Trigger_CHANnel = xlsx.cellAt(row,6)->value().toString();
         c.Trigger_Model = xlsx.cellAt(row,7)->value().toString();
         c.Trigger_Edge = xlsx.cellAt(row,8)->value().toString();
         c.Trigger_Level = xlsx.cellAt(row,9)->value().toInt();

         c.CHANnel1_Status = (0==QString::compare(xlsx.cellAt(row,10)->value().toString(),"TRUE",Qt::CaseInsensitive));
         c.CHANnel1_Name = xlsx.cellAt(row,11)->value().toString();
         c.Channel1_Scale = xlsx.cellAt(row,12)->value().toString();
         c.CHANnel1_Coupling = xlsx.cellAt(row,13)->value().toString();
         c.channel1_bandwidth = xlsx.cellAt(row,14)->value().toString();
         c.channel1_Offset = xlsx.cellAt(row,15)->value().toString();
         c.ch1_measure_type1 = xlsx.cellAt(row,16)->value().toString();
         c.ch1_measure_type2 = xlsx.cellAt(row,17)->value().toString();
         c.ch1_measure_type3 = xlsx.cellAt(row,18)->value().toString();
         c.ch1_measure_type4 = xlsx.cellAt(row,19)->value().toString();
         c.ch1_measure_hold = (0==QString::compare(xlsx.cellAt(row,20)->value().toString(),"TRUE",Qt::CaseInsensitive));

         c.CHANnel2_Status = (0==QString::compare(xlsx.cellAt(row,21)->value().toString(),"TRUE",Qt::CaseInsensitive));
         c.CHANnel2_Name = xlsx.cellAt(row,22)->value().toString();
         c.Channel2_Scale = xlsx.cellAt(row,23)->value().toString();
         c.CHANnel2_Coupling = xlsx.cellAt(row,24)->value().toString();
         c.channel2_bandwidth = xlsx.cellAt(row,25)->value().toString();
         c.channel2_Offset = xlsx.cellAt(row,26)->value().toString();
         c.ch2_measure_type1 = xlsx.cellAt(row,27)->value().toString();
         c.ch2_measure_type2 = xlsx.cellAt(row,28)->value().toString();
         c.ch2_measure_type3 = xlsx.cellAt(row,29)->value().toString();
         c.ch2_measure_type4 = xlsx.cellAt(row,30)->value().toString();
         c.ch2_measure_hold = (0==QString::compare(xlsx.cellAt(row,31)->value().toString(),"TRUE",Qt::CaseInsensitive));

         c.CHANnel3_Status = (0==QString::compare(xlsx.cellAt(row,32)->value().toString(),"TRUE",Qt::CaseInsensitive));
         c.CHANnel3_Name = xlsx.cellAt(row,33)->value().toString();
         c.Channel3_Scale = xlsx.cellAt(row,34)->value().toString();
         c.CHANnel3_Coupling = xlsx.cellAt(row,35)->value().toString();
         c.channel3_bandwidth = xlsx.cellAt(row,36)->value().toString();
         c.channel3_Offset = xlsx.cellAt(row,37)->value().toString();
         c.ch3_measure_type1 = xlsx.cellAt(row,38)->value().toString();
         c.ch3_measure_type2 = xlsx.cellAt(row,39)->value().toString();
         c.ch3_measure_type3 = xlsx.cellAt(row,40)->value().toString();
         c.ch3_measure_type4 = xlsx.cellAt(row,41)->value().toString();
         c.ch3_measure_hold = (0==QString::compare(xlsx.cellAt(row,42)->value().toString(),"TRUE",Qt::CaseInsensitive));

         c.CHANnel4_Status = (0==QString::compare(xlsx.cellAt(row,43)->value().toString(),"TRUE",Qt::CaseInsensitive));
         c.CHANnel4_Name = xlsx.cellAt(row,44)->value().toString();
         c.Channel4_Scale = xlsx.cellAt(row,45)->value().toString();
         c.CHANnel4_Coupling = xlsx.cellAt(row,46)->value().toString();
         c.channel4_bandwidth = xlsx.cellAt(row,47)->value().toString();
         c.channel4_Offset = xlsx.cellAt(row,48)->value().toString();
         c.ch4_measure_type1 = xlsx.cellAt(row,49)->value().toString();
         c.ch4_measure_type2 = xlsx.cellAt(row,50)->value().toString();
         c.ch4_measure_type3 = xlsx.cellAt(row,51)->value().toString();
         c.ch4_measure_type4 = xlsx.cellAt(row,52)->value().toString();
         c.ch4_measure_hold = (0==QString::compare(xlsx.cellAt(row,53)->value().toString(),"TRUE",Qt::CaseInsensitive));
         c.Profile = xlsx.cellAt(row,54)->value().toString();
         //}
         m_conf.insert(std::pair<QString,InstConfig>(c.suiteName,c));
     }

     loadInI();
     loadParam();
     this->Keysight_DSO254A_IP = m_config["Keysight_DSO254A_IP"].toString();
     this->Oscpath = m_config["Oscpath"].toString();
     this->Localpath = m_config["Localpath"].toString();
     this->PicName = m_config["PicName"].toString();
     this->inst_manufacturer = m_config["inst_manufacturer"].toString();
     this->inst_model = m_config["inst_model"].toString();
     this->password = m_config["password"].toString();
     QStringList inst_models = m_config["inst_model_options"].toStringList();
     bool oldState = ui->comboBox_inst_model->blockSignals(true);
     ui->comboBox_inst_model->clear();
     ui->comboBox_inst_model->addItems(inst_models);
     ui->comboBox_inst_model->blockSignals(oldState);

     ui->lineEdit_inst_ip->setText(this->Keysight_DSO254A_IP);
     ui->lineEdit_inst_manufacturer->setText(this->inst_manufacturer);
     ui->comboBox_inst_model->setCurrentText(this->inst_model);
     ui->lineEdit_password->setText(this->password);
     oldState = ui->lineEdit_local_pic_path->blockSignals(true);
     ui->lineEdit_local_pic_path->setText(this->Localpath);
     ui->lineEdit_local_pic_path->blockSignals(oldState);
     ui->lineEdit_pic_share_path->setText(this->Oscpath);
     ui->lineEdit_pic_name->setText(this->PicName);

     oldState = ui->comboBox_vec->blockSignals(true);
     ui->comboBox_vec->clear();
     ui->comboBox_vec->addItems(m_vecs);
     ui->comboBox_vec->blockSignals(oldState);

     const std::map<QString,InstConfig>::iterator it = m_conf.find(m_config["currentSuite"].toString());
     if(m_config["currentSuite"].toString().size()>1 && it!=m_conf.end())
     {
        ui->comboBox_vec->setCurrentText(m_config["currentSuite"].toString());
        ui->widget->setConfig(m_vecs,m_config,&it->second);
     }else{
         bool flag=false;
         for(std::map<QString,InstConfig>::iterator it1=m_conf.begin();it1!=m_conf.end();++it1) {
             if(QString::compare(it1->first,"default",Qt::CaseInsensitive))
             {
                 m_config["currentSuite"] = it1->first;
                 ui->comboBox_vec->setCurrentText(it1->first);
                 ui->widget->setConfig(m_vecs,m_config,&it1->second);
                 flag=true;
                 break;
             }
         }
         if(!flag)
         {
             qDebug("error currentSuite");
         }
     }
}

void MainWindow::on_lineEdit_local_pic_path_textChanged(const QString &val)
{
   Localpath = val;
}

void MainWindow::on_lineEdit_pic_share_path_textEdited(const QString &val)
{
    Oscpath = val;
}

void MainWindow::on_lineEdit_pic_name_textEdited(const QString &val)
{
     PicName = val;
}

void MainWindow::on_lineEdit_password_textEdited(const QString &val)
{
    password = val;
}

void MainWindow::on_comboBox_inst_model_currentIndexChanged(const QString &val)
{
    inst_model = val;
}

void MainWindow::on_lineEdit_inst_manufacturer_textEdited(const QString &val)
{
    inst_manufacturer = val;
}

void MainWindow::on_lineEdit_inst_ip_textEdited(const QString &val)
{
    Keysight_DSO254A_IP = val;
}

void MainWindow::on_comboBox_vec_currentIndexChanged(const QString &val)
{
    if(val.size()>1)
    {
      m_config["currentSuite"] = val;
      const std::map<QString,InstConfig>::iterator it = m_conf.find(m_config["currentSuite"].toString());
      if(it!=m_conf.end()){
          ui->widget->setConfig(m_vecs,m_config,&it->second);
      }else{
          InstConfig c(val,m_conf["default"]);
          m_conf.insert(std::pair<QString,InstConfig>(val,c));
          ui->widget->setConfig(m_vecs,m_config,&m_conf[val]);
      }
    }
}

void MainWindow::on_pushButton_select_local_path_2_clicked()
{
    if(!QDir(Localpath).exists()){
        QDir().mkpath(Localpath);
    }
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setFileMode(QFileDialog::Directory);
    fileDialog->setWindowTitle("选择本地路径");
    fileDialog->setDirectory(QDir(Localpath));
    if(fileDialog->exec() == QDialog::Accepted) {
       QString path = fileDialog->selectedFiles()[0];
       ui->lineEdit_local_pic_path->setText(path);
    }
}

void MainWindow::on_pushButton_open_local_path_2_clicked()
{
    if(!QDir(Localpath).exists()){
        QDir().mkpath(Localpath);
    }
    QDesktopServices::openUrl( QUrl::fromLocalFile(Localpath) );
}

void MainWindow::on_start_toggled(bool running)
{
    //printf("running=%d\n",(int)running);
    if(running){//启动
        if(MainWindow::isRunnig){
            return;
        }
        if(pWorker!=NULL)
        {
            return;
        }
        const std::map<QString,InstConfig>::iterator it = m_conf.find(m_config["currentSuite"].toString());
        if(it == m_conf.end())
        {
            return;
        }
        //MainWindow::isRunnig = true;
        ui->start->setText("stop");
        pWorker = new WorkerThread(this,ui->start,it->second);
        connect( pWorker, &WorkerThread::finished,  pWorker, &QObject::deleteLater);
        pWorker->start();
    }else{//停止
        if(MainWindow::isRunnig){//还在运行
           ui->start->setChecked(true);
           ui->start->setDisabled(true);
           pWorker->stop();
        }else{
            pWorker = NULL;
            ui->start->setText("start");
            ui->start->setDisabled(false);
        }
    }
}


