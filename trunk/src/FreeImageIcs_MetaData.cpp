#include "FreeImageIcs_MetaData.h"
#include "FreeImageIcs_IO.h"

#include <iostream>

static char*
str_strlwr(char *s)
{
   if (s != NULL)
   {
      char *p;

      for (p = s; *p && p; p++)
         *p = tolower(*p);
   }
   
   return s;
}

int DLL_CALLCONV
FreeImageIcs_GetIcsHistoryStringCount(ICS *ics)
{
	int number_of_history_strings;
	
	/* Get ICS history */
	if (IcsGetNumHistoryStrings  (ics, &number_of_history_strings) != IcsErr_Ok)
		return 0;

	if(!number_of_history_strings)
		return 0;
	
	return number_of_history_strings;
}


int DLL_CALLCONV
FreeImageIcs_SplitHistoryString(char *history_string, char *key, char *value)
{
	char *s;

	if((s = strchr (history_string, 0x09)) == NULL) // Finds the splitting character
		return FREEIMAGE_ALGORITHMS_ERROR;
	
	strncpy (key, history_string, (s - history_string)); // Copy the key
	key[(s - history_string)] = '\0'; // Add NULL char
	
	strcpy(value, (s+1));  // copy the value

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


int DLL_CALLCONV
FreeImageIcs_JoinKeyValueIntoHistoryString(char *history_string, char *key, char *value)
{
	strcpy(history_string, key);

	size_t length = strlen(history_string);
	
	history_string[length] = 0x09;
	history_string[length + 1] = '\0';
	strcat(history_string, value);

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


int DLL_CALLCONV
FreeImageIcs_GetFirstIcsHistoryValueWithKey(ICS *ics, char *key, char *value)
{
	Ics_HistoryIterator it;     

	if(IcsNewHistoryIterator (ics, &it, key) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;	

	if(IcsGetHistoryKeyValueI (ics, &it, NULL, value) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;	

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


int DLL_CALLCONV
FreeImageIcs_ReplaceIcsHistoryValueForKey(ICS *ics, char *key, char *value)
{
	Ics_HistoryIterator it;     

	if(IcsNewHistoryIterator (ics, &it, key) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;	

	if(IcsReplaceHistoryStringI (ics, &it, key, value) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;	

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


int DLL_CALLCONV
FreeImageIcs_SetIcsHistoryKeyValueStrings(ICS *ics, ...)
{
	IcsDeleteHistory (ics, NULL);

	char *history_key, *history_string;

	va_list ap;
	va_start(ap, ics);

	while((history_key = va_arg(ap, char*)) != NULL)
	{
		history_string = va_arg(ap, char*);
		IcsAddHistory (ics, history_key, history_string);
	}
			
	va_end(ap);

	return FREEIMAGE_ALGORITHMS_SUCCESS; 
}


int DLL_CALLCONV
FreeImageIcs_SetIcsHistoryStrings(ICS *ics, ...)
{
	IcsDeleteHistory (ics, NULL);

	char *history_string;

	va_list ap;
	va_start(ap, ics);

	while((history_string = va_arg(ap, char*)) != NULL)
		IcsAddHistory (ics, NULL, history_string);
			
	va_end(ap);

	return FREEIMAGE_ALGORITHMS_SUCCESS; 
}


int DLL_CALLCONV
FreeImageIcs_SetIcsHistoryKeyValueStringsFromArray(ICS *ics, char **history_strings, int number_of_items)
{
	IcsDeleteHistory (ics, NULL);

	for(int i=0; i < number_of_items; i++)
		IcsAddHistory (ics, NULL, history_strings[i]);	

	return FREEIMAGE_ALGORITHMS_SUCCESS; 
}


int DLL_CALLCONV
FreeImageIcs_AddIcsHistoryKeyValueStrings(ICS *ics, ...)
{
	char **history_strings = NULL;

	char *history_key, *history_string;

	va_list ap;
	va_start(ap, ics);

	while((history_key = va_arg(ap, char*)) != NULL)
	{
		history_string = va_arg(ap, char*);
		IcsAddHistory (ics, history_key, history_string);
	}
			
	va_end(ap);

	return FREEIMAGE_ALGORITHMS_SUCCESS;  
}