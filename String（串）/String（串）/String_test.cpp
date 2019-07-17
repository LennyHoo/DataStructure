// String（串）.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "String.hpp"

int main()
{
	HString s1, s2;
	s1 = "abcdefg";
	s2 = "hijklmn";
	HString s3 = s1.Concat(s2);
	cout << s1 << s2 << endl;
	cout << s3 << endl;
	cout << s1.StrCompare(s2) << endl;
	s2.ClearString();
	//s2 = s1;
	s2.StrCopy(s1);
	cout << s1.StrCompare(s2) << endl;
	cout << s3.SubString(6, 5) << endl;
	HString s4 = "hi", s5="hahahahahwwwwwwwww";
	cout << s3.Index(s4, 3) << endl;
	s3.Replace(s4, s5);
	cout << s3 << endl;
	HString s6 = "c++";
	s3.StrInsert(3, s6);
	cout << s3 << endl;
	s3.StrDelete(7, 5);
	cout << s3 << endl;
	return 0;
}

