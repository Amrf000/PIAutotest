#include "client.h"

Client::Client()
{
    sharedMemory.setKey("qvnc");
}
Client::~Client()
{
}
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
        default:
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
        printf("Unable to attach to shared memory segment. downerror=%d\n",sharedMemory.error());
        return;
    }else{
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
        printf( "Unable to attach to shared memory segment. uperr=%d\n",sharedMemory.error());
        return;
    }else{
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
