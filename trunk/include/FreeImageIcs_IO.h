#ifndef __FIA_ICS__
#define __FIA_ICS__

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

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageIcs_LoadFIBFromIcsFile (ICS *ics);

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageIcs_LoadFIBFromIcsFilePath (const char* filepath);

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageIcs_LoadFIBFromColourIcsFile (ICS *ics);

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageIcs_GetIcsImageDimensionalDataSlice(ICS *ics, int dimension, int slice);

DLL_API int DLL_CALLCONV
FreeImageIcs_SaveImage (FIBITMAP *dib, const char *filepath, int save_metadata);

DLL_API int DLL_CALLCONV
FreeImageIcs_SaveIcsDataToFile (const char *filepath, void *data, Ics_DataType dt, int ndims, int *dims);

DLL_API int DLL_CALLCONV
FreeImageIcs_SaveIcsImageDataToBinaryFile (const char *filepath, const char *output_filepath);

// Abstracts Ics files which have to be the __cdcel calling convention
DLL_API Ics_Error DLL_CALLCONV
FreeImageIcs_IcsOpen (ICS* *ics, char const* filename, char const* mode);

DLL_API Ics_Error DLL_CALLCONV
FreeImageIcs_IcsClose (ICS* ics);

DLL_API Ics_Error DLL_CALLCONV
FreeImageIcs_IcsAddHistoryString (ICS* ics, char const* key, char const* value);

DLL_API Ics_Error DLL_CALLCONV
FreeImageIcs_IcsSetNativeScale (ICS* ics, int dimension, double origin, double scale, const char *units);

DLL_API Ics_Error DLL_CALLCONV
FreeImageIcs_IcsSetNativeIntensityScale (ICS* ics, double origin, double scale, const char *units);

DLL_API Ics_Error DLL_CALLCONV
FreeImageIcs_IcsNewHistoryIterator (ICS* ics, Ics_HistoryIterator* it, char const* key);

DLL_API Ics_Error DLL_CALLCONV
FreeImageIcs_IcsGetHistoryStringI (ICS* ics, Ics_HistoryIterator* it, char* string);

DLL_API int DLL_CALLCONV
FreeImageIcs_SaveIcsFileWithDimensionsAs(ICS *ics, const char *filepath, size_t* order, int size);

DLL_API int DLL_CALLCONV
FreeImageIcs_SaveIcsFileWithFirstTwoDimensionsAs(ICS *ics, const char *filepath, int first, int second);

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageIcs_SumIntensityProjection(ICS *ics, int dimension);

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageIcs_MaximumIntensityProjection(ICS *ics, int dimension);

#ifdef __cplusplus
}
#endif

#endif