#pragma once

#include <algorithm>
#include <unordered_map>

template <typename Key, typename T>
bool contains(const std::unordered_map<Key, T>& map, const Key& key) {
    return map.find(key) != map.end();
}
