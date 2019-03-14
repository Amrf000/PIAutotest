#ifndef WIDGETCONTAINER_H
#define WIDGETCONTAINER_H

#include <QWidget>
#include <Windows.h>
#include <TlHelp32.h>
#include <QProcess>

class MainWindow;
class WidgetContainer : public QWidget
{
    Q_OBJECT
    struct handle_data {
        DWORD processId;
        HWND wndHandle;
    };
    //handle_data m_data;
    //QProcess* m_process;
public:
    explicit WidgetContainer(QWidget *parent = nullptr);
    static QWidget *createContainer(MainWindow* pm,const QString &iAppName, bool iAutoOpen = true);
    static DWORD findProcessId(const QString &iAppName);
    static DWORD startUpProcess(const QString &iAppName);
    static HANDLE findWindowHandle(const QString & iClassName, const QString &iWindowName);
signals:

public slots:
};

#endif // WIDGETCONTAINER_H
