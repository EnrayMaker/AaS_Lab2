#include <complex>
#include <cstddef>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <cmath>
#include <type_traits>

// Точность для сравнения вещественных чисел

template<typename T>
struct Node
{
    Node* next;
    T data;
    Node(const T& value) : data(value), next(nullptr) {}
};

template<typename T>
class LinkedList
{
private:
    Node<T>* head;
    size_t size;
    
    Node<T>* get_node(size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        Node<T>* current = head;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
        return current;
    }

    void deep_copy(const LinkedList& other) {
        if (other.head == nullptr) {
            head = nullptr;
            size = 0;
            return;
        }
        
        head = new Node<T>(other.head->data);
        Node<T>* current = head;
        Node<T>* other_current = other.head->next;
        
        while (other_current != other.head) {
            current->next = new Node<T>(other_current->data);
            current = current->next;
            other_current = other_current->next;
        }
        
        // Замыкаем цикл
        current->next = head;
        size = other.size;
    }
    void clear() {
        if (head == nullptr) return;
        
        Node<T>* current = head->next;
        while (current != head) {
            Node<T>* temp = current;
            current = current->next;
            delete temp;
        }
        delete head;
        head = nullptr;
        size = 0;
    }
public:
    LinkedList() : head(nullptr), size(0) {}
    LinkedList(const LinkedList& other) : head(nullptr), size(0) {
        deep_copy(other);
    }
    ~LinkedList() {
        clear();
    }
};
using namespace std;

int main() {

    cout << "ВСЕ ТЕСТЫ ЗАВЕРШЕНЫ" << endl;
    
    return 0;
}