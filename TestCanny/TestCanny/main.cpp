#include "stdafx.h"
#include "TestCanny.h"

int main(int argc, char const *argv[]) {
	// 控制变量法
	for (float sigma = 0.5; sigma < 3.0; sigma += 1.0) {
		for (float threshold = 3.0; threshold < 12.0; threshold += 3.0) {
			TestCanny testCanny = TestCanny("test_Data/lena.bmp", sigma, threshold);
			testCanny.runTest();
		}
	}

	return 0;
}