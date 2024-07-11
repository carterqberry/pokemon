#ifndef POKEMONLAB_BST_H
#define POKEMONLAB_BST_H

using namespace std;

//this is the same class used in the BST lab, some things might not be used in this lab

//functions with more than 10 lines are found underneath templated class

template<typename T>
class BST{
public:
    BST() : m_root(NULL){}  //constructor
    ~BST(){ clearTree(); }  //destructor
    BST(const BST& bst) : m_root(NULL){  //deep copy constructor
        this->copy(bst.m_root);
    }

    BST& operator=(const BST& rhs){  //deep assignment operator
        BST<T> newcopy(rhs);
        swap(this->m_root, newcopy.m_root);
        return *this;
    }

    bool addNode(const T& data){  //returns true if node is added to the tree
        return insert(m_root, data);
    }
    bool removeNode(const T& data){  //returns true if the node is removed from the tree
        return remove(NULL, m_root, data);
    }
    bool clearTree();  //returns true if the tree is cleared of all nodes
    bool findVal(const T& data) const{  //finds if value is in the tree
        return search(m_root, data);
    }
    int treeSize()const{  //returns number of nodes in the tree
        return m_BSTsize;
    }

    string toString() const;  //return BST as a string in level order traversal
    friend ostream& operator<< (ostream& os, BST<T>& bst){
        if(bst.m_root == NULL){
            return os << "empty";
        }
        else{
            return os << bst.toString();
        }
    }

private:
    struct Node{
        T m_data;
        Node* m_left;
        Node* m_right;
        Node(const T& data) : m_data(data), m_left(NULL), m_right(NULL){}
    };

    Node* m_root;
    int m_BSTsize = 0;
    bool insert(Node*& node, const T& data);  //inserts a new node
    void abort(Node*& node);  //deletes the tree
    bool search(Node* node, const T& data) const;  //finds if node is in the tree
    bool remove(Node* parent, Node* node, const T& data);  //removes the given node, and fixes tree accordingly
    bool infix(Node* root, int lvl, stringstream& out) const;  //outputs nodes at a given level
    void copy(Node* node){  //copies the tree
        if(node == NULL){
            return;
        }
        insert(this->m_root, node->m_data);
        copy(node->m_left);
        copy(node->m_right);
    }
    Node* predecessor(Node* node){  //finds and deletes the predecessor node
        Node* current = node;
        Node* parent = current;  //sets parent node to current node
        current = current->m_left;  //sets current node to the left node

        while(current && current->m_right != NULL){  //loops to the rightmost leaf
            parent = current;  //sets parent to current node
            current = current->m_right;  //sets current node to the right child node
        }
        if(parent == node){  //deletes left node if parent node is given
            parent->m_left = NULL;
        }
        else{  //otherwise deletes the right node
            parent->m_right =NULL;
        }
        return current;  //return the current predecessor node
    }
};

//functions that were more than 10 lines:

template<typename T>
bool BST<T>::insert(Node*& node, const T& data){  //inserts a new node
    if(node == NULL){  //inserts node if node isnt found
        node = new Node(data);
        m_BSTsize++;
        return true;
    }
    if(data == node->m_data){  //if new node if node with data is found
        return false;
    }
    if(data < node->m_data){  //if data is less than nodes data insert node left
        return insert(node->m_left, data);
    }
    return insert(node->m_right, data);  //otherwise insert node right
}

template<typename T>
void BST<T>::abort(Node*& node){  //deletes the tree
    if(node == NULL){
        return;
    }
    abort(node->m_left);
    abort(node->m_right);
    m_BSTsize--;

    delete(node);
    node = NULL;
    return;
}

template<typename T>
bool BST<T>::search(Node* node, const T& data) const{  //finds if node is in the tree
    if(node == NULL){  //if node is not found returns false
        return false;
    }
    if(node->m_data == data){  //if node is found returns true
        return true;
    }
    if(node->m_data < data){  //if nodes data is less than the given data search right
        return search(node->m_right, data);
    }
    return search(node->m_left, data);  //otherwise return left
}

template<typename T>
bool BST<T>::remove(Node* parent, Node* node, const T& data){  //removes the given node, and fixes tree accordingly
    //base case:
    if(node == NULL){  //if node isnt found return false
        return false;
    }
    if(data < node->m_data){  //if nodes data is less than the given data remove left
        return remove(node, node->m_left, data);
    }
    if(data > node->m_data){  //if given data is greater than nodes data remove right
        return remove(node, node->m_right, data);
    }

    //node has no child:
    if(node->m_left == NULL && node->m_right == NULL){  //if node has no left or right children
        if(parent == NULL){  //if its the only node in the tree
            m_root = NULL;  //delete the root
        }
        else if(parent->m_left == node){  //cleans up the parents link to the node
            parent->m_left = NULL;  //if node is the left child
        }
        else{
            parent->m_right = NULL;  //and if its the right child
        }
        m_BSTsize--;
        delete(node);
        return true;
    }

        //node with only one right child:
    else if(node->m_left == NULL){
        if(parent->m_left == node){
            parent-> m_left = node->m_right;
        }
        else{
            parent->m_right = node->m_right;
        }
        m_BSTsize--;
        delete(node);
        return true;
    }

        //node with only one left child:
    else if(node->m_right == NULL){
        if(parent->m_left == node){
            parent->m_left = node->m_left;
        }
        else{
            parent->m_right = node->m_left;
        }
        m_BSTsize--;
        delete(node);
        return true;
    }

        //lastly node with no children:
    else{
        Node* temp = predecessor(node);  //return and delete the predecessor
        node->m_data = temp->m_data;  //copy predecessor into temp node
        m_BSTsize--;
        delete(temp);
        return true;
    }
}

template<typename T>
bool BST<T>::infix(Node* root, int lvl, stringstream& out) const{  //outputs nodes at a given level
    if(root == NULL){
        return false;
    }
    if(lvl == 1){
        out << " " << root->m_data;
        if((root->m_left != NULL) || (root->m_right != NULL)){
            return true;
        }
        return false;
    }
    if((lvl == 2) && !root->m_left && root->m_right){
        out << " _";
    }
    bool left = infix(root->m_left, lvl - 1, out);
    bool right = infix(root->m_right, lvl - 1, out);
    if((lvl == 2) && root->m_left && !root->m_right){
        out << " _";
    }
    return left || right;
}

template<typename T>
bool BST<T>::clearTree(){  //returns true if the tree is cleared of all nodes
    abort(m_root);
    if(m_BSTsize == 0){  //if list is empty return true
        return true;
    }
    else{
        return false;
    }
}

template<typename T>
string BST<T>::toString() const{  //return BST as a string in level order traversal
    stringstream out;
    if(m_root != NULL){  //if root isnt empty
        int lvl = 0;
        do{
            out << endl << " " << ++lvl << ":"; //outputs the level incremented
        }
        while(infix(m_root, lvl, out));  //outputs the given level
    }
    return out.str();
}

#endif //POKEMONLAB_BST_H
