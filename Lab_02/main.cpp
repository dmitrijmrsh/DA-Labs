#include <iostream>
#include <cstring>
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

        void Print(std::ostream&);

        int GetSize();
        int GetCapacity();

        char operator[] (int) const;
        TString& operator = (const TString&);

        bool operator < (const TString&) const;
        bool operator > (const TString&) const;
        bool operator == (const TString&) const;
        bool operator != (const TString&) const;

        friend std::ostream& operator << (std::ostream&, TString&);

        ~TString() = default;
};

void TString::Reallocate(int newCapacity) {
    if (Arr == nullptr) {
        return;
    }

    char* temp = new char[Size];

    for (int i = 0; i < Size; ++i) {
        temp[i] = Arr[i];
    }

    delete[] Arr;
    Arr = new char[newCapacity];

    for (int i = 0; i < Size; ++i) {
        Arr[i] = temp[i];
    }

    Capacity = newCapacity;
    delete[] temp;
}

TString::TString() {
    Size = 0;
    Capacity = 2;
    Arr = new char[Capacity];
}

TString::TString(int size) {
    Size = size;

    if (Size > 0) {
        Capacity = Size * 2;
    } else {
        Capacity = 2;
    }

    Arr = new char[Capacity];
}

TString::TString(const char* str) {
    Size = strlen(str);
    Capacity = Size + 1;
    Arr = new char[Capacity];
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

char TString::operator[] (int index) const {
    if (index >= Size) {
        throw std::logic_error("Too big index");
    }

    return Arr[index];
}

TString& TString::operator = (const TString& other) {
    delete[] Arr;
    Size = other.Size;
    Capacity = other.Capacity;
    Arr = new char[Capacity];

    for (int i = 0; i < Size; ++i) {
        Arr[i] = other[i];
    }

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

std::ostream& operator << (std::ostream& out, TString& str) {
    str.Print(out);
    return out;
}

/*TString::~TString() {
    Size = 0;
    Capacity = 0;
    delete[] Arr;
    Arr = nullptr;
}*/

//при удалении и добавлении добавь обновление значения

struct TNode {
    TString Key;
    llu Value;
    bool Color;
    TNode* Left;
    TNode* Right;
    TNode* Parent;

    TNode(TString key, llu value, bool color, TNode* left, TNode* right, TNode* parent) {
        Key = key;
        Value = value;
        Color = color;
        Left = left;
        Right = right;
        Parent = parent;
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

    delete[] r->Key.Arr;
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

TNode* rbFind(TString& key) {
    TNode* y = nil;
    TNode* x = root;

    while (x != nil) {
        y = x;

        if (key < x->Key) {
            x = x->Left;
        } else if (key > x->Key) {
            x = x->Right;
        } else {
            y = x;
            x = nil;
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

void bfs() {
    std::queue<TNode*> q;

    if (root != nil) {
        q.push(root);
    }

    TNode* last;
    TString helper;

    while(!q.empty()) {
        last = q.back();

        while (q.front() != last) {
            helper = q.front()->Parent ? q.front()->Parent->Key : "-1";
            std::cout << q.front()->Key << '(' << helper << ')' << q.front()->Color << ' ';

            if (q.front()->Left != nil) {
                q.push(q.front()->Left);
            }

            if (q.front()->Right != nil) {
                q.push(q.front()->Right);
            }

            q.pop();
        }

        helper = q.front()->Parent ? q.front()->Parent->Key : "-1";
        std::cout << last->Key << '(' << helper << ')' << q.front()->Color << std::endl;

        if (last->Left != nil) {
            q.push(last->Left);
        }

        if (last->Right != nil) {
            q.push(last->Right);
        }

        q.pop();
    }

    delete[] helper.Arr;
}

int main() {
    /*nil = new node(-1, true, nullptr, nullptr, nullptr);
    root = nil;

    int val;
    node* temp;

    for (int i = 0; i < 9; ++i) {
        std::cin >> val;

        if (root == nil) {
            root = new node(val, true, nil, nil, nil);
        } else {
            temp = rbFind(val);

            if (temp && temp->key == val) {
                std::cout << "Already exist " << temp->key << std::endl;
            } else {
                node* z = new node(val, false, nil, nil, nil);
                rbInsert(z, temp);
            }
        }
    }

    bfs();
    std::cout << std::endl;

    rbDelete(rbFind(384));
    bfs();
    std::cout << std::endl;

    DeleteBinaryTree(root);

    delete nil;*/

    TString bebra("-1");

    nil = new TNode(bebra, -1, true, nullptr, nullptr, nullptr);
    root = nil;

    FILE* file;
    llu val;
    char str[256];
    //TNode* z;

    file = fopen("in.txt", "r");


    for (int i = 0; i < 10; ++i) {
        fscanf(file, "%s %llu", str, &val);

        //printf("%s %llu\n", str, val);

        TString myStr(str);
        //std::cout << myStr << " " << val << std::endl;

        if (root == nil) {
            root = new TNode(myStr, val, true, nil, nil, nil);
        } else {

            if (rbFind(myStr) && rbFind(myStr)->Key == myStr) {
                std::cout << "Already exist" << std::endl;
            } else {
                //z = new TNode(myStr, val, false, nil, nil, nil);
                RbInsert(new TNode(myStr, val, false, nil, nil, nil), rbFind(myStr));
            }

        }

        delete[] myStr.Arr;
    }
    bfs();

    fclose(file);

    DeleteBinaryTree(root);

    delete[] bebra.Arr;
    delete[] nil->Key.Arr;
    delete nil;

    return 0;
}