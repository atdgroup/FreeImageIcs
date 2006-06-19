#include "CuTest.h"

#include "FreeImageIcs_IO.h"
#include "FreeImageIcs_MetaData.h"

#include "FreeImageIcs_Testing.h"

static void
TestFreeImageIcs_MetaDataAdd(CuTest* tc)
{
	FreeImageIcsPointer fip;
	int err;

	char *file = "C:\\Documents and Settings\\Glenn\\Desktop\\rjl.ics";

	err = FreeImageIcs_OpenIcsFile(&fip, file, "r");

	CuAssertTrue(tc, err != FREEIMAGE_ALGORITHMS_ERROR);

	err = FreeImageIcs_AddIcsHistoryKeyValueStrings(fip, "Glenn Key", "Glenn Value", NULL);

	CuAssertTrue(tc, err != FREEIMAGE_ALGORITHMS_ERROR);

	FreeImageIcs_CloseIcsFile(fip);
}


CuSuite* DLL_CALLCONV
CuGetFreeImageIcsTestSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFreeImageIcs_MetaDataAdd);

	return suite;
}
