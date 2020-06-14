#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>

using namespace std;

typedef struct Node {
    Node* left;
    Node* right;
    Node* parent;
    char data[25];
    bool RED;
};


int SIZE;
void check(Node* root) {


    if (root == NULL)
        return;
    check(root->left);
    std::cout << root->data << "  " << root->RED << " parent  ";
    if (root->parent != NULL)
        std::cout << root->parent->data << endl;
    else
        cout << endl;
    check(root->right);
}
int Search(Node* root, char* key) {
    if (root == NULL)
        return 0;
    else {
        if (_strcmpi(root->data, key) > 0)
            return Search(root->left, key);
        else if (_strcmpi(root->data, key) < 0)
            return Search(root->right, key);
        else {
            return 1;

        }
    }

}

void RotateRight(Node*& root, Node* node) {
    Node* temp = node->left;
    node->left = temp->right;
    if (temp->right != NULL)
        temp->right->parent = node;
    temp->parent = node->parent;
    if (node->parent == NULL)
        root = temp;
    else if (node->parent->left == node)
        node->parent->left = temp;
    else
        node->parent->right = temp;
    temp->right = node;
    node->parent = temp;
}

void RotateLeft(Node*& root, Node* node) {

    Node* temp = node->right;
    node->right = temp->left;
    if (temp->left != NULL)
        temp->parent = node;
    temp->parent = node->parent;
    if (node->parent == NULL)
        root = temp;
    else if (node->parent->left == node)
        node->parent->left = temp;
    else
        node->parent->right = temp;
    temp->left = node;
    node->parent = temp;


}




void fix(Node*& root, Node*& last) {

    Node* U = NULL;
    Node* G = NULL;
    Node* p = NULL;
    while (root != last && last->RED != 0 && last->parent->RED == 1) {
        p = last->parent;
        G = last->parent->parent;
        if (p == G->left) {  //parent left
            U = G->right;
            if (U != NULL && U->RED == 1) { //parent left and uncle is red
                p->RED = 0;
                G->RED = 1;
                U->RED = 0;
                last = G;
            }
            else { //parent left and uncle is black
                if (p->right == last) {//uncle is black + parent left + last is right
                    RotateLeft(root, p);
                    last = p;
                    p = last->parent;
                }
                //uncle is black + parent left + last is left

                RotateRight(root, G);
                swap(p->RED, G->RED);
                last = p;
            }
        }


        else { //parent right
            U = G->left;
            if (U != NULL && U->RED == 1) { //parent is right + uncle is red
                p->RED = 0;
                G->RED = 1;
                U->RED = 0;
                last = G;
            }
            else {//parent is right + uncle is black
                if (last == p->left) {//parent is right +last is left+ uncle is black
                    RotateRight(root, p);
                    last = p;
                    p = last->parent;
                }
                //parent is right +last is right+ uncle is black

                RotateLeft(root, G);
                swap(p->RED, G->RED);
                last = p;

            }
        }

    }
    root->RED = 0;

}


Node* CreateNode(char* line) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy_s(newNode->data, line);
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    newNode->RED = 1;
    SIZE++;
    return newNode;
}



Node* Insert(Node* root, Node* node) {

    if (root == NULL) {
        return node;
    }
    else if (_strcmpi(root->data, node->data) > 0) {

        root->left = Insert(root->left, node);
        root->left->parent = root;

    }
    else {

        root->right = Insert(root->right, node);
        root->right->parent = root;


    }



    return root;
}





int GetHeight(Node* root) {
    if (root == NULL)
        return -1;
    else {
        int left = GetHeight(root->left);
        int right = GetHeight(root->right);
        if (left > right)
            return left + 1;
        else
            return right + 1;
    }
}



int main()
{
    Node* root = NULL;
    Node* newNode = NULL;
    char line[25];
    ifstream file;
    file.open("dictionary.txt");
    while (!file.eof()) {
        file.getline(line, sizeof(line));
        newNode = CreateNode(line);
        root = Insert(root, newNode);
        fix(root, newNode);

    }
    file.close();
    cout << "Tree height :" << GetHeight(root) << endl;
    cout << "Size of dictionary: " << SIZE << endl;
    cout << "Type '1' to insert and '2' to lookup a word or any other key to exit" << endl;
    int x;
    cin >> x;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    int flag;
    while (x == 1 || x == 2) {
        switch (x)
        {
        case 1:
            cout << "Enter the word you want to add:" << endl;
            std::cin.getline(line, sizeof line);
            flag = Search(root, line);
            if (flag == 1)
                cout << "ERROR: Word already in the dictionary!" << endl;
            else {
                newNode = CreateNode(line);
                root = Insert(root, newNode);
                fix(root, newNode);
                cout << "Tree height after insertion:" << GetHeight(root) << endl;
                cout << "Size of dictionary after insertion: " << SIZE << endl;
            }
            cout << endl;
            break;
        case 2:
            cout << "Enter the word you want to lookup:" << endl;
            cin.getline(line, sizeof line);
            flag = Search(root, line);
            if (flag == 1)
                cout << "YES" << endl;
            else
                cout << "NO" << endl;
            cout << endl;
            break;


        default:
            break;
        }
        cout << "Type '1' to insert and '2' to lookup a word or any other key to exit" << endl;
        cin >> x;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return 0;
}
