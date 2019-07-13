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
	T& DeQueue() {
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