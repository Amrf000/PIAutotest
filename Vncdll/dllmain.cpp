#define _AFXDLL
#include <windows.h>
//#include <afxwin.h>
#include <cstdio>
#include <QBuffer>
#include <QDebug>
#include <QString>
#include "detours.h"
#include "client.h"

Client* pClient=NULL;
void Console()
{
    AllocConsole();
    FILE *pFileCon = NULL;
    pFileCon = freopen("vnctxt.txt","w", stdout);//"CONOUT$", "w"

    COORD coordInfo;
    coordInfo.X = 130;
    coordInfo.Y = 9000;

    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coordInfo);
    SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE),ENABLE_QUICK_EDIT_MODE| ENABLE_EXTENDED_FLAGS);

    pFileCon = freopen("vnctxt.txt","a+", stderr);//"CONOUT$", "w"

    SetConsoleScreenBufferSize(GetStdHandle(STD_ERROR_HANDLE), coordInfo);
    SetConsoleMode(GetStdHandle(STD_ERROR_HANDLE),ENABLE_QUICK_EDIT_MODE| ENABLE_EXTENDED_FLAGS);
}
QMap<QString,HWND> wndMap;
HHOOK gWCHook=NULL;
LRESULT CALLBACK hookWCProc( int code,WPARAM wParam,LPARAM lParam )
{
    switch (code)
    {
    case HCBT_CREATEWND:
    {
        HWND hWnd = (HWND)wParam;
        //TCHAR szText[128] = { 0 },szText1[128] = { 0 };
        //GetClassName(hWnd,szText,127);
        //int id=GetWindowLong(hWnd,GWLP_ID);
        //GetWindowText(hWnd,szText1,GetWindowTextLength(msg->hwnd)+1);
        //printf("CBTHook HCBT_CREATEWND hwnd=%d,wPAram=%d,lParam=%d,clas=%ls,text=%ls,id=%d\n",(int)hWnd,(int)msg->wParam,(int)msg->lParam,szText,szText1,id);
        LPCBT_CREATEWND lpCreate = (LPCBT_CREATEWND)lParam;
        LPCWSTR lpszName = IsBadStringPtr(lpCreate->lpcs->lpszName,128)?L"null":lpCreate->lpcs->lpszName;
        LPCWSTR lpszClass = IsBadStringPtr(lpCreate->lpcs->lpszClass,128)?L"null":lpCreate->lpcs->lpszClass;
        printf("CBTHook HCBT_CREATEWND hwnd=%d,hMenu=%d,hwndParent=%d,lpszName=%ls,lpszClass=%ls\n",hWnd,(int)lpCreate->lpcs->hMenu,(int)lpCreate->lpcs->hwndParent,lpszName,lpszClass);
        break;
    }
    case HCBT_DESTROYWND:
    {
        HWND hWnd = (HWND)wParam;
        TCHAR szText[128] = { 0 },szText1[128] = { 0 };
        GetClassName(hWnd,szText,127);
        int id=GetWindowLong(hWnd,GWLP_ID);
        GetWindowText(hWnd,szText1,GetWindowTextLength(hWnd)+1);
        LPCWSTR lpszName = IsBadStringPtr(szText1,128)?L"null":szText1;
        LPCWSTR lpszClass = IsBadStringPtr(szText,128)?L"null":szText;
        printf("CBTHook HCBT_DESTROYWND hwnd=%d,id=%d,lpszName=%ls,lpszClass=%ls\n",(int)hWnd,id,lpszName,lpszClass);
        break;
    }
    default:
        break;
    }
    return(CallNextHookEx(NULL, code, wParam, lParam));
}
ATOM aCDesktopWin=0;
HWND (WINAPI * oldCreateWindowExW)(DWORD,LPCWSTR,LPCWSTR,DWORD,int,int,int,int,HWND,HMENU,HINSTANCE,LPVOID) = CreateWindowExW;
HWND (WINAPI * oldCreateWindowExA)(DWORD,LPCSTR,LPCSTR,DWORD,int,int,int,int,HWND,HMENU,HINSTANCE,LPVOID) = CreateWindowExA;
HWND (WINAPI * oldCreateDialogParamW)(HINSTANCE,LPCWSTR,HWND,DLGPROC,LPARAM) = CreateDialogParamW;
ATOM (WINAPI * oldRegisterClassW)(const WNDCLASSW *)=RegisterClassW;
ATOM (WINAPI * oldRegisterClassExW)(const WNDCLASSEXW *)=RegisterClassExW;
BOOL (WINAPI * oldSetWindowPos)(HWND,HWND,int ,int ,int ,int ,UINT) = SetWindowPos;
BOOL (WINAPI * oldShowWindow)(HWND,int) = ShowWindow;

BOOL WINAPI newShowWindow(HWND hWnd,int  nCmdShow)
{
    if(aCDesktopWin && hWnd==wndMap[QString("vwr::CDesktopWin")])
    {
        printf("newShowWindow CDesktopWin nCmdShow=%d\n",nCmdShow);
        pClient->downShareMemory();
        SyscData* ref=pClient->getLocalDataRef();
        ref->CDesktopWinState = SyscData::C_SHOWWND;
        pClient->upShareMemory();
    }
    return oldShowWindow(hWnd,nCmdShow);
}
BOOL WINAPI newSetWindowPos(  HWND hWnd,  HWND hWndInsertAfter,  int  X,  int  Y,  int  cx,  int  cy,  UINT uFlags)
{
    if(aCDesktopWin && hWnd==wndMap[QString("vwr::CDesktopWin")])
    {
        printf("newSetWindowPos CDesktopWin flag=%d\n",uFlags);
    }
    return oldSetWindowPos(hWnd,hWndInsertAfter,X,Y,cx,cy,uFlags);
}

HWND WINAPI newCreateWindowExW( DWORD dwExStyle,LPCWSTR  lpClassName,LPCWSTR  lpWindowName,DWORD dwStyle,int  X,int  Y,int  nWidth,int  nHeight,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance,LPVOID  lpParam)
{
    LPCWSTR clsName=lpClassName;
    wchar_t buf[128]={0};
    if(IsBadStringPtr(lpClassName,128))
    {
        //WNDCLASSEX lWndClass={0};
        //GetClassInfoEx(NULL,(LPCWSTR)((DWORD)lpClassName&0xffff),&lWndClass);//hInstance
        //clsName = lWndClass.lpszClassName;
        //wchar_t buf[128]={0};
        //GlobalGetAtomName((ATOM)lpClassName,buf,127);
        //clsName = buf;
        ::GlobalGetAtomName((ATOM)lpClassName,buf,127);
        clsName = buf;
        if(IsBadStringPtr(clsName,128))
        {
            ::GetAtomName((ATOM)lpClassName,buf,127);
            clsName = buf;
        }
    }
    HWND res = oldCreateWindowExW(dwExStyle,lpClassName,lpWindowName,dwStyle,X,Y,nWidth,nHeight,hWndParent,hMenu,hInstance,lpParam);
    ATOM atom = IsBadStringPtr(lpClassName,128)?(int)lpClassName:0;
    LPCWSTR text = IsBadStringPtr(lpWindowName,128)?L"badWndName":lpWindowName;
    printf("newCreateWindowExW cls=%ls text=%ls,atom=%d,hMenu=%d,hwnd=%d,dwStyle=%d\n",IsBadStringPtr(clsName,128)?L"badClsName":clsName,text,
           atom,(int)hMenu,res,dwStyle);
    QString qtxt = QString::fromStdWString(text);
    if(atom==aCDesktopWin){
         pClient->downShareMemory();
         SyscData* ref=pClient->getLocalDataRef();
         ref->CDesktopWin = (long)res;
         ref->CDesktopWinState = SyscData::C_CREATEWND;
         pClient->upShareMemory();
         wndMap.insert(QString("vwr::CDesktopWin"),res);
    }if(QString::compare("VNC Viewer",qtxt)==0){
        pClient->downShareMemory();
        SyscData* ref=pClient->getLocalDataRef();
        ref->Viewer = (long)res;
        ref->ViewerState = SyscData::C_CREATEWND;
        pClient->upShareMemory();
        wndMap.insert(QString("Viewer"),res);
    }else if(QString::compare("VNC Viewer - Connection Information",qtxt)==0){\
        pClient->downShareMemory();
        SyscData* ref=pClient->getLocalDataRef();
        ref->Connection = (long)res;
        ref->ConnectionState = SyscData::C_CREATEWND;
        pClient->upShareMemory();
        wndMap.insert(QString("Connection"),res);
    }else if(QString::compare("VNC Viewer - Authentication",qtxt)==0){
        pClient->downShareMemory();
        SyscData* ref=pClient->getLocalDataRef();
        ref->Authentication = (long)res;
        ref->AuthenticationState = SyscData::C_CREATEWND;
        pClient->upShareMemory();
        wndMap.insert(QString("Authentication"),res);
    }
    return res;
}
HWND WINAPI newCreateWindowExA( DWORD dwExStyle,LPCSTR  lpClassName,LPCSTR  lpWindowName,DWORD dwStyle,int  X,int  Y,int  nWidth,int  nHeight,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance,LPVOID  lpParam)
{
    HWND res =oldCreateWindowExA(dwExStyle,lpClassName,lpWindowName,dwStyle,X,Y,nWidth,nHeight,hWndParent,hMenu,hInstance,lpParam);
    printf("oldCreateWindowExA cls=%s text=%s hwnd=%d\n",IsBadStringPtrA(lpClassName,128)?"badClsName":lpClassName,IsBadStringPtrA(lpWindowName,128)?"badWndName":lpWindowName,(int)res);
    return res;
}
HWND newCreateDialogParamW(HINSTANCE hInstance,LPCWSTR lpTemplateName,HWND hWndParent,DLGPROC lpDialogFunc,LPARAM dwInitParam)
{
    printf("oldCreateDialogParamW lpTemplateName=%ls\n",IsBadStringPtr(lpTemplateName,128)?L"badTepName":lpTemplateName);
    return oldCreateDialogParamW(hInstance,lpTemplateName,hWndParent,lpDialogFunc,dwInitParam);
}
ATOM newRegisterClassW(const WNDCLASSW *lpWndClass)
{
    if(gWCHook==NULL)
    {
        gWCHook=SetWindowsHookEx(WH_CBT,hookWCProc,NULL,GetCurrentThreadId());
        if(gWCHook==NULL){
            printf("gWCHook fail\n");
        }else{
            printf("gWCHook success\n");
        }
    }
    ATOM ret = oldRegisterClassW(lpWndClass);
    if(lpWndClass!=NULL)
    {
       printf("oldRegisterClassW cls=%ls,menu=%ls,atom=%d\n",IsBadStringPtr(lpWndClass->lpszClassName,128)?L"badClassName":lpWndClass->lpszClassName,
              IsBadStringPtr(lpWndClass->lpszMenuName,128)?L"badMenuName":lpWndClass->lpszMenuName,(int)ret);
    }
    return ret;
}
ATOM newRegisterClassExW(const WNDCLASSEXW *lpWndClass)
{
    if(gWCHook==NULL)
    {
        gWCHook=SetWindowsHookEx(WH_CBT,hookWCProc,NULL,GetCurrentThreadId());
        if(gWCHook==NULL){
            printf("gWCHook fail\n");
        }else{
            printf("gWCHook success\n");
        }
    }
    ATOM ret = oldRegisterClassExW(lpWndClass);
    if(lpWndClass!=NULL)
    {
       LPCWSTR cls = IsBadStringPtr(lpWndClass->lpszClassName,128)?L"badClassName":lpWndClass->lpszClassName;
       printf("oldRegisterClassExW cls=%ls,menu=%ls,atom=%d\n",cls,
              IsBadStringPtr(lpWndClass->lpszMenuName,128)?L"badMenuName":lpWndClass->lpszMenuName,(int)ret);
       if(wcscmp(L"vwr::CDesktopWin",cls)==0)
       {
          aCDesktopWin = ret;
          printf("CDesktopWin style=%d\n",lpWndClass->style);
       }else if(wcscmp(L"ui::Window::Dialog",cls)==0){
           printf("ui::Window::Dialog style=%d\n",lpWndClass->style);
       }
    }
    return ret;
}
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    LONG error;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
    {
         Console();
         DetourTransactionBegin();
         DetourUpdateThread(GetCurrentThread());
         DetourAttach(&(PVOID&)oldCreateWindowExW, newCreateWindowExW);
         DetourAttach(&(PVOID&)oldCreateWindowExA, newCreateWindowExA);
         DetourAttach(&(PVOID&)oldCreateDialogParamW, newCreateDialogParamW);
         DetourAttach(&(PVOID&)oldRegisterClassW, newRegisterClassW);
         DetourAttach(&(PVOID&)oldRegisterClassExW, newRegisterClassExW);
         DetourAttach(&(PVOID&)oldSetWindowPos,newSetWindowPos);
         DetourAttach(&(PVOID&)oldShowWindow,newShowWindow);
         error = DetourTransactionCommit();
         if (error == NO_ERROR) {
               printf("echofx" DETOURS_STRINGIFY(DETOURS_BITS) ".dll:"" Detoured Echo().\n");
         }else {
               printf("echofx" DETOURS_STRINGIFY(DETOURS_BITS) ".dll:"" Error detouring Echo(): %d\n", error);
         }
         printf("ul_reason_for_call=%d\n",ul_reason_for_call);
         //Create();
        //Sleep(500);
        pClient = new Client;
        pClient->start();
     }
        break;
	case DLL_THREAD_ATTACH:
    {
        printf("ul_reason_for_call=%d\n",ul_reason_for_call);
        break;
    }
	case DLL_THREAD_DETACH:
        printf("ul_reason_for_call=%d\n",ul_reason_for_call);
        break;
	case DLL_PROCESS_DETACH:
    {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&(PVOID&)oldCreateWindowExW, newCreateWindowExW);
        DetourDetach(&(PVOID&)oldCreateWindowExA, newCreateWindowExA);
        DetourDetach(&(PVOID&)oldCreateDialogParamW, newCreateDialogParamW);
        DetourDetach(&(PVOID&)oldRegisterClassW, newRegisterClassW);
        DetourDetach(&(PVOID&)oldRegisterClassExW, newRegisterClassExW);
        DetourDetach(&(PVOID&)oldSetWindowPos,newSetWindowPos);
        DetourDetach(&(PVOID&)oldShowWindow,newShowWindow);
        error = DetourTransactionCommit();
        printf("echofx" DETOURS_STRINGIFY(DETOURS_BITS) ".dll:" " Removed Echo() (result=%d)\n", error);

        printf("ul_reason_for_call=%d\n",ul_reason_for_call);
		break;
    }
	}
	return TRUE;
}

// class CSProxy
// {
// public:
// int c2s_connect;
// int s2c_connect;
// public:
// friend QDataStream& operator<<( QDataStream& out, const CSProxy& d)
// {
//      out<<d.c2s_connect<<d.s2c_connect;
//      return out;
// }
// friend QDataStream& operator>>( QDataStream& in, CSProxy& d)
// {
//      in>>d.c2s_connect>>d.s2c_connect;
//      return in;
// }
// };
/*CSProxy localcs={0};
QSharedMemory sharedMemory;
void downShareMemory()
{
    if (!sharedMemory.attach())
    {
        printf("Unable to attach to shared memory segment. downerror=%d\n",sharedMemory.error());
        return;
    }else{
        printf("attach down success\n");
    }
    QBuffer buffer;
    QDataStream in(&buffer);
    sharedMemory.lock();
    buffer.setData((char*)sharedMemory.constData(), sharedMemory.size());
    buffer.open(QBuffer::ReadOnly);
    in >> localcs;
    sharedMemory.unlock();
    if (!sharedMemory.detach())
    {
        printf("Unable to deattach to shared memory segment. downerr=%d\n",sharedMemory.error());
        return;
    }else{
        printf("deattach down success\n");
    }
}

void upShareMemory()
{
    if (!sharedMemory.attach())
    {
        printf( "Unable to attach to shared memory segment. uperr=%d\n",sharedMemory.error());
        return;
    }else{
        printf( "attach up success\n");
    }
    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);
    QDataStream out(&buffer);
    out << localcs;
    int size = buffer.size();
    sharedMemory.lock();
    char *to = (char*)sharedMemory.data();
    const char *from = buffer.data().data();
    memcpy(to, from, qMin(sharedMemory.size(), size));
    sharedMemory.unlock();
    if (!sharedMemory.detach())
    {
        printf( "Unable to deattach to shared memory segment. uperr=%d\n",sharedMemory.error());
        return;
    }else{
        printf( "deattach up success\n");
    }
}*/
//void CALLBACK HandleWinEvent(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
//{
    /*IAccessible* pAcc = NULL;
    VARIANT varChild;

    HRESULT hr = AccessibleObjectFromEvent(hwnd, idObject, idChild, &pAcc, &varChild);

    if ((hr == S_OK) && (pAcc != NULL))
    {
        VARIANT varResult = {};
        pAcc->get_accRole(varChild, &varResult);

        if (varResult.lVal == ROLE_SYSTEM_PAGETAB || varResult.lVal == NULL)
        {
            BSTR bstrName;
            pAcc->get_accName(varChild, &bstrName);

            if (event == EVENT_OBJECT_CREATE)
            {
                printf("Create: ");
            }
            else if (event == EVENT_OBJECT_DESTROY)
            {
                printf("Destroy:   ");
            }
            printf("%S\n", bstrName);
            SysFreeString(bstrName);
        }
        pAcc->Release();
    }*/
    /*if (!(event == EVENT_OBJECT_CREATE &&
          idObject == OBJID_WINDOW &&
          idChild == INDEXID_CONTAINER))
        return;
  */
    //LONG lStyle;
    //if (!(lStyle = GetWindowLong(hWnd, GWL_STYLE)))
//        return;

    // These conditions are fulfilled by windows with a desktop presence.
    //if ((lStyle & (WS_BORDER | WS_VISIBLE)) != (WS_BORDER | WS_VISIBLE))
    //    return;

//    printf("WinEventCallback: EVENT_OBJECT_CREATE\n");
//}

// struct handle_data {
//     DWORD processId;
//     HWND wndHandle;
// };

// LRESULT CALLBACK MyProc( int code,WPARAM wParam,LPARAM lParam )
// {
//     switch (code)
//     {
//     case HCBT_CREATEWND:
//     {
//         HWND hWnd = (HWND)wParam;
//         //TCHAR szText[128] = { 0 },szText1[128] = { 0 };
//         //GetClassName(hWnd,szText,127);
//         //int id=GetWindowLong(hWnd,GWLP_ID);
//         //GetWindowText(hWnd,szText1,GetWindowTextLength(msg->hwnd)+1);
//         //printf("CBTHook HCBT_CREATEWND hwnd=%d,wPAram=%d,lParam=%d,clas=%ls,text=%ls,id=%d\n",(int)hWnd,(int)msg->wParam,(int)msg->lParam,szText,szText1,id);
//         LPCBT_CREATEWND lpCreate = (LPCBT_CREATEWND)lParam;
//         LPCWSTR lpszName = IsBadStringPtr(lpCreate->lpcs->lpszName,128)?L"null":lpCreate->lpcs->lpszName;
//         LPCWSTR lpszClass = IsBadStringPtr(lpCreate->lpcs->lpszClass,128)?L"null":lpCreate->lpcs->lpszClass;
//         printf("CBTHook HCBT_CREATEWND hwnd=%d,hMenu=%d,hwndParent=%d,lpszName=%ls,lpszClass=%ls\n",hWnd,(int)lpCreate->lpcs->hMenu,(int)lpCreate->lpcs->hwndParent,lpszName,lpszClass);
//         break;
//     }
//     case HCBT_DESTROYWND:
//     {
//         HWND hWnd = (HWND)wParam;
//         TCHAR szText[128] = { 0 },szText1[128] = { 0 };
//         GetClassName(hWnd,szText,127);
//         int id=GetWindowLong(hWnd,GWLP_ID);
//         GetWindowText(hWnd,szText1,GetWindowTextLength(hWnd)+1);
//         LPCWSTR lpszName = IsBadStringPtr(szText1,128)?L"null":szText1;
//         LPCWSTR lpszClass = IsBadStringPtr(szText,128)?L"null":szText;
//         printf("CBTHook HCBT_DESTROYWND hwnd=%d,id=%d,lpszName=%ls,lpszClass=%ls\n",(int)hWnd,id,lpszName,lpszClass);
//         break;
//     }
//     default:
//         break;
//     }
//     return(CallNextHookEx(NULL, code, wParam, lParam));
// }

// WNDPROC OldWindowProc=NULL;
// HHOOK gConnectDlgWndHook=NULL;

// LRESULT CALLBACK NewWindowProc(HWND hwnd,UINT uMsg,WPARAM wPAram,LPARAM lParam)
// {
//     if(gConnectDlgWndHook==NULL)
//     {
//         gConnectDlgWndHook=SetWindowsHookEx(WH_CALLWNDPROC,hookWndProc,NULL,GetCurrentThreadId());
//         if(gConnectDlgWndHook==NULL){
//             printf("ConnectDlgWndHook fail\n");
//         }else{
//             printf("ConnectDlgWndHook success\n");
//         }
//     }
//     if(gWCHook==NULL)
//     {
//         gWCHook=SetWindowsHookEx(WH_CBT,hookWCProc,NULL,GetCurrentThreadId());
//         if(gWCHook==NULL){
//             printf("gWCHook fail\n");
//         }else{
//             printf("gWCHook success\n");
//         }
//     }
//      switch(uMsg)
//      {
//      case WM_CLOSE:
//          printf("connectDlg WM_ClOSE hwnd=%d,wPAram=%d,lParam=%d\n",(int)hwnd,(int)wPAram,(int)lParam);
//          break;
//      case WM_DESTROY:
//          printf("connectDlg WM_DESTROY hwnd=%d,wPAram=%d,lParam=%d\n",(int)hwnd,(int)wPAram,(int)lParam);
//          break;
//      case WM_SHOWWINDOW:
//          printf("connectDlg WM_SHOWWINDOW hwnd=%d,wPAram=%d,lParam=%d\n",(int)hwnd,(int)wPAram,(int)lParam);
//          break;
//      default:
//          break;
//      }
//      return CallWindowProc(OldWindowProc,hwnd,uMsg,wPAram,lParam);
// }
// void proc()
// {
//     handle_data data;
//     data.processId = GetCurrentProcessId();//procId;
//     data.wndHandle = nullptr;
//     while(data.wndHandle == nullptr)
//     {
//         EnumWindows([](HWND wnd, LPARAM lParam)->BOOL
//         {
//             handle_data &data = *(handle_data*)lParam;
//             DWORD thProcId = 0;
//             GetWindowThreadProcessId(wnd, &thProcId);
//             //check if it is mainwindow
//             BOOL isMainWnd = GetWindow(wnd, GW_OWNER) == nullptr && IsWindowVisible(wnd);

//             if (thProcId == data.processId && isMainWnd) {
//                 TCHAR szText[64] = { 0 };
//                 GetClassName(wnd,szText,63);
//                 printf("mainclass=%ls\n",szText);
//                 //if(0==wcsncmp(szText,L"ConsoleWindowClass",18))
//                 //{
//                 //    return 1;
//                // }else{
//                     data.wndHandle = wnd;
//                     return -1;
//                // }
//             }
//             return 1;
//         }
//         , (LPARAM)&data);
//         Sleep(50);
//     }
//     OldWindowProc=(WNDPROC)GetWindowLongPtr(data.wndHandle,GWLP_WNDPROC);
//     SetWindowLongPtr(data.wndHandle,GWLP_WNDPROC,(LONG_PTR)NewWindowProc);
// }


// static LPCWSTR s_szWndClassName = L"TheWindowIwillCreate";
// static HWND s_hWnd = NULL;
// ATOM RegisterWndClass(WNDPROC WndProc)
// {
//     WNDCLASSW wc = { 0 };
//     wc.style = CS_HREDRAW | CS_VREDRAW ;
//     wc.lpfnWndProc = WndProc;
//     wc.cbClsExtra = 0;
//     wc.cbWndExtra = 0;
//     wc.hIcon = NULL;
//     wc.hInstance = GetModuleHandle(NULL);
//     wc.hCursor		= LoadCursor(NULL, IDC_ARROW);
//     wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
//     wc.lpszMenuName = NULL;
//     wc.lpszClassName = s_szWndClassName;

//     return (RegisterClassW(&wc));
// }

// LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
// {
//     LRESULT lResult = -1;

//     switch (uMsg)
//     {
//     /*case WM_PAINT:
//         {
//             PAINTSTRUCT paint = { 0 };
//             HDC hDC = ::BeginPaint(hWnd, &paint);
//             ::EndPaint(hWnd, &paint);
//             lResult = 0;
//         }
//         break;
//     */
//     default:
//         lResult = DefWindowProc(hWnd, uMsg, wParam, lParam);
//         break;
//     }

//     if(gConnectDlgWndHook==NULL)
//     {
//         gConnectDlgWndHook=SetWindowsHookEx(WH_CALLWNDPROC,hookWndProc,NULL,GetCurrentThreadId());
//         if(gConnectDlgWndHook==NULL){
//             printf("ConnectDlgWndHook fail\n");
//         }else{
//             printf("ConnectDlgWndHook success\n");
//         }
//     }
//     if(gWCHook==NULL)
//     {
//         gWCHook=SetWindowsHookEx(WH_CBT,hookWCProc,NULL,GetCurrentThreadId());
//         if(gWCHook==NULL){
//             printf("gWCHook fail\n");
//         }else{
//             printf("gWCHook success\n");
//         }
//     }

//     return lResult;
// }

// void Create()
// {
//     WNDCLASSW wndcls = { 0 };
//     if (!::GetClassInfoW(GetModuleHandle(NULL), s_szWndClassName, &wndcls))
//     {
//         if (!RegisterWndClass(WindowProc))
//             return;
//     }

//     s_hWnd = CreateWindowW(s_szWndClassName,
//         NULL, WS_POPUP, 100, 100, 300, 300, NULL, (HMENU)0,
//         GetModuleHandle(NULL), NULL);

//     //DWORD nErr = GetLastError();

//     if (s_hWnd == NULL)
//         return;

//     ShowWindow(s_hWnd, SW_HIDE);//SW_SHOWNORMAL
//     //UpdateWindow(s_hWnd);
//     //::SetFocus(s_hWnd);

//     //MSG msg = { 0 };
//     // Pump messages until a PostQuitMessage.
//     //while(::GetMessage(&msg, NULL, 0, 0))
//     //{
//     //    ::TranslateMessage(&msg);
//     //    ::DispatchMessage(&msg);
//     //}
// }

// void Destroy()
// {
//     if (::IsWindow(s_hWnd))
//     {
//         ::SendMessage(s_hWnd, WM_CLOSE, 0, 0);
//         s_hWnd = NULL;
//     }
// }
// LRESULT CALLBACK hookWndProc( int code,WPARAM wParam,LPARAM lParam )
// {
//     if (code >= 0) {
//         LPMSG msg = (LPMSG)lParam;
//         int result = CallNextHookEx(NULL, code, wParam, lParam);
//         if (msg->message == WM_LBUTTONDOWN) {
//             //printf("Mouse down event happened \n");
//             //msg->message = WM_NULL;
//         }else if(msg->message == WM_CREATE){
//             printf("hook WM_CREATE hwnd=%d,wPAram=%d,lParam=%d\n",(int)msg->hwnd,(int)msg->wParam,(int)msg->lParam);
//         }else if(msg->message == WM_CLOSE){
//             printf("hook WM_CLOSE hwnd=%d,wPAram=%d,lParam=%d\n",(int)msg->hwnd,(int)msg->wParam,(int)msg->lParam);
//         }else if(msg->message == WM_DESTROY){
//             printf("hook WM_DESTROY hwnd=%d,wPAram=%d,lParam=%d\n",(int)msg->hwnd,(int)msg->wParam,(int)msg->lParam);
//         }else if(msg->message == WM_SHOWWINDOW){
//             TCHAR szText[128] = { 0 },szText1[128] = { 0 };
//             GetClassName(msg->hwnd,szText,127);
//             int id=GetWindowLong(msg->hwnd,GWLP_ID);
//             GetWindowText(msg->hwnd,szText1,GetWindowTextLength(msg->hwnd)+1);
//             printf("hook WM_SHOWWINDOW hwnd=%d,wPAram=%d,lParam=%d,clas=%ls,text=%ls,id=%d\n",(int)msg->hwnd,(int)msg->wParam,(int)msg->lParam,szText,szText1,id);
//         }else if(msg->message == WM_INITDIALOG){
//             printf("hook WM_INITDIALOG hwnd=%d,wPAram=%d,lParam=%d\n",(int)msg->hwnd,(int)msg->wParam,(int)msg->lParam);
//         }
//         return result;
//     }
//     return(CallNextHookEx(NULL, code, wParam, lParam));
// }
