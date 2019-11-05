#include "xdelta3/xdelta3.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>

#include <gccore.h>
#include <wiiuse/wpad.h>
#include <fat.h>
#include <sdcard/wiisd_io.h>

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

// Filesystem directory listing (for debugging)
void dirlist(char* path);


// Filesystem Initialization / Deinitialization
void SDCard_Init() {
	__io_wiisd.startup();
	fatMountSimple("sd", &__io_wiisd);
}

void SDCard_DeInit() {
	fatUnmount("sd");
	__io_wiisd.shutdown();
}

void InitUSB() {
	fatUnmount("usb:/"); 
	bool isMounted = fatMountSimple("usb", &__io_usbstorage);
	 
	if(!isMounted) {
		fatUnmount("usb:/");
		fatMountSimple("usb", &__io_usbstorage);
		 
		bool isInserted = __io_usbstorage.isInserted();
		 
		if(isInserted) {
			int retry = 10;
			 
			while(retry) { 
				isMounted = fatMountSimple("usb", &__io_usbstorage);
				if (isMounted) break;
				sleep(1);
				retry--;
			}
		}          
	}
}

void DeInitUSB() {
	fatUnmount("usb:/");
	__io_usbstorage.shutdown(); 
}

// Program Exit Functions
int waitForHomeToExit();
int programEnd(int loadedDevice);