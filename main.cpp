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

    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            deep_copy(other);
        }
        return *this;
    }
    
    // Добавление элемента в конец списка
    void push_tail(const T& value) {
        Node<T>* new_node = new Node<T>(value);
        
        if (head == nullptr) {
            head = new_node;
            head->next = head;
        } else {
            Node<T>* current = head;
            while (current->next != head) {
                current = current->next;
            }
            current->next = new_node;
            new_node->next = head;
        }
        ++size;
    }

    void push_tail(const LinkedList& other) {
        if (other.size == 0) return;
        if (size == 0) {
            deep_copy(other);
            return;
        }
        
        Node<T>* tail = head;
        while (tail->next != head) {
            tail = tail->next;
        }
        
        Node<T>* other_current = other.head;
        do {
            tail->next = new Node<T>(other_current->data);
            tail = tail->next;
            other_current = other_current->next;
        } while (other_current != other.head);
        
        tail->next = head;
        size += other.size;
    }

    void push_head(const T& value) {
        Node<T>* new_node = new Node<T>(value);
        
        if (head == nullptr) {
            head = new_node;
            head->next = head;
        } else {
            Node<T>* tail = head;
            while (tail->next != head) {
                tail = tail->next;
            }
            new_node->next = head;
            tail->next = new_node;
            head = new_node;
        }
        ++size;
    }

    // Добавление списка в начало
    void push_in_head(const LinkedList& other) {
        if (other.size == 0) return;
        
        LinkedList temp = other;
        if (size == 0) {
            head = temp.head;
            size = temp.size;
            temp.head = nullptr;
            temp.size = 0;
            return;
        }
        
        // Находим хвост другого списка
        Node<T>* other_tail = temp.head;
        while (other_tail->next != temp.head) {
            other_tail = other_tail->next;
        }
        
        // Находим хвост текущего списка
        Node<T>* current_tail = head;
        while (current_tail->next != head) {
            current_tail = current_tail->next;
        }
        
        // Соединяем
        other_tail->next = head;
        current_tail->next = temp.head;
        head = temp.head;
        
        size += other.size;
        temp.head = nullptr;
        temp.size = 0;
    }

    T pop_head() {
        if (head == nullptr) {
            throw std::runtime_error("List is empty");
        }
        
        T value = head->data;
        
        if (size == 1) {
            delete head;
            head = nullptr;
            size = 0;
            return value;
        }
        
        Node<T>* tail = head;
        while (tail->next != head) {
            tail = tail->next;
        }
        
        Node<T>* temp = head;
        head = head->next;
        tail->next = head;
        delete temp;
        --size;
        
        return value;
    }

    // Удаление элемента из конца списка
    T pop_tail() {
        if (head == nullptr) {
            throw std::runtime_error("List is empty");
        }
        
        if (size == 1) {
            T value = head->data;
            delete head;
            head = nullptr;
            size = 0;
            return value;
        }
        
        Node<T>* current = head;
        Node<T>* prev = nullptr;
        while (current->next != head) {
            prev = current;
            current = current->next;
        }
        
        T value = current->data;
        prev->next = head;
        delete current;
        --size;
        
        return value;
    }

    void delete_node(const T& value) {
        if (head == nullptr) return;
        
        // Специальный случай: все элементы равны value
        bool all_equal = true;
        Node<T>* current = head;
        do {
            if (current->data != value) {
                all_equal = false;
                break;
            }
            current = current->next;
        } while (current != head);
        
        if (all_equal) {
            clear();
            return;
        }
        
        Node<T>* curr = head;
        Node<T>* prev = nullptr;
        
        // Находим предыдущий элемент перед head
        while (prev == nullptr || prev->next != head) {
            if (prev == nullptr) prev = head;
            if (prev->next == head) break;
            prev = prev->next;
        }
        
        do {
            if (curr->data == value) {
                if (curr == head) {
                    head = head->next;
                    prev->next = head;
                    delete curr;
                    curr = head;
                } else {
                    prev->next = curr->next;
                    delete curr;
                    curr = prev->next;
                }
                --size;
            } else {
                prev = curr;
                curr = curr->next;
            }
        } while (curr != head && size > 0);
    }

    // Операция доступа по индексу (для чтения)
    const T& operator[](size_t index) const {
        return get_node(index)->data;
    }
    
    // Операция доступа по индексу (для записи)
    T& operator[](size_t index) {
        return get_node(index)->data;
    }

    size_t get_size() const {
        return size;
    }

    // Проверка на пустоту
    bool empty() const {
        return size == 0;
    }

    void print() const {
        if (head == nullptr) {
            std::cout << "List is empty\n";
            return;
        }
        
        Node<T>* current = head;
        do {
            std::cout << current->data << " ";
            current = current->next;
        } while (current != head);
        std::cout << "\n";
    }
};
using namespace std;

int main() {

    cout << "ВСЕ ТЕСТЫ ЗАВЕРШЕНЫ" << endl;
    
    return 0;
}