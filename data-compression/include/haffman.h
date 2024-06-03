// Copyright 2024 Dmitriy Stepanov

#ifndef HAFFMAN_H_
#define HAFFMAN_H_

#include <string>
#include <unordered_map>

struct Node {
  char data;
  size_t freq;

  Node* left;
  Node* right;
};

class HaffmanCoder {
 public:
  HaffmanCoder() noexcept : h_tree_root_(nullptr) {}
  ~HaffmanCoder() noexcept;

  void analyse_text(const std::string& in_filename) noexcept;

  void fill_codebase(const Node* cur_node,
                     std::vector<bool>& cur_code) noexcept;

  void create_code();

  void write_to_file(const std::string& in_filename,
                     const std::string& out_filename);

 private:
  Node* h_tree_root_;

  std::unordered_map<char, size_t> char_freqs_;
  std::unordered_map<char, std::vector<bool>> codebase_;
};

#endif  // HAFFMAN_H_
