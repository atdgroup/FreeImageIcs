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
	int line, pitch, width;
	int test;

	//char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\rjl.ics";
	char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\shit.ics";
	char *save_file = "C:\\Documents and Settings\\Pierce\\Desktop\\problem2.ics";

	err = IcsOpen (&ics, file, "r");

	fib = FreeImageIcs_LoadFIBFromIcsFile (ics, 0);

	assert(fib != NULL);

	// Resample
	fib2 = FreeImage_Rescale(fib, 200, 200, FILTER_BOX);


	width = FreeImage_GetWidth(fib2);
	line = FreeImage_GetLine(fib2);
	pitch = FreeImage_GetPitch(fib2);

	//ShowImage(fib2);

	FreeImageIcs_SaveImage(fib2, save_file);
	
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
