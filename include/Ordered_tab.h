#ifndef ORDERED_TAB_H
#define ORDERED_TAB_H

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

template <class TKey, class TVal>
class Ordered_tab {
private:
    std::vector<std::pair<TKey, TVal>> data;

    using StorageIterator = typename std::vector<std::pair<TKey, TVal>>::iterator;
    using ConstStorageIterator = typename std::vector<std::pair<TKey, TVal>>::const_iterator;

    StorageIterator lower_bound(const TKey& key) {
        return std::lower_bound(data.begin(), data.end(), key,
            [](const std::pair<TKey, TVal>& item, const TKey& value) {
                return item.first < value;
            });
    }

    ConstStorageIterator lower_bound(const TKey& key) const {
        return std::lower_bound(data.begin(), data.end(), key,
            [](const std::pair<TKey, TVal>& item, const TKey& value) {
                return item.first < value;
            });
    }

public:
    class Iterator {
        StorageIterator iter;
    public:
        explicit Iterator(StorageIterator it) : iter(it) {}
        std::pair<TKey, TVal>& operator*() { return *iter; }
        std::pair<TKey, TVal>* operator->() { return &(*iter); }
        Iterator& operator++() {
            ++iter;
            return *this;
        }
        bool operator!=(const Iterator& other) const { return iter != other.iter; }
        bool operator==(const Iterator& other) const { return iter == other.iter; }
    };

    class ConstIterator {
        ConstStorageIterator iter;
    public:
        explicit ConstIterator(ConstStorageIterator it) : iter(it) {}
        const std::pair<TKey, TVal>& operator*() const { return *iter; }
        const std::pair<TKey, TVal>* operator->() const { return &(*iter); }
        ConstIterator& operator++() {
            ++iter;
            return *this;
        }
        bool operator!=(const ConstIterator& other) const { return iter != other.iter; }
        bool operator==(const ConstIterator& other) const { return iter == other.iter; }
    };

    bool empty() const { return data.empty(); }
    size_t size() const { return data.size(); }
    void clear() { data.clear(); }

    Iterator begin() { return Iterator(data.begin()); }
    Iterator end() { return Iterator(data.end()); }
    ConstIterator begin() const { return ConstIterator(data.begin()); }
    ConstIterator end() const { return ConstIterator(data.end()); }
    ConstIterator cbegin() const { return ConstIterator(data.cbegin()); }
    ConstIterator cend() const { return ConstIterator(data.cend()); }

    Iterator find(const TKey& key) {
        auto it = lower_bound(key);
        if (it != data.end() && !(key < it->first) && !(it->first < key)) {
            return Iterator(it);
        }
        return end();
    }

    ConstIterator find(const TKey& key) const {
        auto it = lower_bound(key);
        if (it != data.end() && !(key < it->first) && !(it->first < key)) {
            return ConstIterator(it);
        }
        return end();
    }

    void insert(const std::pair<TKey, TVal>& p) {
        auto it = lower_bound(p.first);
        if (it != data.end() && !(p.first < it->first) && !(it->first < p.first)) {
            // Key exists - update value
            it->second = p.second;
            return;
        }
        data.insert(it, p);
    }

    size_t erase(const TKey& key) {
        auto it = lower_bound(key);
        if (it != data.end() && !(key < it->first) && !(it->first < key)) {
            data.erase(it);
            return 1;
        }
        return 0;
    }

    TVal& operator[](const TKey& key) {
        auto it = lower_bound(key);
        if (it != data.end() && !(key < it->first) && !(it->first < key)) {
            return it->second;
        }
        it = data.insert(it, std::pair<TKey, TVal>{key, TVal{}});
        return it->second;
    }

    TVal& at(const TKey& key) {
        auto it = find(key);
        if (it == end()) {
            throw std::out_of_range("Key not found");
        }
        return it->second;
    }

    const TVal& at(const TKey& key) const {
        auto it = find(key);
        if (it == end()) {
            throw std::out_of_range("Key not found");
        }
        return it->second;
    }
};

#endif // ORDERED_TAB_H
