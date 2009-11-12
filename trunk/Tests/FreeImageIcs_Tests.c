#include "CuTest.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_LinearScale.h"

//#include "BasicWin32Window.h"

#include "FreeImageIcs_IO.h"
#include "FreeImageIcs_MetaData.h"
#include "FreeImageIcs_Viewer.h"

#include <assert.h>

#define TEST_IMAGE_DIR "C:\\Documents and Settings\\Pierce\\Desktop\\Working Area\\Test Images\\"



static void
TestFreeImageIcs_SaveIcsTest(CuTest* tc)
{
	ICS *ics = NULL, *new_ics;
	int err;
	FIBITMAP* fib;

    char *out_file = "C:\\Documents and Settings\\Pierce\\Desktop\\test.ics";

    fib = FreeImage_AllocateT(FIT_FLOAT, 800, 600, 32, 0, 0, 0);

FreeImageIcs_SaveImage (fib, out_file, 1);  
}

static void
TestFreeImageIcs_SwapDimensionIcsTest(CuTest* tc)
{
	ICS *ics = NULL, *new_ics;
	int err;
	FIBITMAP* fib;
	int order[3] = {1, 2, 0};

	char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\Csarseven.ics";
    char *out_file = "C:\\Documents and Settings\\Pierce\\Desktop\\test.ics";


	err = FreeImageIcs_IcsOpen (&ics, file, "r");

    CuAssertTrue(tc, ics != NULL);

	err = FreeImageIcs_SaveIcsFileWithFirstTwoDimensionsAs(ics, out_file, 2, 1);
	
    CuAssertTrue(tc, err == FIA_SUCCESS);
}



static void
TestFreeImageIcs_SumIntensityProjection(CuTest* tc)
{
	ICS *ics;
	Ics_Error err;
	FIBITMAP* fib;

	char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\SarSeven_txy.ics";
  
	err = FreeImageIcs_IcsOpen (&ics, file, "r");

    CuAssertTrue(tc, err == IcsErr_Ok);
	
	fib = FreeImageIcs_SumIntensityProjection(ics, 2);

    CuAssertTrue(tc, fib != NULL);

	FreeImageAlgorithms_SaveFIBToFile (fib, "C:\\Documents and Settings\\Pierce\\Desktop\\sum_intensity_project.bmp", BIT8);

	FreeImage_Unload(fib);
	FreeImageIcs_IcsClose(ics);

}


static void
TestFreeImageIcs_MaxIntensityProjection(CuTest* tc)
{
	ICS *ics;
	Ics_Error err;
	FIBITMAP* fib;

	char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\imageStack1.ics";
  
	err = FreeImageIcs_IcsOpen (&ics, file, "r");

    CuAssertTrue(tc, err == IcsErr_Ok);
	
	fib = FreeImageIcs_MaximumIntensityProjection(ics, 2);

    CuAssertTrue(tc, fib != NULL);

	FreeImageAlgorithms_SaveFIBToFile (fib, "C:\\Documents and Settings\\Pierce\\Desktop\\max_intensity_project.bmp", BIT8);

	FreeImage_Unload(fib);
	FreeImageIcs_IcsClose(ics);

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
	int bpp;

	char *file = "C:\\ColourTest.ics";

	err = FreeImageIcs_IcsOpen (&ics, file, "r");

	fib = FreeImageIcs_LoadFIBFromIcsFile (ics);

	assert(fib != NULL);

	bpp = FreeImage_GetBPP(fib);

	//ShowImage(fib);

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

	char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\Csarseven.ics";

	err = FreeImageIcs_IcsOpen (&ics, file, "r");

    CuAssertTrue(tc, err == IcsErr_Ok);

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



		


static void
TestFreeImageIcs_MetaDataAdd(CuTest* tc)
{
	ICS *ics;
	Ics_Error err;
	FIBITMAP* fib;
    int size;
    char order[50], label[50];

	char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\te.ics";

	err = FreeImageIcs_IcsOpen (&ics, file, "rw");

	err = FreeImageIcs_IcsAddHistoryString (ics, NULL, "Test1");
    CuAssertTrue(tc, err == IcsErr_Ok);

    err = FreeImageIcs_IcsAddHistoryString (ics, NULL, "Test2");
    CuAssertTrue(tc, err == IcsErr_Ok);
 
	err = FreeImageIcs_IcsClose(ics);
    CuAssertTrue(tc, err == IcsErr_Ok);

}



static void
TestFreeImageAlgorithms_LinearScaleTest(CuTest* tc)
{
	double min_found, max_found;
    ICS *ics;
    int err;
    FIBITMAP *dib;
    FIBITMAP *scaled_dib;
    FREE_IMAGE_TYPE type;

	char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\Working Area\\Test Images\\test_float.ics";

	err = FreeImageIcs_IcsOpen (&ics, file, "r");

    CuAssertTrue(tc, err == IcsErr_Ok);

    dib = FreeImageIcs_LoadFIBFromIcsFile(ics);
	
    CuAssertTrue(tc, dib != NULL);

    type = FreeImage_GetImageType(dib);

    //FIBITMAP *dib = FreeImage_ConvertToType(old_dib, FIT_INT16, 1);

    //

    //PROFILE_START("LinearScale");

	//for(int i=0; i < 1000; i++) {
		
    scaled_dib = FreeImageAlgorithms_LinearScaleToStandardType(dib, 0, 90, &min_found, &max_found);  

    BasicWin32Window("Test", 100, 200, 400, 400, scaled_dib);
       
  //      if(i < 999)
//		    FreeImage_Unload(scaled_dib);
//	}

    //PROFILE_STOP("LinearScale");


    //FreeImageAlgorithms_SaveFIBToFile(scaled_dib, "C:\\Documents and Settings\\Pierce\\Desktop\\output.bmp", BIT8);

    FreeImage_Unload(scaled_dib);
	FreeImage_Unload(dib);

}


CuSuite*
CuGetFreeImageIcsTestSuite(void)
{
	CuSuite* suite = CuSuiteNew();

    //SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_LinearScaleTest);

    //SUITE_ADD_TEST(suite, TestFreeImageIcs_SumIntensityProjection);
    //SUITE_ADD_TEST(suite, TestFreeImageIcs_MaxIntensityProjection);

	//SUITE_ADD_TEST(suite, TestFreeImageIcs_MetaDataAdd);
	//SUITE_ADD_TEST(suite, TestFreeImageIcs_ReadMultiDimensionalGreyScale);
	//SUITE_ADD_TEST(suite, TestFreeImageIcs_ReadMultiDimensionalGreyScaleSlice);
	SUITE_ADD_TEST(suite, TestFreeImageIcs_ReadMultiDimensionalColour);
	//SUITE_ADD_TEST(suite, TestFreeImageIcs_LoadTest);
    //SUITE_ADD_TEST(suite, TestFreeImageIcs_SwapDimensionIcsTest);

    //SUITE_ADD_TEST(suite, TestFreeImageIcs_GetDimensionalDetailTest);

	return suite;
}
