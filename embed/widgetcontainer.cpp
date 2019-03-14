#include "widgetcontainer.h"
#include <Windows.h>
#include <QCoreApplication>
#include <QWindow>
#include <string>
#include <functional>
#include <iostream>
#include <fstream>
#include <QDebug>
#include <QProcess>
#include "mainwindow.h"

WidgetContainer::WidgetContainer(QWidget *parent)
  :QWidget(parent)
{
    //m_data.processId=0;
    //m_data.wndHandle=NULL;
    //m_process =NULL;
}

/*WidgetContainer::~WidgetContainer()
{
   if(m_process)
   {
       m_process->terminate();
   }
}*/

QWidget *WidgetContainer::createContainer(MainWindow* pm,const QString &iAppName, bool iAutoOpen)
{
    /*DWORD procId =(DWORD)findProcessId(iAppName);
    if (procId == 0 && iAutoOpen) {
        procId = startUpProcess(iAppName);
        if (procId == 0) {
            return nullptr;
        }
        Sleep(2000);
    } else if (procId == 0 && !iAutoOpen) {
        return nullptr;
    }*/
    QProcess* process = new QProcess(pm);
    QString wordPath = QCoreApplication::applicationDirPath()+"/"+iAppName;
    process->start(wordPath);
   // Sleep(500);
    if(true==process->waitForStarted(50000))
    {
        handle_data data;
        data.processId = process->processId();//procId;
        data.wndHandle = nullptr;

        printf("+++++%d\n",data.processId);

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

        printf("+++++%d\n",data.wndHandle);

        if (nullptr == data.wndHandle) {
            return nullptr;
        }
        printf("-----\n");
        QWindow *foreign = QWindow::fromWinId((WId) data.wndHandle);
        if (nullptr != foreign) {
            QWidget *container = QWidget::createWindowContainer(foreign);
            container->setProperty("processId",(long)data.processId);
            container->setProperty("wndHandle",(long)data.wndHandle);
            container->setProperty("m_process",(long)process);
            return container;
        }
        return nullptr;
    }
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

DWORD WidgetContainer::startUpProcess(const QString &iAppName)
{
    /*std::wstring filePath = iAppName.toStdWString();
    std::wifstream ifExists(filePath);
    bool fileExists = true;
    if (!ifExists.good()) {
        fileExists = false;
        QString path(std::getenv("path"));
        for (const QString& onePath: path.split(";")) {
            std::wifstream ifExists(QString(onePath + "/" + iAppName).toStdWString());
            if (ifExists.good()) {
                filePath = QString(onePath + "/" + iAppName).toStdWString();
                fileExists = true;
                break;
            }
        }
    }

    if (!fileExists) {
        return 0;
    }*/

    QString filePath = QCoreApplication::applicationDirPath()+"/"+iAppName;

    // additional information
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // set the size of the structures
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    // start the program up
    CreateProcess( filePath.toStdWString().c_str(),   // the path .c_str()
        NULL,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory
        &si,            // Pointer to STARTUPINFO structure
        &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
        );
    // Close process and thread handles.
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
    return pi.dwProcessId;
}

HANDLE WidgetContainer::findWindowHandle(const QString &iClassName, const QString &iWindowName)
{
    //L"Notepad++", L"new 1 - Notepad++ [Administrator]"
#ifdef UNICODE
    return FindWindow(iClassName.toStdWString().data(), iWindowName.toStdWString().data());
#else
    return FindWindow(iClassName.toStdString().data(), iWindowName.toStdString().data());
#endif
}