#ifndef __GRAPH_H
#define __GRAPH_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <optional>
#include <string>
#include <algorithm>
#include <cstdint>

template <typename T>
class Graph {
private:
    // Hash function to compute a 64-bit hash for byte sequences
    uint64_t computeHash(const std::vector<unsigned char>& byteSequence) const {
        uint64_t hash = 0xcbf29ce484222325; // FNV-1a 64-bit offset basis
        for (unsigned char byte : byteSequence) {
            hash ^= byte;
            hash *= 0x100000001b3; // FNV-1a 64-bit prime
        }
        return hash;
    }

    // Graph represented as an unordered_map with 64-bit hashes as keys
    std::unordered_map<uint64_t, T> graph;

public:
    using iterator = typename std::unordered_map<uint64_t, T>::iterator;
    using const_iterator = typename std::unordered_map<uint64_t, T>::const_iterator;

    // Adds or updates an element associated with a byte sequence
    T& operator[](const std::vector<unsigned char>& byteSequence) {
        uint64_t hash = computeHash(byteSequence);
        return graph[hash];
    }

    // Checks if two graphs are equal
    bool operator==(const Graph& other) const {
        return graph == other.graph;
    }

    // Checks if two graphs are not equal
    bool operator!=(const Graph& other) const {
        return !(*this == other);
    }

    // Merges two graphs
    Graph operator+(const Graph& other) const {
        Graph result = *this;
        for (const auto& [key, value] : other.graph) {
            result.graph[key] = value;
        }
        return result;
    }

    // Adds elements from another graph into this graph
    Graph& operator+=(const Graph& other) {
        for (const auto& [key, value] : other.graph) {
            graph[key] = value;
        }
        return *this;
    }

    // Removes an element by its associated byte sequence
    bool removeElement(const std::vector<unsigned char>& byteSequence) {
        uint64_t hash = computeHash(byteSequence);
        return graph.erase(hash) > 0;
    }

    // Finds an element by its associated byte sequence
    iterator find(const std::vector<unsigned char>& byteSequence) {
        uint64_t hash = computeHash(byteSequence);
        return graph.find(hash);
    }

    const_iterator find(const std::vector<unsigned char>& byteSequence) const {
        uint64_t hash = computeHash(byteSequence);
        return graph.find(hash);
    }

    // Finds the most relevant element for a subsequence and returns an iterator
    iterator findRelevant(const std::vector<unsigned char>& byteSequence) {
        for (size_t len = byteSequence.size(); len > 0; --len) {
            std::vector<unsigned char> subSequence(byteSequence.begin(), byteSequence.begin() + len);
            uint64_t hash = computeHash(subSequence);
            auto it = graph.find(hash);
            if (it != graph.end()) {
                return it;
            }
        }
        return graph.end();
    }

    const_iterator findRelevant(const std::vector<unsigned char>& byteSequence) const {
        for (size_t len = byteSequence.size(); len > 0; --len) {
            std::vector<unsigned char> subSequence(byteSequence.begin(), byteSequence.begin() + len);
            uint64_t hash = computeHash(subSequence);
            auto it = graph.find(hash);
            if (it != graph.end()) {
                return it;
            }
        }
        return graph.end();
    }

    // Returns an iterator to the beginning
    iterator begin() {
        return graph.begin();
    }

    const_iterator begin() const {
        return graph.begin();
    }

    // Returns an iterator to the end
    iterator end() {
        return graph.end();
    }

    const_iterator end() const {
        return graph.end();
    }

    // Checks if the graph is empty
    bool empty() const {
        return graph.empty();
    }

    // Returns the size of the graph
    size_t size() const {
        return graph.size();
    }

    // Lists all elements in the graph
    void listElements() const {
        for (const auto& pair : graph) {
            std::cout << "Hash: 0x" << std::hex << pair.first << " -> Element: " << pair.second << std::endl;
        }
    }
};
#endif