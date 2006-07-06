#include "CuTest.h"

#include "libics.h"

#include "FreeImageIcs_IO.h"
#include "FreeImageIcs_MetaData.h"

#include "FreeImageIcs_Testing.h"


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


	//CuAssertTrue(tc, fiError == FREEIMAGE_ALGORITHMS_ERROR);

	//FreeImageIcs_SetIcsHistoryStrings(ics, "Glenn New Key", "Glenn New Value", NULL);
	//FreeImageIcs_SetIcsHistoryKeyValueStrings(ics, "Glenn New Key", "Glenn New Value",
	//	"Glenn New Key1", "Glenn New Value1", NULL);

	//FreeImageIcs_AddIcsHistoryKeyValueStrings(ics, "Help1", "Help1_Val","Help2", "Help2_Val", NULL);

	IcsClose(ics);
}


CuSuite*
CuGetFreeImageIcsTestSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFreeImageIcs_MetaDataAdd);

	return suite;
}
