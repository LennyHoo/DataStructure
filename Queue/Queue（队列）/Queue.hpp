#pragma once
#include "iostream"
#include "malloc.h"

using namespace std;
typedef int Status;
typedef void(*pvist)(void *);

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1

template <typename T>
struct QNode {
	T data;
	struct QNode *next;
};

// 链队列
template <typename T>
class Queue {
private:
	QNode<T> *front;
	QNode<T> *rear;
	int size;
public:
	Queue() {
		this->front = this->rear = (QNode<T>*)malloc(sizeof(QNode<T>));
		if (this->front == nullptr) exit(OVERFLOW);
		this->front->next = nullptr;
	}
	Status InitQueue() {
		this->front = this->rear = (QNode<T>*)malloc(sizeof(QNode<T>));
		if (this->front == nullptr) exit(OVERFLOW);
		this->front->next = nullptr;
		return OK;
	}
	Status ClearQueue() {
		QNode<T> *p = this->front->next;
		QNode<T> *q = nullptr;
		while (p != nullptr) {
			q = p->next;
			free(p);
			p = q;
		}
		this->rear = this->front;
		this->front->next = nullptr;
		this->size = 0;
		return OK;
	}
	bool QueueEmpty() {
		return (this->front == this->rear) && (this->size == 0);
	}
	int QueueLength() {
		return this->size;
	}
	T& GetHead() {
		return this->front->data;
	}
	Status EnQueue(T e) {
		QNode<T> *pnew = (QNode<T>*)malloc(sizeof(QNode<T>));
		if (pnew == nullptr) exit(OVERFLOW);
		pnew->data = e;
		pnew->next = nullptr;
		this->rear->next = pnew;
		this->rear = pnew;
		++this->size;
		return OK;
	}
	T DeQueue() {
		if (this->front == this->rear) throw "队列为空！";
		QNode<T> *p = this->front->next;
		T *copy = new T;
		*copy = p->data;
		this->front->next = p->next;
		if (this->rear == p) this->rear = this->front;
		free(p);
		--this->size;
		return *copy;
	}
	Status QueueTraverse(pvist visit) {
		QNode<T> *p = this->front;
		while (p->next != nullptr) {
			p = p->next;
			visit(&(p->data));
		}
		return OK;
	}
};


// 队列的顺序表示实现
template <typename T>
class Queue_Sq {
private:
	T *base;
	int front;
	int rear;
	int MAXQSIZE;
public:
	Queue_Sq(int MAXQSIZE) {
		this->MAXQSIZE = MAXQSIZE;
		this->base = (T*)malloc(sizeof(T)*MAXQSIZE);
		if (this->base == nullptr) exit(OVERFLOW);
		this->front = this->rear = 0;
	}
	~Queue_Sq() {
		if (this->base != nullptr) free(this->base);
	}
	Status InitQueue(int MAXQSIZE) {
		this->MAXQSIZE = MAXQSIZE;
		this->base = (T*)malloc(sizeof(T)*MAXQSIZE);
		if (this->base == nullptr) exit(OVERFLOW);
		this->front = this->rear = 0;
		return OK;
	}
	Status ClearQueue() {
		this->front = this->rear = 0;
		return OK;
	}
	bool QueueEmpty() {
		return (this->front == this->rear);
	}
	int QueueLength() {
		return (this->rear - this->front + this->MAXQSIZE) % (this->MAXQSIZE);
	}
	T& GetHead() {
		if (this->QueueEmpty()) throw "栈为空！";
		return this->base[this->front];
	}
	Status EnQueue(T e) {
		if ((this->rear + 1) % this->MAXQSIZE == this->front) throw "队列已满！";
		this->base[this->rear] = e;
		this->rear = (this->rear + 1) % this->MAXQSIZE;
		return OK;
	}
	T& DeQueue() {
		if (this->front == this->rear) throw "队列空！";
		T *copy = new T;
		*copy = this->base[this->front];
		this->front = (this->front + 1) % this->MAXQSIZE;
		return *copy;
	}
	Status QueueTraverse(pvist visit) {
		for (int i = 0; i < this->QueueLength(); i++) {
			visit(&(this->base[i]));
		}
		return OK;
	}
};