#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"
//#include "json/json.h"

#ifdef WIN32
#include "Common/Interface/crash_interface.h"
#include <tchar.h>
const TCHAR* SCENE_CRASH_DUMP = _T("SceneCrashDump");
#endif

USING_NS_CC;

// uncomment below line, open debug console
// #define USE_WIN32_CONSOLE

//int ParseJsonFromString()
//{
//    const char* str = "{\"uploadid\": \"UP0我勒个去\",\"code\": 100,\"msg\": \"\",\"files\": \"\"}";
//
//    Json::Reader reader;
//    Json::Value root;
//    if (reader.parse(str, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素
//    {
//        std::string upload_id = root["uploadid"].asString();  // 访问节点，upload_id = "UP000000"
//        int code = root["code"].asInt();    // 访问节点，code = 100
//    }
//    return 0;
//}

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
