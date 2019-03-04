#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init();

private slots:
    void on_start_toggled(bool checked);

signals:
    void append(QString s);


private:
    Ui::MainWindow *ui;
public:
    static bool isRunnig;
};

#endif // MAINWINDOW_H
