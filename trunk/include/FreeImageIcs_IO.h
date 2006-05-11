#ifndef __FREEIMAGE_ALGORITHMS_ICS__
#define __FREEIMAGE_ALGORITHMS_ICS__

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeImageAlgorithms.h"


/* Defines 'FreeImageIcsPointer' as a pointer to a structure that is not
 * defined here and so is not accessible to the outside world
 */
typedef struct FreeImageIcsStr FreeImageIcs, *FreeImageIcsPointer;

DLL_API int DLL_CALLCONV
FreeImageIcs_IsIcsFile (const char *filepath);

DLL_API int DLL_CALLCONV
FreeImageIcs_NumberOfDimensions (FreeImageIcsPointer fip);

DLL_API int DLL_CALLCONV
FreeImageIcs_GetLabelForDimension (FreeImageIcsPointer fip, int dimension, char *label);

DLL_API int DLL_CALLCONV
FreeImageIcs_GetDimensionDetails (FreeImageIcsPointer fip, int dimension, char* order, char *label, int* size);

DLL_API int DLL_CALLCONV
FreeImageIcs_IsIcsFileColourFile(FreeImageIcsPointer fip);

DLL_API int DLL_CALLCONV 
FreeImageIcs_OpenExistingIcsFileInWriteMode(FreeImageIcsPointer *fip, const char *filepath, int maintain_history);

DLL_API int DLL_CALLCONV
FreeImageIcs_OpenIcsFile(FreeImageIcsPointer *fip, const char *filepath, const char *access_mode);

DLL_API int DLL_CALLCONV
FreeImageIcs_CloseIcsFile(FreeImageIcsPointer fip);

DLL_API int DLL_CALLCONV
IsIcsFilePaddded (char *filepath);

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageIcs_LoadFIBFromIcsFile (FreeImageIcsPointer fip, int padded);

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageIcs_LoadFIBFromIcsFilePath (const char* filepath, int padded);

DLL_API int DLL_CALLCONV
FreeImageIcs_SaveFIBToIcsFile (FIBITMAP *dib, const char *pathname);

DLL_API FIBITMAP* DLL_CALLCONV
GetIcsDimensionXYImage(FreeImageIcsPointer fip, ...);

DLL_API FIBITMAP* DLL_CALLCONV
GetIcsXYImageForDimensionSlice(FreeImageIcsPointer fip, int dimension, int slice);

DLL_API int DLL_CALLCONV
FreeImageIcs_GetNumberOfDimensions (FreeImageIcsPointer fip);

#ifdef __cplusplus
}
#endif

#endif