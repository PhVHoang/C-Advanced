// Copyright (C) 2016 Nguyen Ba Ngoc

#include <stdlib.h>
#include <stdio.h>

struct TestCmd {
  int id;
  char args[100];
} cmd_list[9] = { {1, "-h"},
                  {2, "-v exam.txt"},
                  {3, "-w exam.txt 2 8"},
                  {4, "-w exam.txt 5 9"},
                  {5, "-w exam.txt 9 5"},
                  {6, "-w exam.txt 9 1"},
                  {7, "-p exam.txt 1 9"},
                  {8, "-n exam.txt 5"},
                  {9, "-s exam.txt 1 2 8 3"}};

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Usage: run pr. Here pr is the compiled solution.\n");
    return 0;
  }

  // Execute and evaluate
  float total = 0.0,
        scores[9];
  int i = 0;
  for (i = 0; i < 9; i++) {
    char cmd[100];
    sprintf(cmd, "./%s %s", argv[1], cmd_list[i].args);
    printf("%s\n", cmd);
    system(cmd);
    printf("\nScore for test %d: ", cmd_list[i].id);
    scanf("%f", &scores[i]);
    total += scores[i];
  }

  // Report
  printf("Test: \t");
  for (i = 0; i < 9; i++)
    printf("%5d", cmd_list[i].id);
  printf("\nScore: \t");
  for (i = 0; i < 9; i++)
    printf("%5.1f", scores[i]);
  printf("\nTotal: \t%5.1f\n", total);
  return 0;
}