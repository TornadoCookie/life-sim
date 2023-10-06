#include "network.hpp"
#include <curl/curl.h>

void InitNetwork()
{
    static bool inited = false;

    if (!inited) curl_global_init(CURL_GLOBAL_DEFAULT);
    inited = true;
}

void StopNetwork()
{
    static bool stopped = false;

    if (!stopped) curl_global_cleanup();
    stopped = true;
}
