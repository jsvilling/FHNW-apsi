
#include <string.h>
#include <stdio.h>

void test (int argc, char *argv[]) {
  char s2[4] = "yes";    // set s2 to "yes"
  char s1[4] = "abc";    // set s1 to "abc"
  strcpy(s1, argv[1]);   // copy argv[1] into s1
  puts(s2);              // print s2
}

int main (int argc, char *argv[]) {
  test(argc, argv);
  puts("end\n");
}