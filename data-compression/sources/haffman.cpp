// Copyright 2024 Dmitriy Stepanov

#include "haffman.h"

#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

HaffmanCoder::~HaffmanCoder() noexcept {
  std::queue<Node*> grbg_collector({h_tree_root_});
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

void HaffmanCoder::analyse_text(const std::string& in_filename) noexcept {
  std::ifstream in_file(in_filename, std::ios_base::in);
  char buf[1024];
  while (true) {
    in_file.get(buf, 1024);
    for (size_t i = 0; i < 1024 && buf[i] != '\0'; ++i) {
      char_freqs_[buf[i]]++;
    }

    if (in_file.eof()) {
      break;
    }
  }
}

void HaffmanCoder::fill_codebase(const Node* cur_node,
                                 std::vector<bool>& cur_code) noexcept {
  if (cur_node->left) {
    cur_code.emplace_back(0);
    fill_codebase(cur_node->left, cur_code);
    cur_code.pop_back();
  }

  if (cur_node->right) {
    cur_code.emplace_back(1);
    fill_codebase(cur_node->right, cur_code);
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

void HaffmanCoder::create_code() {
  auto cmp = [](const auto a, const auto b) { return a->freq > b->freq; };
  std::priority_queue<Node*, std::deque<Node*>, decltype(cmp)> ordered_queue(
      cmp);

  for (auto elem : char_freqs_) {
    ordered_queue.push(new Node({elem.first, elem.second, nullptr, nullptr}));
  }

  while (ordered_queue.size() > 1) {
    auto node_right = ordered_queue.top();
    ordered_queue.pop();
    auto node_left = ordered_queue.top();
    ordered_queue.pop();

    auto new_node = new Node(
        {NULL, node_left->freq + node_right->freq, node_left, node_right});
    ordered_queue.push(new_node);
  }

  h_tree_root_ = ordered_queue.top();
  std::vector<bool> code_value;

  fill_codebase(h_tree_root_, code_value);
}

void HaffmanCoder::write_to_file(const std::string& in_filename,
                                 const std::string& out_filename) {
  std::ifstream in_file(in_filename, std::ios_base::in);
  std::ofstream out_file(out_filename + "_haffman.txt",
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
