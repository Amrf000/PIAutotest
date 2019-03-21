#include "mainwindow.h"
#include <QApplication>
#include <windows.h>
#include <iostream>
#include <QDebug>
#include <Registry.hpp>
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
    const std::wstring testSubKey = L"Software\\RealVNC\\vncviewer";
    winreg::RegKey key(HKEY_CURRENT_USER, testSubKey);
    key.SetStringValue(L"Scaling", L"AspectFit");
    key.Close();
    //Console();
    std::cout<<"start@@";
    std::cerr<<"sss";
    qDebug()<<"start!";
    QApplication a(argc, argv);
    pMain = new MainWindow();
    pMain->init();
    pMain->setWindowState(Qt::WindowMaximized);
    pMain->show();
    int RetVal = a.exec();
    delete pMain;
    return RetVal;
}
