#include <iostream>
#include <chrono>
#include <random>
#include <queue>
#include <vector>


using namespace::std;


class MySet {
private:
    struct Node {
        int value;
        Node* left;
        Node* right;
        Node(int v) : value(v), left(nullptr), right(nullptr) {}
    };

    Node* root;

    void tree_destructor(Node* node) {
        if (node != nullptr) {
            tree_destructor(node->left);
            tree_destructor(node->right);
            delete node;
        }
    }

    Node* copy(Node* node) {
        if (node == nullptr) {
            return nullptr;
        }
        Node* new_node = new Node(node->value);
        new_node->left = copy(node->left);
        new_node->right = copy(node->right);
        return new_node;
    }

    void print(Node* node) {
        if (node != nullptr) {
            print(node->left);
            std::cout << node->value << " ";
            print(node->right);
        }
    }

    bool insert(Node*& node, int key) {
        if (node == nullptr) {
            node = new Node(key);
            return true;
        }
        if (key < node->value) {
            return insert(node->left, key);
        }
        else if (key > node->value) {
            return insert(node->right, key);
        }
        else {
            return false;
        }
    }

    bool contains(Node* node, int key) {
        if (node == nullptr) {
            return false;
        }
        if (key < node->value) {
            return contains(node->left, key);
        }
        else if (key > node->value) {
            return contains(node->right, key);
        }
        else {
            return true;
        }
    }

    bool erase(Node*& node, int val) {
        if (node == nullptr) {
            return false;
        }
        if (val < node->value) {
            return erase(node->left, val);
        }
        else if (val > node->value) {
            return erase(node->right, val);
        }
        else {
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                node = nullptr;
            }
            else if (node->left == nullptr) {
                Node* temp = node;
                node = node->right;
                delete temp;
            }
            else if (node->right == nullptr) {
                Node* temp = node;
                node = node->left;
                delete temp;
            }
            else {
                Node* min_right = node->right;
                while (min_right->left != nullptr) {
                    min_right = min_right->left;
                }
                node->value = min_right->value;
                erase(node->right, min_right->value);
            }
            return true;
        }
    }

public:
    MySet() : root(nullptr) {}

    MySet(const MySet& other) : root(copy(other.root)) {}

    ~MySet() {
        tree_destructor(root);
    }

    MySet& operator=(const MySet& other) {
        if (this != &other) {
            tree_destructor(root);
            root = copy(other.root);
        }
        return *this;
    }

    void print() {
        print(root);
        std::cout << std::endl;
    }

    bool insert(int val) {
        return insert(root, val);
    }

    bool contains(int val) {
        return contains(root, val);
    }

    bool erase(int val) {
        return erase(root, val);
    }

    vector<int> tree_to_vector() {
        vector<int> result;
        if (root == nullptr) {
            return result;
        }

        queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            Node* current = q.front();
            q.pop();
            result.push_back(current->value);

            if (current->left != nullptr) {
                q.push(current->left);
            }
            if (current->right != nullptr) {
                q.push(current->right);
            }
        }

        return result;
    }
};


int lcg() {
    static int x = 0;
    x = (1021 * x + 24631) % 116640;
    return x;
}


//Вариант 2: напишите функции, возвращающие объединение и симметрическую разность 2-х множеств целых чисел.


MySet my_union(MySet first, MySet second) {
    MySet res;
    vector<int> v1 = first.tree_to_vector();
    for (size_t i = 0; i < v1.size(); ++i) {
        int elem = v1[i];
        if (second.contains(elem)) {
            res.insert(elem);
        }
    }
    return res;
}


MySet simmertic_difference(MySet first, MySet second) {
    MySet res;
    vector<int> v1 = first.tree_to_vector(), v2 = second.tree_to_vector();
    for (size_t i = 0; i < v1.size(); ++i) {
        int elem = v1[i];
        if (!second.contains(elem)) {
            res.insert(elem);
        }
    }

    for (size_t i = 0; i < v2.size(); ++i) {
        int elem = v2[i];
        if (!first.contains(elem) && !res.contains(elem)) {
            res.insert(elem);
        }
    }
    return res;
}


int find_elem(vector<int> vec, int elem) {
    if (!vec.size()) return -1;
    for (int ind = 0; ind < vec.size(); ++ind) {
        if (vec[ind] == elem) return ind;
    }
    return -1;
}


void compare() {
    int num_trials = 100;
    const int sizes[] = { 1000, 10000, 100000 };

    for (int size : sizes) {
        double total_time1 = 0.0, total_time2 = 0.0, total_time3 = 0.0,
            total_time1_1 = 0.0, total_time2_1 = 0.0, total_time3_1 = 0.0;
        for (int i = 0; i < num_trials; ++i) {
            vector<int> vec;
            MySet set;
            auto start1 = chrono::high_resolution_clock::now();
            for (int j = 0; j < size; ++j) {
                set.insert(lcg());
                if (j == round(size / 2)) set.insert(1166);
            }
            auto end1 = chrono::high_resolution_clock::now();
            chrono::duration<double> diff1 = end1 - start1;
            total_time1 += diff1.count();

            auto start1_1 = chrono::high_resolution_clock::now();
            for (int j = 0; j < size; ++j) {
                vec.push_back(lcg());
                if (j == round(size / 2)) vec.push_back(1166);
            }
            auto end1_1 = chrono::high_resolution_clock::now();
            chrono::duration<double> diff1_1 = end1_1 - start1_1;
            total_time1_1 += diff1_1.count();

            auto start2 = chrono::high_resolution_clock::now();
            set.contains(1166);
            auto end2 = chrono::high_resolution_clock::now();
            chrono::duration<double> diff2 = end2 - start2;
            total_time2 += diff2.count();

            auto start2_1 = chrono::high_resolution_clock::now();
            find_elem(vec, 1166);
            auto end2_1 = chrono::high_resolution_clock::now();
            chrono::duration<double> diff2_1 = end2_1 - start2_1;
            total_time2_1 += diff2_1.count();


            auto start3 = chrono::high_resolution_clock::now();
            set.insert(1167);
            set.erase(1167);
            auto end3 = chrono::high_resolution_clock::now();
            chrono::duration<double> diff3 = end3 - start3;
            total_time3 += diff3.count();

            auto start3_1 = chrono::high_resolution_clock::now();
            vec.insert(vec.begin() + 5, 1167);
            vec.erase(vec.begin() + 5);
            auto end3_1 = chrono::high_resolution_clock::now();
            chrono::duration<double> diff3_1 = end3_1 - start3_1;
            total_time3_1 += diff3_1.count();

        }
        double avg_time1 = total_time1 / num_trials, avg_time1_1 = total_time1_1 / num_trials, avg_time2 = total_time2 / num_trials * 10, avg_time2_1 = total_time2_1 / num_trials * 10, avg_time3 = total_time3 / num_trials * 10, avg_time3_1 = total_time3_1 / num_trials * 10;
        cout
            << "Average time to fill MySet with " << size << " elems: " << avg_time1 << " seconds. " << endl
            << "Average time to fill vector with " << size << " elems: " << avg_time1_1 << " seconds. " << endl
            << "difference in time: " << max(avg_time1, avg_time1_1) - min(avg_time1, avg_time1_1) << endl << endl

            << "Average time to find elem in MySet with " << size << " elems: " << avg_time2 << " seconds. " << endl
            << "Average time to find elem in vector with " << size << " elems: " << avg_time2_1 << " seconds. " << endl
            << "difference in time: " << max(avg_time2, avg_time2_1) - min(avg_time2, avg_time2_1) << endl << endl

            << "Average time to insert and erase elem in MySet with " << size << " elems: " << avg_time3 << " seconds. " << endl
            << "Average time to insert and erase elem in vector with " << size << " elems: " << avg_time3_1 << " seconds. "
            << "difference in time: " << max(avg_time3, avg_time3_1) - min(avg_time3, avg_time3_1) << endl << endl;
    }
}


int main() {
    // main part

    compare();

    // idz part

    MySet frst;
    frst.insert(1);
    frst.insert(2);
    frst.insert(4);
    frst.insert(7);
    frst.insert(3);
    frst.insert(8);

    MySet scnd;
    scnd.insert(1);
    scnd.insert(2);
    scnd.insert(5);
    scnd.insert(7);
    scnd.insert(9);
    scnd.insert(10);

    my_union(frst, scnd).print();
    simmertic_difference(frst, scnd).print();

    return 0;
}
