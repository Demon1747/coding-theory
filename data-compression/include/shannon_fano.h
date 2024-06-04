// Copyright 2024 Dmitriy Stepanov

#ifndef SHANNON_FANO_H_
#define SHANNON_FANO_H_

#include <string>
#include <unordered_map>

struct SFNode {
  char data;
  double freq;

  SFNode* left;
  SFNode* right;
};

class ShannonFanoCoder {
 public:
  ShannonFanoCoder() noexcept : sf_tree_root_(nullptr) {}
  ~ShannonFanoCoder() noexcept;

  void AnalyseText(const std::string& in_filename) noexcept;
  void CreateCode();
  void WriteToFile(const std::string& in_filename,
                   const std::string& out_filename);

 private:
  SFNode* Divider(const std::list<SFNode*>::iterator& iter_first,
                  const std::list<SFNode*>::iterator& iter_last,
                  double cur_freq);
  void FillCodebase(const SFNode* cur_node,
                    std::vector<bool>& cur_code) noexcept;

  SFNode* sf_tree_root_;

  std::unordered_map<char, double> char_freqs_;
  std::unordered_map<char, std::vector<bool>> codebase_;
};

#endif  // SHANNON_FANO_H_
