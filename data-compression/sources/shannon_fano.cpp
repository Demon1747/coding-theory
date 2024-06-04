// Copyright 2024 Dmitriy Stepanov

#include "shannon_fano.h"

#include <fstream>
#include <iostream>
#include <list>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

ShannonFanoCoder::~ShannonFanoCoder() noexcept {
  if (!sf_tree_root_) {
    return;
  }

  std::queue<SFNode*> grbg_collector({sf_tree_root_});

  while (!grbg_collector.empty()) {
    if (grbg_collector.front()->left) {
      grbg_collector.push(grbg_collector.front()->left);
    }

    if (grbg_collector.front()->right) {
      grbg_collector.push(grbg_collector.front()->right);
    }

    delete grbg_collector.front();
    grbg_collector.pop();
  }
}

void ShannonFanoCoder::AnalyseText(const std::string& in_filename) noexcept {
  std::ifstream in_file(in_filename, std::ios_base::in);
  char buf[1024];
  double ctr = 0;

  while (true) {
    in_file.get(buf, 1024);
    for (size_t i = 0; i < 1024 && buf[i] != '\0'; ++i) {
      char_freqs_[buf[i]]++;
      ctr++;
    }

    if (in_file.eof()) {
      break;
    }
  }

  for (auto& el : char_freqs_) {
    el.second /= ctr;
  }
}

void ShannonFanoCoder::FillCodebase(const SFNode* cur_node,
                                    std::vector<bool>& cur_code) noexcept {
  if (cur_node->left) {
    cur_code.emplace_back(0);
    FillCodebase(cur_node->left, cur_code);
    cur_code.pop_back();
  }

  if (cur_node->right) {
    cur_code.emplace_back(1);
    FillCodebase(cur_node->right, cur_code);
    cur_code.pop_back();
  }

  if (cur_node->data) {
    codebase_[cur_node->data] = cur_code;
    std::cout << "Code for '" << cur_node->data << "' is: ";
    for (auto bit : cur_code) {
      std::cout << static_cast<unsigned int>(bit);
    }
    std::cout << std::endl;
  }
}

SFNode* ShannonFanoCoder::Divider(
    const std::list<SFNode*>::iterator& iter_first,
    const std::list<SFNode*>::iterator& iter_last, double total_freq) {
  if (std::distance(iter_first, iter_last) == 1) {
    return new SFNode(
        {(*iter_first)->data, (*iter_first)->freq, nullptr, nullptr});
  }

  double mid_freq = 0;
  for (auto iter_mid = iter_first; iter_mid != iter_last; ++iter_mid) {
    mid_freq += (*iter_mid)->freq;
    if (mid_freq >= total_freq / 2) {
      SFNode* left_leaf = Divider(iter_first, std::next(iter_mid, 1), mid_freq);
      SFNode* right_leaf =
          Divider(std::next(iter_mid, 1), iter_last, total_freq - mid_freq);

      return new SFNode({NULL, total_freq, left_leaf, right_leaf});
    }
  }
}

void ShannonFanoCoder::CreateCode() {
  std::list<SFNode*> nodes;

  for (auto elem : char_freqs_) {
    nodes.push_back(new SFNode({elem.first, elem.second, nullptr, nullptr}));
  }

  nodes.sort([](const auto a, const auto b) { return a->freq > b->freq; });

  sf_tree_root_ = Divider(nodes.begin(), nodes.end(), 1.0);
  std::vector<bool> code_value;

  std::cout << "Shannon-Fano code:" << std::endl;
  FillCodebase(sf_tree_root_, code_value);
  std::cout << std::endl;
}

void ShannonFanoCoder::WriteToFile(const std::string& in_filename,
                                   const std::string& out_filename) {
  std::ifstream in_file(in_filename, std::ios_base::in);
  std::ofstream out_file(out_filename + "_shannon_fano.txt",
                         std::ios_base::out | std::ios_base::binary);

  unsigned char byte_to_write(0);
  uint8_t ctr = 0;
  char buf[1024];

  while (true) {
    in_file.get(buf, 1024);
    for (size_t i = 0; i < 1024 && buf[i] != '\0'; ++i) {
      for (auto bit : codebase_[buf[i]]) {
        byte_to_write |= bit;
        ++ctr;

        if (ctr == 8) {
          out_file << byte_to_write;
          byte_to_write &= 0;
          ctr = 0;
        } else {
          byte_to_write <<= 1;
        }
      }
    }

    if (in_file.eof()) {
      break;
    }
  }

  for (; ctr < 7; ++ctr) {
    byte_to_write <<= 1;
  }
  out_file << byte_to_write;

  out_file.close();
}
