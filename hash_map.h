#pragma once

// Copyright 2020 Ilya Chukanov

#include <algorithm>
#include <cmath>
#include <vector>
#include <list>
#include <stdexcept>

template <class KeyType, class ValueType, class Hash = std::hash<KeyType>>
class HashMap {
 public:
    using KeyValuePair = std::pair<const KeyType, ValueType>;
    using const_iterator = typename std::list<KeyValuePair>::const_iterator;
    using iterator = typename std::list<KeyValuePair>::iterator;

    HashMap(Hash hasher_obj = Hash());
    
    HashMap(std::initializer_list<std::pair<KeyType, ValueType>> list,
            Hash hasher_obj = Hash());
            
    template <typename IteratorType>
    HashMap(IteratorType begin, IteratorType end, Hash hasher_obj = Hash());

    Hash hash_function() const;

    bool empty() const;

    void insert(std::pair<KeyType, ValueType> key_value_pair);

    iterator begin();

    const_iterator end() const;

    iterator end();

    HashMap& operator=(HashMap const& other);

    iterator find(const KeyType key);

    void erase(const KeyType key);

    const_iterator begin() const;

    size_t size() const;

    ValueType& operator[](const KeyType key);

    const ValueType& at(const KeyType key) const;

    const_iterator find(const KeyType key) const;

    void clear();

    void DoubleSize();

 private:
    const size_t initialTableSize_ = 20;

    size_t hash_table_size_quantity_elements_;

    size_t num_hash_table_elements_ = 0;

    std::vector<std::list<iterator>> hash_table_;

    std::list<KeyValuePair> all_elements_;

    Hash function_hash_number_;
    
};


template <class KeyType, class ValueType, class Hash>
void HashMap<KeyType, ValueType, Hash>::insert(
    std::pair<KeyType, ValueType> key_value_pair) {
    size_t hash_table_cell_id = function_hash_number_(key_value_pair.first) %
                                hash_table_size_quantity_elements_;
    for (auto& element : hash_table_[hash_table_cell_id]) {
        if (element->first == key_value_pair.first) {
            return;
        }
    }
    all_elements_.push_back(key_value_pair);
    hash_table_[hash_table_cell_id].push_back(std::prev(all_elements_.end()));
    ++num_hash_table_elements_;

    DoubleSize();
}

template <class KeyType, class ValueType, class Hash>
void HashMap<KeyType, ValueType, Hash>::DoubleSize() {
    if (hash_table_size_quantity_elements_ > num_hash_table_elements_ * 2) {
        return;
    }
    std::list<std::pair<KeyType, ValueType>> position_of_scale;
    for (auto& key_value_pair : all_elements_) {
        std::pair<KeyType, ValueType> include_one;
        include_one.first = key_value_pair.first;
        include_one.second = key_value_pair.second;
        position_of_scale.push_back(include_one);
    }
    hash_table_.clear();
    all_elements_.clear();
    hash_table_size_quantity_elements_ = hash_table_size_quantity_elements_ * 2;
    hash_table_.resize(hash_table_size_quantity_elements_);
    num_hash_table_elements_ = 0;
    for (auto& system : position_of_scale) {
        insert(system);
    }
    position_of_scale.clear();
}


template <class KeyType, class ValueType, class Hash>
void HashMap<KeyType, ValueType, Hash>::clear() {
    for (auto& element : all_elements_) {
        size_t hashFunction = function_hash_number_(element.first) %
                              hash_table_size_quantity_elements_;
        hash_table_[hashFunction].clear();
    }
    all_elements_.clear();
    num_hash_table_elements_ = 0;
}

template <class KeyType, class ValueType, class Hash>
auto HashMap<KeyType, ValueType, Hash>::find(KeyType key) const -> const_iterator {
    size_t hashFunc = function_hash_number_(key) %
                      hash_table_size_quantity_elements_;
    for (auto& It : hash_table_[hashFunc]) {
        if (key == It->first) {
            return It;
        }
    }
    return all_elements_.end();
}

template <class KeyType, class ValueType, class Hash>
const ValueType& HashMap<KeyType, ValueType, Hash>::at(
    KeyType key) const {
    auto iterator_for_find_key = find(key);
    if (all_elements_.end() == iterator_for_find_key) {
        throw std::out_of_range("");
    }
    return iterator_for_find_key->second;
}

template <class KeyType, class ValueType, class Hash>
ValueType& HashMap<KeyType, ValueType, Hash>::operator[](
    KeyType key) {
    auto iterator_for_find_key = find(key);
    if (iterator(all_elements_.end()) == iterator_for_find_key) {
        insert({key, ValueType()});
        iterator_for_find_key = find(key);
    }
    return iterator_for_find_key->second;
}

template <class KeyType, class ValueType, class Hash>
void HashMap<KeyType, ValueType, Hash>::erase(const KeyType key) {
    size_t hash_table_cell =
        function_hash_number_(key) % hash_table_size_quantity_elements_;
    for (auto It = hash_table_[hash_table_cell].begin();
         It != hash_table_[hash_table_cell].end(); ++It) {
        if (key == (*It)->first) {
            all_elements_.erase(*It);
            hash_table_[hash_table_cell].erase(It);
            num_hash_table_elements_--;
            return;
        }
    }
}

template <class KeyType, class ValueType, class Hash>
auto HashMap<KeyType, ValueType, Hash>::find(const KeyType key) -> iterator {
    size_t hashFunc = function_hash_number_(key) %
                      hash_table_size_quantity_elements_;
    for (auto& It : hash_table_[hashFunc]) {
        if (key == It->first) {
            return iterator(It);
        }
    }
    return iterator(all_elements_.end());
}

template <class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>& HashMap<KeyType, ValueType, Hash>::operator=(
    HashMap<KeyType, ValueType, Hash> const& other) {
    auto It = this;
    if (&other == It) {
        return *It;
    }
    for (auto& element : all_elements_) {
        size_t hashFunction = function_hash_number_(element.first) %
                              hash_table_size_quantity_elements_;
        hash_table_[hashFunction].clear();
    }
    all_elements_.clear();
    num_hash_table_elements_ = 0;
    function_hash_number_ = other.hash_function();
    hash_table_.resize(other.hash_table_size_quantity_elements_);
    for (auto& element_in_other : other) {
        insert(element_in_other);
    }
    return *It;
}

template <class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>::HashMap(Hash hasher_obj)
    : function_hash_number_(hasher_obj) {
    hash_table_size_quantity_elements_ = initialTableSize_;
    hash_table_.resize(hash_table_size_quantity_elements_);
}

template <class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>::HashMap(
    std::initializer_list<std::pair<KeyType, ValueType>> list, Hash hasher_obj)
    : function_hash_number_(hasher_obj) {
    hash_table_size_quantity_elements_ = initialTableSize_;
    hash_table_.resize(hash_table_size_quantity_elements_);
    for (auto& pos_in_list : list) {
        insert(pos_in_list);
    }
}

template <class KeyType, class ValueType, class Hash>
template <typename IteratorType>
HashMap<KeyType, ValueType, Hash>::HashMap(IteratorType begin, IteratorType end,
                                           Hash hasher_obj)
    : function_hash_number_(hasher_obj) {
    hash_table_size_quantity_elements_ = initialTableSize_;
    hash_table_.resize(hash_table_size_quantity_elements_);
    for (; begin != end; ++begin) {
        insert(*begin);
    }
}

template <class KeyType, class ValueType, class Hash>
Hash HashMap<KeyType, ValueType, Hash>::hash_function() const {
    return function_hash_number_;
}

template <class KeyType, class ValueType, class Hash>
bool HashMap<KeyType, ValueType, Hash>::empty() const {
    return !num_hash_table_elements_;
}

template <class KeyType, class ValueType, class Hash>
auto HashMap<KeyType, ValueType, Hash>::begin() -> iterator {
    return iterator(all_elements_.begin());
}

template <class KeyType, class ValueType, class Hash>
auto HashMap<KeyType, ValueType, Hash>::end() const -> const_iterator 
{
    return all_elements_.end();
}

template <class KeyType, class ValueType, class Hash>
auto HashMap<KeyType, ValueType, Hash>::end() -> iterator {
    return iterator(all_elements_.end());
}

template <class KeyType, class ValueType, class Hash>
auto HashMap<KeyType, ValueType, Hash>::begin() const -> const_iterator {
    return all_elements_.begin();
}

template <class KeyType, class ValueType, class Hash>
size_t HashMap<KeyType, ValueType, Hash>::size() const {
    return num_hash_table_elements_;
}
