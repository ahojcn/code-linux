#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[], char* env[]) {
  printf("argc %d\n", argc);

  for (int i = 0; i < argc; ++i) {
    printf("%s\n", argv[i]);
  }

  for (int i = 0; env[i] != NULL; ++i) {
    printf("%s\n", env[i]);
  }

  char* res = getenv("PATH");
  printf("%s\n", res);
}
