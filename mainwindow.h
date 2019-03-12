#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "xlsxdocument.h"
#include "instconfig.h"

namespace Ui {
class MainWindow;
}

class WorkerThread;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();
    void loadInI();
    void loadParam();
    void SaveInI();
    void SaveParam();
    void init();

private slots:
//    void on_start_toggled(bool checked);

signals:
    void append(QString s);

private:
    Ui::MainWindow *ui;
public:
    static bool isRunnig;
    std::map<QString,InstConfig> m_conf;
    QVariantMap m_config;
    QStringList m_vecs;
    WorkerThread* pWorker;
public:
    QString Keysight_DSO254A_IP;//"10.121.92.193"
    QString Oscpath;//"\\10.121.92.193\z00377089"
    QString Localpath;//"E:\z00377089\Orlando\Auto_test_data\PI_test_Maxpower"
    QString PicName;
    QString password;
    QString inst_manufacturer;
    QString inst_model;
private slots:
    void on_lineEdit_local_pic_path_textChanged(const QString &arg1);
    void on_lineEdit_pic_share_path_textEdited(const QString &arg1);
    void on_lineEdit_pic_name_textEdited(const QString &arg1);
    void on_lineEdit_password_textEdited(const QString &arg1);
    void on_comboBox_inst_model_currentIndexChanged(const QString &arg1);
    void on_lineEdit_inst_manufacturer_textEdited(const QString &arg1);
    void on_lineEdit_inst_ip_textEdited(const QString &arg1);
    void on_comboBox_vec_currentIndexChanged(const QString &arg1);
    void on_pushButton_select_local_path_2_clicked();
    void on_pushButton_open_local_path_2_clicked();
    void on_start_toggled(bool checked);
};

#endif // MAINWINDOW_H
