#pragma once

#include <cstddef> // size_t
#include <iterator> // std::bidirectional_iterator_tag
#include <type_traits> // std::is_same, std::enable_if

template <class T>
class List {
    private:
    struct Node {
        Node *next, *prev;
        T data;
        explicit Node(Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev} {}
        explicit Node(const T& data, Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev}, data{data} {}
        explicit Node(T&& data, Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev}, data{std::move(data)} {}
    };

    template <typename pointer_type, typename reference_type>
    class basic_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = pointer_type;
        using reference         = reference_type;
    private:
        friend class List<value_type>;
        using Node = typename List<value_type>::Node;

        Node* node;

        explicit basic_iterator(Node* ptr) noexcept : node{ptr} {}
        explicit basic_iterator(const Node* ptr) noexcept : node{const_cast<Node*>(ptr)} {}

    public:
        basic_iterator() : node(nullptr) {};
        basic_iterator(const basic_iterator&) = default;
        basic_iterator(basic_iterator&&) = default;
        ~basic_iterator() = default;
        basic_iterator& operator=(const basic_iterator&) = default;
        basic_iterator& operator=(basic_iterator&&) = default;

        reference operator*() const {
            return node->data;
        }
        pointer operator->() const {
            return &(node->data);
        }

        // Prefix Increment: ++a
        basic_iterator& operator++() {
            node = node->next;
            return *this;
        }
        // Postfix Increment: a++
        basic_iterator operator++(int) {
            basic_iterator temp(*this);
            node = node->next;
            return temp;
        }
        // Prefix Decrement: --a
        basic_iterator& operator--() {
            node = node->prev;
            return *this;
        }
        // Postfix Decrement: a--
        basic_iterator operator--(int) {
            basic_iterator temp(*this);
            node = node->prev;
            return temp;
        }

        bool operator==(const basic_iterator& other) const noexcept {
            return node == other.node;
        }
        bool operator!=(const basic_iterator& other) const noexcept {
            return node != other.node;
        }
    };

public:
    using value_type      = T;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using iterator        = basic_iterator<pointer, reference>;
    using const_iterator  = basic_iterator<const_pointer, const_reference>;

private:
    Node head, tail;
    size_type _size;

public:

    List() : head(), tail(), _size(0) {

        head.next = &tail;
        tail.prev = &head;

    }

    List(size_type count, const T& value ) : head(nullptr), tail(nullptr), _size(0) {

        head.next=&tail;
        tail.prev=&head;

        Node *curr = &head;

        for (size_t i = 0; i < count; i++)
        {
            Node *t= new Node(T(value));
            //set the current node's next to the new node
            curr->next = t;
            //will link the new node to the previous one
            t->prev = curr;
            //set the new current node
            curr = t;
        }
        //reset the sentinal list
        _size = count;
        curr->next = &tail;
        tail.prev = curr;
        

    }
    explicit List(size_type count ) {

        head.next=&tail;
        tail.prev=&head;

        Node *curr = &head;
        //loops through and adds each node use template T
        for (size_t i = 0; i < count; i++)
        {
            Node *t= new Node(T());

            t->prev = curr;
            curr->next = t;
            curr = t;
        }

        curr->next = &tail;
        tail.prev = curr;
        _size = count;

    }
    
    List( const List& other ) {

    head.next = &tail;
    tail.prev = &head;

    Node* curr = &head;
    Node* other_curr = other.head.next;
    //loop through and set each node accordingly
    for (size_t i = 0; i < other._size; i++) {
        Node* t = new Node(other_curr->data);

        t->prev = curr;
        curr->next = t;
        curr = t;
        other_curr = other_curr->next;
    }

    //sets the size 
    _size = other._size;
    curr->next = &tail;
    tail.prev = curr;


    }

    List( List&& other ) {
        //checks if empty, if it is then will return empty, else will steal the contents of other
        if (other.empty()) {
            head.next = &tail;
            tail.prev = &head;
            _size = 0;
        }
        else {
            _size = other._size;
            head.next = other.head.next;
            tail.prev = other.tail.prev;
            other.tail.prev->next = &tail;
            other.head.next->prev = &head;

            other.head.next = &other.tail;
            other.tail.prev = &other.head;
            other._size = 0;
        }
        
    }

    ~List() {

        clear();

    }


    List& operator=( const List& other ) {

        if (this == &other) //checks if it is self-assignment
        {
            return *this;
        }

        clear();

        //set the sentinal nodes
        Node *curr = &head;
        Node *curr_other = other.head.next;

        //loop through and set each node accordingly
        for (size_t i = 0; i < other._size; i++)
        {
            Node *t = new Node(curr_other->data);
            curr->next = t;
            t->prev = curr;
            curr = t;
            curr_other = curr_other->next;
        }

        //sets the nodes and size
        _size = other._size; 
        curr->next = &tail;
        tail.prev = curr;
        
        return *this;
    
    }
    
    List& operator=( List&& other ) noexcept {

        if (this == &other) {
            return *this;
        }


        //clear the current list
        clear();

        //set the first nodes
        head.next = other.head.next;
        tail.prev = other.tail.prev;

        //replace pointers
        other.head.next->prev = &head;
        other.tail.prev->next = &tail;
        
        //set other to empty state
        other.head.next = &other.tail;
        other.tail.prev = &other.head;
        _size = other._size;
        other._size = 0;

        return *this;



    }

    reference front() {
        return head.next->data;
    }
    const_reference front() const {
        return head.next->data;
    }
	
    reference back() {
        return tail.prev->data;
    }
    const_reference back() const {
        return tail.prev->data;
    }
	
    iterator begin() noexcept {
        if(empty())
            return iterator(&tail);
        return iterator(head.next);
    }
    const_iterator begin() const noexcept {
        if(empty())
            return const_iterator(&tail);
        return const_iterator(head.next);
    }
    const_iterator cbegin() const noexcept {
        if(empty())
            return const_iterator(&tail);
        return const_iterator(head.next);
    }

    iterator end() noexcept {
        return iterator(&tail);
    }
    const_iterator end() const noexcept {
        return const_iterator(&tail);
    }
    const_iterator cend() const noexcept {
        return const_iterator(&tail);
    }

    bool empty() const noexcept {
        return _size == 0;
    }

    size_type size() const noexcept {
        return _size;
    }

    void clear() noexcept {
        //same as destructor
        Node* prev = head.next;
        Node* curr = head.next;

        while (curr != &tail) {
            curr = curr->next;
            delete prev;
            prev = curr;
        }

        head.next = &tail;
        tail.prev = &head;
        prev = nullptr;
        curr = nullptr;
        _size = 0;

    }

    //point to node you just inserted
    iterator insert( const_iterator pos, const T& value ) {

        //new node created
        Node* newNode = new Node(value, pos.node->prev, pos.node);

        //current node
        Node* curr = pos.node;

        //sets the new nodes previous' next to the inserted one
        newNode->prev->next = newNode;
        //sets the current nodes previous to the new node
        curr->prev = newNode;

        _size++;
        //iterator to new node
        return iterator(newNode);


    }

    iterator insert( const_iterator pos, T&& value ) {
        //create a new node that is linked in the list
        Node* newNode = new Node(std::move(value), pos.node->prev, pos.node);
        

        //current node
        Node* curr = pos.node;

        //sets the current nodes previous to the new node
        curr->prev = newNode;
        //sets the new nodes previous' next to the inserted one
        newNode->prev->next = newNode;
        
        _size++;
        //iterator to new node
        return iterator(newNode);

    }

    //point to node after the one you just deleted
    iterator erase( const_iterator pos ) {

        Node* n = pos.node; //pointer to the node to be deleted

        //gets the previous and node after
        Node* previous = pos.node->prev;
        Node* after = pos.node->next;

        //sets previous next node to the after node and the after previous node to the previous node
        previous->next = after;
        after->prev = previous;

        //break the links and delete node
        n->next = nullptr;
        n->prev = nullptr;
        delete n;

        _size--;

        return iterator(after);

    }

    void push_back( const T& value ) {
        insert(const_iterator(&tail), value);
    }
    void push_back( T&& value ) {
        insert(const_iterator(&tail), std::move(value));
    }

    void pop_back() {
        erase(iterator(tail.prev));
    }
	
    void push_front( const T& value ) {
        insert(const_iterator(head.next), value);
    }
	void push_front( T&& value ) {
        insert(const_iterator(head.next), std::move(value));
    }

    void pop_front() {
        erase(iterator(head.next));
    }

    /*
    You do not need to modify these methods!
    
    These method provide the non-const complement 
    for the const_iterator methods provided above.
    */

    iterator insert( iterator pos, const T & value) { 
        return insert((const_iterator &) (pos), value);
    }

    iterator insert( iterator pos, T && value ) {
        return insert((const_iterator &) (pos), std::move(value));
    }

    iterator erase( iterator pos ) {
        return erase((const_iterator&)(pos));
    }
};


/*
    You do not need to modify these methods!

    These method provide a overload to compare const and 
    non-const iterators safely.
*/

namespace {
    template<typename Iter, typename ConstIter, typename T>
    using enable_for_list_iters = typename std::enable_if<
        std::is_same<
            typename List<typename std::iterator_traits<Iter>::value_type>::iterator, 
            Iter
        >{} && std::is_same<
            typename List<typename std::iterator_traits<Iter>::value_type>::const_iterator,
            ConstIter
        >{}, T>::type;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator==(const Iterator & lhs, const ConstIter & rhs) {
    return (const ConstIter &)(lhs) == rhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator==(const ConstIter & lhs, const Iterator & rhs) {
    return (const ConstIter &)(rhs) == lhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator!=(const Iterator & lhs, const ConstIter & rhs) {
    return (const ConstIter &)(lhs) != rhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator!=(const ConstIter & lhs, const Iterator & rhs) {
    return (const ConstIter &)(rhs) != lhs;
}