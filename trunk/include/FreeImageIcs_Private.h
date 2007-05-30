#ifndef __FREEIMAGE_ALGORITHMS_PRIVATE__
#define __FREEIMAGE_ALGORITHMS_PRIVATE__

#include "FreeImageIcs_IO.h"

extern const char* const Errors[];

FREE_IMAGE_TYPE DLL_CALLCONV
IcsTypeToFreeImageType (Ics_DataType icsType);

Ics_DataType DLL_CALLCONV
FreeImageTypeToIcsType (FREE_IMAGE_TYPE fit);

int DLL_CALLCONV
GetIcsDataTypeBPP (Ics_DataType icsType);

#endif

