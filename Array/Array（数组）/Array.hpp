#pragma once

#include "iostream"
#include "stdarg.h"
#include "malloc.h"

using namespace std;

typedef int Status;

#define OK 1
#define ERROR 0

template <typename T>
class Array {
private:
	T * base;		   // 数组元素基地址
	int dim;
	int *bounds;	   // 数组维界基地址
	int *constants;    // 数组映像函数常量地址
	int MAX_ARRAY_DIM;
public:
	Array(int MAD, int dim, ...) {
		this->MAX_ARRAY_DIM = MAD;
		if (dim<1 || dim>this->MAX_ARRAY_DIM) throw "ERROR!";
		this->dim = dim;
		this->bounds = (int *)malloc(dim * sizeof(int));
		if (this->bounds == nullptr) exit(OVERFLOW);
		int elemtotal = 1;
		va_list ap;
		va_start(ap, dim);
		for (int i = 0; i < dim; ++i) {
			this->bounds[i] = va_arg(ap, int);
			if (this->bounds[i] < 0) throw "UNDERFLOW!";
			elemtotal *= this->bounds[i];
		}
		va_end(ap);
		this->base = (T*)malloc(elemtotal * sizeof(T));
		if (this->base == nullptr) exit(OVERFLOW);
		this->constants = (int*)malloc(dim * sizeof(int));
		if (this->constants == nullptr) exit(OVERFLOW);
		this->constants[dim - 1] = 1;
		for (int i = dim - 2; i >= 0; --i)
			this->constants[i] = this->bounds[i + 1] * this->constants[i + 1];
	}
	Status InitArray(int MAD, int dim, ...) {
		this->MAX_ARRAY_DIM = MAD;
		if (dim<1 || dim>this->MAX_ARRAY_DIM) throw "ERROR!";
		this->dim = dim;
		this->bounds = (int *)malloc(dim * sizeof(int));
		if (this->bounds == nullptr) exit(OVERFLOW);
		int elemtotal = 1;
		va_list ap;
		va_start(ap, dim);
		for (int i = 0; i < dim; ++i) {
			this->bounds[i] = va_arg(ap, int);
			if (this->bounds[i] < 0) throw "UNDERFLOW!";
			elemtotal *= this->bounds[i];
		}
		va_end(ap);
		this->base = (T*)malloc(elemtotal * sizeof(T));
		if (this->base == nullptr) exit(OVERFLOW);
		this->constants = (int*)malloc(dim * sizeof(int));
		if (this->constants == nullptr) exit(OVERFLOW);
		this->constants[dim - 1] = 1;
		for (int i = dim - 2; i >= 0; --i)
			this->constants[i] = this->bounds[i + 1] * this->constants[i + 1];
		return OK;
	}
	int Locate(va_list ap) {
		int off = 0;
		for (int i = 0; i < this->dim; ++i) {
			int ind = va_arg(ap, int);
			if (ind < 0 || ind >= this->bounds[i])	return OVERFLOW;
			off += this->constants[i] * ind;
		}
		return off;
	}
	T& Value(const char *e, ...) {
		va_list ap;
		va_start(ap, e);
		int *result = new int;
		if ((*result = Locate(ap)) < 0) return *result;
		return *((T*)this->base + Locate(ap));
	}
	Status Assign(T e, ...) {
		va_list ap;
		va_start(ap, e);
		int *result = new int;
		if ((*result = Locate(ap)) < 0) return *result;
		*(this->base + Locate(ap)) = e;
		return OK;
	}
};

template <typename T>
struct Triple {
	int i, j;
	T e;
};

template <typename T>
class TSMatrix {
private:
	Triple<T> *data;
	int mu, nu, tu, MAXSIZE; // 矩阵的行数、列数和非零元个数 最大非零元个数
	friend ostream& operator<<(ostream &out, TSMatrix<T> tsmx) {
		for (int i = 1, pos = 0; i <= tsmx.mu; ++i) {
			for (int j = 1; j <= tsmx.nu; ++j) {
				if (i == tsmx.data[pos].i&&j == tsmx.data[pos].j) {
					cout << tsmx.data[pos].e << " ";
					++pos;
				}
				else cout << "0" << " ";
			}
			cout << endl;
		}
		return out;
	}
public:
	TSMatrix(int mu, int nu, int tu) {
		this->MAXSIZE = mu * nu;
		this->data = (Triple<T>*)malloc((MAXSIZE + 1)*sizeof(Triple<T>));
		this->mu = mu; this->nu = nu; this->tu = tu; 
	}
	Status CreateSMatrix() {
		for (int i = 0; i < this->tu; ++i) { cin >> this->data[i].i >> this->data[i].j >> this->data[i].e; }
		for (int i = 0; i < this->tu - 1; ++i) {
			for (int j = this->tu - 1; j > i; --j) {
				if (this->data[j].i < this->data[j - 1].i) {
					Triple<T> temp = this->data[j]; this->data[j] = this->data[j - 1]; this->data[j - 1] = temp; 
				}
				else if (this->data[j].i == this->data[j - 1].i) {
					if (this->data[j].j < this->data[j - 1].j) {
						Triple<T> temp = this->data[j]; this->data[j] = this->data[j - 1]; this->data[j - 1] = temp;
					}
				}
			}
			
		}
		return OK;
	}		   
	Status PrintSMatrix() {
		for (int i = 1, pos = 0; i <= this->mu; ++i) {
			for (int j = 1; j <= this->nu; ++j) {
				if (i == this->data[pos].i&&j == this->data[pos].j) {
					cout << this->data[pos].e << " ";
					++pos;
				}
				else cout << "0" << " ";
			}
			cout << endl;
		}
		return OK;
	}
	TSMatrix<T> CopySMatrix() {
		return *this;
	}
	TSMatrix<T>& AddSMatrix(TSMatrix<T> N) {
		if (this->mu != N.mu || this->nu != N.nu) throw "MATH ERROR!";
		TSMatrix<T> *Q = new TSMatrix<T>(this->mu, this->nu, 0);
		memcpy_s(Q->data, Q->MAXSIZE * sizeof(Triple<T>), this->data, this->tu * sizeof(Triple<T>)); 
		Q->tu += this->tu;
		int Qtu = Q->tu;
		for (int i = 0; i < N.tu; ++i) {
			int flag = 1;
			for (int j = 0; j < Qtu; ++j) {
				if (Q->data[j].i == N.data[i].i&&Q->data[j].j == N.data[i].j) {
					Q->data[j].e += N.data[i].e;
					flag = 0;
					break;
				}
			}
			if (flag) {
				Q->data[Q->tu].i = N.data[i].i; Q->data[Q->tu].j = N.data[i].j; Q->data[Q->tu].e = N.data[i].e;
				++Q->tu;
			}
		}
		for (int i = 0; i < Q->tu - 1; ++i) {
			for (int j = Q->tu - 1; j > i; --j) {
				if (Q->data[j].i < Q->data[j - 1].i) {
					Triple<T> temp = Q->data[j]; Q->data[j] = Q->data[j - 1]; Q->data[j - 1] = temp;
				}
				else if (Q->data[j].i == Q->data[j - 1].i) {
					if (Q->data[j].j < Q->data[j - 1].j) {
						Triple<T> temp = Q->data[j]; Q->data[j] = Q->data[j - 1]; Q->data[j - 1] = temp;
					}
				}
			}
		}
		return *Q;
	}
	TSMatrix<T>& SubtMatrix(TSMatrix<T> N) {
		if (this->mu != N.mu || this->nu != N.nu) throw "MATH ERROR!";
		TSMatrix<T> *Q = new TSMatrix<T>(this->mu, this->nu, 0);
		memcpy_s(Q->data, Q->MAXSIZE * sizeof(Triple<T>), this->data, this->tu * sizeof(Triple<T>));
		Q->tu += this->tu;
		int Qtu = Q->tu;
		for (int i = 0; i < N.tu; ++i) {
			int flag = 1;
			for (int j = 0; j < Qtu; ++j) {
				if (Q->data[j].i == N.data[i].i&&Q->data[j].j == N.data[i].j) {
					Q->data[j].e -= N.data[i].e;
					flag = 0;
					break;
				}
			}
			if (flag) {
				Q->data[Q->tu].i = N.data[i].i; Q->data[Q->tu].j = N.data[i].j; Q->data[Q->tu].e = -N.data[i].e;
				++Q->tu;
			}
		}
		for (int i = 0; i < Q->tu - 1; ++i) {
			for (int j = Q->tu - 1; j > i; --j) {
				if (Q->data[j].i < Q->data[j - 1].i) {
					Triple<T> temp = Q->data[j]; Q->data[j] = Q->data[j - 1]; Q->data[j - 1] = temp;
				}
				else if (Q->data[j].i == Q->data[j - 1].i) {
					if (Q->data[j].j < Q->data[j - 1].j) {
						Triple<T> temp = Q->data[j]; Q->data[j] = Q->data[j - 1]; Q->data[j - 1] = temp;
					}
				}
			}
		}
		return *Q;
	}
	TSMatrix<T>& TransposeSMatrix() {
		TSMatrix<T> *tsmx = new TSMatrix<T>(this->MAXSIZE, this->nu, this->mu, this->tu);
		if (tsmx->tu) {
			int q = 0;
			for (int col = 1; col < this->nu; col++) {	   // 因为之前已经排好序了， 为了转置之后还是顺序的
				for (int p = 0; p < this->tu; ++p) {
					if (this->data[p].j == col) {
						tsmx->data[q].i = this->data[p].j; tsmx->data[q].j = this->data[p].i;
						tsmx->data[q].e = this->data[p].e; ++q;
					}
				}
			}
		}
		return *tsmx;
	}
	TSMatrix<T>& FastTransposeSMatrix() {
		TSMatrix<T> *tsmx = new TSMatrix<T>(this->nu, this->mu, this->tu);
		int *num = new int[this->nu + 1], *cpot = new int[this->nu + 1];
		if (tsmx->tu) {
			for (int col = 1; col <= this->nu; ++col) num[col] = 0;
			for (int t = 1; t <= this->tu; ++t) ++num[this->data[t-1].j];
			cpot[1] = 1;
			for (int col = 2; col <= this->nu; ++col) cpot[col] = cpot[col - 1] + num[col - 1];
			for (int p = 1; p <= this->tu; ++p) {
				int col = this->data[p-1].j; 
				int q = cpot[col];
				tsmx->data[q-1].i = this->data[p-1].j; tsmx->data[q-1].j = this->data[p-1].i;
				tsmx->data[q-1].e = this->data[p-1].e; ++cpot[col];
			}
		}
		delete[] num; delete[] cpot;
		return *tsmx;
	}
	TSMatrix<T>& MultSMatrix(TSMatrix<T> N) {
		TSMatrix<T> *Q = new TSMatrix<T>(this->mu, N.nu, 0);
		T *ctemp = new T[Q->nu + 1];
		int *this_rpos = new int[this->mu + 1], *N_rpos = new int[this->mu + 1], *Q_rpos = new int[Q->mu + 1];
		int *num = new int[this->mu + 1];
		for (int i = 0; i < this->mu + 1; ++i) num[i] = 0;
		for (int i = 0; i < this->tu; ++i) ++num[this->data[i].i];
		this_rpos[1] = 1;
		for (int i = 2; i <= this->mu; ++i) {
			this_rpos[i] = this_rpos[i - 1] + num[i - 1];
		}
		num = new int[N.mu + 1];
		for (int i = 0; i < N.mu + 1; ++i) num[i] = 0;
		for (int i = 0; i < N.tu; ++i) ++num[N.data[i].i];
		N_rpos[1] = 1;
		for (int i = 2; i <= N.mu; ++i) {
			N_rpos[i] = N_rpos[i - 1] + num[i - 1];
		}
		delete[] num;
		int tp, t;
		if (this->nu != N.mu) throw "MATH ERROR!";
		if (this->tu*N.tu != 0) {
			for (int arow = 1; arow <= this->mu; ++arow) {
				memset(ctemp, 0, (Q->nu + 1) * sizeof(T));
				Q_rpos[arow] = Q->tu + 1;
				if (arow < this->mu) tp = this_rpos[arow + 1]; else tp = this->tu + 1;
				for (int p = this_rpos[arow]; p < tp; ++p) {
					int brow = this->data[p - 1].j;
					if (brow < N.mu) t = N_rpos[brow + 1]; else t = N.tu + 1;
					for (int q = N_rpos[brow]; q < t; ++q) {
						int ccol = N.data[q - 1].j;
						ctemp[ccol] += this->data[p - 1].e * N.data[q - 1].e;
					}
				}
				for (int ccol = 1; ccol <= Q->nu; ++ccol) {
					if (ctemp[ccol]) {
						Q->data[Q->tu].i = arow; Q->data[Q->tu].j = ccol; Q->data[Q->tu].e = ctemp[ccol];
						++Q->tu;
					}
				}
			}
		}
		delete[] ctemp, this_rpos, N_rpos, Q_rpos;
		return *Q;
	}
};

template <typename T>
class RLSMatrix {
private:
	Triple<T> *data;
	int *rpos;
	int mu, nu, tu, MAXSIZE;
	friend ostream& operator<<(ostream &out, RLSMatrix<T> tsmx) {
		for (int i = 1, pos = 0; i <= tsmx.mu; ++i) {
			for (int j = 1; j <= tsmx.nu; ++j) {
				if (i == tsmx.data[pos].i&&j == tsmx.data[pos].j) {
					cout << tsmx.data[pos].e << " ";
					++pos;
				}
				else cout << "0" << " ";
			}
			cout << endl;
		}
		return out;
	}
public:
	RLSMatrix(int mu, int nu, int tu) {
		this->MAXSIZE = mu * nu;
		this->data = (Triple<T>*)malloc((MAXSIZE + 1) * sizeof(Triple<T>));
		this->mu = mu; this->nu = nu; this->tu = tu;  
		this->rpos = new int[this->mu + 1];
	}
	Status CreateSMatrix() {
		for (int i = 0; i < this->tu; ++i) { cin >> this->data[i].i >> this->data[i].j >> this->data[i].e; }
		for (int i = 0; i < this->tu - 1; ++i) {
			for (int j = this->tu - 1; j > i; --j) {
				if (this->data[j].i < this->data[j - 1].i) {
					Triple<T> temp = this->data[j]; this->data[j] = this->data[j - 1]; this->data[j - 1] = temp;
				}
				else if (this->data[j].i == this->data[j - 1].i) {
					if (this->data[j].j < this->data[j - 1].j) {
						Triple<T> temp = this->data[j]; this->data[j] = this->data[j - 1]; this->data[j - 1] = temp;
					}
				}
			}

		}
		int *num = new int[this->mu + 1];
		for (int i = 0; i < this->mu + 1; ++i) num[i] = 0;
		for (int i = 0; i < this->tu; ++i) ++num[this->data[i].i];
		this->rpos[1] = 1;
		for (int i = 2; i <= this->mu; ++i) {
			this->rpos[i] = this->rpos[i - 1] + num[i - 1];
		}
		delete[] num;
		return OK;
	}
	Status PrintSMatrix() {
		for (int i = 1, pos = 0; i <= this->mu; ++i) {
			for (int j = 1; j <= this->nu; ++j) {
				if (i == this->data[pos].i&&j == this->data[pos].j) {
					cout << this->data[pos].e << " ";
					++pos;
				}
				else cout << "0" << " ";
			}
			cout << endl;
		}
		return OK;
	}
	RLSMatrix<T> CopySMatrix() {
		return *this;
	}
	RLSMatrix<T>& AddSMatrix(RLSMatrix<T> N) {
		if (this->mu != N.mu || this->nu != N.nu) throw "MATH ERROR!";
		RLSMatrix<T> *Q = new RLSMatrix<T>(this->mu, this->nu, 0);
		memcpy_s(Q->data, Q->MAXSIZE * sizeof(Triple<T>), this->data, this->tu * sizeof(Triple<T>));
		Q->tu += this->tu;
		int Qtu = Q->tu;
		for (int i = 0; i < N.tu; ++i) {
			int flag = 1;
			for (int j = 0; j < Qtu; ++j) {
				if (Q->data[j].i == N.data[i].i&&Q->data[j].j == N.data[i].j) {
					Q->data[j].e += N.data[i].e;
					flag = 0;
					break;
				}
			}
			if (flag) {
				Q->data[Q->tu].i = N.data[i].i; Q->data[Q->tu].j = N.data[i].j; Q->data[Q->tu].e = N.data[i].e;
				++Q->tu;
			}
		}
		for (int i = 0; i < Q->tu - 1; ++i) {
			for (int j = Q->tu - 1; j > i; --j) {
				if (Q->data[j].i < Q->data[j - 1].i) {
					Triple<T> temp = Q->data[j]; Q->data[j] = Q->data[j - 1]; Q->data[j - 1] = temp;
				}
				else if (Q->data[j].i == Q->data[j - 1].i) {
					if (Q->data[j].j < Q->data[j - 1].j) {
						Triple<T> temp = Q->data[j]; Q->data[j] = Q->data[j - 1]; Q->data[j - 1] = temp;
					}
				}
			}
		}
		return *Q;
	}
	RLSMatrix<T>& SubtMatrix(RLSMatrix<T> N) {
		if (this->mu != N.mu || this->nu != N.nu) throw "MATH ERROR!";
		RLSMatrix<T> *Q = new RLSMatrix<T>(this->mu, this->nu, 0);
		memcpy_s(Q->data, Q->MAXSIZE * sizeof(Triple<T>), this->data, this->tu * sizeof(Triple<T>));
		Q->tu += this->tu;
		int Qtu = Q->tu;
		for (int i = 0; i < N.tu; ++i) {
			int flag = 1;
			for (int j = 0; j < Qtu; ++j) {
				if (Q->data[j].i == N.data[i].i&&Q->data[j].j == N.data[i].j) {
					Q->data[j].e -= N.data[i].e;
					flag = 0;
					break;
				}
			}
			if (flag) {
				Q->data[Q->tu].i = N.data[i].i; Q->data[Q->tu].j = N.data[i].j; Q->data[Q->tu].e = -N.data[i].e;
				++Q->tu;
			}
		}
		for (int i = 0; i < Q->tu - 1; ++i) {
			for (int j = Q->tu - 1; j > i; --j) {
				if (Q->data[j].i < Q->data[j - 1].i) {
					Triple<T> temp = Q->data[j]; Q->data[j] = Q->data[j - 1]; Q->data[j - 1] = temp;
				}
				else if (Q->data[j].i == Q->data[j - 1].i) {
					if (Q->data[j].j < Q->data[j - 1].j) {
						Triple<T> temp = Q->data[j]; Q->data[j] = Q->data[j - 1]; Q->data[j - 1] = temp;
					}
				}
			}
		}
		return *Q;
	}
	RLSMatrix<T>& TransposeSMatrix() {
		RLSMatrix<T> *tsmx = new RLSMatrix<T>(this->nu, this->mu, this->tu);
		if (tsmx->tu) {
			int q = 0;
			for (int col = 1; col < this->nu; col++) {	   // 因为之前已经排好序了， 为了转置之后还是顺序的
				for (int p = 0; p < this->tu; ++p) {
					if (this->data[p].j == col) {
						tsmx->data[q].i = this->data[p].j; tsmx->data[q].j = this->data[p].i;
						tsmx->data[q].e = this->data[p].e; ++q;
					}
				}
			}
		}
		return *tsmx;
	}
	RLSMatrix<T>& FastTransposeSMatrix() {
		RLSMatrix<T> *tsmx = new RLSMatrix<T>(this->MAXSIZE, this->nu, this->mu, this->tu);
		int *num = new int[this->nu + 1], *cpot = new int[this->nu + 1];
		if (tsmx->tu) {
			for (int col = 1; col <= this->nu; ++col) num[col] = 0;
			for (int t = 1; t <= this->tu; ++t) ++num[this->data[t - 1].j];
			cpot[1] = 1;
			for (int col = 2; col <= this->nu; ++col) cpot[col] = cpot[col - 1] + num[col - 1];
			for (int p = 1; p <= this->tu; ++p) {
				int col = this->data[p - 1].j;
				int q = cpot[col];
				tsmx->data[q - 1].i = this->data[p - 1].j; tsmx->data[q - 1].j = this->data[p - 1].i;
				tsmx->data[q - 1].e = this->data[p - 1].e; ++cpot[col];
			}
		}
		delete[] num; delete[] cpot;
		return *tsmx;
	}
	RLSMatrix<T>& MultSMatrix(RLSMatrix<T> N) {
		RLSMatrix<T> *Q = new RLSMatrix<T>(this->mu, N.nu, 0);
		T *ctemp = new T[Q->nu + 1];
		int tp, t;
		if (this->nu != N.mu) throw "MATH ERROR!";
		if (this->tu*N.tu != 0) {
			for (int arow = 1; arow <= this->mu; ++arow) {
				memset(ctemp, 0, (Q->nu + 1) * sizeof(T));
				Q->rpos[arow] = Q->tu + 1;
				if (arow < this->mu) tp = this->rpos[arow + 1]; else tp = this->tu + 1;
				for (int p = this->rpos[arow]; p < tp; ++p) {
					int brow = this->data[p-1].j;
					if (brow < N.mu) t = N.rpos[brow + 1]; else t = N.tu + 1;
					for (int q = N.rpos[brow]; q < t; ++q) {
						int ccol = N.data[q-1].j;
						ctemp[ccol] += this->data[p-1].e * N.data[q-1].e;
					}
				}
				for (int ccol = 1; ccol <= Q->nu; ++ccol) {
					if (ctemp[ccol]) {
						Q->data[Q->tu].i = arow; Q->data[Q->tu].j = ccol; Q->data[Q->tu].e = ctemp[ccol];
						++Q->tu;
					}
				}
			}
		}
		delete[] ctemp;
		return *Q;
	}
};

