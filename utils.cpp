//
// Created by Alejo Acevedo on 15/09/2019.
//

#include "utils.h"


void print_byte_as_bits(char val) {
  for (int i = 7; 0 <= i; i--) {
    printf("%c", (val & (1 << i)) ? '1' : '0');
  }
}

void print_bits(unsigned char * bytes, size_t num_bytes) {
  printf("[ ");
  for (size_t i = 0; i < num_bytes; i++) {
    print_byte_as_bits(bytes[i]);
    printf(" ");
  }
  printf("]");
}

