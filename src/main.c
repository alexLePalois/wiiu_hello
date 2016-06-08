#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include "dynamic_libs/os_functions.h"
#include "dynamic_libs/vpad_functions.h"

static char * consoleArrayTv;
static char * consoleArrayDrc;

void console_printf(const char *format, ...)
{
	char * tmp = NULL;

	va_list va;
	va_start(va, format);
	if((vasprintf(&tmp, format, va) >= 0) && tmp)
	{
		if(strlen(tmp) > 79)
			tmp[79] = 0;

		consoleArrayTv = (char*)malloc(strlen(tmp) + 1);
		if(consoleArrayTv)
			strcpy(consoleArrayTv, tmp);

		consoleArrayDrc = (tmp);
	}
	va_end(va);

	// Clear screens
	OSScreenClearBufferEx(0, 0);
	OSScreenClearBufferEx(1, 0);

	if(consoleArrayTv)
		OSScreenPutFontEx(0, 0, 0, consoleArrayTv);
	if(consoleArrayDrc)
		OSScreenPutFontEx(1, 0, 0, consoleArrayDrc);

	OSScreenFlipBuffersEx(0);
	OSScreenFlipBuffersEx(1);
}

/* Entry point */
int Menu_Main(void)
{
	//!*******************************************************************
	//!                   Initialize function pointers                   *
	//!*******************************************************************
	//! do OS (for acquire) and sockets first so we got logging
	InitOSFunctionPointers();
	InitVPadFunctionPointers();

	consoleArrayTv = NULL;
	consoleArrayDrc = NULL;

	VPADInit();

	// Prepare screen
	int screen_buf0_size = 0;

	// Init screen and screen buffers
	OSScreenInit();
	screen_buf0_size = OSScreenGetBufferSizeEx(0);
	OSScreenSetBufferEx(0, (void *)0xF4000000);
	OSScreenSetBufferEx(1, (void *)(0xF4000000 + screen_buf0_size));

	OSScreenEnableEx(0, 1);
	OSScreenEnableEx(1, 1);

	// Clear screens
	OSScreenClearBufferEx(0, 0);
	OSScreenClearBufferEx(1, 0);

	// Flip buffers
	OSScreenFlipBuffersEx(0);
	OSScreenFlipBuffersEx(1);

	console_printf("Hello Word! (Home Button to quit)");

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

	//! free memory
	if(consoleArrayTv) free(consoleArrayTv);
	if(consoleArrayDrc) free(consoleArrayDrc);

	return EXIT_SUCCESS;
}

