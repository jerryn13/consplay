#include "../../dependency/fetch/fetch.c"

int main()
{
    fetchParams params = {
        url : L"https://blog.csdn.net/tangketan/article/details/7760446",
        method : L"GET",
        body : L"",
        timeout : 10000,
        enableSSE : 0,
        onSSEChunkReceived : NULL
    };

    fetchResponse res = fetch(&params);

    printf("%s", res.body);

    return 0;
}