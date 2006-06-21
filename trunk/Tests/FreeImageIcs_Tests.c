#include "CuTest.h"

#include "FreeImageIcs_IO.h"
#include "FreeImageIcs_MetaData.h"

#include "FreeImageIcs_Testing.h"


static void
TestFreeImageIcs_IsIcsFileTest(CuTest* tc)
{
	//char *file = "C:\\Documents and Settings\\Glenn\\My Documents\\Test Images\\black_test.jpg";
	char *file = "C:\\Documents and Settings\\Pierce\\My Documents\\Test Images\\kitten.jpg";

	int err = FreeImageIcs_IsIcsFile (file);   

	CuAssertTrue(tc, err != FREEIMAGE_ALGORITHMS_ERROR);
}

static void
TestFreeImageIcs_MetaDataAdd(CuTest* tc)
{
	FreeImageIcsPointer fip;
	int err;

	char *file = "C:\\Documents and Settings\\Pierce\\My Documents\\Test Images\\rjl.ics";

	err = FreeImageIcs_OpenIcsFile(&fip, file, "r");

	CuAssertTrue(tc, err != FREEIMAGE_ALGORITHMS_ERROR);

	err = FreeImageIcs_AddIcsHistoryKeyValueStrings(fip, "Glenn Key", "Glenn Value", NULL);

	CuAssertTrue(tc, err != FREEIMAGE_ALGORITHMS_ERROR);

	err = FreeImageIcs_SetIcsHistoryKeyValueStrings(fip, "Test1", "Cool", "Test2", "Cool2", NULL);

	CuAssertTrue(tc, err != FREEIMAGE_ALGORITHMS_ERROR);

	FreeImageIcs_CloseIcsFile(fip);
}


CuSuite* DLL_CALLCONV
CuGetFreeImageIcsTestSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFreeImageIcs_MetaDataAdd);
	//SUITE_ADD_TEST(suite, TestFreeImageIcs_IsIcsFileTest);

	return suite;
}
