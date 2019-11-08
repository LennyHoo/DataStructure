#include "iostream"
#include "stack"
#include "queue"

using namespace std;

#define OK 1
#define ERROR -1

typedef int Status;

typedef enum { Link, Thread } PointerTag;

class BiThrTree;

Status PrintElement(string *e){
    cout << *e << " ";
    return OK;
}

// ������������ͣ���ʽ�洢�ṹ��
class BiTree{
public:
    friend class BiThrTree;
private:
    string *data{};
    BiTree *l_child{}, *r_child{}, *parent{};
    enum{FIRST, SECOND, THIRD, OKAY} view = FIRST;
    PointerTag LTag{}, RTag{};
public:
    virtual Status CreateBiTree(){
        // ���򴴽�������
        // �������������ʾ�Ķ�����
        string s;
        cin >> s;
        if(s=="NULL") this->data = nullptr;
        else{
            this->data = new string;
            *(this->data) = s;
            this->l_child = new BiTree;
            this->r_child = new BiTree;
            this->l_child->parent = this;
            this->r_child->parent = this;
            this->l_child->view = FIRST;
            this->r_child->view = FIRST;
            this->l_child->CreateBiTree();
            this->r_child->CreateBiTree();
        }
        return OK;
    }
    Status PreOrderTraverse(Status (*Visit)(string *e)){
        // ���ö�������洢�ṹ��Visit �Ƕ�����Ԫ�ز�����Ӧ�ú���
        // ��������������ĵݹ��㷨����ÿ��Ԫ�ص��ú��� Visit
        // ��򵥵� Visit �����ǣ�
        // Status PrintElement(string *e){
        //      cout << *e << " ";
        //      return OK;
        // }
        if(this->data){
            if(Visit(this->data)){
                if(this->l_child->PreOrderTraverse(Visit)){
                    if(this->r_child->PreOrderTraverse(Visit))
                        return OK;
                }
            }
            return ERROR;
        }else return OK;
    }

    virtual Status InOrderTraverse(Status (*Visit)(string *e)){
        // ���ö�������洢�ṹ��Visit �Ƕ�����Ԫ�ز�����Ӧ�ú���
        // ��������������ķǵݹ��㷨����ÿ������Ԫ�ص��ú��� Visit
        stack<BiTree*> s;
        s.push(this);
        while(!s.empty()){
            BiTree* p = s.top();
            while(p->data){
                s.push(p->l_child);
                p = s.top();
            }
            p = s.top();
            s.pop();
            if(!s.empty()){
                p = s.top();
                s.pop();
                if(!Visit(p->data)) return ERROR;
                s.push(p->r_child);
            }
        }
        return OK;
    }
    Status PostOrderTraverse(Status (*Visit)(string *e)){
        // ����������������˫���򣩴洢�ṹ��Visit �Ƕ�����Ԫ�ز�����Ӧ�ú���
        // ��������������ķǵݹ��㷨����ÿ������Ԫ�ص��ú��� Visit
        BiTree *T = this;
        BiTree *p = T;
        while(T->view!=OKAY){
            switch(p->view){
                case FIRST:
                    p->view = SECOND;
                    if(p->l_child->data)    p = p->l_child;
                    break;
                case SECOND:
                    p->view = THIRD;
                    if(p->r_child->data)    p = p->r_child;
                    break;
                case THIRD:
                    p->view = OKAY;
                    Visit(p->data);
                    p = p->parent;
                    break;
                case OKAY:
                    break;
            }
        }
        return OK;
    }
    Status DepthOrderTraverse(Status (*Visit)(string *e)){
        // ���ö�������洢�ṹ��Visit �Ƕ�����Ԫ�ز�����Ӧ�ú���
        // ��������������ķǵݹ��㷨����ÿ������Ԫ�ص��ú��� Visit
        queue<BiTree*> q;
        q.push(this);
        while(!q.empty()){
            BiTree *p = q.front();
            q.pop();
            Visit(p->data);
            if(p->l_child->data)  q.push(p->l_child);
            if(p->r_child->data)    q.push(p->r_child);
        }
        return OK;
    }
};

// �����������������ͣ���ʽ�洢�ṹ��
class BiThrTree{
public:
    friend class BiTree;
private:
    string *data{};
    BiThrTree *l_child{}, *r_child{};
    PointerTag LTag{}, RTag{};
    BiThrTree *pre{};
public:
    Status CreateBiTree(){
        // ���򴴽�������
        // �������������ʾ�Ķ�����
        string s;
        cin >> s;
        if(s=="NULL") this->data = nullptr;
        else{
            this->data = new string;
            *(this->data) = s;
            this->l_child = new BiThrTree;
            this->r_child = new BiThrTree;
            this->l_child->CreateBiTree();
            this->r_child->CreateBiTree();
        }
        return OK;
    }
    Status CreateBiThrTree(){
        // ��������������
        CreateBiTree();
        auto *T = new BiThrTree;
        *T = *this;
        InOrderThreading(T);
        return OK;
    }
    Status InOrderThreading(BiThrTree *T){
        // �������������������������������
        this->data = new string;
        this->LTag = Link;  this->RTag = Thread;
        this->r_child = this;
        if(!T->data)    this->l_child = this;
        else{
            this->l_child = T;    this->pre = this;
            InThreading(T);
            this->pre->r_child = this;    this->pre->RTag = Thread;
            this->r_child = this->pre;
        }
        return OK;
    }
    void InThreading(BiThrTree *p){
        if(p->data){
            InThreading(p->l_child);    // ������������
            if(!p->l_child->data){p->LTag = Thread; p->l_child = this->pre;}  // ǰ������
            if(!this->pre->r_child->data){pre->RTag = Thread; this->pre->r_child = p;} // �������
            this->pre = p;
            InThreading(p->r_child); // ������������
        }
    }
    Status InOrderTraverse(Status (*Visit)(string *e)){
        BiThrTree *p = this->l_child;
        while(p!=this){
            while(p->LTag==Link)    p = p->l_child;
            if(!Visit(p->data)) return ERROR;
            while(p->RTag==Thread&&p->r_child!=this){
                p = p->r_child; Visit(p->data);
            }
            p = p->r_child;
        }
        return OK;
    }
};


int main() {
    BiTree tree{};
    cout << "��������";
    tree.CreateBiTree();
    cout << "ǰ�������";
    tree.PreOrderTraverse(PrintElement);
    cout << endl;
    cout << "���������";
    tree.InOrderTraverse(PrintElement);
    cout << endl;
    cout << "���������";
    tree.PostOrderTraverse(PrintElement);
    cout << endl;
    cout << "���������";
    tree.DepthOrderTraverse(PrintElement);
    return 0;
}
