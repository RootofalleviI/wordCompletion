#ifndef WORDCOMPLETION
#define WORDCOMPLETION

#include <vector>
#include <string>
#include <iostream>
#include <exception>
#include <map>

using std::vector;
using std::string;
using std::cout;
using std::pair;
using std::map;

#define CHAR_SIZE 26

// ======================================================================
// Max-oriented heap, vector implementation
// ======================================================================

class PQ {

    vector<pair<int, int>> pq;  // storing pairs of (word_id, freq)

    int PARENT(uint i) {
      if (i == 0) return -1;
      return (i - 1) / 2;
    }

    int LEFT(uint i) { return (2 * i + 1); }
    int RIGHT(uint i){ return (2 * i + 2); }

    void fix_up(uint i) {
      if (PARENT(i) != -1 && pq[PARENT(i)].second < pq[i].second) {
        swap(pq[i], pq[PARENT(i)]);
        fix_up(PARENT(i));
      }
    }

    void fix_down(uint i) {
      int left = LEFT(i);
      int right = RIGHT(i);
      int largest = i;
      if (left < size() && pq[left].second > pq[i].second)
        largest = left;
      if (right < size() && pq[right].second > pq[largest].second)
        largest = right;
      if (largest != i) {
        swap(pq[i], pq[largest]);
        fix_down(largest);
      }
    }

public:
    int size() { return pq.size(); }
    bool empty() { return size() == 0; }

    // Insert a new pair (int, 1) pair to this->pq.
    // Call this when id is not present in this->pq.
    // O(log n) time.
    void insert(int id) {
        this->pq.emplace_back(std::make_pair(id, 1));
        fix_up(size()-1);
    }

    // Increment priority of key id in this->pq.
    // Call this when id is present in this->pq.
    // O(log n) time.
    void changePriority(int id){
      for (int i = 0; i < size(); ++i) {
        if (pq[i].first == id) {
          pq[i].second += 1;
          fix_up(i);
          fix_down(i);
          return;
        }
      }
      throw std::invalid_argument("ID not present in PQ."); // sanity check
    };

    // Pop and return the pair with max frequency from PQ.
    // O(log n) time.
    pair<int, int> deleteMax(){
      pair<int, int> max = pq[0];
      swap(pq[0], pq[size()-1]);
      pq.pop_back();
      fix_down(0);
      return max;
    };

    // Get the k word_ids with max frequencies.
    // O(k log n) time.
    vector<int> getKMax(int k){
      vector<pair<int,int>> temp = pq;  // back up for restoring purpose
      vector<int> res;
      for (int i = 0; i < k; ++i) {
        if (pq.empty())
          res.emplace_back(-1); // if nothing left in current PQ, append -1
        else
          res.emplace_back(deleteMax().first);
      }
      pq = temp;  // restoring
      return res;
    };

    // Debugging function for printing out the content in PQ.
    void print(){
      for (int i = 0; i < size(); ++i) {
        cout << i << ": " << pq[i].first << " " << pq[i].second << '\n';
      }
    };
};


// ======================================================================
// Trie, standard implementation
// ======================================================================

class Trie {
    Trie* children[CHAR_SIZE];
    PQ* pq;  // a reference to a PQ instance, whose pq stores pairs (word_id, freq)
public:

    Trie() {
      for (int i = 0; i < CHAR_SIZE; ++i)
        children[i] = nullptr;
      pq = new PQ();
    }

    int getIdChild(char c) { return c - 97; }

    vector<int> getKMax(int k) {
      return this->pq->getKMax(k);
    }

    // Insert a new id to PQs of current and subsequent Trie nodes.
    // Call this when id is not present in this->pq.
    // O(|S|log n) time, where S is the key and n is the size of this->pq.
    void insert(std::string key, int id){
      Trie* curr = this;
      curr->pq->insert(id);
      for (int i = 0; i < key.length(); i++) {
        if (curr->children[getIdChild(key[i])] == nullptr)
          curr->children[getIdChild(key[i])] = new Trie();
        curr = curr->children[getIdChild(key[i])];
        curr->pq->insert(id);
      }
    };

    // Increment the frequency of given key by 1.
    // Call this when id already exists in this->pq.
    // O(|S|log n) time, where S is the key and n is the size of this->pq.
    void changePriority(std::string key, int id){
      Trie* curr = this;
      curr->pq->changePriority(id);
      for (int i = 0; i < key.length(); ++i) {
        curr = curr->children[getIdChild(key[i])];
        curr->pq->changePriority(id);
      }
    };

    // Put a search path (ptrs to Trie nodes) to the vector out.
    // O(|S|) time where S is the key.
    void search(std::string key, vector<Trie*> &out){
      Trie* curr = this;
      out.emplace_back(curr);
      for (int i = 0; i < key.length(); ++i) {
        if (curr)
          curr = curr->children[getIdChild(key[i])];
        out.emplace_back(curr);
      }
    };

    ~Trie() {
      delete pq;
      for (Trie* c : children) {
        delete c;
      }
    }
};



// ======================================================================
// wordCompletion
// ======================================================================

class wordCompletion{
    Trie* Dict;
    map<string, int> Words;
    int id_counter;

    // Returns True if key exists in Word.
    // O(log n) time.
    int checkId(string key){
      if (Words.count(key))
        return Words[key];
      return -1;
    };

    // Register a new key in Words, O(1) time.
    int registerKey(string key){
      Words.emplace(std::make_pair(key, id_counter));
      return id_counter++;
    };

public:
    ~wordCompletion();

    // Debugging
    void print(){
      for (auto& it : Words)
        cout << it.first << " " << it.second << "\n";
    };

    wordCompletion();

    int access(std::string s);

    std::vector<std::vector<int> > getCompletions(std::string w, int k);
    
};

#endif
