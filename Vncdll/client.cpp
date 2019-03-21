#include "client.h"
#include <Windows.h>

Client::Client():Eexit(0),missing(0)
{
    sharedMemory.setKey("qvnc");
}
Client::~Client()
{

}
void Client::stop()
{
    Eexit = 1;
}
extern HWND desktopHWND;
void Client::run()
{
	while(true)
	{
        downShareMemory();
        switch(localData.sCmdType)
        {
        case SyscData::S_CONNECT:
            printf("sCmdType=%d\n",localData.sCmdType);
            localData.sCmdType = 0;
            break;
        case SyscData::S_ACTIVE:
            printf("sCmdType=%d\n",localData.sCmdType);
            if(desktopHWND)
            {
                SetActiveWindow(desktopHWND);
                SetFocus(desktopHWND);
            }
            localData.sCmdType = 0;
            break;
        default:
            break;
        }
        if(Eexit==1 || localData.Eexit==1 || missing>100)
        {
            localData.Eexit = 1;
            upShareMemory();
            msleep(10);
            TerminateProcess(GetCurrentProcess(),0);
            break;
        }
        upShareMemory();
        msleep(20);
	}
}
SyscData* Client::getLocalDataRef()//目前不需要考虑消息堆积
{
     return &localData;
}

void Client::downShareMemory()
{
    if (!sharedMemory.attach())
    {
        missing++;
        printf("Unable to attach to shared memory segment. downerror=%d\n",sharedMemory.error());
        return;
    }else{
        missing=0;
        //printf("attach down success\n");
    }
    sharedMemory.lock();
    memcpy((void*)&localData,(char*)sharedMemory.constData(), sizeof(SyscData));
    sharedMemory.unlock();
    if (!sharedMemory.detach())
    {
        printf("Unable to deattach to shared memory segment. downerr=%d\n",sharedMemory.error());
        return;
    }else{
        //printf("deattach down success\n");
    }
}

void Client::upShareMemory()
{
    if (!sharedMemory.attach())
    {
        missing++;
        printf( "Unable to attach to shared memory segment. uperr=%d\n",sharedMemory.error());
        return;
    }else{
        missing=0;
        //printf( "attach up success\n");
    }
    sharedMemory.lock();
    char *to = (char*)sharedMemory.data();
    const char *from = (char*)&localData;
    memcpy(to, from, sizeof(SyscData));
    sharedMemory.unlock();
    if (!sharedMemory.detach())
    {
        printf( "Unable to deattach to shared memory segment. uperr=%d\n",sharedMemory.error());
        return;
    }else{
        //printf( "deattach up success\n");
    }
}
