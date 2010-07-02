#include <stdio.h>
#include "profile.h"

#include "CuTest.h"

#ifdef __cplusplus
extern "C" {
#endif

CuSuite*
CuGetFreeImageIcsTestSuite(void);

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

int main()
{
	char string[10];

	RunAllTests();

	PROFILE_PRINT();

	gets(string);

	return 0;
}

#ifdef __cplusplus
}
#endif