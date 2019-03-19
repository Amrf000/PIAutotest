#ifndef WIDGETCONTAINER_H
#define WIDGETCONTAINER_H
#ifndef _AFXDLL
#define _AFXDLL
#endif
#include <afxwin.h>
#include <QWidget>
//#include <Windows.h>
#include <TlHelp32.h>
#include <QProcess>
#include <QGridLayout>
#include <QWindow>
//#include <QSharedMemory>
#include "server.h"

class MainWindow;
class QLocalServer;
class QLocalSocket;
class WidgetContainer : public QWidget
{
    Q_OBJECT
    struct handle_data {
        DWORD processId;
        HWND wndHandle;
    };
    QString m_appName;
    handle_data m_data;
    QProcess* m_process;
    QWidget * m_widget;
    QWindow *m_connectDlg;

    QWidget * m_vncwidget;
    QWindow * m_vncDlg;
    QGridLayout *m_layout;
    Server* pServer;
public:
    explicit WidgetContainer(QWidget* parent=nullptr);
    virtual ~WidgetContainer();
    QWidget *createContainer(MainWindow* pm,const QString &iAppName, bool iAutoOpen = true);
    static DWORD findProcessId(const QString &iAppName);
    //static DWORD startUpProcess(const QString &iAppName);
    static HANDLE findWindowHandle(const QString & iClassName, const QString &iWindowName);
signals:

public slots:
    void onConnectDlgVisibleChanged(bool arg);
    void onConnectDlgFinished(int flag);
    void onConnectDlgVisibilityChanged(QWindow::Visibility visibility);
    void onConnectDlgWindowStateChanged(Qt::WindowState windowState);
    void onConnectDlgWidthChanged(int arg);
    void onConnectDlgdestroyed(QObject *obj = Q_NULLPTR);
    void onCDesktopWinCreate(long hwnd);
    void onViewerCreate(long hwnd);
    void onConnectionCreate(long hwnd);
    void onAuthenticationCreate(long hwnd);
    void onVisChanged(QWindow::Visibility flag);
    void onCDesktopWinShow(long hwnd);
    void onViewerShow(long hwnd);
    void onactiveChanged();
protected:
    void mousePressEvent(QMouseEvent *event);
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

};

#endif // WIDGETCONTAINER_H
