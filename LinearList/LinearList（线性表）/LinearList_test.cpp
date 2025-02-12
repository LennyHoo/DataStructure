// LinearList（线性表）.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "iostream"
#include "cstdio"
#include "LinearList.hpp"


Status visit(void *temp) {
	int *t = (int*)temp;
	std::cout << *t << " ";
	return OK;
}

int main()
{
	LinkList<int> lkt, lkt2;
	for (int i = 0; i < 3; i++) {
		LNode<int> *temp = new LNode<int>;
		temp->data = i + 1;
		lkt.InsFirst(temp);
	}
	for (int i = 0; i < 6; i++) {
		LNode<int> *temp = new LNode<int>;
		temp->data = i + 1;
		lkt2.InsFirst(temp);
	}
	LNode<int> t;
	lkt.DelFirst(&t);
	lkt.DelFirst(&t);
	lkt.Remove();
	lkt.Append(lkt2.GetHead()->next);
	std::cout << "length: " << lkt.ListLength() << std::endl;
	lkt.ClearList();
	for (int i = 0; i < 3; i++) {
		LNode<int> *temp = new LNode<int>;
		temp->data = i + 1;
		lkt.InsFirst(temp);
	}
	lkt.ListTraverse(visit);
	return 0;
}

