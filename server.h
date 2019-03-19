#ifndef SERVER_H
#define SERVER_H
#include <windows.h>
#include <QThread>
#include <QSharedMemory>

class SyscData
{
public:
    SyscData():sCmdType(0),sHwnd(0),CDesktopWin(0),CDesktopWinState(0),
                                   Viewer(0),ViewerState(0),
                                   Connection(0),ConnectionState(0),
                                   Authentication(0),AuthenticationState(0),Eexit(0)
    {
    }
    enum SyscEnumCmd
    {
       C_CREATEWND=1111,
       C_DESTROY=1112,
       C_SHOWWND=1113,
       S_CONNECT=9990,
       S_ACTIVE=9991
    };
public:
    //int  cCmdType;//1111:createwnd 1112:destroywnd
    //long  cHwnd;
    long  sCmdType;//9999:connect(dest+password)
    long sHwnd;
    long CDesktopWin;
    long CDesktopWinState;
    long Viewer;
    long ViewerState;
    long Connection;
    long ConnectionState;
    long Authentication;
    long AuthenticationState;
    long Eexit;
};
class Server:public QThread
{
    Q_OBJECT
public:
    explicit Server();
    virtual ~Server();
    virtual void run();
    void stop();
    SyscData* getLocalDataRef();
private:
	void downShareMemory();
	void upShareMemory();
private:
    SyscData localData;
    long Eexit;
private:
	QSharedMemory sharedMemory;
signals:
    void onCDesktopWinCreate(long hwnd);
    void onViewerCreate(long hwnd);
    void onConnectionCreate(long hwnd);
    void onAuthenticationCreate(long hwnd);
    void onCDesktopWinShow(long hwnd);
    void onViewerShow(long hwnd);
};

#endif // SERVER_H
