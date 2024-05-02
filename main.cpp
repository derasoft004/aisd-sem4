#include <iostream>
#include <vector>
#include <stdexcept>
#include <deque>
#include <cmath>


using namespace std;

template <typename K, typename V>
class HashTable {
private:
    struct Pair {
        K _key;
        V _value;
        Pair* next;
        Pair(K key, V value) : _key(key), _value(value), next(nullptr) {}
    };
    vector<Pair*> _data;
    size_t _size;

    size_t hashFunction(K key) {
        long long int h = (long long int)key * 2654435761;

        h %= (long long int)(pow(2, 32));
        h >>= (32 - powe(_size));

        return h;
    }
    size_t powe(size_t n) {
        size_t p = 0;
        while (n > 1) {
            n >>= 1;
            ++p;
        }
        return p;
    }
public:
    HashTable(size_t size) {
        if (size == 0)
            throw invalid_argument("Size zero");
        _size = size;
        _data.resize(size);
    };

    HashTable(size_t size, const V& max, const V& min) {
        if (size == 0)
            throw invalid_argument("Size zero");
        _size = size;
        _data.resize(size);
        for (int i = 0; i < size; ++i) {
            V value = rand() % (max - min + 1) + min;
            _data[i] = new Pair(i, value);
        }
    };

    HashTable(const HashTable& other) {
        _size = other.get_size();
        _data.resize(_size);
        for (size_t i = 0; i < _size; ++i) {
            Pair* current = other._data[i];
            while (current != nullptr) {
                insert(current->_key, current->_value);
                current = current->next;
            }
        }
    }

    ~HashTable() {
        for (size_t i = 0; i < _size; i++) {
            Pair* current = _data[i];
            while (current) {
                Pair* tmp = current;
                current = current->next;
                delete tmp;
            }
        }
        _size = 0;
    }

    HashTable& operator=(const HashTable& other) {
        if (this != &other) {
            for (size_t i = 0; i < _size; i++) {
                Pair* current = _data[i];
                while (current != nullptr) {
                    Pair* tmp = current;
                    current = current->next;
                    delete tmp;
                }
            }
            _size = other._size;
            _data.resize(_size);
            for (size_t i = 0; i < _size; ++i) {
                Pair* current = other._data[i];
                while (current != nullptr) {
                    insert(current->key, current->value);
                    current = current->next;
                }
                ++_size;
            }

        }
        return *this;
    };

    size_t getSize() const {
        return _size;
    }

    void print() {

        for (int i = 0; i < _size; ++i) {
            Pair* current = _data[i];

            while (current) {
                cout << current->_key << ": ";
                cout << current->_value << " ";
                cout << endl;
                current = current->next;
            }
        }
    };

    void insert(K key, V value) {
        size_t index = hashFunction(key);
        Pair* newPair = new Pair(key, value);
        newPair->next = _data[index];
        _data[index] = newPair;
        _size++;
    }

    void insertOrAssign(K key, V value) {
        size_t index = hashFunction(key);
        Pair* current = _data[index];
        while (current) {
            if (current->_key == key) {
                current->_value = value;
                return;
            }
            current = current->next;
        }
        insert(key, value);
    }

    bool erase(K key) {
        size_t index = hashFunction(key);
        Pair* current = _data[index];
        Pair* prev = nullptr;
        while (current) {
            if (current->_key == key) {
                if (prev) {
                    prev->next = current->next;
                }
                else {
                    _data[index] = current->next;
                }
                delete current;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }

    bool contains(V value) const {
        for (int i = 0; i < _size; ++i) {
            Pair* current = _data[i];
            while (current) {
                if (current->_value == value)
                    return true;
                current = current->next;
            }
        }
        return false;
    };

    V* search(K key) {
        size_t index = hashFunction(key);
        Pair* tmp = _data[index];
        while (tmp) {
            if (tmp->_key != key)
                tmp = tmp->next;
            return &(tmp->_value);
        }
        return nullptr;
    };

    int count(K key) {
        size_t index = hashFunction(key);
        int count = 0;
        Pair* tmp = _data[index];
        while (tmp) {
            ++count;
            tmp = tmp->next;
        }
        return count;
    };
};

size_t hashRom(const char& c) {
    switch (c) {
    case 'I': return 1;
    case 'V': return 5;
    case 'X': return 10;
    case 'L': return 50;
    case 'C': return 100;
    case 'D': return 500;
    case 'M': return 1000;
    }
    return 0;
}


size_t romanToArabish(const string& key) {
    deque<size_t> values;
    for (char c : key) {
        values.push_front(hashRom(c));
    }
    size_t answer = 0;
    size_t previous = 0;
    for (size_t num : values) {
        if (num >= previous)
            answer += num;
        else
            answer -= num;
        previous = num;
    }
    return answer;
}



int main()
{
    HashTable<int, string> t1(4);

    t1.insert(1, "a");
    t1.insert(2, "b");
    t1.insert(2, "c");
    t1.insertOrAssign(2, "d");
    t1.insert(10, "e");
    t1.insert(4, "f");
    t1.erase(2);
    t1.print();
    cout << romanToArabish("IV") << ' ' << romanToArabish("XVI") << endl;
}

