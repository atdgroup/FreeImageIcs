#include "CuTest.h"
#include "FreeImageAlgorithms_IO.h"

#include "FreeImageIcs_IO.h"
#include "FreeImageIcs_MetaData.h"
#include "FreeImageIcs_Viewer.h"

#include <assert.h>

#define TEST_IMAGE_DIR "C:\\Documents and Settings\\Pierce\\Desktop\\Working Area\\Test Images\\"


static void
TestFreeImageIcs_SwapDimensionIcsTest(CuTest* tc)
{
	ICS *ics, *new_ics;
	Ics_Error err;
	FIBITMAP* fib;

	//char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\xyt.ics";
    char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\testy.ics";
    char *out_file = "C:\\Documents and Settings\\Pierce\\Desktop\\ics_swapped2.ics";

	err = FreeImageIcs_IcsOpen (&ics, file, "r");

    CuAssertTrue(tc, err == IcsErr_Ok);
	
    FreeImageIcs_SaveIcsFileWithDimensionsSwapped(ics, out_file, 0, 2);

    
	FreeImageIcs_IcsClose(ics);

    err = FreeImageIcs_IcsOpen (&ics, out_file, "r");

    CuAssertTrue(tc, err == IcsErr_Ok);

    FreeImageIcs_SaveIcsFileWithDimensionsSwapped(ics, "C:\\Documents and Settings\\Pierce\\Desktop\\ics_swapped_reversed.ics", 2, 0);


}

static void
TestFreeImageIcs_ReadMultiDimensionalGreyScale(CuTest* tc)
{
	ICS *ics;
	Ics_Error err;
	FIBITMAP* fib;

	char *file = TEST_IMAGE_DIR "multidimensional.ics";

	err = FreeImageIcs_IcsOpen (&ics, file, "r");

    CuAssertTrue(tc, err == IcsErr_Ok);

	fib = FreeImageIcs_LoadFIBFromIcsFile (ics);

	assert(fib != NULL);

	ShowImage(fib);

	FreeImage_Unload(fib);

	FreeImageIcs_IcsClose(ics);
}


static void
TestFreeImageIcs_ReadMultiDimensionalGreyScaleSlice(CuTest* tc)
{
	ICS *ics;
	Ics_Error err;
	int fiError;
	FIBITMAP* fib;

	char *file = TEST_IMAGE_DIR "multidimensional2.ics";

	err = FreeImageIcs_IcsOpen (&ics, file, "r");

    CuAssertTrue(tc, err == IcsErr_Ok);

	fib = FreeImageIcs_GetIcsImageXYDataSlice(ics, 1, 1);

	assert(fib != NULL);

	ShowImage(fib);

	FreeImage_Unload(fib);

	FreeImageIcs_IcsClose(ics);
}


static void
TestFreeImageIcs_ReadMultiDimensionalColour(CuTest* tc)
{
	ICS *ics;
	Ics_Error err;
	FIBITMAP* fib;

	char *file = TEST_IMAGE_DIR "colour_test.ics";

	err = FreeImageIcs_IcsOpen (&ics, file, "r");

	fib = FreeImageIcs_LoadFIBFromIcsFile (ics);

	assert(fib != NULL);

	ShowImage(fib);

	FreeImage_Unload(fib);

	FreeImageIcs_IcsClose(ics);
}


static void
TestFreeImageIcs_GetDimensionalDetailTest(CuTest* tc)
{
	ICS *ics;
	Ics_Error err;
	FIBITMAP* fib;
    int size;
    char order[50], label[50];

	char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\Copy of SarSeven_txy.ics";

	err = FreeImageIcs_IcsOpen (&ics, file, "r");

	FreeImageIcs_GetDimensionDetails (ics, 0, order, label, &size);

	FreeImageIcs_IcsClose(ics);
}


static void
TestFreeImageIcs_LoadTest(CuTest* tc)
{
	FIBITMAP *dib1, *dib2, *dib3;
	Ics_Error err;
	ICS *ics;

	char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\xyt.ics";

	err = FreeImageIcs_IcsOpen(&ics, file, "r");

	if(err != IcsErr_Ok)
		return;

	dib1 = FreeImageIcs_LoadFIBFromIcsFile(ics);
	
	CuAssertTrue(tc, dib1 != NULL);

	FreeImageAlgorithms_SaveFIBToFile (dib1, "C:\\Documents and Settings\\Pierce\\Desktop\\here.bmp", BIT8);

	FreeImage_Unload(dib1);
	FreeImageIcs_IcsClose(ics);
}


CuSuite*
CuGetFreeImageIcsTestSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	//SUITE_ADD_TEST(suite, TestFreeImageIcs_MetaDataAdd);
	SUITE_ADD_TEST(suite, TestFreeImageIcs_ReadMultiDimensionalGreyScale);
	//SUITE_ADD_TEST(suite, TestFreeImageIcs_ReadMultiDimensionalGreyScaleSlice);
	//SUITE_ADD_TEST(suite, TestFreeImageIcs_ReadMultiDimensionalColour);
	//SUITE_ADD_TEST(suite, TestFreeImageIcs_GetDimensionalDetailTest);
	//SUITE_ADD_TEST(suite, TestFreeImageIcs_LoadTest);
    //SUITE_ADD_TEST(suite, TestFreeImageIcs_SwapDimensionIcsTest);

	return suite;
}
