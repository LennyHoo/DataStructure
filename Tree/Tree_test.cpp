#include "iostream"
#include "Tree.hpp"

using namespace std;

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
