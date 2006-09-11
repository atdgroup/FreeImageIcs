#include "CuTest.h"

#include "libics.h"

#include "FreeImageAlgorithms_IO.h"

#include "FreeImageIcs_IO.h"
#include "FreeImageIcs_MetaData.h"
#include "FreeImageIcs_Viewer.h"

#include <assert.h>

#define TEST_IMAGE_DIR "C:\\Documents and Settings\\Pierce\\Desktop\\development\\Working Area\\Test Images\\"

static void
TestFreeImageIcs_MetaDataAdd(CuTest* tc)
{
	ICS *ics;
	Ics_Error err;
	int fiError;
	char value[200];

	char text[1000];

	char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\te.ics";

	memset(text, 0, 1000);
	FreeImageIcs_GetHistoryTextFromFile(file, text);


	err = IcsOpen (&ics, file, "rw");

	CuAssertTrue(tc, err == IcsErr_Ok);

//	fiError = FreeImageIcs_AddIcsHistoryKeyValueStrings(ics, "Glenn Key", "Glenn Value", NULL);

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
	FIBITMAP* fib;

	char *file = TEST_IMAGE_DIR "multidimensional.ics";

	err = IcsOpen (&ics, file, "r");

	fib = FreeImageIcs_LoadFIBFromIcsFile (ics);

	assert(fib != NULL);

	ShowImage(fib);

	FreeImage_Unload(fib);

	IcsClose(ics);
}


static void
TestFreeImageIcs_ReadMultiDimensionalGreyScaleSlice(CuTest* tc)
{
	ICS *ics, *save_ics;
	Ics_Error err;
	int fiError;
	char value[200];
	FIBITMAP* fib;

	char *file = TEST_IMAGE_DIR "multidimensional2.ics";

	err = IcsOpen (&ics, file, "r");

	fib = FreeImageIcs_GetIcsImageDataSlice(ics, 2, 15);

	assert(fib != NULL);

	ShowImage(fib);

	FreeImage_Unload(fib);

	IcsClose(ics);
}


static void
TestFreeImageIcs_ReadMultiDimensionalColour(CuTest* tc)
{
	ICS *ics, *save_ics;
	Ics_Error err;
	int fiError;
	char value[200];
	FIBITMAP* fib;

	char *file = TEST_IMAGE_DIR "colour_test.ics";

	err = IcsOpen (&ics, file, "r");

	fib = FreeImageIcs_LoadFIBFromIcsFile (ics);

	assert(fib != NULL);

	ShowImage(fib);

	FreeImage_Unload(fib);

	IcsClose(ics);
}


static void
TestFreeImageIcs_Read12BitIcs(CuTest* tc)
{
	ICS *ics, *save_ics;
	Ics_Error err;
	int fiError;
	char value[200];
	FIBITMAP* fib;

	char *file = TEST_IMAGE_DIR "12bittest.ics";

	err = IcsOpen (&ics, file, "r");

	fib = FreeImageIcs_LoadFIBFromIcsFile (ics);

	assert(fib != NULL);

	ShowImage(fib);

	FreeImage_Unload(fib);

	IcsClose(ics);
}

CuSuite*
CuGetFreeImageIcsTestSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFreeImageIcs_MetaDataAdd);
	SUITE_ADD_TEST(suite, TestFreeImageIcs_ReadMultiDimensionalGreyScale);
	SUITE_ADD_TEST(suite, TestFreeImageIcs_ReadMultiDimensionalGreyScaleSlice);
	SUITE_ADD_TEST(suite, TestFreeImageIcs_ReadMultiDimensionalColour);
	SUITE_ADD_TEST(suite, TestFreeImageIcs_Read12BitIcs);

	return suite;
}
