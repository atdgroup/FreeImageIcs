#include "CuTest.h"

#include "libics.h"

#include "FreeImageAlgorithms_IO.h"

#include "FreeImageIcs_IO.h"
#include "FreeImageIcs_MetaData.h"
#include "FreeImageIcs_Viewer.h"

#include <assert.h>

#define TEST_IMAGE_DIR "C:\\Documents and Settings\\Pierce\\Desktop\\Working Area\\Test Images\\"

static void
TestFreeImageIcs_MetaDataAdd(CuTest* tc)
{
	ICS *ics;
	Ics_Error err;
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
};

static void
TestFreeImageIcs_SwapDimensionIcsTest(CuTest* tc)
{
	ICS *ics;
	Ics_Error err;
	FIBITMAP* fib;

	char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\SarSeven_xyt.ics";

	err = IcsOpen (&ics, file, "r");

    CuAssertTrue(tc, err == IcsErr_Ok);

    FreeImageIcs_SaveIcsFileWithDimensionsSwapped(ics, "C:\\Documents and Settings\\Pierce\\Desktop\\ics_swapped.ics",
        0, 2);

	IcsClose(ics);
}

static void
TestFreeImageIcs_ReadMultiDimensionalGreyScale(CuTest* tc)
{
	ICS *ics;
	Ics_Error err;
	FIBITMAP* fib;

	char *file = TEST_IMAGE_DIR "multidimensional.ics";

	err = IcsOpen (&ics, file, "r");

    CuAssertTrue(tc, err == IcsErr_Ok);

	fib = FreeImageIcs_LoadFIBFromIcsFile (ics);

	assert(fib != NULL);

	ShowImage(fib);

	FreeImage_Unload(fib);

	IcsClose(ics);
}


static void
TestFreeImageIcs_ReadMultiDimensionalGreyScaleSlice(CuTest* tc)
{
	ICS *ics;
	Ics_Error err;
	int fiError;
	FIBITMAP* fib;

	char *file = TEST_IMAGE_DIR "multidimensional2.ics";

	err = IcsOpen (&ics, file, "r");

    CuAssertTrue(tc, err == IcsErr_Ok);

	fib = FreeImageIcs_GetIcsImageXYDataSlice(ics, 1, 1);

	assert(fib != NULL);

	ShowImage(fib);

	FreeImage_Unload(fib);

	IcsClose(ics);
}


static void
TestFreeImageIcs_ReadMultiDimensionalColour(CuTest* tc)
{
	ICS *ics;
	Ics_Error err;
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
	ICS *ics;
	Ics_Error err;
	FIBITMAP* fib;

	char *file = TEST_IMAGE_DIR "12bittest.ics";

	err = IcsOpen (&ics, file, "r");

	fib = FreeImageIcs_LoadFIBFromIcsFile (ics);

	assert(fib != NULL);

	ShowImage(fib);

	FreeImage_Unload(fib);

	IcsClose(ics);
}


static void
TestFreeImageIcs_ResizeTest(CuTest* tc)
{
	FIBITMAP *dib1, *dib2, *dib3;
	Ics_Error err;
	ICS *ics;

	char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\test\\what.ics";
	//char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\test\\rjl.ics";

	err = IcsOpen(&ics, file, "r");

	if(err != IcsErr_Ok)
		return;

	dib1 = FreeImageIcs_LoadFIBFromIcsFile(ics);

	dib3 = FreeImage_ConvertToStandardType(dib1, 1);

	dib2 = FreeImage_Rescale(dib3, 64, 64, FILTER_BOX);  

	
	CuAssertTrue(tc, dib1 != NULL);
	CuAssertTrue(tc, dib2 != NULL);

	FreeImageAlgorithms_SaveFIBToFile (dib1, "C:\\temp\\1.bmp", BIT24);
	FreeImageAlgorithms_SaveFIBToFile (dib2, "C:\\temp\\2.bmp", BIT24);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);

	IcsClose(ics);
}


CuSuite*
CuGetFreeImageIcsTestSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	//SUITE_ADD_TEST(suite, TestFreeImageIcs_MetaDataAdd);
	//SUITE_ADD_TEST(suite, TestFreeImageIcs_ReadMultiDimensionalGreyScale);
	//SUITE_ADD_TEST(suite, TestFreeImageIcs_ReadMultiDimensionalGreyScaleSlice);
	//SUITE_ADD_TEST(suite, TestFreeImageIcs_ReadMultiDimensionalColour);
	//SUITE_ADD_TEST(suite, TestFreeImageIcs_Read12BitIcs);
	//SUITE_ADD_TEST(suite, TestFreeImageIcs_ResizeTest);
    SUITE_ADD_TEST(suite, TestFreeImageIcs_SwapDimensionIcsTest);

	return suite;
}
