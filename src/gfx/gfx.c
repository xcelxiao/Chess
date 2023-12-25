#include "gfx.h"
#include <fileioc.h>

#define gfx_HEADER_SIZE 0

unsigned char *gfx_appvar[13] =
{
    (unsigned char*)0,
    (unsigned char*)64,
    (unsigned char*)395,
    (unsigned char*)568,
    (unsigned char*)872,
    (unsigned char*)1154,
    (unsigned char*)1329,
    (unsigned char*)1730,
    (unsigned char*)2066,
    (unsigned char*)2205,
    (unsigned char*)2477,
    (unsigned char*)2701,
    (unsigned char*)2837,
};

unsigned char gfx_init(void)
{
    unsigned int data, i;
    uint8_t appvar;

    appvar = ti_Open("gfx", "r");
    if (appvar == 0)
    {
        return 0;
    }

    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)gfx_appvar[0] + gfx_HEADER_SIZE;
    for (i = 0; i < 13; i++)
    {
        gfx_appvar[i] += data;
    }

    ti_Close(appvar);

    return 1;
}

