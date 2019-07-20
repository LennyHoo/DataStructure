// Array（数组）.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Array.hpp"

int main()
{
	/*Array<int> arr(8, 1, 128);
	for (int i = 0; i < 128; ++i) {
		arr.Assign(i+1, i);
	}
	for (int i = 0; i < 128; ++i) {
		cout << arr.Value("ready", i) << " ";
	}*/
	RLSMatrix<int> M(2, 2, 2), N(2, 2, 2);
	M.CreateSMatrix();
	N.CreateSMatrix();
	//tsmx.PrintSMatrix();
	cout << M << endl;
	cout << N << endl;
	cout << M.MultSMatrix(N) << endl; 
	return 0;
}

