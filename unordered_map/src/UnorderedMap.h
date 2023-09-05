#include <cstddef>    // size_t
#include <functional> // std::hash
#include <ios>
#include <utility>    // std::pair
#include <iostream>

#include "primes.h"



template <typename Key, typename T, typename Hash = std::hash<Key>, typename Pred = std::equal_to<Key>>
class UnorderedMap {
    public:

    using key_type = Key;
    using mapped_type = T;
    using const_mapped_type = const T;
    using hasher = Hash;
    using key_equal = Pred;
    using value_type = std::pair<const key_type, mapped_type>;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    private:

    struct HashNode {
        HashNode *next;
        value_type val;

        HashNode(HashNode *next = nullptr) : next{next} {}
        HashNode(const value_type & val, HashNode * next = nullptr) : next { next }, val { val } { }
        HashNode(value_type && val, HashNode * next = nullptr) : next { next }, val { std::move(val) } { }
    };

    size_type _bucket_count;
    HashNode **_buckets;

    HashNode * _head;
    size_type _size;

    Hash _hash;
    key_equal _equal;

    static size_type _range_hash(size_type hash_code, size_type bucket_count) {
        return hash_code % bucket_count;
    }

    public:

    template <typename pointer_type, typename reference_type, typename _value_type>
    class basic_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = _value_type;
        using difference_type = ptrdiff_t;
        using pointer = value_type *;
        using reference = value_type &;

    private:
        friend class UnorderedMap<Key, T, Hash, key_equal>;
        using HashNode = typename UnorderedMap<Key, T, Hash, key_equal>::HashNode;

        const UnorderedMap * _map;
        HashNode * _ptr;

        explicit basic_iterator(UnorderedMap const * map, HashNode *ptr) noexcept : _ptr(ptr), _map(map) {}

    public:
        basic_iterator() : _ptr(nullptr), _map(nullptr) {};

        basic_iterator(const basic_iterator &) = default;
        basic_iterator(basic_iterator &&) = default;
        ~basic_iterator() = default;
        basic_iterator &operator=(const basic_iterator &) = default;
        basic_iterator &operator=(basic_iterator &&) = default;

        reference operator*() const {
            return _ptr->val;
        }

        pointer operator->() const {
            return &(_ptr->val);
        }

        basic_iterator &operator++() {
            //checks if there is a next pointer
            if (_ptr->next) {
                //will move it forward
                _ptr = _ptr->next;
                return *this;
            }
            //if not it will find the next bucket with a value
            size_t b = _map->_bucket(_ptr->val);
            //std::cout << bucket << std::endl;
            //will iterate while less than the bucket count
            while (_map->_bucket_count > ++b) {
                //checks if there is a valid pointer, if there is then it will return this new value
                _ptr = _map->_buckets[b];
                if (_ptr != nullptr) {
                    return *this;
                }
            }
            //if not then it will be a nullptr
            _ptr = nullptr;
            //return the new iterator
            return *this;
        }

        basic_iterator operator++(int) {
//same as above except it will return

            //set a temp to store the value
            basic_iterator temp = basic_iterator(*this);
            //checks if there is a next pointer
            if (_ptr->next) {
                //will move it forward
                _ptr = _ptr->next;
                return temp;
            }
            //if not it will find the next bucket with a value
            size_t b = _map->_bucket(_ptr->val);
            //std::cout << bucket << std::endl;
            //will iterate while less than the bucket count
            while (_map->_bucket_count > ++b) {
                //checks if there is a valid pointer, if there is then it will return this new value
                _ptr = _map->_buckets[b];
                if (_ptr != nullptr) {
                    return temp;
                }
            }
            //if not then it will be a nullptr
            _ptr = nullptr;
            //return the previous temp iterator
            return temp;
        }
        
        bool operator==(const basic_iterator &other) const noexcept { return _ptr == other._ptr; }
        bool operator!=(const basic_iterator &other) const noexcept { return _ptr != other._ptr; }
    };

    using iterator = basic_iterator<pointer, reference, value_type>;
    using const_iterator = basic_iterator<const_pointer, const_reference, const value_type>;

    class local_iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = std::pair<const key_type, mapped_type>;
            using difference_type = ptrdiff_t;
            using pointer = value_type *;
            using reference = value_type &;

        private:
            friend class UnorderedMap<Key, T, Hash, key_equal>;
            using HashNode = typename UnorderedMap<Key, T, Hash, key_equal>::HashNode;

            HashNode * _node;

            explicit local_iterator( HashNode * node ) noexcept : _node(node) {}

        public:
            local_iterator() : _node(nullptr) {}

            local_iterator(const local_iterator &) = default;
            local_iterator(local_iterator &&) = default;
            ~local_iterator() = default;
            local_iterator &operator=(const local_iterator &) = default;
            local_iterator &operator=(local_iterator &&) = default;
            reference operator*() const {
                return _node->val;
            }
            pointer operator->() const {
                return &(_node->val);
            }
            local_iterator & operator++() {
                //if there exists a next node
                if (_node->next) {
                    _node = _node->next;
                }
                //else it will be a nullptr
                else {
                    _node = nullptr;
                }
                //return the new moved forward iterator
                return *this;
                
            }
            local_iterator operator++(int) {
                //store the original
                local_iterator temp = *this;
                //check for valid next node
                if (_node->next) {
                    _node = _node->next;
                }
                //if not then set to nullptr
                else {
                    _node = nullptr;
                }
                //return the iterator before hand
                return temp;
            }

            bool operator==(const local_iterator &other) const noexcept { return _node == other._node; }
            bool operator!=(const local_iterator &other) const noexcept { return _node != other._node; }
    };

private:

    size_type _bucket(size_t code) const { return _range_hash(code, _bucket_count); }
    size_type _bucket(const Key & key) const { return _bucket(_hash(key)); }
    size_type _bucket(const value_type & val) const {
        //std::cout << val.first << std::endl;
        return _bucket(val.first); 
    }

    HashNode*& _find(size_type code, size_type bucket, const Key & key) {
        //sets the current node pointer
        HashNode** curr = &_buckets[bucket];
        //goes until nullptr
        while (*curr != nullptr) {
            //if the hashcode is the same as the code and the key it will return
            if (_equal((*curr)->val.first, key)) {
                if (_hash((*curr)->val.first) == code) {
                    return *curr;
                }
            }
                
            //go to the next one
            curr = &(*curr)->next;
        }
        //if not found it will get a nullptr
        return *curr;
    }

    //calls the find function
    HashNode*& _find(const Key & key) { return _find(_hash(key), _bucket(key), key); }

    HashNode * _insert_into_bucket(size_type bucket, value_type && value) {
        //will insert item into the bucket
        
        HashNode* n = new HashNode(std::move(value), _buckets[bucket]);

        //checks if it is valid
        //if there is not a head or the bucket is less than or equal to the head val in bucket set the head to the new node
        if (!_head) {
            _head = n;
        }

        //if the bucket is out of range
        if (bucket <= _bucket(_head->val)) {
            _head = n;
        }
        //return the new head node and increment size
        _size++;
        _buckets[bucket] = n;


        return n;

    }

    void _move_content(UnorderedMap & src, UnorderedMap & dst) {
        src = dst;
    }

public:
    explicit UnorderedMap(size_type bucket_count, const Hash & hash = Hash { }, const key_equal & equal = key_equal { }) {
        //set the default values
        _head = nullptr;
        _equal = equal;
        _size = 0;
        _bucket_count = next_greater_prime(bucket_count);
        //sets the new list with hashnodes
        _buckets = new HashNode*[_bucket_count]();
        _hash = hash;
    }

    ~UnorderedMap() { 
        clear(); 
        delete[] _buckets;
    }

    UnorderedMap(const UnorderedMap & other) {
        //reset the current bucket to the new values
        _bucket_count = other._bucket_count;
        _size = 0;
        _equal = other._equal;
        _hash = other._hash;
        _head = nullptr;
        //new buckets list with hashnodes
        _buckets = new HashNode*[_bucket_count]();

        //insert the items
        for (const_iterator it = other.cbegin(); it != other.cend(); it++) {
            insert(it._ptr->val);
        }
    }

    UnorderedMap(UnorderedMap && other) {
        //set the new attributes (steal it)
        _bucket_count = other._bucket_count;
        _equal = other._equal;
        _buckets = other._buckets;
        _hash = other._hash;
        _head = other._head;
        _size = other._size;

        //zero out the attributes
        other._equal = key_equal{};
        other._size = 0;
        other._hash = Hash{};
        other._buckets = new HashNode*[other._bucket_count]();
        other._head = nullptr;
        

    }

    UnorderedMap & operator=(const UnorderedMap & other) {

        //self assignment
        if (this == &other) {
            return *this;
        }

        //clear the unordered map
        clear();
        delete[] _buckets;

        //set new values and insert the other values
        _bucket_count = other._bucket_count;
        _hash = other._hash;
        _head = nullptr;
        _equal = other._equal;
        _buckets = new HashNode*[_bucket_count]();
        _size = 0;

        //loop through and insert each element from other
        for (const_iterator it = other.cbegin(); it != other.cend(); it++) {
            insert(it._ptr->val);
        }
        return *this;
    }

    UnorderedMap & operator=(UnorderedMap && other) {
        //self assignment
        if (this == &other) {
            return *this;
        }

        //clear the unordered map
        clear();
        delete[] _buckets;

        //set the new attributes
        _bucket_count = other._bucket_count;
        _equal = other._equal;
        _buckets = other._buckets;
        _hash = other._hash;
        _head = other._head;
        _size = other._size;

        //zero out the attributes
        other._equal = key_equal{};
        other._size = 0;
        other._hash = Hash{};
        other._buckets = new HashNode*[other._bucket_count]();
        other._head = nullptr;

        return *this;
    }

    void clear() noexcept {

        //loop through to delete every value
        while (_size > 0) {
            //std::cout << begin()._ptr->val.first << std::endl;
            erase(begin());
        }
    
    }

    size_type size() const noexcept { return _size; }

    bool empty() const noexcept { return _size == 0; }

    size_type bucket_count() const noexcept { return _bucket_count; }

    iterator begin() { return iterator(this, _head); }
    iterator end() { return iterator(this, nullptr); }

    const_iterator cbegin() const { return const_iterator(this, _head); };
    const_iterator cend() const { return const_iterator(this, nullptr); };

    local_iterator begin(size_type n) { return local_iterator(_buckets[n]); }
    local_iterator end(size_type n) { return local_iterator(nullptr); }

    size_type bucket_size(size_type n) {
        size_t count = 0;
        //first element in the bucket
        local_iterator it = begin(n);
        //will go through and count each element in the bucket
        while (it != end(n)) {
            it++;
            count++;
        }
        return count;
    }

    float load_factor() const {
        //get the size and divide by bucket count
        return float(size()) / float(bucket_count());
    }

    size_type bucket(const Key & key) const { return _bucket(key); }

    std::pair<iterator, bool> insert(value_type && value) {

        //create a new head node using std move
        HashNode* head = _insert_into_bucket(_bucket(std::move(value)), std::move(value));
        //if nullptr will make it false
        if (head == nullptr){
            
            return std::make_pair(iterator(this, head), false);
        }
        //if there is already a node there it will be true since it is inserted
        else {
            return std::make_pair(iterator(this, head), true);
        }
    }

    std::pair<iterator, bool> insert(const value_type & value) {
        //create instance of the value
        value_type temp = value;
        //create a new head node without using std move
        HashNode* head = _insert_into_bucket(_bucket(temp), std::move(temp));
        if (head == nullptr){
            return std::make_pair(iterator(this, head), false);
        }
        //if nullptr will make it false
        else {
            return std::make_pair(iterator(this, head), true);
            
        }
        
    }

    iterator find(const Key & key) { return iterator(this, _find(key)); }

    T& operator[](const Key & key) {
        //gets current node, if there is a node it will return the value, if not it will insert a node and return the reference
        HashNode* curr = _find(key);
        //std::cout << curr->val.first << std::endl;
        //return value
        if (curr == nullptr) {
            //will insert the new pair and return the reference to the element 
            return (insert(std::make_pair(key, T()))).first->second;
        }
        else {
            //returns the value of the element
            return curr->val.second;
        }
    }

    iterator erase(iterator pos) {

        //if it is already at the end just return pos
        if (pos == end()) {
            return pos;
        }
        //finds the node
        HashNode*& temp = _find(pos._ptr->val.first);
        //std::cout << temp->val.first << std::endl;

        //if nullptr
        if (temp == nullptr) {
            return pos;
        }
        
        //if its the first item must iterate forward
        if(_head == temp){
            //find the new head of the list so you can delete
            iterator it = iterator(this, temp);
            it++;
            _head = it._ptr;
        }

        //the node to delete
        HashNode* dnode = temp;
        //move iterator forward and the node as well
        temp = temp->next;
        _size--;
        iterator it = ++iterator(this, dnode);
        delete dnode;
        return it;
    }

    size_type erase(const Key & key) {
        //find the node
        HashNode*& temp = _find(key);
        //std::cout << node->val.first << std::endl;
        if (temp == nullptr) {
            return 0;
        }

        //will go until nullptr
        while (temp != nullptr) {
            //if the temp value is the same as the key, then it will erase it using the function above
            if (_equal(temp->val.first, key)) {
                erase(find(key));
                return 1;
            }
        }
        
    }

    template<typename KK, typename VV>
    friend void print_map(const UnorderedMap<KK, VV> & map, std::ostream & os);
};

template<typename K, typename V>
void print_map(const UnorderedMap<K, V> & map, std::ostream & os = std::cout) {
    using size_type = typename UnorderedMap<K, V>::size_type;
    using HashNode = typename UnorderedMap<K, V>::HashNode;

    for(size_type bucket = 0; bucket < map.bucket_count(); bucket++) {
        os << bucket << ": ";

        HashNode const * node = map._buckets[bucket];

        while(node) {
            os << "(" << node->val.first << ", " << node->val.second << ") ";
            node = node->next;
        }

        os << std::endl;
    }
}
