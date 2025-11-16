#include <iostream>
#include <stdexcept>
#include <random>
#include <ctime>

template <typename T>
struct Node
{
    Node *next;
    T data;
    Node(const T &value) : data(value), next(nullptr) {}
};

template <typename T>
class LinkedList
{
private:
    Node<T> *head;
    size_t size;

    Node<T> *get_node(size_t index) const
    {
        if (index >= size)
        {
            throw std::out_of_range("Index out of range");
        }
        Node<T> *current = head;
        for (size_t i = 0; i < index; ++i)
        {
            current = current->next;
        }
        return current;
    }

    void deep_copy(const LinkedList &other)
    {
        if (other.head == nullptr)
        {
            head = nullptr;
            size = 0;
            return;
        }

        head = new Node<T>(other.head->data);
        Node<T> *current = head;
        Node<T> *other_current = other.head->next;

        while (other_current != other.head)
        {
            current->next = new Node<T>(other_current->data);
            current = current->next;
            other_current = other_current->next;
        }

        // Замыкаем цикл
        current->next = head;
        size = other.size;
    }

    void clear()
    {
        if (head == nullptr)
            return;

        Node<T> *current = head->next;
        while (current != head)
        {
            Node<T> *temp = current;
            current = current->next;
            delete temp;
        }
        delete head;
        head = nullptr;
        size = 0;
    }

public:
    LinkedList() : head(nullptr), size(0) {}

    LinkedList(const LinkedList &other) : head(nullptr), size(0)
    {
        deep_copy(other);
    }

    explicit LinkedList(size_t count, const T &min_val = T(), const T &max_val = T()) : head(nullptr), size(0)
    {
        std::mt19937 gen(static_cast<unsigned>(std::time(nullptr))); // Генератор, Приводит к int, Время

        if constexpr (std::is_floating_point_v<T>)
        {
            std::uniform_real_distribution<T> dist(min_val, max_val);
            for (size_t i = 0; i < count; ++i)
            {
                push_tail(dist(gen));
            }
        }
        else
        {
            std::uniform_int_distribution<int> dist(static_cast<int>(min_val), static_cast<int>(max_val));
            for (size_t i = 0; i < count; ++i)
            {
                push_tail(static_cast<T>(dist(gen)));
            }
        }
    }

    ~LinkedList()
    {
        clear();
    }

    LinkedList &operator=(const LinkedList &other)
    {
        if (this != &other)
        {
            clear();
            deep_copy(other);
        }
        return *this;
    }

    // Добавление элемента в конец списка
    void push_tail(const T &value)
    {
        Node<T> *new_node = new Node<T>(value);
        if (head == nullptr)
        {
            head = new_node;
            head->next = head;
            size = 1;
            return;
        }

        // Находим хвост
        Node<T> *tail = head;
        while (tail->next != head)
        {
            tail = tail->next;
        }

        tail->next = new_node;
        new_node->next = head;
        ++size;
    }
    // push_tail для списка в конец
    void push_tail(const LinkedList &other)
    {
        if (other.size == 0)
            return;
        Node<T> *other_current = other.head;
        do
        {
            push_tail(other_current->data);
            other_current = other_current->next;
        } while (other_current != other.head);
    }

    void push_head(const T &value)
    {
        Node<T> *new_node = new Node<T>(value);

        if (head == nullptr)
        {
            head = new_node;
            head->next = head;
        }
        else
        {
            Node<T> *tail = head;
            while (tail->next != head)
            {
                tail = tail->next;
            }
            new_node->next = head;
            tail->next = new_node;
            head = new_node;
        }
        ++size;
    }

    // Добавление списка в начало
    void push_in_head(const LinkedList &other)
    {
        if (other.size == 0)
            return;

        LinkedList temp = other;
        if (size == 0)
        {
            head = temp.head;
            size = temp.size;
            temp.head = nullptr;
            temp.size = 0;
            return;
        }

        // Находим хвост другого списка
        Node<T> *other_tail = temp.head;
        while (other_tail->next != temp.head)
        {
            other_tail = other_tail->next;
        }

        // Находим хвост текущего списка
        Node<T> *current_tail = head;
        while (current_tail->next != head)
        {
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

    T pop_head()
    {
        if (head == nullptr)
        {
            throw std::runtime_error("List is empty");
        }

        T value = head->data;

        if (size == 1)
        {
            delete head;
            head = nullptr;
            size = 0;
            return value;
        }

        Node<T> *tail = head;
        while (tail->next != head)
        {
            tail = tail->next;
        }

        Node<T> *temp = head;
        head = head->next;
        tail->next = head;
        delete temp;
        --size;

        return value;
    }

    // Удаление элемента из конца списка
    T pop_tail()
    {
        if (head == nullptr)
        {
            throw std::runtime_error("List is empty");
        }

        if (size == 1)
        {
            T value = head->data;
            delete head;
            head = nullptr;
            size = 0;
            return value;
        }

        Node<T> *current = head;
        Node<T> *prev = nullptr;
        while (current->next != head)
        {
            prev = current;
            current = current->next;
        }

        T value = current->data;
        prev->next = head;
        delete current;
        --size;

        return value;
    }

    void delete_node(const T &value)
    {
        if (head == nullptr)
            return;

        // Проверяем, все ли элементы равны value
        bool all_equal = true;
        Node<T> *check = head;
        do
        {
            if (check->data != value)
            {
                all_equal = false;
                break;
            }
            check = check->next;
        } while (check != head);

        if (all_equal)
        {
            clear();
            return;
        }

        Node<T> *curr = head;
        Node<T> *prev = nullptr;

        // Находим узел перед head
        Node<T> *tail = head;
        while (tail->next != head)
        {
            tail = tail->next;
        }
        prev = tail;

        do
        {
            if (curr->data == value)
            {
                if (curr == head)
                {
                    head = head->next;
                    prev->next = head;
                    delete curr;
                    curr = head;
                }
                else
                {
                    prev->next = curr->next;
                    delete curr;
                    curr = prev->next;
                }
                --size;
            }
            else
            {
                prev = curr;
                curr = curr->next;
            }
        } while (curr != head && size > 0);
    }

    // Операция доступа по индексу константный
    const T &operator[](size_t index) const
    {
        return get_node(index)->data;
    }

    // Операция доступа по индексу
    T &operator[](size_t index)
    {
        return get_node(index)->data;
    }

    size_t get_size() const
    {
        return size;
    }

    // Проверка на пустоту
    bool empty() const
    {
        return size == 0;
    }

    void print() const
    {
        if (head == nullptr || size == 0)
        {
            std::cout << "List is empty\n";
            return;
        }

        Node<T> *current = head;
        size_t count = 0;
        do
        {
            std::cout << current->data << " ";
            current = current->next;
            ++count;

            // Защита от бесконечного цикла (на случай повреждения)
            if (count > size)
            {
                std::cout << "\n[ERROR: Cycle broken or corrupted list]\n";
                return;
            }
        } while (current != head);

        std::cout << "\n";
    }
    void reverse() {
    if (size <= 1) return;
    Node<T>* prev = nullptr;
    Node<T>* current = head;
    Node<T>* next = nullptr;
    for (size_t i = 0; i < size; ++i) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    head = prev;           // новый head — бывший хвост
    current->next = head;  // замыкаем: последний -> первый
}
};
using namespace std;

int main()
{
    std::cout << "=== Заданиe 2: Разворот списка ===\n\n";

    // Создаём список: 10->- 20 -> 30 -> 40
    LinkedList<int> list;
    list.push_tail(10);
    list.push_tail(20);
    list.push_tail(30);
    list.push_tail(40);

    std::cout << "Исходный список: ";
    list.print(); // 10 20 30 40
    list.reverse();

    std::cout << "После разворота: ";
    list.print(); // 40 30 20 10

    // Добавим ещё элемент и развернём снова
    list.push_tail(50);
    std::cout << "Добавили 50: ";
    list.print(); // 40 30 20 10 50

    list.reverse();
    std::cout << "Ещё раз развернули: ";
    list.print(); // 50 10 20 30 40

    // 1 элемент
    LinkedList<int> single;
    single.push_tail(999);
    std::cout << "\nСписок из 1 элемента: ";
    single.print(); 

    single.reverse();
    std::cout << "После reverse(): ";
    single.print();

    // Пустой список
    LinkedList<int> empty;
    std::cout << "\nПустой список: ";
    empty.print(); 

    empty.reverse();
    std::cout << "После reverse(): ";
    empty.print(); 

    std::cout << "\nЗадание 2 выполнено\n";
    return 0;
}