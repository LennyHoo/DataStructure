#include "iostream"
#include "Tree.hpp"

using namespace std;

int main() {
    BiTree tree{};
    cout << "创建树：";
    tree.CreateBiTree();
    cout << "前序遍历：";
    tree.PreOrderTraverse(PrintElement);
    cout << endl;
    cout << "中序遍历：";
    tree.InOrderTraverse(PrintElement);
    cout << endl;
    cout << "后序遍历：";
    tree.PostOrderTraverse(PrintElement);
    cout << endl;
    cout << "层序遍历：";
    tree.DepthOrderTraverse(PrintElement);
    return 0;
}
