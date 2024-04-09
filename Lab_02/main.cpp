#include <iostream>
#include <queue>

//при удалении и добавлении добавь обновление значения

struct node {
    int key;
    bool color;
    node* left;
    node* right;
    node* parent;

    node(int _key, bool _color, node* _left, node* _right, node* _parent) {
        key = _key;
        color = _color;
        left = _left;
        right = _right;
        parent = _parent;
    }

    ~node() = default;
};

node* root = nullptr;
node* nil = nullptr;

void DeleteBinaryTree(node* r) {
    if (r == nil) {
        return;
    }

    DeleteBinaryTree(r->left);
    DeleteBinaryTree(r->right);

    delete r;
}

node* TreeMinimum(node* x) {
    node* temp = x;

    while (temp->left != nil) {
        temp = temp->left;
    }

    return temp;
}

node* TreeSuccessor(node* x) {
    if (x->right != nil) {
        return TreeMinimum(x->right);
    }

    node* y = x->parent;
    node* temp = x;

    while (y != nil && temp == y->right) {
        temp = y;
        y = y->parent;
    }

    return y;
}

void leftRotate(node* x) {
    node* y = x->right;
    x->right = y->left;

    if (y->left != nil) {
        y->left->parent = x;
    }

    y->parent = x->parent;

    if (x->parent == nil) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

void rightRotate(node* y) {
    node* x = y->left;
    y->left = x->right;

    if (x->right != nil) {
        x->right->parent = y;
    }

    x->parent = y->parent;

    if (y->parent == nil) {
        root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }

    x->right = y;
    y->parent = x;
}

node* rbFind(int key) {
    node* y = nil;
    node* x = root;

    while (x != nil) {
        y = x;

        if (key < x->key) {
            x = x->left;
        } else if (key > x->key) {
            x = x->right;
        } else {
            y = x;
            x = nil;
        }
    }

    return y;
}

void rbInsertFixUp(node* z) {
    while (z->parent->color == false) {
        if (z->parent == z->parent->parent->left) {

            node* y = z->parent->parent->right;
            
            if (y->color == false) {

                z->parent->color = true;
                y->color = true;
                z->parent->parent->color = false;
                z = z->parent->parent;

            } else if (z == z->parent->right) {

                z = z->parent;
                leftRotate(z);

            } else {

                z->parent->color = true;
                z->parent->parent->color = false;
                rightRotate(z->parent->parent);

            }

        } else {
            node* y = z->parent->parent->left;

            if (y->color == false) {
                
                z->parent->color = true;
                y->color = true;
                z->parent->parent->color = false;
                z = z->parent->parent;

            } else if (z == z->parent->left) {

                z = z->parent;
                rightRotate(z);

            } else {

                z->parent->color = true;
                z->parent->parent->color = false;
                leftRotate(z->parent->parent);

            }
        }
    }

    root->color = true;
}

void rbInsert(node* z, node* y) {
    z->parent = y;

    if (y == nil) {
        root = z;
    } else if (z->key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }

    z->left = nil;
    z->right = nil;
    z->color = false;

    rbInsertFixUp(z);
}

void rbDeleteFixUp(node* x) {
    while (x != root && x->color == true) {
        if (x == x->parent->left) {

            node* w = x->parent->right;

            if (w->color == false) {

                w->color = true;
                x->parent->color = false;
                leftRotate(x->parent);
                w = x->parent->right;

            } else if (w->left->color == true && w->right->color == true) {

                w->color = false;
                x = x->parent;

            } else if (w->right->color == true) {

                w->left->color = true;
                w->color = false;
                rightRotate(w);
                w = x->parent->right;

            } else {

                w->color = x->parent->color;
                x->parent->color = true;
                w->right->color = true;
                leftRotate(x->parent);
                x = root;

            }

        } else {

            node* w = x->parent->left;

            if (w->color == false) {

                w->color = true;
                x->parent->color = false;
                rightRotate(x->parent);
                w = x->parent->left;

            } else if (w->left->color == true && w->right->color == true) {
                
                w->color = false;
                x = x->parent;

            } else if (w->left->color == true) {

                w->right->color = true;
                w->color = false;
                leftRotate(w);
                w = x->parent->left;

            } else {

                w->color = x->parent->color;
                x->parent->color = true;
                w->left->color = true;
                rightRotate(x->parent);
                x = root;

            }

        }
    }

    x->color = true;
}

void rbDelete(node* z) {
    node* y;
    node* x;
    
    if (z->left == nil || z->right == nil) {
        y = z;
    } else {
        y = TreeSuccessor(z);
    }

    if (y->left != nil) {
        x = y->left;
    } else {
        x = y->right;
    }

    x->parent = y->parent;

    if (y->parent == nil) {
        root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }

    if (y != z) {
        z->key = y->key;
    }

    if (y->color == true) {
        rbDeleteFixUp(x);
    }

    delete y;
}

void bfs() {
    std::queue<node*> q;

    if (root != nil) {
        q.push(root);
    }

    node* last;

    while(!q.empty()) {
        last = q.back();

        while (q.front() != last) {
            std::cout << q.front()->key << '(' << (q.front()->parent ? q.front()->parent->key : -1) << ')' << q.front()->color << ' ';

            if (q.front()->left != nil) {
                q.push(q.front()->left);
            }

            if (q.front()->right != nil) {
                q.push(q.front()->right);
            }

            q.pop();
        }

        std::cout << last->key << '(' << (q.front()->parent ? q.front()->parent->key : -1) << ")" << q.front()->color << std::endl;

        if (last->left != nil) {
            q.push(last->left);
        }

        if (last->right != nil) {
            q.push(last->right);
        }

        q.pop();
    }
}

int main() {
    nil = new node(-1, true, nullptr, nullptr, nullptr);
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

    rbDelete(root->left);

    bfs();
    std::cout << std::endl;

    rbDelete(root);

    bfs();
    std::cout << std::endl;

    rbDelete(root->right);

    bfs();
    std::cout << std::endl;

    node* z = new node(4, false, nil, nil, nil);
    rbInsert(z, rbFind(4));
    bfs();

    DeleteBinaryTree(root);

    delete nil;

    return 0;
}