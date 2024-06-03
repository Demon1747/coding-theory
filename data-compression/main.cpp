// Copyright 2024 Dmitriy Stepanov

#include <iostream>

#include "include\haffman.h"

// args: in_filename, out_filename
int main(int argc, char** argv) {
  // std::string in_filename(argv[1]);
  // std::string out_filename(argv[2]);
  std::string in_filename("../../input_data/data3.txt");
  std::string out_filename("../../out_data/data3");
  HaffmanCoder haffman_coder;

  haffman_coder.analyse_text(in_filename);
  haffman_coder.create_code();
  haffman_coder.write_to_file(in_filename, out_filename);

  return 0;
}
