#ifndef CLIENT_H
#define CLIENT_H
#include <QThread>
#include <QSharedMemory>

class SyscData
{
public:
    SyscData():sCmdType(0),sHwnd(0),CDesktopWin(0),CDesktopWinState(0),
                                   Viewer(0),ViewerState(0),
                                   Connection(0),ConnectionState(0),
                                   Authentication(0),AuthenticationState(0)
    {
    }
    enum SyscEnumCmd
    {
       C_CREATEWND=1111,
       C_DESTROY=1112,
       C_SHOWWND=1113,
       S_CONNECT=9999
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
};
class Client:public QThread
{
public:
    explicit Client();
    virtual ~Client();
    virtual void run();
    SyscData* getLocalDataRef();
	void downShareMemory();
	void upShareMemory();
private:
    SyscData localData;
private:
	QSharedMemory sharedMemory;
};

#endif // CLIENT_H
