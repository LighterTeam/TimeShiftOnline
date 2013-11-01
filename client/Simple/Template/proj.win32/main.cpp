/*  write by xingming
 *  time:2012��10��19��15:51:53
 *  for: test regex
 *  */

#include <regex>
#include <iostream>
#include <stdio.h>
#include <string>

#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"
//#include "json/json.h"

#ifdef WIN32
#include "Common/Interface/crash_interface.h"
#include <tchar.h>
const TCHAR* SCENE_CRASH_DUMP = _T("SceneCrashDump");
#endif

using namespace std;

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    //ParseJsonFromString();
#ifdef WIN32
    ICrashDump* pDumpSys = nullptr;
    HRESULT hr = CoTsCreateInstance(CRASH_DUMP_LIBRARY,ICrashDumpGUID,(void**)&pDumpSys);
    pDumpSys->Initialize(SCENE_CRASH_DUMP);
#endif

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef USE_WIN32_CONSOLE
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
#endif

    //std::string s = "000$^aaa^$2a000$^bbb^$2a000$^ccc^$2a000$^ddd^$2a";
    //std::regex re("\\$\\^.*?\\^\\$");
    //std::sregex_token_iterator i(s.begin(), s.end(), re);
    //std::sregex_token_iterator j;
    //unsigned count = 0;
    //while(i != j)
    //{
    //    cout << *i++ << endl;
    //    count++;
    //}

    // create the application instance
    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();

    eglView->setFrameSize(640/2, 1136/2);

    int ret = CCApplication::sharedApplication()->run();

#ifdef USE_WIN32_CONSOLE
    FreeConsole();
#endif

    return ret;
}
