#ifndef POKEMONLAB_HASHMAP_H
#define POKEMONLAB_HASHMAP_H

#include "Pair.h"
#include "HashMapInterface.h"
using namespace std;

template<typename K, typename V>
class HashMap : public HashMapInterface<K,V>{
public:
    HashMap() :  //constructor
        capacity(DEFAULT_MAP_HASH_TABLE_SIZE),
        pairs(new Pair<K,V>[DEFAULT_MAP_HASH_TABLE_SIZE]),
        load(0.00){}
    ~HashMap(){ delete[] pairs; }  //deconstructor

    virtual V& operator[](const K& key){  //read and write [] operator
        load = (static_cast<double>(size()) / static_cast<double>(capacity)) * 100;
        if(load >= LOAD_THRESHOLD){
            cout << "Time to reallocate!";
            reallocate();
        }
        int index = hash(key);
        return pairs[index].second;
    }
    virtual size_t count(const K& key){  //return number of elements in the map from the given key
        int items = 0;
        for(int i = 0; i < capacity; i++){
            if(pairs[i].first == key){
                items++;
            }
        }
        return items;
    }
    virtual void clear(){  //removes all items in the map
        delete[] pairs;
        new Pair<K, V>[DEFAULT_MAP_HASH_TABLE_SIZE];
        capacity = DEFAULT_MAP_HASH_TABLE_SIZE;
    }
    virtual size_t size() const{  //returns number of key-value pairs in the map
        int items = 0;
        for(int i = 0; i < capacity; i++){
            if(!pairs[i].isEmpty())
            {
                items++;
            }
        }
        return items;
    }
    virtual size_t max_size() const{  //returns the maximum number of key-value pairs in the map
        return capacity;
    }
    virtual string toString() const{  //returns key-value paris in a string
        stringstream os, os_loop;
        int items = 0;
        for(size_t i = 0; i < capacity; ++i){
            if(!pairs[i].isEmpty()){
                os_loop << endl << "  [" << i << ":" << pairs[i].first << "->" << pairs[i].second << "]";
                items++;
            }
        }
        os << ": " << items << "/" << capacity << os_loop.str();// << endl;
        return os.str();
    }
    friend ostream& operator<<(ostream& os, HashMap& map){  //friend << operator
        return os << map.toString();
    }

private:
    int capacity;
    Pair<K,V>* pairs;
    double load;
    int hashVal = HASH_CONSTANT;

    void reallocate(){  //reallocates array so it is twice as large much like a vector
        Pair<K,V>* temp_pairs = new Pair<K,V>[2 * capacity];
        capacity = 2 * capacity;
        std::swap(pairs, temp_pairs);
        for(int i = 0; i < capacity / 2; i++){  //moves everything from the old array to the new array
            if(!temp_pairs[i].isEmpty()){
                pairs[hash(temp_pairs[i].first)].second = temp_pairs[i].second;
            }
        }
        delete[] temp_pairs;  //deletes the old array
    }
    int hash(const K& key){  //returns a hash value from the given key
        int index;
        long long sum = 0;
        int n = key.length();
        for(int i = 0; i < key.length(); i++){  //loops through the whole string
            long long exponent = 1;
            n--;
            for(int j = 0; j < n; j++){
                exponent *= HASH_CONSTANT;
            }
            sum += key[i] * exponent;
        }
        index = sum % capacity;
        int a = 1;
        while(1){
            if(pairs[index].first.length() == 0){  //if the key is empty insert one there
                cout << " Insert " << key << endl;
                pairs[index].first = key;
                break;
            }
            else if(pairs[index].first == key){  //break if it already exists
                break;
            }
            cout << endl << " " << key << " collided with " << pairs[index].first;
            index = (index + a) % capacity;
            a += 2;
        }
        return index;
    }
};

#endif //POKEMONLAB_HASHMAP_H
