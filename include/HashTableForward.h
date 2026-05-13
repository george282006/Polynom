//
// Created by ilya on 08.05.2026.
//

#ifndef POLYNOMIAL_HASHTABLEFORWARD_H
#define POLYNOMIAL_HASHTABLEFORWARD_H
#include <vector>
#include "list.h"
#include "polynomial.h"

template <class TKey, class TVal>
class HashTableForward {
private:
    std::vector< List <std::pair<TKey, TVal> > > data;
    size_t size_;
    size_t cap_;

    size_t hash(const TKey& key) const;
    
    void rehash() {
        cap_ *= 2;
        std::vector< List <std::pair<TKey, TVal> > > old_data = data;
        data.clear();
        data.resize(cap_);
        
        for (const auto& bucket : old_data) {
            for (const auto& pair : bucket) {
                size_t index = hash(pair.first);
                data[index].push_front(pair);
            }
        }
    }

public:
    HashTableForward(size_t capacity = 10) : size_(0), cap_(capacity) {
        data.resize(capacity);
    }

    void insert(const TKey& key, const TVal& value) {
        size_t index = hash(key);
        //Обновление существующего элемента
        for (auto& pair : data[index]) {
            if (pair.first == key) {
                pair.second = value;
                return;
            }
        }

        //Добавление нового
        data[index].push_front({key, value});
        size_++;
        
        double delta = (double)size_ / cap_;
        if (delta > 0.9) {
            rehash();
        }
    }

    bool find(const TKey& key, TVal& value) const {
        size_t index = hash(key);
        for (const auto& pair : data[index]) {
            if (pair.first == key) {
                value = pair.second;
                return true;
            }
        }
        return false;
    }

    bool erase(const TKey& key) {
        // Сначала проверяем наличие элемента
        TVal dummy;
        if (!find(key, dummy)) {
            return false;
        }
        
        size_t index = hash(key);  // Вычисляем индекс по хешу
        List<std::pair<TKey, TVal>>& bucket = data[index];  // Берём цепочку
        
        auto it = bucket.begin();  // Текущий элемент
        auto prev = bucket.begin();  // Предыдущий элемент
        
        // Проверяем первый элемент
        if (it != bucket.end() && it->first == key) {
            bucket.pop_front();  // Удаляем с начала
            size_--;
            return true;
        }
        
        ++it;  // Переходим ко второму элементу
        
        // Ищем нужный элемент в остальной цепочке
        while (it != bucket.end()) {
            if (it->first == key) {
                bucket.erase_after(prev);  // Удаляем элемент после prev
                size_--;
                return true;
            }
            ++prev;  // Сдвигаем prev
            ++it;    // Сдвигаем it
        }
        
        return false;  // Не найден
    }

    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }
    double load_factor() const { return (double)size_ / cap_; }
};

template <>
size_t HashTableForward<Polynomial, int>::hash(const Polynomial& poly) const {
    size_t hash_value = 0;
    for (const auto& monom : poly.get_monoms()) {
        hash_value += monom.i + monom.j + monom.k + (size_t)monom.coeff;
    }
    return hash_value % cap_;
}

#endif //POLYNOMIAL_HASHTABLEFORWARD_H