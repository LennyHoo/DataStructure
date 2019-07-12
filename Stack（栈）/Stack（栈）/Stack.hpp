#pragma once
#include "stdafx.h"
#include "iostream"
#include "fstream"
#include "string"

using namespace std;
typedef int Status;
typedef void (*pvist)(void *);

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1

template <typename T>
struct Node {
	T data;
	Node *next;
};

// ��ջ
template <typename T>
class Stack {
private:
	Node<T> *pbase;
	Node<T> *ptop;
	int stacksize;
	friend ostream& operator<<(ostream &out, Stack stk) {
		Node<T> *p = stk.ptop;
		while (p->next!= nullptr) {
			cout << p->data << " ";
			p = p->next;
		}
		return out;
	}
	friend ofstream& operator<<(ofstream &out, Stack stk) {
		Stack<T> temp;
		while (stk.stacksize)	temp.Push(stk.Pop());
		while (temp.stacksize)	out << temp.Pop();
		return out;
	}
public:
	Stack() {
		this->ptop = new Node<T>;
		this->pbase = this->ptop;
		ptop->next = nullptr;
		pbase->next = nullptr;
		this->stacksize = 0;
	}
	~Stack() {
		Node<T> *p = this->ptop;
		Node<T> *q = p;
		while (q->next != nullptr) {
			p = q->next;
			if (q != nullptr)	delete q;
			q = p;
		}
		this->stacksize = 0;
		ptop = pbase;
	}
	Status InitStack() {
		this->ptop = &(this->top);
		this->pbase = this->ptop;
		ptop->next = nullptr;
		this->stacksize = 0;
		return OK;
	}
	Status ClearStack() {
		Node<T> *p = this->ptop;
		Node<T> *q = p;
		while (q->next != nullptr) {
			p = q->next;
			if(q!=nullptr)	delete q;
			q = p;
		}
		this->stacksize = 0;
		ptop = pbase;
		return OK;
	}
	bool StackEmpty() {
		return ((this->pbase == this->ptop) && this->stacksize==0);
	}
	int StackLength() {
		return this->stacksize;
	}

	T& GetTop() {
		if (this->pbase == this->ptop)	throw "ջΪ�գ�";
		return	this->ptop->data;
	}
	Status Push(const T &e) {
		Node<T> *pnew = new Node<T>;
		if (pnew == nullptr) {
			cout << "�ڴ����ʧ�ܣ�" << endl;
			exit(OVERFLOW);
		}
		pnew->data = e;
		pnew->next = this->ptop;
		this->ptop = pnew;
		++this->stacksize;
		return OK;
	}
	T& Pop() {
		if (this->pbase == this->ptop) throw "ջΪ�գ�";
		Node<T>* p = new Node<T>;
		p = this->ptop;
		this->ptop = this->ptop->next;
		--this->stacksize;
		return p->data;
	}
	Status StackTraverse(pvist visit) {
		Node<T> *p = this->ptop;
		while (p->next!= nullptr) {
			visit(&(p->data));
			p = p->next;
		}
		return OK;
	}
};

class StackApplication {
public:
	void conversion(int N) {
		// ��ʮ������ת���ɰ˽�����
		Stack<int> stk;
		while (N) {
			stk.Push(N % 8);
			N /= 8;
		}
		while (!stk.StackEmpty()) {
			cout << stk.Pop();
		}
	}
	bool IsBracketsMatch(const char *s) {
		// ����ƥ��
		int len = strlen(s);
		Stack<char> stk;
		for (int i = 0; i < len; i++) {
			if (stk.StackEmpty()) stk.Push(s[i]);
			else if ((s[i] == ']'&&stk.GetTop() == '[') || (s[i] == ')' && stk.GetTop() == '(')) stk.Pop();
			else stk.Push(s[i]);
		}
		if (stk.StackEmpty()) return true;
		else return false;
	}
	void LineEdit() {
		// �б༭����
		char s[2048]="";
		ofstream fout("./text.txt", ios::out);
		Stack<char> *stk = new Stack<char>;
		while (true) {
			//cin.sync();
			//cin.getline(s, 128, '\n');
			gets_s(s);
			int len = strlen(s);
			if (!strcmp(s, "save!!")) return;
			for (int i = 0; i < len; i++) {
				if (s[i] == '#' && !(stk->StackEmpty())) stk->Pop();
				else if (s[i] == '@' && !(stk->StackEmpty())) stk->ClearStack();
				else if (s[i] != '#'&&s[i] != '@') stk->Push(s[i]);
			}
			fout << *stk << endl;
			if (!(stk->StackEmpty())) stk->ClearStack();
		}
	}
	bool MazeSolution() {
		// ���ݷ� �Թ����
		struct MazePoint {
		public:
			int i, j;
		public:					 
			MazePoint & operator=(MazePoint p) {
				this->i = p.i; this->j = p.j;
				return *this;
			}
			bool operator==(MazePoint p) {
				return ((this->i == p.i) && (this->j == p.j));
			}
		};
		Stack<MazePoint> stk;
		int r, c;
		cout << "�������Թ�������������: ";
		cin >> r >> c;
		char **maze = new char*[r];
		cout << "�������Թ��� &��ʾ�ϰ�� #��ʾ�յ�" << endl;
		for (int i = 0; i < r; i++)	maze[i] = new char[c];
		for (int i = 0; i < r; i++) for (int j = 0; j < c; j++) cin >> maze[i][j];
		MazePoint start, curpos, end;
		cout << "�������������: " ;
		cin >> start.i >> start.j;
		cout << "�������յ�����: " ;
		cin >> end.i >> end.j;
		curpos = start;
		stk.Push(start);
		do {
			if (curpos == end) { 
				while (!stk.StackEmpty()) {
					MazePoint p = stk.Pop();
					maze[p.i][p.j] = 'G';
				}
				for (int i = 0; i < r; i++) {
					for (int j = 0; j < c; j++)	cout << maze[i][j];
					cout << endl;
				}
				return true; 
			}
			if (maze[curpos.i][curpos.j + 1] == '#' ) {
				++curpos.j;
				stk.Push(curpos);
				maze[curpos.i][curpos.j] = 'X';
			}	
			else if (maze[curpos.i + 1][curpos.j] == '#') {
				++curpos.i;
				stk.Push(curpos);
				maze[curpos.i][curpos.j] = 'X';
			}
			else if (maze[curpos.i][curpos.j-1] == '#') {
				--curpos.j;
				stk.Push(curpos);
				maze[curpos.i][curpos.j] = 'X';
			}
			else if (maze[curpos.i - 1][curpos.j] == '#') {
				--curpos.i;
				stk.Push(curpos);
				maze[curpos.i][curpos.j] = 'X';
			}
			else {
				stk.Pop();
				curpos = stk.GetTop();
			}
		} while (!stk.StackEmpty());
		return false;
	}
	double EvaluateExpression(const char *exp) {
		// ���ʽ��ֵ
		struct Tools {
		public:
			bool InOps(char ch) {
				return (ch == '+') + (ch == '-') + (ch == '*') + (ch == '/') + (ch == '(') + (ch == ')') + (ch == '#');
			}
			char Precede(char a, char b) {
				if (a == '*' || a == '/') return '>';
				else if ((a == '+' || a == '-') && (b == '+' || b == '-')) return '>';
				else if ((a == '+' || a == '-') && (b == '*' || b == '/')) return '<';
				else if (a == '#'|| a == '(') return '<';
				else return '=';
			}
			double compute(double a, double b, char ch) {
				if (ch == '*') return a * b;
				else if (ch == '/') return a / b;
				else if (ch == '+') return a + b;
				else return a - b;
			}
		};
		Stack<char> OPTR, OPND;
		int len = strlen(exp);
		Tools tls;
		OPTR.Push('#');
		for (int i = 0; i < len; ++i) {
			char cur = exp[i];
			if (!(tls.InOps(cur)))	OPND.Push(cur);
			else if (cur == '(') OPTR.Push(cur);
			else if (cur == ')') {
				while (OPTR.GetTop() != '(') OPND.Push(OPTR.Pop());
				OPTR.Pop();
			}
			else if (cur == '#') {
				while (OPTR.GetTop() != '#') OPND.Push(OPTR.Pop());
				OPTR.Pop();
			}
			else if (tls.Precede(OPTR.GetTop(), cur) == '<') OPTR.Push(cur);
			else if (tls.Precede(OPTR.GetTop(), cur) == '>') {
				OPND.Push(OPTR.Pop()); OPTR.Push(cur);
			} 
		}
		//while (!OPND.StackEmpty()) cout << OPND.Pop() << " ";
		Stack<char> stk;
		while (!OPND.StackEmpty()) stk.Push(OPND.Pop());
		Stack<double> rst;
		while (!stk.StackEmpty()) {
			if (!tls.InOps(stk.GetTop())) rst.Push((stk.Pop()-'0'));
			else {
				rst.Push(tls.compute(rst.Pop(), rst.Pop(), stk.Pop()));
			}
		}
		return rst.GetTop();
	}
};