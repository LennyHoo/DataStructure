//
// Created by ASUS on 2019/11/8.
//

#ifndef TREE_TREE_HPP
#define TREE_TREE_HPP

#endif //TREE_TREE_HPP

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

// 定义二叉树类型（链式存储结构）
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
        // 先序创建二叉树
        // 构建二叉链表表示的二叉树
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
        // 采用二叉链表存储结构，Visit 是对数据元素操作的应用函数
        // 先序遍历二叉树的递归算法，对每个元素调用函数 Visit
        // 最简单的 Visit 函数是：
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
        // 采用二叉链表存储结构，Visit 是对数据元素操作的应用函数
        // 中序遍历二叉树的非递归算法，对每个数据元素调用函数 Visit
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
        // 采用三叉链表（即带双亲域）存储结构，Visit 是对数据元素操作的应用函数
        // 后序遍历二叉树的非递归算法，对每个数据元素调用函数 Visit
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
        // 采用二叉链表存储结构，Visit 是对数据元素操作的应用函数
        // 层序遍历二叉树的非递归算法，对每个数据元素调用函数 Visit
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

// 定义线索二叉树类型（链式存储结构）
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
        // 先序创建二叉树
        // 构建二叉链表表示的二叉树
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
        // 生成线索二叉树
        CreateBiTree();
        auto *T = new BiThrTree;
        *T = *this;
        InOrderThreading(T);
        return OK;
    }
    Status InOrderThreading(BiThrTree *T){
        // 中序遍历二叉树，并将其中序线索化
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
            InThreading(p->l_child);    // 左子树线索化
            if(!p->l_child->data){p->LTag = Thread; p->l_child = this->pre;}  // 前驱线索
            if(!this->pre->r_child->data){pre->RTag = Thread; this->pre->r_child = p;} // 后继线索
            this->pre = p;
            InThreading(p->r_child); // 右子树线索化
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