#ifndef __FIA_METADATA__
#define __FIA_METADATA__

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeImageIcs_IO.h"

DLL_API int DLL_CALLCONV
FreeImageIcs_GetIcsHistoryStringCount(ICS *ics);

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
Delete all history in a file.
*/
DLL_API int DLL_CALLCONV
FreeImageIcs_IcsDeleteHistory(ICS *ics);

/*
Sets all the history string in one go.
The var argument must end in NULL.
Old history strings are removed.
*/
DLL_API int DLL_CALLCONV
FreeImageIcs_SetIcsHistoryStrings(ICS *ics, ...);

/*
Sets all the history keys and values in one go.
The var argument must end in NULL.
Old history strings are removed.
*/
DLL_API int DLL_CALLCONV
FreeImageIcs_SetIcsHistoryKeyValueStrings(ICS *ics, ...);


/*
Add more than one history strings in one go.
The var argument must end in NULL.
*/
DLL_API int DLL_CALLCONV
FreeImageIcs_AddIcsHistoryKeyValueStrings(ICS *ics, ...);

/*
Add history strings stored in an array.
*/
DLL_API int DLL_CALLCONV
FreeImageIcs_SetIcsHistoryKeyValueStringsFromArray(ICS *ics, char **history_strings, int number_of_items);

/*
Replace history string stored in an array.
*/
DLL_API int DLL_CALLCONV
FreeImageIcs_ReplaceIcsHistoryValueForKey(ICS *ics, const char *key, const char *value);


/*
Get the first history string value for the corresponding key.
*/
DLL_API int DLL_CALLCONV
FreeImageIcs_GetFirstIcsHistoryValueWithKey(ICS *ics, char *key, char *value);


/*
Is the Iterator valid
*/
DLL_API int DLL_CALLCONV
IsIcsHistoryIteratorValid (ICS* ics, Ics_HistoryIterator* it);


DLL_API int DLL_CALLCONV
FreeImageIcs_GetHistoryText(ICS *ics, char *text);


DLL_API int DLL_CALLCONV
FreeImageIcs_GetHistoryTextFromFile(const char *filepath, char *text);


DLL_API int DLL_CALLCONV
FreeImageIcs_CopyHistoryText(ICS *ics, ICS *dst_ics);

#ifdef __cplusplus
}
#endif

#endif