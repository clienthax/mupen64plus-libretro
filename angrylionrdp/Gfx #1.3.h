/**********************************************************************************
Common gfx plugin spec, version #1.3 maintained by zilmar (zilmar@emulation64.com)

All questions or suggestions should go through the mailing list.
http://www.egroups.com/group/Plugin64-Dev
***********************************************************************************

Notes:
------

Setting the approprate bits in the MI_INTR_REG and calling CheckInterrupts which 
are both passed to the DLL in InitiateGFX will generate an Interrupt from with in 
the plugin.

The Setting of the RSP flags and generating an SP interrupt  should not be done in
the plugin

**********************************************************************************/
#ifndef _GFX_H_INCLUDED__
#define _GFX_H_INCLUDED__

#include <stdint.h>
#ifdef M64P_STATIC_PLUGINS
#define M64P_PLUGIN_PROTOTYPES 1
#include "m64p_types.h"
#include "m64p_plugin.h"

/***** Structures *****/
typedef struct {
    unsigned short Version;        /* Set to 0x0103 */
    unsigned short Type;           /* Set to PLUGIN_TYPE_GFX */
    char Name[100];      /* Name of the DLL */

    /* If DLL supports memory these memory options then set them to TRUE or FALSE
       if it does not support it */
    int NormalMemory;    /* a normal uint8_t array */ 
    int MemoryBswaped;  /* a normal uint8_t array where the memory has been pre
                              bswap on a dword (32 bits) boundry */
} PLUGIN_INFO;

#ifdef __cplusplus
extern "C" {
#endif

EXPORT m64p_error CALL PluginGetVersionVideo(m64p_plugin_type *PluginType, int *PluginVersion, int *APIVersion, const char **PluginNamePtr, int *Capabilities);

#ifdef __cplusplus
}
#endif

#else

#if defined(__cplusplus)
extern "C" {
#endif

/* Plugin types */
#define PLUGIN_TYPE_GFX             2

#define EXPORT                      
#define CALL                       

/***** Structures *****/
typedef struct {
    uint16_t Version;        /* Set to 0x0103 */
    uint16_t Type;           /* Set to PLUGIN_TYPE_GFX */
    char Name[100];      /* Name of the DLL */

    /* If DLL supports memory these memory options then set them to TRUE or FALSE
       if it does not support it */
    int NormalMemory;    /* a normal uint8_t array */ 
    int MemoryBswaped;  /* a normal uint8_t array where the memory has been pre
                              bswap on a dword (32 bits) boundry */
} PLUGIN_INFO;

typedef struct {

    unsigned char * HEADER;  // This is the rom header (first 40h bytes of the rom
                    // This will be in the same memory format as the rest of the memory.
    unsigned char * RDRAM;
    unsigned char * DMEM;
    unsigned char * IMEM;

    unsigned int * MI_INTR_REG;

    unsigned int * DPC_START_REG;
    unsigned int * DPC_END_REG;
    unsigned int * DPC_CURRENT_REG;
    unsigned int * DPC_STATUS_REG;
    unsigned int * DPC_CLOCK_REG;
    unsigned int * DPC_BUFBUSY_REG;
    unsigned int * DPC_PIPEBUSY_REG;
    unsigned int * DPC_TMEM_REG;

    unsigned int * VI_STATUS_REG;
    unsigned int * VI_ORIGIN_REG;
    unsigned int * VI_WIDTH_REG;
    unsigned int * VI_INTR_REG;
    unsigned int * VI_V_CURRENT_LINE_REG;
    unsigned int * VI_TIMING_REG;
    unsigned int * VI_V_SYNC_REG;
    unsigned int * VI_H_SYNC_REG;
    unsigned int * VI_LEAP_REG;
    unsigned int * VI_H_START_REG;
    unsigned int * VI_V_START_REG;
    unsigned int * VI_V_BURST_REG;
    unsigned int * VI_X_SCALE_REG;
    unsigned int * VI_Y_SCALE_REG;

    void (*CheckInterrupts)( void );

#ifdef HAVE_DIRECTDRAW
    int hWnd;          /* Render window */
    int hStatusBar;    /* if render window does not have a status bar then this is NULL */
#endif
#if 0
    int MemoryBswaped;    // If this is set to TRUE, then the memory has been pre
                           //   bswap on a dword (32 bits) boundry
                           //   eg. the first 8 bytes are stored like this:
                           //        4 3 2 1   8 7 6 5
#endif

} GFX_INFO;

extern GFX_INFO gfx_info;

/******************************************************************
  Function: ChangeWindow
  Purpose:  to change the window between fullscreen and window 
            mode. If the window was in fullscreen this should 
            change the screen to window mode and vice vesa.
  input:    none
  output:   none
*******************************************************************/ 
EXPORT void CALL ChangeWindow (void);

/******************************************************************
  Function: CloseDLL
  Purpose:  This function is called when the emulator is closing
            down allowing the dll to de-initialise.
  input:    none
  output:   none
*******************************************************************/ 
EXPORT void CALL CloseDLL (void);

/******************************************************************
  Function: DllAbout
  Purpose:  This function is optional function that is provided
            to give further information about the DLL.
  input:    a handle to the window that calls this function
  output:   none
*******************************************************************/ 
EXPORT void CALL DllAbout ( int hParent );

/******************************************************************
  Function: DllConfig
  Purpose:  This function is optional function that is provided
            to allow the user to configure the dll
  input:    a handle to the window that calls this function
  output:   none
*******************************************************************/ 
EXPORT void CALL DllConfig ( int hParent );

/******************************************************************
  Function: DllTest
  Purpose:  This function is optional function that is provided
            to allow the user to test the dll
  input:    a handle to the window that calls this function
  output:   none
*******************************************************************/ 
EXPORT void CALL DllTest ( int hParent );

/******************************************************************
  Function: DrawScreen
  Purpose:  This function is called when the emulator receives a
            WM_PAINT message. This allows the gfx to fit in when
            it is being used in the desktop.
  input:    none
  output:   none
*******************************************************************/ 
EXPORT void CALL DrawScreen (void);

/******************************************************************
  Function: GetDllInfo
  Purpose:  This function allows the emulator to gather information
            about the dll by filling in the PluginInfo structure.
  input:    a pointer to a PLUGIN_INFO stucture that needs to be
            filled by the function. (see def above)
  output:   none
*******************************************************************/ 
EXPORT void CALL GetDllInfo ( PLUGIN_INFO * PluginInfo );

/******************************************************************
  Function: InitiateGFX
  Purpose:  This function is called when the DLL is started to give
            information from the emulator that the n64 graphics
            uses. This is not called from the emulation thread.
  Input:    Gfx_Info is passed to this function which is defined
            above.
  Output:   TRUE on success
            FALSE on failure to initialise
             
  ** note on interrupts **:
  To generate an interrupt set the appropriate bit in MI_INTR_REG
  and then call the function CheckInterrupts to tell the emulator
  that there is a waiting interrupt.
*******************************************************************/ 
EXPORT int CALL InitiateGFX (GFX_INFO Gfx_Info);

/******************************************************************
  Function: MoveScreen
  Purpose:  This function is called in response to the emulator
            receiving a WM_MOVE passing the xpos and ypos passed
            from that message.
  input:    xpos - the x-coordinate of the upper-left corner of the
            client area of the window.
            ypos - y-coordinate of the upper-left corner of the
            client area of the window. 
  output:   none
*******************************************************************/ 
EXPORT void CALL MoveScreen (int xpos, int ypos);

/******************************************************************
  Function: ProcessDList
  Purpose:  This function is called when there is a Dlist to be
            processed. (High level GFX list)
  input:    none
  output:   none
*******************************************************************/ 
EXPORT void CALL ProcessDList(void);

/******************************************************************
  Function: ProcessRDPList
  Purpose:  This function is called when there is a Dlist to be
            processed. (Low level GFX list)
  input:    none
  output:   none
*******************************************************************/ 
EXPORT void CALL ProcessRDPList(void);

/******************************************************************
  Function: RomClosed
  Purpose:  This function is called when a rom is closed.
  input:    none
  output:   none
*******************************************************************/ 
EXPORT void CALL RomClosed (void);

/******************************************************************
  Function: RomOpen
  Purpose:  This function is called when a rom is open. (from the 
            emulation thread)
  input:    none
  output:   none
*******************************************************************/ 
EXPORT void CALL RomOpen (void);

/******************************************************************
  Function: ShowCFB
  Purpose:  Useally once Dlists are started being displayed, cfb is
            ignored. This function tells the dll to start displaying
            them again.
  input:    none
  output:   none
*******************************************************************/ 
EXPORT void CALL ShowCFB (void);

/******************************************************************
  Function: UpdateScreen
  Purpose:  This function is called in response to a vsync of the
            screen were the VI bit in MI_INTR_REG has already been
            set
  input:    none
  output:   none
*******************************************************************/ 
EXPORT void CALL UpdateScreen (void);

/******************************************************************
  Function: ViStatusChanged
  Purpose:  This function is called to notify the dll that the
            ViStatus registers value has been changed.
  input:    none
  output:   none
*******************************************************************/ 
EXPORT void CALL ViStatusChanged (void);

/******************************************************************
  Function: ViWidthChanged
  Purpose:  This function is called to notify the dll that the
            ViWidth registers value has been changed.
  input:    none
  output:   none
*******************************************************************/
EXPORT void CALL ViWidthChanged (void);

/******************************************************************
  Function: ReadScreen
  Purpose:  Capture the current screen
  Input:    none
  Output:   dest - 24-bit RGB data (flipped horizontally)
            width - width of image
            height - height of image
 ******************************************************************/
EXPORT void CALL ReadScreen (void **dest, int *width, int *height);

/******************************************************************
   NOTE: THIS HAS BEEN ADDED FOR MUPEN64PLUS AND IS NOT PART OF THE
         ORIGINAL SPEC
  Function: SetConfigDir
  Purpose:  To pass the location where config files should be read/
            written to.
  input:    path to config directory
  output:   none
*******************************************************************/
EXPORT void CALL SetConfigDir( char *configDir );

/******************************************************************
   NOTE: THIS HAS BEEN ADDED FOR MUPEN64PLUS AND IS NOT PART OF THE
         ORIGINAL SPEC
  Function: SetRenderingCallback
  Purpose:  Allows emulator to register a callback function that will
            be called by the graphics plugin just before the the
            frame buffers are swapped.
            This was added as a way for the emulator to draw emulator-
            specific things to the screen, e.g. On-screen display.
  input:    pointer to callback function. The function expects
            to receive the current window width and height.
  output:   none
*******************************************************************/
EXPORT void CALL SetRenderingCallback(void (*callback)());

#if defined(__cplusplus)
}
#endif
#endif
#endif


