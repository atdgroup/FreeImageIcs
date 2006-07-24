#ifndef __FREEIMAGE_ALGORITHMS_ICS__
#define __FREEIMAGE_ALGORITHMS_ICS__

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeImageAlgorithms.h"
#include "libics.h"

DLL_API int DLL_CALLCONV
FreeImageIcs_IsIcsFile (const char *filepath);

DLL_API int DLL_CALLCONV
FreeImageIcs_NumberOfDimensions (ICS *ics);

DLL_API int DLL_CALLCONV
FreeImageIcs_GetLabelForDimension (ICS *ics, int dimension, char *label);

DLL_API int DLL_CALLCONV
FreeImageIcs_GetDimensionDetails (ICS *ics, int dimension, char* order, char *label, int* size);

DLL_API int DLL_CALLCONV
FreeImageIcs_IsIcsFileColourFile(ICS *ics);

DLL_API int DLL_CALLCONV
FreeImageIcs_OpenIcsFile(ICS *ics, const char *filepath, const char *access_mode);

DLL_API int DLL_CALLCONV
FreeImageIcs_CloseIcsFile(ICS *ics);

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageIcs_LoadFIBFromIcsFile (ICS *ics);

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageIcs_LoadFIBFromIcsFilePath (const char* filepath);

DLL_API int DLL_CALLCONV
FreeImageIcs_GetNumberOfDimensions (ICS *ics);

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageIcs_GetIcsImageDataSlice(ICS *ics, int dimension, int slice);

DLL_API int DLL_CALLCONV
FreeImageIcs_SaveGreyScaleImage (FIBITMAP *dib, const char *filepath);

DLL_API int DLL_CALLCONV
FreeImageIcs_SaveColourImage (FIBITMAP *dib, const char *filepath);

DLL_API int DLL_CALLCONV
FreeImageIcs_SaveImage (FIBITMAP *dib, const char *filepath);

#ifdef __cplusplus
}
#endif

#endif