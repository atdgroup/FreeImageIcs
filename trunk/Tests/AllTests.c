#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "CuTest.h"

CuSuite* CuGetFreeImageIcsTestSuite();


void RunAllTests(void)
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	CuSuiteAddSuite(suite, CuGetFreeImageIcsTestSuite());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
}

int __cdecl main(void)
{
	char string[10];

	RunAllTests();

	gets(string);

	return 0;
}

#ifdef __cplusplus
}
#endif