#include <gccore.h>
#include <wiiuse/wpad.h>
#include <fat.h>
#include <sdcard/wiisd_io.h>

#include "main_wii.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>

//---------------------------------------------------------------------------------
int main(int argc, char **argv) {
//---------------------------------------------------------------------------------

	// Initialise the video system
	VIDEO_Init();

	// This function initialises the attached controllers
	WPAD_Init();

	// Obtain the preferred video mode from the system
	// This will correspond to the settings in the Wii menu
	rmode = VIDEO_GetPreferredMode(NULL);

	// Allocate memory for the display in the uncached region
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

	// Initialise the console, required for printf
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);

	// Set up the video registers with the chosen mode
	VIDEO_Configure(rmode);

	// Tell the video hardware where our display memory is
	VIDEO_SetNextFramebuffer(xfb);

	// Make the display visible
	VIDEO_SetBlack(FALSE);

	// Flush the video register changes to the hardware
	VIDEO_Flush();

	// Wait for Video setup to complete
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();


	// Position cursor on row 2, column 0
	printf("\x1b[2;0H");

	
	// Select device (SD or USB)
	int deviceSelect = 0;
	printf("Select the device with your game and patch files\n\n");
	printf("\tPress [1] for Front SD\n");
	printf("\tPress [2] for USB\n");
	printf("\tPress [HOME] to exit\n\n");
	
	while (deviceSelect == 0) {
		WPAD_ScanPads();
		u32 pressed = WPAD_ButtonsDown(0);
		if (pressed & WPAD_BUTTON_1) {
			deviceSelect = 1;
		} else if (pressed & WPAD_BUTTON_2) {
			deviceSelect = 2;
		} else if (pressed & WPAD_BUTTON_HOME) {
			printf("Exiting...\n");
			exit(0);
		} else if (pressed) {
			printf("Invalid input.\n");
		}
	}
	
	switch (deviceSelect) {
		// SD
		case 1: printf("SD Card selected.\n\n");
			// Initialize SD
			printf("Mounting SD...\n\n");
			SDCard_Init();
			
			// List SD Contents
			dirlist("sd:/");
			
			// Deinitialize SD
			printf("Unmounting SD...\n\n");
			SDCard_DeInit();
			
			break;
			
		//USB
		case 2: printf("USB selected.\n\n");
			// Initialize USB
			printf("Mounting USB...\n\n");
			InitUSB();
			
			// List USB Contents
			dirlist("usb:/");
			
			// Deinitialize USB
			DeInitUSB();
			
			break;
	}
	
	printf("Press [HOME] to exit.\n\n");
	return waitForHomeToExit();
}

int waitForHomeToExit() {
	//Wait for user to press HOME button to exit
	while(1) {

		// Call WPAD_ScanPads each loop, this reads the latest controller states
		WPAD_ScanPads();

		// WPAD_ButtonsDown tells us which buttons were pressed in this loop
		// this is a "one shot" state which will not fire again until the button has been released
		u32 pressed = WPAD_ButtonsDown(0);

		// We return to the launcher application via exit
		if ( pressed & WPAD_BUTTON_HOME ) {
			printf("Exiting...\n");
			exit(0);
		}

		// Wait for the next frame
		VIDEO_WaitVSync();
	}
	
	return 0;
}

void dirlist(char* path) {
	DIR* pdir = opendir(path);

	if (pdir != NULL) {
		while(true) {
			struct dirent* pent = readdir(pdir);
			if(pent == NULL) break;
			
			if(strcmp(".", pent->d_name) != 0 && strcmp("..", pent->d_name) != 0) {
				char dnbuf[260];
				sprintf(dnbuf, "%s/%s", path, pent->d_name);
				
				struct stat statbuf;
				stat(dnbuf, &statbuf);
				
				if(S_ISDIR(statbuf.st_mode)) {
					printf("\t%s <DIR>\n", dnbuf);
					dirlist(dnbuf);
				} else {
					printf("\t%s (%d)\n", dnbuf, (int)statbuf.st_size);
				}
				
			}
		}
		printf("\n");
		closedir(pdir);
	}
	else {
		printf("opendir() failure.\n\n");
	}
}

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