#pragma once

#include "iostream"
#include "cstring"
#include "malloc.h"

typedef int Status;

#define OK 1
#define ERROR 0

using namespace std;

// 串的定长顺序表示
#define MAXSTRLEN 255
typedef unsigned char SString[MAXSTRLEN + 1];


// 串的堆分配存储表示
class HString {
public:
	char *ch;
	int length;
	friend ostream& operator<<(ostream &out, HString &s) {
		for (int i = 0; i < s.length; ++i) out << s.ch[i];
		return out;
	}
public:
	HString(const char *s="") {
		int len = strlen(s);
		if (this->ch != nullptr) delete[] this->ch;
		if (!len) {
			this->ch = nullptr; this->length = 0;
		}
		else {
			this->ch = new char[len];
			for (int i = 0; i < len; ++i) this->ch[i] = s[i];
		}
		this->length = len;
	}
	Status StrAssign(const char *chars) {
		int len = strlen(chars);
		if (this->ch != nullptr) delete[] this->ch;
		if (!len) {
			this->ch = nullptr; this->length = 0;
		}
		else {
			this->ch = new char[len];
			for (int i = 0; i < len; ++i) this->ch[i] = chars[i];
		}
		this->length = len;
		return OK;
	}
	Status StrCopy(HString s) {
		this->length = s.StrLength();
		if (this->ch != nullptr) { delete[] this->ch; this->ch = nullptr; }
		this->ch = new char[this->length];
		for (int i = 0; i < this->length; ++i) this->ch[i] = s.ch[i];
		return OK;
	}
	HString& operator=(const char *chars) {
		int len = strlen(chars);
		if (this->ch != nullptr) delete[] this->ch;
		if (!len) {
			this->ch = nullptr; this->length = 0;
		}
		else {
			this->ch = new char[len];
			for (int i = 0; i < len; ++i) this->ch[i] = chars[i];
		}
		this->length = len;
		return *this;
	}
	HString& operator=(HString &s) {
		this->length = s.StrLength();
		if (this->ch != nullptr) { delete[] this->ch; this->ch = nullptr; }
		this->ch = new char[this->length];
		for (int i = 0; i < this->length; ++i) this->ch[i] = s.ch[i];
		return *this;
	}
	bool StrEmpty() {
		return !(this->length);
	}
	int StrLength() {
		return this->length;
	}
	int Index(HString T, int pos) {
		if (!(pos >= 1 && pos <= this->length)) return 0;
		int len = T.StrLength();
		for (int i = pos - 1; i < this->length - len + 1; ++i) {
			if (!strncmp(this->ch + i, T.ch, len)) return i+1;
		}
		return 0;
	}
	Status Replace(HString &T, HString &V) {
		int len_t = T.StrLength(), len_v = V.StrLength();
		for (int i = 0; i < this->length - len_t + 1; ++i) {
			if (!strncmp(this->ch+i, T.ch, len_t)) {
				if (len_t < len_v) {
					this->ch = (char*)realloc(this->ch, this->length + len_v - len_t);
					for (int j = this->length-1; j>=i+len_t; --j) this->ch[j + len_v-len_t] = this->ch[j];
					this->length += len_v-len_t;
					for (int j = 0; j < len_v; ++j) this->ch[i + j] = V.ch[j];
				}
				else if (len_t == len_v) {
					for (int j = i; j < i + len_v; ++j) this->ch[j] = V.ch[j-i];
				}
				else {
					for (int j = i; j < i + len_v; ++j) this->ch[j] = V.ch[j-i];
					for (int j = i + len_v; j < this->length - len_t + len_v; ++j) this->ch[j] = this->ch[j + len_t - len_v];  
					this->length -= len_t - len_v;
				}
				i += len_v;
			}
		}
		return OK;
	}
	Status StrInsert(int pos, HString T) {
		if (!(pos >= 1 && pos <= this->length + 1)) return ERROR;
		this->ch = (char*)realloc(this->ch, this->length + T.StrLength());
		for (int i = this->length - 1; i >= pos - 1; --i) this->ch[i + T.StrLength()] = ch[i];
		for (int i = pos - 1; i < pos - 1 + T.StrLength(); ++i) this->ch[i] = T.ch[i - pos + 1];
		this->length += T.StrLength();
		return OK;
	}
	Status StrDelete(int pos, int len) {
		if (!(pos >= 1 && pos <= this->length - len + 1)) return ERROR;
		for (int i = 0; i < len; ++i) this->ch[pos + i-1] = this->ch[pos + i + len-1];
		this->length -= len;
		return OK;
	}
	int StrCompare(HString T) {
		for (int i = 0; i < this->StrLength() && i < T.StrLength(); ++i) {
			if (this->ch[i] != T.ch[i]) return this->ch[i] - T.ch[i];
		}
		return this->StrLength() - T.StrLength();
	}
	Status ClearString() {
		if (this->ch != nullptr) { delete[] this->ch; this->ch = nullptr; }
		this->length = 0;
		return OK;
	}
	HString& Concat(HString &S2) {
		HString *T = new HString("");
		if (T->ch != nullptr) { delete[] T->ch; T->ch = nullptr; }
		T->ch = new char[this->length+S2.StrLength()];
		if (T->ch == nullptr) exit(OVERFLOW);  
		for (int i = 0; i < this->length; ++i) T->ch[i] = this->ch[i];
		T->length = this->length + S2.StrLength();
		for (int i = this->length; i < T->length; ++i) T->ch[i] = S2.ch[i - this->length];
		return *T;
	}
	HString& SubString(int pos, int len) {
		HString *Sub = new HString("");
		if (pos<1 || pos>this->StrLength() || len < 0 || len>this->StrLength() - pos + 1) throw ERROR;
		if (Sub->ch != nullptr) { delete[] Sub->ch; Sub->ch = nullptr; }
		if (!len) { Sub->ch = nullptr; Sub->length = 0; }
		else {
			Sub->ch = new char[len];
			for (int i = 0; i < len; ++i) Sub->ch[i] = this->ch[pos + i - 1];
			Sub->length = len;
		}
		return *Sub;
	}
	char& operator[](int index) {
		return this->ch[index - 1];
	}
};

class StringApplication {
public:
	int SimpleIndex(HString &S, HString &T, int pos) {
		int i = pos, j = 1;
		while (i <= S.StrLength() && j <= T.StrLength()) {
			if (S[i] == T[j]) { ++i; ++j; }
			else { i = i - j + 2; j = 1;}
		}
		if (j > T.StrLength()) return i - T.StrLength();
		else return 0;
	}
	int Index_KMP(HString &S, HString &T, int pos) {
		struct Tools {
		public:
			void get_nextval(HString T, int nextval[]) {
				int i = 1; nextval[1] = 0; int j = 0;
				while (i < T.StrLength()) {
					if (j == 0 || T[i] == T[j]) {
						++i; ++j;
						if (T[i] != T[j]) nextval[i] = j;
						else nextval[i] = nextval[j];
					}
					else j = nextval[j];
				}
			}
		};
		int i = pos, j = 1, next[128];
		Tools tls;
		tls.get_nextval(T, next);
		while (i <= S.StrLength() && j <= T.StrLength()) {
			if (j == 0 || S[i] == T[j]) { ++i; ++j; }
			else j = next[j];
		}
		if (j > T.StrLength()) return i - T.StrLength();
		else return 0;
	}
};