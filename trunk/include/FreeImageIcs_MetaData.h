#ifndef __FREEIMAGE_ALGORITHMS_METADATA__
#define __FREEIMAGE_ALGORITHMS_METADATA__

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeImageAlgorithms.h"
#include "FreeImageIcs_IO.h"

#define ICS_LINE_LENGTH 256

typedef int FreeImageIcsHistoryIterator;

/*
Determines if the ics file is opened in write mode.
*/
DLL_API int DLL_CALLCONV
FreeImageIcs_IsIcsFileInWriteMode(FreeImageIcsPointer fip);

/*
Get the number of history strings within the ics file.
*/
DLL_API int DLL_CALLCONV
FreeImageIcs_GetIcsHistoryStringCount(FreeImageIcsPointer fip);

/*
Get the iterator which points to the first history string.
*/
DLL_API FreeImageIcsHistoryIterator DLL_CALLCONV
FreeImageIcs_GetIcsHistoryBeginIterator(FreeImageIcsPointer fip);

/*
Get the iterator which points to the last history string.
*/
DLL_API FreeImageIcsHistoryIterator DLL_CALLCONV
FreeImageIcs_GetIcsHistoryEndIterator(FreeImageIcsPointer fip);

/*
Get the next iterator.
*/
DLL_API FreeImageIcsHistoryIterator DLL_CALLCONV
FreeImageIcs_GetIcsHistoryNextIterator(FreeImageIcsPointer fip);


/*
Split a history string into key and value.
*/
DLL_API int DLL_CALLCONV
FreeImageIcs_SplitHistoryString(char *history_string, char *key, char *value);

/*
Join a key and value to construct a well formatted ics history string.
*/
DLL_API int DLL_CALLCONV
FreeImageIcs_JoinKeyValueIntoHistoryString(char *history_string, char *key, char *value);

/*
Get the iterator which points to history string with the key specified.
*/
DLL_API FreeImageIcsHistoryIterator DLL_CALLCONV
FreeImageIcs_GetIcsHistoryIteratorForKey(FreeImageIcsPointer fip, FreeImageIcsHistoryIterator start_iterator, char *key);

/*
Get the string pointed to by iterator.
*/
DLL_API int DLL_CALLCONV
FreeImageIcs_GetIcsHistoryString(FreeImageIcsPointer fip, FreeImageIcsHistoryIterator iterator, char *history_string);

/*
Get the key and value pointed to by iterator.
*/
DLL_API int DLL_CALLCONV
FreeImageIcs_GetIcsHistoryKeyValue(FreeImageIcsPointer fip, FreeImageIcsHistoryIterator iterator, char *key, char *value);

/*
Get the first history string value for the corresponding key.
*/
DLL_API int DLL_CALLCONV
FreeImageIcs_GetFirstIcsHistoryValueWithKey(FreeImageIcsPointer fip, char *key, char *value);

/*
Sets all the history string in one go.
The var argument must end in NULL.
Old history strings are removed.
*/
DLL_API int DLL_CALLCONV
FreeImageIcs_SetIcsHistoryStrings(FreeImageIcsPointer fip, ...);

/*
Sets all the history keys and values in one go.
The var argument must end in NULL.
Old history strings are removed.
*/
DLL_API int DLL_CALLCONV
FreeImageIcs_SetIcsHistoryKeyValueStrings(FreeImageIcsPointer fip, ...);


/*
Add history_String.
*/
DLL_API int DLL_CALLCONV
FreeImageIcs_AddIcsHistoryString(FreeImageIcsPointer fip, char *history_string);

/*
Add key value to history.
*/
DLL_API int DLL_CALLCONV
FreeImageIcs_AddIcsHistoryKeyValue(FreeImageIcsPointer fip, char *key, char *value);


/*
Add more than one history strings in one go.
The var argument must end in NULL.
*/
DLL_API int DLL_CALLCONV
FreeImageIcs_AddIcsHistoryKeyValueStrings(FreeImageIcsPointer fip, ...);

/*
Add history strings stored in an array.
*/
DLL_API int DLL_CALLCONV
FreeImageIcs_SetIcsHistoryKeyValueStringsFromArray(FreeImageIcsPointer fip, char **history_strings, int number_of_items);

/*
Replace history string stored in an array.
*/
DLL_API int DLL_CALLCONV
FreeImageIcs_ReplaceIcsHistoryValueForKey(FreeImageIcsPointer fip, char *key, char *value);


#ifdef __cplusplus
}
#endif

#endif