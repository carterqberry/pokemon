#ifndef POKEMONLAB_SET_H
#define POKEMONLAB_SET_H

#include <string>
#include "SetInterface.h"
#include "BST.h"

using namespace std;

//this class gets all its functionality from the BST class.

template <typename T>
class Set : public SetInterface<T>{
public:
    Set(){}
    ~Set(){ bst.clearTree(); }

    virtual bool insert(const T& item){  //if the set doesnt contain the item, inserts the item.
        return bst.addNode(item);
    }
    virtual void clear(){  //removes all the items in the set
        bst.clearTree();
    }
    virtual size_t size() const{  //returns the number of elements in the set
        return bst.treeSize();
    }
    virtual size_t count(const T& item){
        return (bst.findVal(item) ? 1 : 0);
    }
    virtual string toString() const{  //returns string containing the items in the set
        return bst.toString();
    }
    friend ostream& operator<< (ostream& os, Set set){  //friend operator <<
        return os << set.toString();
    }

private:
    BST<string> bst;
};
#endif //POKEMONLAB_SET_H
