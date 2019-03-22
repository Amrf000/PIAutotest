#include "mainwindow.h"
#include <windows.h>
#include <iostream>
#include <QDebug>
#include <QtSingleApplication>
#include <QMessageBox>
#include "Registry.hpp"
using namespace winreg;

#ifdef main
#undef main
#endif
void Console()
{
    AllocConsole();
    FILE *pFileCon = NULL;
    pFileCon = freopen("CONOUT$", "w", stdout);

    COORD coordInfo;
    coordInfo.X = 130;
    coordInfo.Y = 9000;

    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coordInfo);
    SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE),ENABLE_QUICK_EDIT_MODE| ENABLE_EXTENDED_FLAGS);

    pFileCon = freopen("CONOUT$", "w", stderr);

    SetConsoleScreenBufferSize(GetStdHandle(STD_ERROR_HANDLE), coordInfo);
    SetConsoleMode(GetStdHandle(STD_ERROR_HANDLE),ENABLE_QUICK_EDIT_MODE| ENABLE_EXTENDED_FLAGS);
}

MainWindow* pMain = NULL;
int main(int argc, char *argv[])
{
    //Console();
    const std::wstring testSubKey = L"SOFTWARE\\RealVNC\\vncviewer";
    winreg::RegKey key(HKEY_CURRENT_USER, testSubKey);
    key.SetStringValue(L"Scaling", L"AspectFit");
    key.SetStringValue(L"EulaAccepted", L"db346d1e0fd4a6d2872111de6217b84d04adc9e0");
    key.Close();

    std::cout<<"start@@";
    //std::cerr<<"sss";
    qDebug()<<"start!";
    QtSingleApplication app(argc, argv);
    if (app.isRunning())
    {
         QMessageBox::information(NULL, "PIAuto","already run",QMessageBox::Ok);
         app.sendMessage("raise_window_noop");
         return EXIT_SUCCESS;
    }
    pMain = new MainWindow();
    pMain->init();
    pMain->setWindowState(Qt::WindowMaximized);
    pMain->show();
    int RetVal = app.exec();
    delete pMain;
    return RetVal;
}
