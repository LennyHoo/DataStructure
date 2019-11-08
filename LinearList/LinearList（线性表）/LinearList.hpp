#pragma once
#include "stdafx.h"
#include "iostream"
#include "malloc.h"

typedef int Status;
typedef Status (*func_cmp)(void *a,void *b);
//typedef Status(*func_cmp)(double &a, double &b);
typedef Status (*pvisit)(void *temp);

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1

template <typename T>
class ADTList {
public:
	virtual Status InitList() = 0;
	virtual Status DestroyList() = 0;
	virtual Status ClearList() = 0;
	virtual bool ListEmpty() = 0;
	virtual int ListLength() = 0;
	virtual T& GetElem(int index) = 0;
	virtual int LocateElem(T e, func_cmp *p) = 0;
	virtual T& PriorElem(T cur_e) = 0;
	virtual T& NextElem(T cur_e) = 0;
	virtual Status ListInsert(int index, T e) = 0;
	virtual Status ListDelete(int index, T &e) = 0;
	virtual Status ListTraverse(pvisit *p) = 0;
};


template <typename T>
class LinearList_Sq {
private:
	T * elem;
	int length;
	int listsize;
	int listincrement;
public:
	LinearList_Sq(int listsize=0, int listincrement=10) {
		this->elem = (T*)(malloc(listsize * sizeof(T)));
		if (this->elem == NULL) {
			exit(OVERFLOW);
		}
		this->listsize = listsize;
		this->length = 0;
		this->listincrement = listincrement;
	}
	~LinearList_Sq() {
		if (this->elem != NULL) free(this->elem);
		this->length = 0;
		this->listsize = 0;
		this->listincrement = 0;
	}
	Status InitList(int listsize=0, int listincrement = 10) {
		// ����һ���յ����Ա�L
		this->elem = (T*)(malloc(listsize * sizeof(T)));
		if (this->elem == NULL) {		  // �洢����ʧ��
			exit(OVERFLOW);
		}
		this->listsize = listsize;	// ��ʼ���洢����
		this->length = 0;			// �ձ���Ϊ0
		this->listincrement = listincrement;
		return OK;
	}
	Status DestroyList() {
		if (this->elem != NULL) free(this->elem);
		this->length = 0;
		this->listsize = 0;
		this->listincrement = 0;
		return OK;
	}
	Status ClearList() {
		memset(this->elem, NULL , this->length * sizeof(T));
		this->length = 0;
		return OK;
	}
	bool ListEmpty() {
		return (bool)(!(this->length));
	}
	int ListLength() {
		return this->length;
	}
	T& PriorElem(T cur_e) {
		T *p = this->elem;
		while (*(p + 1) != cur_e) ++p;
		return *p;
	}
	T& NextElem(T cur_e) {
		T *p = this->elem;
		while (*p != cur_e) ++p;
		return *(++p);
	}
	Status ListInsert(int i, T e) {
		// ��˳�����Ա�L�е�i��λ��֮ǰ������Ԫ��e
		// i�ĺϷ�ֵΪ1 <= i <= (ListLength+1)
		if (i<1 || i>(this->length + 1)) return ERROR;		// iֵ���Ϸ�
		if (this->length >= this->listsize) {			// ��ǰ�洢�ռ������� ���ӷ���
			T *newbase = (T*)realloc(this->elem, (this->listsize + this->listincrement) * sizeof(T));	  
			if (newbase == NULL) exit(OVERFLOW);			// �洢����ʧ��
			this->elem = newbase;						  // �»�ַ
			this->listsize += this->listincrement;		 // ���Ӵ洢����
		}
		T *q = &(this->elem[i - 1]);					 // qΪ����λ��
		for (T *p = &(this->elem[this->length - 1]); p >= q; --p) *(p + 1) = *p;	 //����λ�ü���֮���Ԫ������
		*q = e;
		++(this->length);
		return OK;
	}
	Status ListDelete(int i, T &e) {
		// ��˳�����Ա�L��ɾ����i��Ԫ�أ� ����e������ֵ
		// i�ĺϷ�ֵΪ 1 <= i <= ListLength
		if ((i < 1) || (i > this->length)) return ERROR;   // iֵ���Ϸ�
		T *p = &(this->elem[i - 1]);				// pΪ��ɾ��Ԫ�ص�λ��
		e = *p;										// ��ɾ��Ԫ�ص�ֵ����e
		T *q = this->elem + this->length - 1;		// ��βԪ�ص�λ��
		for (++p; p <= q; ++p) *(p - 1) = *p;		// ��ɾ��Ԫ��֮���Ԫ������
		--this->length;								// ����1
		return OK;
	}
	int LocateElem(T e, func_cmp compare) {
		// ��˳�����Ա�L�в��ҵ�һ��ֵ��e����compare()��Ԫ�ص�λ��
		// ���ҵ��� �򷵻�����L�е�λ�� ���򷵻�0
		int i = 1;		// i�ĳ�ֵ�ĵ�һ��Ԫ�ص�λ��
		T *p = this->elem;	   // p�ĳ�ֵΪ��һ��Ԫ�صĴ洢λ��
		while (i <= this->length && !(*compare)(p++, &e)) ++i;
		if (i <= this->length) return i;
		else return 0;
	}
	T& GetElem(int i) {
		return this->elem[i];
	}
	Status ListTraverse(pvisit visit) {
		T *p = this->elem;
		T *q = this->elem + this->length;
		while (p != q) {
			++p;
			visit(p);
		}
	}
	LinearList_Sq& MergeList(LinearList_Sq &Lb) {
		// ��֪˳�����Ա�La��Lb��Ԫ�ذ�ֵ�ǵݼ�����
		// �鲢La��Lb�õ��µ�˳�����Ա�Lc�� Lc��Ԫ��Ҳ��ֵ�ǵݼ�����
		LinearList_Sq *Lc = new LinearList_Sq;
		T *pa = this->elem;
		T *pb = Lb.elem;
		Lc->listsize = Lc->length = this->length + Lb.length;
		T *pc = Lc->elem = (T*)malloc(Lc->length * sizeof(T));
		if (Lc->elem == NULL) exit(OVERFLOW); // �洢����ʧ��
		T *pa_last = this->elem + this->length - 1;
		T *pb_last = Lb.elem + Lb.length - 1;
		while (pa <= pa_last && pb <= pb_last) {		  //�鲢
			if (*pa <= *pb) *pc++ = *pa++;
			else *pc++ = *pb++;
		}
		while (pa <= pa_last) *pc++ = *pa++;	 // ����ʣ���Ԫ��
		while (pb <= pb_last) *pc++ = *pb++;
		return *Lc;
	}
};

template <typename T>
struct SLKNode {
	T data;
	int cur;
};

template <typename T>
class SLinkList {
private:
	SLinkList * space;
	int size;
public:
	SLinkList(int size) {
		this->size = size;
		// ��һά����space�и���������һ�����ñ� space[0].curΪͷָ��
		// "0"��ʾ��ָ��
		space = new SLKNode<T>[size];
		for (int i = 0; i < this->size - 1; ++i)	space[i].cur = i + 1;
		space[size - 1].cur = 0;
	}
	void InitSpace_SL(int size) {
		this->size = size;
		// ��һά����space�и���������һ�����ñ� space[0].curΪͷָ��
		// "0"��ʾ��ָ��
		space = new SLKNode<T>[size];
		for (int i = 0; i < this->size - 1; ++i)	space[i].cur = i + 1;
		space[size - 1].cur = 0;
	}
	int Malloc_SL() {
		// �����ÿռ�����ǿգ� �򷵻ط���Ľ���±꣬ ���򷵻�0
		int i = this->space[0].cur;
		if (this->space[0].cur) this->space[0].cur = this->space[i].cur;
		return i;
	}
	void Free_SL(int k) {
		// ���±�Ϊk�Ŀ��н����յ���������
		this->space[k].cur = this->space[0].cur;
		this->space[0].cur = k;
	}
};

template <typename T>
struct LNode{
	T data;
	struct LNode *next;
};


// ������
template <typename T>
class LinkList {
private:
	LNode<T> *phead, *ptail;
	LNode<T> head;
	int len;
public:
	LinkList() {       
		phead = &head;
		ptail = phead;
		phead->next = nullptr;
		this->len = 0;
	}
	Status InitList() {	       // ����һ���յ���������L
		phead = &head;
		ptail = phead;
		phead->next = nullptr;
		this->len = 0;
		return OK;
	}
	Status ClearList() {		// ������������Ϊ�ձ� ���ͷ�ԭ����Ľ��ռ�
		LNode<T> *p = this->phead->next;
		LNode<T> *q = nullptr;
		while (p != nullptr) {
			q = p->next;
			free(p);
			p = q;
		}
		this->len = 0;
		ptail = phead;
		phead->next = nullptr;
		return OK;
	}
	Status InsFirst(LNode<T> *s) {
		// ��s��ָ�������ڵ�һ�����֮ǰ
		s->next = this->phead->next;
		this->phead->next = s;
		if (this->phead == this->ptail) this->ptail = s;
		++this->len;
		return OK;
	}
	Status DelFirst(LNode<T> *s) {
		LNode<T> *q = this->phead->next;
		*s = *q;
		this->phead->next = this->phead->next->next;
		delete q;
		q = nullptr;
		--this->len;
		return OK;
	}
	Status Append(LNode<T> *s) {
		// ��ָ��sָ���һ�������������������һ�����֮�� ���ı������βָ��ָ���µĽ��
		LNode<T> *p = s;
		int plus = 1;
		while (p->next != nullptr) {
			p = p->next;
			++plus;
		}
		this->ptail->next = s;
		this->ptail = p;
		this->ptail->next = nullptr;
		this->len = this->len + plus;
		return OK;
	}
	LNode<T>& Remove() {
		LNode<T> *p = this->phead;
		while (p->next != this->ptail) p = p->next;
		LNode<T> *tmp = new LNode<T>;
		tmp = this->ptail;
		free(this->ptail);
		this->ptail = p;
		--this->len;
		return *tmp;
	}
	Status InsBefore(LNode<T>* &p, LNode<T> *s) {
		LNode<T> *cur = this->phead;
		while (cur->next != p) cur = cur->next;
		s->next = p;
		cur->next = s;
		++this->len;
		return OK;
	}
	Status InsAfter(LNode<T>* &p, LNode<T> *s) {
		s->next = p->next;
		p->next = s;
		++this->len;
		return OK;
	}
	Status SetCurElem(LNode<T>* &p, T e) {
		p->data = e;
		return OK;
	}
	T& GetCurElem(LNode<T> *p) {
		return p->data;
	}
	Status ListEmpty() {
		return (this->phead == this->ptail);
	}
	int ListLength() {
		return this->len;
	}
	LNode<T>* GetHead() {
		return this->phead;
	}
	LNode<T>* GetLast() {
		return this->ptail;
	}
	LNode<T>* PriorPos(LNode<T> *p) {
		LNode<T> *cur = this->phead;
		while (cur->next != p && cur->next!=nullptr) cur = cur->next;
		if (cur->next == p) return p;
		else return NULL;
	}
	LNode<T>* NextPos(LNode<T> *p) {
		if (p->next != NULL) return p->next;
		else return NULL;
	}
	LNode<T>* LocatePos(int i) {
		if (i > this->len) return ERROR;
		LNode<T> *p = new LNode<T>;
		p = this->phead;
		int cnt = 0;
		while (cnt < i) {
			++cnt;
			p = p->next;
		}
		return p;
	}
	LNode<T>* LocateElem(T e, func_cmp compare) {
		LNode<T> *p = new LNode<T>;
		p = this->phead;
		while (p->next != nullptr) {
			p = p->next;
			if (compare(p, e)) return p;
		}	
		return nullptr;
	}
	Status ListTraverse(pvisit visit) {
		LNode<T> *p = this->phead;
		LNode<T> *q = this->ptail;
		while (p != q) {
			p = p->next;
			visit((int*)(&(p->data)));
		}
		return OK;
	}
};
