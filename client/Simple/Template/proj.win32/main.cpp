#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"
#include "json/json.h"

USING_NS_CC;

// uncomment below line, open debug console
// #define USE_WIN32_CONSOLE

int ParseJsonFromString()
{
    const char* str = "{\"uploadid\": \"UP000000\",\"code\": 100,\"msg\": \"\",\"files\": \"\"}";

    Json::Reader reader;
    Json::Value root;
    if (reader.parse(str, root))  // reader��Json�ַ���������root��root������Json��������Ԫ��
    {
        std::string upload_id = root["uploadid"].asString();  // ���ʽڵ㣬upload_id = "UP000000"
        int code = root["code"].asInt();    // ���ʽڵ㣬code = 100
    }
    return 0;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    ParseJsonFromString();

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

    eglView->setFrameSize(1136, 640);

    int ret = CCApplication::sharedApplication()->run();

#ifdef USE_WIN32_CONSOLE
    FreeConsole();
#endif

    return ret;
}
