#ifndef HASH_OPEN_ADDRESS_H
#define HASH_OPEN_ADDRESS_H

#include <vector>
#include <stdexcept>
#include <functional>
#include <iostream>
#include <utility>

template <class TKey, class TVal>
class HashOpenAddress {
private:
    enum class State { EMPTY, ACTIVE, DELETED };

    using value_type = std::pair<TKey, TVal>;

    struct Node {
        value_type kv;
        State state;

        Node() : kv{}, state(State::EMPTY) {}
        Node(const TKey& k, const TVal& v) : kv(k, v), state(State::ACTIVE) {}
    };

    std::vector<Node> table;
    size_t count;
    size_t deleted_count;
    std::hash<TKey> hasher;

    static constexpr size_t npos = static_cast<size_t>(-1);

    size_t hash(const TKey& key) const {
        return hasher(key) % table.size();
    }

    void insert_no_rehash(const TKey& key, TVal&& val) {
        bool found;
        size_t idx = find_insert_index(key, found);
        if (found) {
            table[idx].kv.second = std::move(val);
            return;
        }

        if (table[idx].state == State::DELETED) --deleted_count;
        table[idx] = Node(key, std::move(val));
        ++count;
    }

    void rehash() {
        std::vector<Node> old_table = std::move(table);
        table.assign(old_table.size() * 2, Node());
        count = 0;
        deleted_count = 0;

        for (auto& node : old_table) {
            if (node.state == State::ACTIVE) {
                insert_no_rehash(node.kv.first, std::move(node.kv.second));
            }
        }
    }

    size_t find_index(const TKey& key) const {
        if (table.empty()) return npos;

        size_t idx = hash(key);
        size_t start_idx = idx;

        while (table[idx].state != State::EMPTY) {
            if (table[idx].state == State::ACTIVE && table[idx].kv.first == key) {
                return idx;
            }
            idx = (idx + 1) % table.size();
            if (idx == start_idx) break;
        }
        return npos;
    }

    size_t find_insert_index(const TKey& key, bool& found) const {
        size_t idx = hash(key);
        size_t start_idx = idx;
        size_t first_deleted = npos;

        while (table[idx].state != State::EMPTY) {
            if (table[idx].state == State::ACTIVE && table[idx].kv.first == key) {
                found = true;
                return idx;
            }
            if (table[idx].state == State::DELETED && first_deleted == npos) {
                first_deleted = idx;
            }
            idx = (idx + 1) % table.size();
            if (idx == start_idx) break;
        }

        found = false;
        return (first_deleted != npos) ? first_deleted : idx;
    }

public:
    explicit HashOpenAddress(size_t initial_capacity = 16) : count(0), deleted_count(0) {
        if (initial_capacity == 0) initial_capacity = 16;
        table.resize(initial_capacity);
    }

    class Iterator {
    private:
        std::vector<Node>* table;
        size_t index;

        void advance_to_valid() {
            while (index < table->size() && (*table)[index].state != State::ACTIVE) {
                ++index;
            }
        }

    public:
        using reference = value_type&;
        using pointer = value_type*;

        Iterator(std::vector<Node>* t, size_t idx) : table(t), index(idx) {
            advance_to_valid();
        }

        reference operator*() const {
            if (index >= table->size()) throw std::out_of_range("Iterator out of bounds");
            return (*table)[index].kv;
        }

        pointer operator->() const {
            if (index >= table->size()) throw std::out_of_range("Iterator out of bounds");
            return &(*table)[index].kv;
        }

        Iterator& operator++() {
            ++index;
            advance_to_valid();
            return *this;
        }

        bool operator!=(const Iterator& other) const { return index != other.index; }
        bool operator==(const Iterator& other) const { return index == other.index; }
    };

    class ConstIterator {
    private:
        const std::vector<Node>* table;
        size_t index;

        void advance_to_valid() {
            while (index < table->size() && (*table)[index].state != State::ACTIVE) {
                ++index;
            }
        }

    public:
        using reference = const value_type&;
        using pointer = const value_type*;

        ConstIterator(const std::vector<Node>* t, size_t idx) : table(t), index(idx) {
            advance_to_valid();
        }

        reference operator*() const {
            if (index >= table->size()) throw std::out_of_range("Iterator out of bounds");
            return (*table)[index].kv;
        }

        pointer operator->() const {
            if (index >= table->size()) throw std::out_of_range("Iterator out of bounds");
            return &(*table)[index].kv;
        }

        ConstIterator& operator++() {
            ++index;
            advance_to_valid();
            return *this;
        }

        bool operator!=(const ConstIterator& other) const { return index != other.index; }
        bool operator==(const ConstIterator& other) const { return index == other.index; }
    };

    Iterator begin() { return Iterator(&table, 0); }
    Iterator end() { return Iterator(&table, table.size()); }

    ConstIterator begin() const { return ConstIterator(&table, 0); }
    ConstIterator end() const { return ConstIterator(&table, table.size()); }

    ConstIterator cbegin() const { return ConstIterator(&table, 0); }
    ConstIterator cend() const { return ConstIterator(&table, table.size()); }

    TVal& operator[](const TKey& key) {
        if (count + deleted_count >= table.size() / 2) {
            rehash();
        }

        bool found;
        size_t idx = find_insert_index(key, found);

        if (found) {
            return table[idx].kv.second;
        }

        if (table[idx].state == State::ACTIVE) throw std::runtime_error("Hash table is full");
        if (table[idx].state == State::DELETED) --deleted_count;

        table[idx] = Node(key, TVal{});
        ++count;

        return table[idx].kv.second;
    }

    const TVal& at(const TKey& key) const {
        size_t idx = find_index(key);
        if (idx == npos) throw std::out_of_range("Key not found");
        return table[idx].kv.second;
    }

    void insert(TKey key, TVal val) {
        if (count + deleted_count >= table.size() / 2) {
            rehash();
        }

        bool found;
        size_t idx = find_insert_index(key, found);

        if (found) {
            table[idx].kv.second = std::move(val);
            return;
        }

        if (table[idx].state == State::ACTIVE) throw std::runtime_error("Hash table is full");
        if (table[idx].state == State::DELETED) --deleted_count;

        table[idx] = Node(key, std::move(val));
        ++count;
    }

    bool erase(const TKey& key) {
        size_t idx = find_index(key);
        if (idx != npos) {
            table[idx].state = State::DELETED;
            --count;
            ++deleted_count;

            if (deleted_count > table.size() / 2) {
                rehash();
            }
            return true;
        }
        return false;
    }

    TVal* find(const TKey& key) {
        size_t idx = find_index(key);
        return idx != npos ? &table[idx].kv.second : nullptr;
    }

    const TVal* find(const TKey& key) const {
        size_t idx = find_index(key);
        return idx != npos ? &table[idx].kv.second : nullptr;
    }

    size_t size() const { return count; }
    bool empty() const { return count == 0; }

    void clear() {
        for (auto& node : table) {
            node.state = State::EMPTY;
        }
        count = 0;
        deleted_count = 0;
    }

    void print() const {
        for (size_t i = 0; i < table.size(); ++i) {
            if (table[i].state == State::ACTIVE) {
                std::cout << "[" << i << "] " << table[i].kv.first << " : " << table[i].kv.second << "\n";
            }
        }
    }
};

#endif // HASH_OPEN_ADDRESS_H

