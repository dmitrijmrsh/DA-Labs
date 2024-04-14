#include <iostream>
#include <cstring>
#include <fstream>
#include <cctype>
#include <queue>

using llu = long long unsigned int;

class TString {
    public:
        int Size;
        int Capacity;
        char* Arr;

        void Reallocate(int);

        TString();
        TString(int);
        TString(const char*);

        void PushBack(const char);
        void PopBack();
        void Erase(int);
        void Clear();

        void ToLower();
        void Print(std::ostream&);

        int GetSize();
        int GetCapacity();

        char& operator[] (int) const;
        TString& operator = (const TString&);

        bool operator < (const TString&) const;
        bool operator > (const TString&) const;
        bool operator == (const TString&) const;
        bool operator != (const TString&) const;

        friend std::ostream& operator << (std::ostream&, TString&);

        ~TString();
};

void TString::Reallocate(int newCapacity) {
    if (Arr == nullptr) {
        return;
    }

    char* temp;
    try {
        temp = new char[Size];
    } catch (const std::bad_alloc& ex) {
        std::cout << "ERROR: fail to allocate the requested storage space" << std::endl;
        exit(EXIT_SUCCESS);
    }

    for (int i = 0; i < Size; ++i) {
        temp[i] = Arr[i];
    }

    delete[] Arr;

    try {
        Arr = new char[newCapacity];
    } catch (const std::bad_alloc& ex) {
        std::cout << "ERROR: fail to allocate the requested storage space" << std::endl;
        exit(EXIT_SUCCESS);
    }

    for (int i = 0; i < Size; ++i) {
        Arr[i] = temp[i];
    }

    Capacity = newCapacity;
    delete[] temp;
}

TString::TString() {
    Size = 0;
    Capacity = 2;

    try {
        Arr = new char[Capacity];
    } catch (const std::bad_alloc& ex) {
        std::cout << "ERROR: fail to allocate the requested storage space" << std::endl;
        exit(EXIT_SUCCESS);
    }
}

TString::TString(int size) {
    Size = size;

    if (Size > 0) {
        Capacity = Size + 1;
    } else {
        Capacity = 2;
    }
    
    try {
        Arr = new char[Capacity];
    } catch (const std::bad_alloc& ex) {
        std::cout << "ERROR: fail to allocate the requested storage space" << std::endl;
        exit(EXIT_SUCCESS);
    }
}

TString::TString(const char* str) {
    Size = strlen(str);
    Capacity = Size + 1;

    try {
        Arr = new char[Capacity];
    } catch (const std::bad_alloc& ex) {
        std::cout << "ERROR: fail to allocate the requested storage space" << std::endl;
        exit(EXIT_SUCCESS);
    }

    strcpy(Arr, str);
}

void TString::PushBack(const char elem) {
    if (Size >= Capacity) {
        this->Reallocate(Capacity * 2);
    }

    Arr[Size++] = elem;
}

void TString::PopBack() {
    if (!Size) {
        return;
    }

    Size--;
}

void TString::Erase(int index) {
    if (index >= Size) {
        return;
    }

    for (int i = index + 1; i < Size; ++i) {
        Arr[i - 1] = Arr[i];
    }

    Size--;
}

void TString::Clear() {
    Size = 0;
}

char& TString::operator[] (int index) const {
    if (index >= Size) {
        throw std::logic_error("Too big index");
    }

    return Arr[index];
}

TString& TString::operator = (const TString& other) {
    delete[] Arr;
    Size = other.Size;
    Capacity = other.Capacity;

    try {
        Arr = new char[Capacity];
    } catch (const std::bad_alloc& ex) {
        std::cout << "ERROR: fail to allocate the requested storage space" << std::endl;
        exit(EXIT_SUCCESS);
    }

    strcpy(Arr, other.Arr);

    return *this;
}

int TString::GetSize() {
    return Size;
}

int TString::GetCapacity() {
    return Capacity;
}

bool TString::operator == (const TString& other) const {
    return strcmp(Arr, other.Arr) == 0;
}

bool TString::operator != (const TString& other) const {
    return !(*this == other);
}

bool TString::operator < (const TString& other) const {
    return strcmp(Arr, other.Arr) < 0;
}

bool TString::operator > (const TString& other) const {
    return strcmp(Arr, other.Arr) > 0;
}

void TString::Print(std::ostream& out) {
    for (int i = 0; i < Size; ++i) {
        std::cout << Arr[i];
    }
}

void TString::ToLower() {
    for (int i = 0; i < Size; ++i) {
        Arr[i] = tolower(Arr[i]);
    }
}

std::ostream& operator << (std::ostream& out, TString& str) {
    str.Print(out);
    return out;
}

TString::~TString() {
    Size = 0;
    Capacity = 0;
    delete[] Arr;
    Arr = nullptr;
}

struct TNode {
    TString Key;
    llu Value;
    bool Color;
    TNode* Left;
    TNode* Right;
    TNode* Parent;
    int Id;

    TNode(TString& key, llu value, bool color, TNode* left, TNode* right, TNode* parent) {
        Key = key;
        Value = value;
        Color = color;
        Left = left;
        Right = right;
        Parent = parent;
        Id = -1;
    }

    ~TNode() = default;
};

TNode* root = nullptr;
TNode* nil = nullptr;

void DeleteBinaryTree(TNode* r) {
    if (r == nil) {
        return;
    }

    DeleteBinaryTree(r->Left);
    DeleteBinaryTree(r->Right);

    delete r;
}

TNode* TreeMinimum(TNode* x) {
    TNode* temp = x;

    while (temp->Left != nil) {
        temp = temp->Left;
    }

    return temp;
}

TNode* TreeSuccessor(TNode* x) {
    if (x->Right != nil) {
        return TreeMinimum(x->Right);
    }

    TNode* y = x->Parent;
    TNode* temp = x;

    while (y != nil && temp == y->Right) {
        temp = y;
        y = y->Parent;
    }

    return y;
}

void LeftRotate(TNode* x) {
    TNode* y = x->Right;
    x->Right = y->Left;

    if (y->Left != nil) {
        y->Left->Parent = x;
    }

    y->Parent = x->Parent;

    if (x->Parent == nil) {
        root = y;
    } else if (x == x->Parent->Left) {
        x->Parent->Left = y;
    } else {
        x->Parent->Right = y;
    }

    y->Left = x;
    x->Parent = y;
}

void RightRotate(TNode* y) {
    TNode* x = y->Left;
    y->Left = x->Right;

    if (x->Right != nil) {
        x->Right->Parent = y;
    }

    x->Parent = y->Parent;

    if (y->Parent == nil) {
        root = x;
    } else if (y == y->Parent->Left) {
        y->Parent->Left = x;
    } else {
        y->Parent->Right = x;
    }

    x->Right = y;
    y->Parent = x;
}

TNode* RbFind(TString& key) {
    TNode* y = nil;
    TNode* x = root;

    while (x != nil) {
        y = x;

        if (key < x->Key) {
            x = x->Left;
        } else if (key > x->Key) {
            x = x->Right;
        } else {
            break;
        }
    }

    return y;
}

void RbInsertFixUp(TNode* z) {
    while (z->Parent->Color == false) {
        if (z->Parent == z->Parent->Parent->Left) {

            TNode* y = z->Parent->Parent->Right;
            
            if (y->Color == false) {

                z->Parent->Color = true;
                y->Color = true;
                z->Parent->Parent->Color = false;
                z = z->Parent->Parent;

            } else {

                if (z == z->Parent->Right) {
                    z = z->Parent;
                    LeftRotate(z);
                }

                z->Parent->Color = true;
                z->Parent->Parent->Color = false;
                RightRotate(z->Parent->Parent);

            }

        } else {
            TNode* y = z->Parent->Parent->Left;

            if (y->Color == false) {
                
                z->Parent->Color = true;
                y->Color = true;
                z->Parent->Parent->Color = false;
                z = z->Parent->Parent;

            } else {

                if (z == z->Parent->Left) {
                    z = z->Parent;
                    RightRotate(z);
                }

                z->Parent->Color = true;
                z->Parent->Parent->Color = false;
                LeftRotate(z->Parent->Parent);

            }
        }
    }

    root->Color = true;
}

void RbInsert(TNode* z, TNode* y) {
    z->Parent = y;

    if (y == nil) {
        root = z;
    } else if (z->Key < y->Key) {
        y->Left = z;
    } else {
        y->Right = z;
    }

    z->Left = nil;
    z->Right = nil;
    z->Color = false;

    RbInsertFixUp(z);
}

void RbDeleteFixUp(TNode* x) {
    while (x != root && x->Color == true) {
        if (x == x->Parent->Left) {

            TNode* w = x->Parent->Right;

            if (w->Color == false) {

                w->Color = true;
                x->Parent->Color = false;
                LeftRotate(x->Parent);
                w = x->Parent->Right;

            }

            if (w->Left->Color == true && w->Right->Color == true) {
                
                w->Color = false;
                x = x->Parent;

            } else {

                if (w->Right->Color == true) {

                    w->Left->Color = true;
                    w->Color = false;
                    RightRotate(w);
                    w = x->Parent->Right;

                }

                w->Color = x->Parent->Color;
                x->Parent->Color = true;
                w->Right->Color = true;
                LeftRotate(x->Parent);
                x = root;

            } 

        } else {

            TNode* w = x->Parent->Left;

            if (w->Color == false) {

                w->Color = true;
                x->Parent->Color = false;
                RightRotate(x->Parent);
                w = x->Parent->Left;

            }

            if (w->Left->Color == true && w->Right->Color == true) {

                x->Color = false;
                x = x->Parent;

            } else {

                if (w->Left->Color == true) {

                    w->Right->Color = true;
                    w->Color = false;
                    LeftRotate(w);
                    w = x->Parent->Left;

                }

                w->Color = x->Parent->Color;
                x->Parent->Color = true;
                w->Left->Color = true;
                RightRotate(x->Parent);
                x = root;

            }
        }
    }

    x->Color = true;
}

void RbDelete(TNode* z) {
    TNode* y;
    TNode* x;
    
    if (z->Left == nil || z->Right == nil) {
        y = z;
    } else {
        y = TreeSuccessor(z);
    }

    if (y->Left != nil) {
        x = y->Left;
    } else {
        x = y->Right;
    }

    x->Parent = y->Parent;

    if (y->Parent == nil) {
        root = x;
    } else if (y == y->Parent->Left) {
        y->Parent->Left = x;
    } else {
        y->Parent->Right = x;
    }

    if (y != z) {
        z->Key = y->Key;
        z->Value = y->Value;
    }

    if (y->Color == true) {
        RbDeleteFixUp(x);
    }

    delete y;
}

void EnumNodes(TNode* root, TNode** arr, int* index) {
    root->Id = *index;
    arr[*index] = root;
    ++(*index);

    if (root->Left != nil) {
        EnumNodes(root->Left, arr, index);
    }

    if (root->Right != nil) {
        EnumNodes(root->Right, arr, index);
    }
}

void Save(std::ofstream& file, int* treeSize) {
    file.write((const char*)treeSize, sizeof(int));

    TNode** nodes;

    try {
        nodes = new TNode*[*treeSize + 1];
    } catch (const std::bad_alloc& ex) {
        std::cout << "ERROR: fail to allocate the requested storage space" << std::endl;
        return;
    }

    nodes[0] = nil;
    nil->Id = 0;
    int index = 1;

    EnumNodes(root, nodes, &index);

    TNode* node;
    for (int i = 1; i < *treeSize + 1; ++i) {
        node = nodes[i];
        file.write((const char*)&(node->Key.Size), sizeof(int));
        file.write(node->Key.Arr, sizeof(char) * node->Key.Size);
        file.write((const char*)&(node->Value), sizeof(llu));
        file.write((const char*)&(node->Color), sizeof(bool));
        file.write((const char*)&(node->Parent->Id), sizeof(int));
        file.write((const char*)&(node->Left->Id), sizeof(int));
        file.write((const char*)&(node->Right->Id), sizeof(int));
    }

    delete[] nodes;
}

void Load(std::ifstream& file, int* treeSize) {
    int size;
    file.read((char*)&size, sizeof(int));

    *treeSize = size;

    if (!size) {
        return;
    }

    TNode** nodes;

    try {
        nodes = new TNode*[size + 1];
    } catch (std::bad_alloc& ex) {
        std::cout << "ERROR: fail to allocate the requested storage space" << std::endl;
        return;
    }

    nodes[0] = nil;
    nil->Id = 0;
    TString myStr("-1");

    for (int i = 1; i < size + 1; ++i) {
        nodes[i] = new TNode(myStr, -1, false, nil, nil, nil);
    }

    char* str;
    llu value;
    bool color;
    int parentId, leftId, rightId, strSize;


    for (int i = 1; i < size + 1; ++i) {
        if (i == 1) {
            root = nodes[i];
        }

        file.read((char*)&(strSize), sizeof(int));

        if (strSize) {
            try {
                str = new char[strSize + 1];
            } catch (const std::bad_alloc& ex) {
                std::cout << "ERROR: fail to allocate the requested storage space" << std::endl;
                return;
            }

            str[strSize] = '\0';
        }

        file.read(str, sizeof(char)*strSize);
        file.read((char*)&(value), sizeof(llu));
        file.read((char*)&(color), sizeof(bool));
        file.read((char*)&(parentId), sizeof(int));
        file.read((char*)&(leftId), sizeof(int));
        file.read((char*)&(rightId), sizeof(int));

        TString helper(str);

        nodes[i]->Key = helper;
        nodes[i]->Color = color;
        nodes[i]->Value = value;
        nodes[i]->Parent = nodes[parentId];
        nodes[i]->Left = nodes[leftId];
        nodes[i]->Right = nodes[rightId];

        delete[] str;
    }

    delete[] nodes;

    return;
}

void bfs() {
    std::queue<TNode*> q;

    if (root != nil) {
        q.push(root);
    }

    TNode* last;

    while(!q.empty()) {
        last = q.back();

        while (q.front() != last) {
            std::cout << q.front()->Key;

            if (q.front()->Parent != nil) {
                std::cout << '(' << q.front()->Parent->Key << ')';
            } else {
                std::cout << "(-1)";
            }

            std::cout << q.front()->Color << ' ';


            if (q.front()->Left != nil) {
                q.push(q.front()->Left);
            }

            if (q.front()->Right != nil) {
                q.push(q.front()->Right);
            }

            q.pop();
        }

        std::cout << last->Key;

        if (last->Parent != nil) {
            std::cout << '(' << last->Parent->Key << ')';
        } else {
            std::cout << "(-1)";
        }

        std::cout << last->Color << std::endl;

        if (last->Left != nil) {
            q.push(last->Left);
        }

        if (last->Right != nil) {
            q.push(last->Right);
        }

        q.pop();
    }
}

int main() {
    TString nilStr("-1");
    int treeSize = 0;

    try {
        nil = new TNode(nilStr, -1, true, nullptr, nullptr, nullptr);
    } catch (const std::bad_alloc& ex) {
        std::cout << "ERROR: fail to allocate the requested storage space" << std::endl;
        exit(EXIT_SUCCESS);
    }

    root = nil;

    TNode* temp;
    TNode* newNode;
    llu val;
    char str[256];
    TString myStr;

    std::ofstream out;
    std::ifstream in;

    while (std::cin  >> str) {
        switch (str[0]) {
            case '+':
                std::cin >> str >> val;
                myStr = str;
                myStr.ToLower();

                if (root == nil) {
                    try {
                        root = new TNode(myStr, val, true, nil, nil, nil);
                    } catch (const std::bad_alloc& ex) {
                        std::cout << "ERROR: fail to allocate the requested storage space" << std::endl;
                        exit(EXIT_SUCCESS);
                    }

                    ++treeSize;

                    std::cout << "OK" << std::endl;
                    break;
                }

                temp = RbFind(myStr);
                
                if (temp->Key == myStr) {
                    std::cout << "Exist" << std::endl;
                } else {
                    try {
                        newNode = new TNode(myStr, val, false, nil, nil, nil);
                    } catch (const std::bad_alloc& ex) {
                        std::cout << "ERROR: fail to allocate the requested storage space" << std::endl;
                        exit(EXIT_SUCCESS);
                    }

                    RbInsert(newNode, temp);
                    ++treeSize;
                    std::cout << "OK" << std::endl;
                }
                break;
            case '-':
                std::cin >> str;
                myStr = str;
                myStr.ToLower();
                temp = RbFind(myStr);

                if (root == nil || temp->Key != myStr) {
                    std::cout << "NoSuchWord" << std::endl;
                } else {
                    RbDelete(temp);
                    --treeSize;
                    std::cout << "OK" << std::endl;
                }
                break;
            case '!':
                std::cin >> str;

                if (!strcmp(str, "Save")) {
                    std::cin >> str;
                    out.open(str, std::ios::out | std::ios::binary | std::ios::trunc);

                    Save(out, &treeSize);
                    std::cout << "OK" << std::endl;
                    out.close();

                } else {
                    std::cin >> str;
                    in.open(str, std::ios::in | std::ios::binary);

                    DeleteBinaryTree(root);
                    root = nil;
                    treeSize = 0;

                    Load(in, &treeSize);

                    std::cout << "OK" << std::endl;
                    in.close();
                }

                break;
            default:
                myStr = str;
                myStr.ToLower();
                temp = RbFind(myStr);

                if (root == nil || temp->Key != myStr) {
                    std::cout << "NoSuchWord" << std::endl;
                } else {
                    std::cout << "OK: " << temp->Value << std::endl;
                }
                break;
        }
    }

    //bfs();

    DeleteBinaryTree(root);

    delete nil;

    return 0;
}