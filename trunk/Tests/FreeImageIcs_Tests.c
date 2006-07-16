#include "CuTest.h"

#include "libics.h"

#include "FreeImageAlgorithms_IO.h"

#include "FreeImageIcs_IO.h"
#include "FreeImageIcs_MetaData.h"
#include "FreeImageIcs_Viewer.h"

#include <assert.h>

static void
TestFreeImageIcs_MetaDataAdd(CuTest* tc)
{
	ICS *ics;
	Ics_Error err;
	int fiError;
	char value[200];

	char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\rjl.ics";

	err = IcsOpen (&ics, file, "rw");

	CuAssertTrue(tc, err == IcsErr_Ok);

	fiError = FreeImageIcs_AddIcsHistoryKeyValueStrings(ics, "Glenn Key", "Glenn Value", NULL);

	FreeImageIcs_GetFirstIcsHistoryValueWithKey(ics, "Glenn Key", value);

	CuAssertTrue(tc, strcmp(value, "Glenn Value") == 0);

	IcsClose(ics);
}


static void
TestFreeImageIcs_ReadMultiDimensionalGreyScale(CuTest* tc)
{
	ICS *ics, *save_ics;
	Ics_Error err;
	int fiError;
	char value[200];
	FIBITMAP* fib, *fib2;
	int dims[2] = {500,500};

	char *file = "C:\\Documents and Settings\\Glenn\\Desktop\\test16bit.ics";
	char *save_file = "C:\\Documents and Settings\\Glenn\\Desktop\\newtesty.ics";

	err = IcsOpen (&ics, file, "r");

	fib = FreeImageIcs_LoadFIBFromIcsFile (ics, 0);

	assert(fib != NULL);

	ShowImage(fib);

	FreeImageIcs_SaveImage(fib, save_file);
	
	FreeImage_Unload(fib);

	IcsClose(ics);
}


CuSuite*
CuGetFreeImageIcsTestSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFreeImageIcs_MetaDataAdd);
	SUITE_ADD_TEST(suite, TestFreeImageIcs_ReadMultiDimensionalGreyScale);

	return suite;
}
