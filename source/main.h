static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

// Filesystem directory listing (for debugging)
void dirlist(char* path);

// FAT Filesystem Initialization / Deinitialization
void SDCard_Init();
void SDCard_DeInit();
void InitUSB();
void DeInitUSB();

// Program Exit Functions
int waitForHomeToExit();
int programEnd(int loadedDevice);