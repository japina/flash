#include "flash.h"

void to_binary(int number, int string_len, char *binary_value) {
  int i, value, len;
  int binvalue;
  binary_value[string_len] = '\0';

  value=number;
  i=0;
  while(value>0){
    binvalue=value%2;
    value=value>>1;
    if (binvalue==1){
    	binary_value[string_len - i - 1]='1';
    } else {
    	binary_value[string_len - i - 1]='0';
    }
    i++;
  }
  while(i<string_len){
    binary_value[string_len - i - 1]='0';
    i++;
  }
}

void write_flash(char *binary_value) {

}