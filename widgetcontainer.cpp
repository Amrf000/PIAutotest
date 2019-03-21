#include "widgetcontainer.h"
//#include <Windows.h>
#include <QApplication>
#include <QCoreApplication>
#include <string>
#include <functional>
#include <iostream>
#include <fstream>
#include <QDebug>
#include <QProcess>
#include <QBuffer>
#include <QLocalServer>
#include <QLocalSocket>
#include "mainwindow.h"
#include "injection.h"
#include "server.h"

class CloseEventFilter : public QObject
{
//     Q_OBJECT
    char _flag[8];
public:
     CloseEventFilter(QObject *parent,const char* flag) : QObject(parent) {
         strncpy(_flag,flag,7);
     }

protected:
     bool eventFilter(QObject *obj, QEvent *event)
     {
         //QEvent::Close;
          /*if (event->type() == QEvent::Close)
          {
               // Do something interesting, emit a signal for instance.
              printf("QEvent::Close");
          }else if(event->type() == QEvent::Hide){
              printf("QEvent::Hide");
          }else if(event->type() == QEvent::Resize){
              printf("QEvent::Resize");
          }else{
              printf("e=%d\n",(int)event->type());
          }*/
           printf("%s ev=%d\n",_flag,(int)event->type());
          return QObject::eventFilter(obj, event);
     }

};

WidgetContainer::WidgetContainer(QWidget* parent):QWidget(parent),pServer(NULL)
{
    m_data.processId=0;
    m_data.wndHandle=NULL;
    m_process =NULL;
    m_widget=NULL;
    m_vncwidget=NULL;
    m_connectDlg=NULL;
    m_vncDlg=NULL;
    m_layout=NULL;
    m_appName ="";

    //QTimer *timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(onactiveChanged()));
    //timer->start(2000);
    //this->setAttribute(Qt::WA_ShowWithoutActivating);
    //this->setFocusPolicy(Qt::NoFocus);
}

WidgetContainer::~WidgetContainer()
{
    if(pServer)
        pServer->stop();
}

void WidgetContainer::onactiveChanged()
{
    printf("vncDlg active change\n");
    //if(m_vncDlg && IsWindowVisible((HWND)m_vncDlg->winId()))
    //{
       //SetActiveWindow((HWND)m_vncDlg->winId());
       //Sleep(1000);
       //SetFocus((HWND)m_vncDlg->winId());
       //SetForegroundWindow((HWND)m_vncDlg->winId());
    //}
}

void ShowInTaskbar(HWND hWnd, BOOL bshow)
{
    HRESULT hr;
    ITaskbarList *pTaskbarList;

    hr = CoCreateInstance(CLSID_TaskbarList,NULL,CLSCTX_INPROC_SERVER, IID_ITaskbarList,(void**)&pTaskbarList);

    if ( hr == S_OK )
    {
        hr = pTaskbarList->HrInit();

        if ( hr == S_OK )
        {
            if(bshow)
            {
                pTaskbarList->AddTab(hWnd);
            }
            else
            {
                pTaskbarList->DeleteTab(hWnd);
            }
        }

        pTaskbarList->Release();
    }
}


void WidgetContainer::onCDesktopWinCreate(long hwnd)
{
    if(m_vncDlg==NULL)
    {
        m_vncDlg = QWindow::fromWinId((WId) hwnd);
        //m_vncDlg->setMouseGrabEnabled(true);
        //m_vncDlg->setKeyboardGrabEnabled(true);
        this->m_vncwidget = QWidget::createWindowContainer(m_vncDlg,this);
        //connect(m_vncDlg,SIGNAL(activeChanged()),this,SLOT(onactiveChanged()));
        ShowInTaskbar((HWND)hwnd,false);
        if(m_layout==NULL)
           m_layout=new QGridLayout(this);
        m_layout->addWidget(this->m_vncwidget,0,0);
        //this->m_vncwidget->setFocusPolicy(Qt::NoFocus);
        //printf("flag=%d\n",(long)m_vncDlg->flags());
        //connect(m_vncDlg,SIGNAL(visibilityChanged(QWindow::Visibility)),this,SLOT(onVisChanged(QWindow::Visibility)));
        //CloseEventFilter *closeFilter = new CloseEventFilter(m_vncwidget,"vnc");
        //m_vncwidget->installEventFilter(closeFilter);
        //m_vncwidget->setGeometry(this->geometry());
    }
}
void WidgetContainer::onCDesktopWinShow(long hwnd)
{
    emit m_vncDlg->setGeometry(this->geometry());
    //m_connectDlg->hide();bmark not use
    m_widget->hide();//bmark

    //this->m_widget->hide();

    //m_vncDlg->requestActivate();
    //m_vncDlg->setKeyboardGrabEnabled(true);
    //m_vncDlg->setMouseGrabEnabled(true);
    //QTimer::singleShot(2000,this, [this]{
    //     SetActiveWindow((HWND)m_vncDlg->winId());

    //});
     //QTimer::singleShot(3000,this, [this]{
     //   SetFocus((HWND)m_vncDlg->winId());
     //});
    //QApplication::setActiveWindow(this->m_vncDlg);
}

void WidgetContainer::onViewerShow(long hwnd)
{
    //ShowInTaskbar((HWND)hwnd,false);
}

void WidgetContainer::mousePressEvent(QMouseEvent *event)
{
    printf("focusInEvent\n");
}

void WidgetContainer::focusInEvent(QFocusEvent *event)
{
    printf("focusInEvent\n");
}
void WidgetContainer::focusOutEvent(QFocusEvent *event)
{
    printf("focusOutEvent\n");
}
bool WidgetContainer::nativeEvent(const QByteArray &eventType, void *message, long *result)//bmark
{
    printf("nativeEvent\n");
    return QWidget::nativeEvent(eventType,message,result);

}

void WidgetContainer::onVisChanged(QWindow::Visibility flag)
{
     printf("onVisChanged=%d\n",flag);

}

void WidgetContainer::onViewerCreate(long hwnd)
{
    if(m_connectDlg ==NULL)
    {
        m_connectDlg = QWindow::fromWinId((WId) hwnd);
        //m_connectDlg->setMouseGrabEnabled(true);
        //m_connectDlg->setKeyboardGrabEnabled(true);
        this->m_widget = QWidget::createWindowContainer(m_connectDlg,this);
        //ShowInTaskbar((HWND)hwnd,false);
        ShowInTaskbar((HWND)m_widget->winId(),false);//bmark
        //SetFocus(hwnd);
        //ShowInTaskbar((HWND)hwnd,false);this->m_widget->setFocusPolicy(Qt::NoFocus);
        if(m_layout==NULL)
           m_layout=new QGridLayout(this);
        m_layout->addWidget(this->m_widget,0,0);

        //QTimer::singleShot(100,this, [hwnd]{
         //      ShowInTaskbar((HWND)hwnd,false);;
        //});

        //ShowInTaskBar((HWND)this->m_widget->winId(),false);
        //CWnd *pCWnd=CWnd::FromHandle((HWND)this->m_widget->winId());
        //pCWnd->ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
        //CloseEventFilter *closeFilter = new CloseEventFilter(m_widget,"view");
        //m_widget->installEventFilter(closeFilter);
    }
}
void WidgetContainer::onConnectionCreate(long hwnd)
{

}
void WidgetContainer::onAuthenticationCreate(long hwnd)
{

}

QWidget *WidgetContainer::createContainer(MainWindow* pm,const QString &iAppName, bool iAutoOpen)
{
    //CloseEventFilter *closeFilter = new CloseEventFilter(this,"this");
    //this->installEventFilter(closeFilter);

    pServer = new Server;
    connect(pServer,SIGNAL(onCDesktopWinCreate(long)),this,SLOT(onCDesktopWinCreate(long)));
    connect(pServer,SIGNAL(onViewerCreate(long)),this,SLOT(onViewerCreate(long)));
    connect(pServer,SIGNAL(onConnectionCreate(long)),this,SLOT(onConnectionCreate(long)));
    connect(pServer,SIGNAL(onAuthenticationCreate(long)),this,SLOT(onAuthenticationCreate(long)));
    connect(pServer,SIGNAL(onCDesktopWinShow(long)),this,SLOT(onCDesktopWinShow(long)));
    connect(pServer,SIGNAL(onViewerShow(long)),this,SLOT(onViewerShow(long)));
    pServer->start();
    m_appName = iAppName;
    //HookInjection(const_cast<TCHAR*>(iAppName.toStdWString().c_str()),const_cast<TCHAR*>((QString("Vncdll.dll").toStdWString().c_str())));

    QProcess* process = new QProcess;//(pm);
    QString wordPath = QCoreApplication::applicationDirPath()+"/"+iAppName;
    process->start(wordPath);
    connect(process,SIGNAL(finished(int)),this,SLOT(onConnectDlgFinished(int)));
    if(true==process->waitForStarted(50000))
    {
        //Sleep(40*1000);
        //Dll_Injection(const_cast<TCHAR*>((QString("Vncdll.dll").toStdWString().c_str())),const_cast<TCHAR*>(iAppName.toStdWString().c_str()));


        /*handle_data data;
        data.processId = process->processId();//procId;
        data.wndHandle = nullptr;
        while(data.wndHandle == nullptr)
        {
            EnumWindows([](HWND wnd, LPARAM lParam)->BOOL
            {
                handle_data &data = *(handle_data*)lParam;
                DWORD thProcId = 0;
                GetWindowThreadProcessId(wnd, &thProcId);
                //check if it is mainwindow
                BOOL isMainWnd = GetWindow(wnd, GW_OWNER) == nullptr && IsWindowVisible(wnd);

                if (thProcId == data.processId && isMainWnd) {
                    TCHAR szText[64] = { 0 };
                    GetClassName(wnd,szText,63);
                    printf("mainclass=%ls\n",szText);
                    data.wndHandle = wnd;
                    return -1;
                }
                return 1;
            }
            , (LPARAM)&data);
            Sleep(50);
        }
        if (nullptr == data.wndHandle) {
            return nullptr;
        }

       m_connectDlg = QWindow::fromWinId((WId) data.wndHandle);

       CloseEventFilter *closeFilter = new CloseEventFilter(m_connectDlg);
       m_connectDlg->installEventFilter(closeFilter);

        if (nullptr != m_connectDlg) {
            connect(m_connectDlg,SIGNAL(visibleChanged(bool)),this,SLOT(onConnectDlgVisibleChanged(bool)));
            connect(m_connectDlg,SIGNAL(visibilityChanged(QWindow::Visibility)),this,SLOT(onConnectDlgVisibilityChanged(QWindow::Visibility)));
            connect(m_connectDlg,SIGNAL(windowStateChanged(Qt::WindowState)),this,SLOT(onConnectDlgWindowStateChanged(Qt::WindowState)));
            connect(m_connectDlg,SIGNAL(widthChanged(int)),this,SLOT(onConnectDlgWidthChanged(int)));
            connect(m_connectDlg,SIGNAL(destroyed(QObject*)),this,SLOT(onConnectDlgdestroyed(QObject*)));

            //CloseEventFilter *closeFilter1 = new CloseEventFilter(this->m_widget);
            //this->m_widget->installEventFilter(closeFilter1);

            this->m_data = data;
            this->m_process = process;
            return m_widget;
        }
        return nullptr;*/
    }
    return nullptr;
}
void WidgetContainer::onConnectDlgdestroyed(QObject *obj )
{
    printf("onConnectDlgdestroyed\n");

}
void WidgetContainer::onConnectDlgWidthChanged(int arg)
{
     printf("onConnectDlgWidthChanged w=%d\n",arg);
}
void WidgetContainer::onConnectDlgWindowStateChanged(Qt::WindowState windowState)
{
   printf("ConnectDlgWindowStateChanged=%d\n",(int)windowState);
}
void WidgetContainer::onConnectDlgVisibilityChanged(QWindow::Visibility visibility)
{
    QString vstr="";
    switch(visibility)
    {
       case QWindow::Windowed:
        vstr="QWindow::Windowed\n";
        break;
    case QWindow::Minimized:
     vstr="QWindow::Minimized\n";
     break;
    case QWindow::Maximized:
     vstr="QWindow::Maximized\n";
     break;
 case QWindow::FullScreen:
  vstr="QWindow::FullScreen\n";
  break;
    case QWindow::AutomaticVisibility:
     vstr="QWindow::AutomaticVisibility\n";
     break;
 case QWindow::Hidden:
  vstr="QWindow::Hidden\n";
  break;

    }
    printf("ConnectDlgVisibilityChanged=%s\n",vstr.toLocal8Bit().data());

}

void WidgetContainer::onConnectDlgVisibleChanged(bool arg)
{
    printf("ConnectDlgVisibleChanged=%s\n",arg?"true":"false");
    if(arg==false && this->parentWidget()->isVisible()){
        /*handle_data data;
        data.processId = m_data.processId;//procId;
        data.wndHandle = nullptr;
        while(data.wndHandle == nullptr)
        {
            EnumWindows([](HWND wnd, LPARAM lParam)->BOOL
            {
                handle_data &data = *(handle_data*)lParam;
                DWORD thProcId = 0;
                GetWindowThreadProcessId(wnd, &thProcId);
                //check if it is mainwindow
                BOOL isMainWnd = GetWindow(wnd, GW_OWNER) == nullptr && IsWindowVisible(wnd);

                if (thProcId == data.processId && isMainWnd) {
                    data.wndHandle = wnd;
                    return -1;
                }
                return 1;
            }
            , (LPARAM)&data);
            Sleep(50);
        }
        printf("new wnd is=%d\n",(int)data.wndHandle);
        m_vncDlg = QWindow::fromWinId((WId) data.wndHandle);
        if (nullptr != m_vncDlg) {
           this->m_vncwidget = QWidget::createWindowContainer(m_vncDlg);
           m_layout->addWidget(this->m_vncwidget,0,0);
        }*/
       /* DWORD procId = 0;
        while(procId == 0)
        {
           procId = (DWORD)findProcessId(m_appName);
           Sleep(50);
        }
        printf("findProcessId=%d\n",procId);
        handle_data data;
        data.processId = procId;//procId;
        data.wndHandle = nullptr;
        while(data.wndHandle == nullptr)
        {
            EnumWindows([](HWND wnd, LPARAM lParam)->BOOL
            {
                 handle_data &data = *(handle_data*)lParam;
                 DWORD thProcId = 0;
                 GetWindowThreadProcessId(wnd, &thProcId);
                        //check if it is mainwindow
                 BOOL isMainWnd = GetWindow(wnd, GW_OWNER) == nullptr && IsWindowVisible(wnd);

                 if (thProcId == data.processId && isMainWnd) {
                      data.wndHandle = wnd;
                      return -1;
                 }
                 return 1;
            }
            , (LPARAM)&data);
            Sleep(50);
        }
        printf("GetWindow=%d\n",(int)data.wndHandle);
        QWindow *foreign = QWindow::fromWinId((WId) data.wndHandle);
        if (nullptr != foreign) {
            this->m_vncwidget = QWidget::createWindowContainer(foreign);
            m_layout->addWidget(this->m_vncwidget,0,0);
        }*/
    }
}

void WidgetContainer::onConnectDlgFinished(int flag)
{
    printf("onConnectDlgFinished=%s\n",flag?"true":"false");
}


DWORD WidgetContainer::findProcessId(const QString &iAppName)
{
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;

    // Take a snapshot of all processes in the system.
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return 0;
    }

    // Set the size of the structure before using it.
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Retrieve information about the first process,
    // and exit if unsuccessful
    if (!Process32First(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap);
        return 0;
    }

    // Now walk the snapshot of processes, and
    // find out the app's handle
    do {
//        hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pe32.th32ProcessID);
//        if (hProcess == nullptr) {
//            continue;
//        }
        if (std::wstring(pe32.szExeFile).find(iAppName.toStdWString()) != std::wstring::npos) {
//            CloseHandle(hProcess);
            return pe32.th32ProcessID;
        }
    } while (Process32Next(hProcessSnap, &pe32));

    return 0;
}

// DWORD WidgetContainer::startUpProcess(const QString &iAppName)
// {
//     /*std::wstring filePath = iAppName.toStdWString();
//     std::wifstream ifExists(filePath);
//     bool fileExists = true;
//     if (!ifExists.good()) {
//         fileExists = false;
//         QString path(std::getenv("path"));
//         for (const QString& onePath: path.split(";")) {
//             std::wifstream ifExists(QString(onePath + "/" + iAppName).toStdWString());
//             if (ifExists.good()) {
//                 filePath = QString(onePath + "/" + iAppName).toStdWString();
//                 fileExists = true;
//                 break;
//             }
//         }
//     }

//     if (!fileExists) {
//         return 0;
//     }*/

//     QString filePath = QCoreApplication::applicationDirPath()+"/"+iAppName;

//     // additional information
//     STARTUPINFO si;
//     PROCESS_INFORMATION pi;

//     // set the size of the structures
//     ZeroMemory( &si, sizeof(si) );
//     si.cb = sizeof(si);
//     ZeroMemory( &pi, sizeof(pi) );

//     // start the program up
//     CreateProcess( filePath.toStdWString().c_str(),   // the path .c_str()
//         NULL,        // Command line
//         NULL,           // Process handle not inheritable
//         NULL,           // Thread handle not inheritable
//         FALSE,          // Set handle inheritance to FALSE
//         0,              // No creation flags
//         NULL,           // Use parent's environment block
//         NULL,           // Use parent's starting directory
//         &si,            // Pointer to STARTUPINFO structure
//         &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
//         );
//     // Close process and thread handles.
//     CloseHandle( pi.hProcess );
//     CloseHandle( pi.hThread );
//     return pi.dwProcessId;
// }

HANDLE WidgetContainer::findWindowHandle(const QString &iClassName, const QString &iWindowName)
{
    //L"Notepad++", L"new 1 - Notepad++ [Administrator]"
#ifdef UNICODE
    return FindWindow(iClassName.toStdWString().data(), iWindowName.toStdWString().data());
#else
    return FindWindow(iClassName.toStdString().data(), iWindowName.toStdString().data());
#endif
}
