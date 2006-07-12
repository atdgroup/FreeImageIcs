#include "CuTest.h"

#include "libics.h"

#include "FreeImageIcs_IO.h"
#include "FreeImageIcs_MetaData.h"

#include "FreeImageIcs_Viewer.h"

#include "FreeImageAlgorithms_IO.h"

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
	FIBITMAP* fib;
	int dims[2] = {500,500};

	//char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\test.jpg";

	//char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\development\\Working Area\\Test Images\\multidimensional.ics";
	char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\development\\Working Area\\Test Images\\colour_test.ics";
	//char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\development\\Working Area\\Test Images\\ics_test.ics";
	char *save_file = "C:\\Documents and Settings\\Pierce\\Desktop\\newtest.ics";

	err = IcsOpen (&ics, file, "r");

	//

//	CuAssertTrue(tc, err == IcsErr_Ok);

	//fib = GetIcsDimensionXYImageData(ics, 0, -1);
	fib = FreeImageIcs_LoadFIBFromIcsFile (ics, 0);
	//fib = GetIcsImageDataSlice(ics, 1, 0);

	//fib = GetIcsXYImageForDimensionSlice(ics, 2, 0);

	assert(fib != NULL);

	//fib = FreeImageAlgorithms_LoadFIBFromFile(file);

	ShowImage(fib);


	//FreeImageIcs_SaveGreyScaleImage (fib, save_file);

	FreeImageIcs_SaveColourImage (fib, save_file);

	//save_ics = FreeImage_IcsCreateIcsFile(save_file, Ics_uint8, 2, dims, 0);


	//SetIcsImageDataSlice(save_ics, fib, 1, 0);



	//SetIcsImageDataSlice(ICS *ics, FIBITMAP *dib, int dimension, int slice)


	//FreeImageAlgorithms_SaveFIBToFile(fib, "C:\\Documents and Settings\\Pierce\\Desktop\\test.jpg");

	//FreeImage_Unload(fib);


	//fib = GetIcsXYImageForDimensionSlice(ics, 2, 30);

	//FreeImageAlgorithms_SaveFIBToFile(fib, "C:\\Documents and Settings\\Pierce\\Desktop\\test.jpg");

	//ShowImage(fib);

	FreeImage_Unload(fib);

	IcsClose(ics);
	//
}


CuSuite*
CuGetFreeImageIcsTestSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFreeImageIcs_MetaDataAdd);
	SUITE_ADD_TEST(suite, TestFreeImageIcs_ReadMultiDimensionalGreyScale);

	return suite;
}
