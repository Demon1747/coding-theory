// Copyright 2024 Dmitriy Stepanov

#ifndef HAFFMAN_H_
#define HAFFMAN_H_

#include <string>
#include <unordered_map>

struct HNode {
  char data;
  size_t freq;

  HNode* left;
  HNode* right;
};

class HaffmanCoder {
 public:
  HaffmanCoder() noexcept : h_tree_root_(nullptr) {}
  ~HaffmanCoder() noexcept;

  void AnalyseText(const std::string& in_filename) noexcept;
  void CreateCode();
  void WriteToFile(const std::string& in_filename,
                   const std::string& out_filename);

 private:
  void FillCodebase(const HNode* cur_node, std::vector<bool>& cur_code) noexcept;

  HNode* h_tree_root_;

  std::unordered_map<char, size_t> char_freqs_;
  std::unordered_map<char, std::vector<bool>> codebase_;
};

#endif  // HAFFMAN_H_
