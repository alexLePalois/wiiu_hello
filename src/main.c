#include <stdlib.h>
#include "dynamic_libs/os_functions.h"
#include "dynamic_libs/vpad_functions.h"

/* Entry point */
int Menu_Main(void)
{
    //!*******************************************************************
    //!                   Initialize function pointers                   *
    //!*******************************************************************
    //! do OS (for acquire) and sockets first so we got logging
    InitOSFunctionPointers();
    InitVPadFunctionPointers();

    VPADInit();

    int vpadError = -1;
    VPADData vpad;
    int vpadReadCounter = 0;

    while(1)
    {
        //! update only at 50 Hz, thats more than enough
        if(++vpadReadCounter >= 20)
        {
            vpadReadCounter = 0;

            VPADRead(0, &vpad, 1, &vpadError);

            if(vpadError == 0 && ((vpad.btns_d | vpad.btns_h) & VPAD_BUTTON_HOME))
                break;
        }

		usleep(1000);
    }

    return EXIT_SUCCESS;
}

