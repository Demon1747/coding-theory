// Copyright 2024 Dmitriy Stepanov

#include <iostream>

#include "include\haffman.h"
#include "include\shannon_fano.h"

// args: in_filename, out_filename
int main(int argc, char** argv) {
  // std::string in_filename(argv[1]);
  // std::string out_filename(argv[2]);
  std::string in_filename("../../input_data/data3.txt");
  std::string out_filename("../../out_data/data3");
  HaffmanCoder haffman_coder;
  ShannonFanoCoder shannon_fano_coder;

  haffman_coder.AnalyseText(in_filename);
  haffman_coder.CreateCode();
  haffman_coder.WriteToFile(in_filename, out_filename);

  shannon_fano_coder.AnalyseText(in_filename);
  shannon_fano_coder.CreateCode();
  shannon_fano_coder.WriteToFile(in_filename, out_filename);

  return 0;
}
