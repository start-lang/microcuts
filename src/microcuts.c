#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "microcuts.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

int assert_no = INT_MIN;
int failed = 0;
char * section_name = NULL;
int total_failed = 0;

void start_tests(){
  assert_no = INT_MIN;
  failed = 0;
  section_name = NULL;
  total_failed = 0;
}

void end_tests(){
  if (total_failed == 0){
    printf("%s", KGRN);
    printf("\n=============================================\n");
    printf("> OK\n");
    printf("=============================================\n");
    printf("%s", KNRM);
  } else {
    printf("%s", KYEL);
    printf("\n=============================================\n");
    printf("> Check messages above. %d assertion%s failed\n", total_failed, (total_failed == 1? "" : "s"));
    printf("=============================================\n");
    printf("%s", KNRM);
  }
}

void begin_section(const char* name){
  section_name = malloc(sizeof(char) * strlen(name));
  strcpy(section_name, name);
  assert_no = 1;
  failed = 0;
}

void end_section(int action){
  if (failed){

  } else {
    printf("%s", KGRN);
    printf("\nAll tests on '%s' passed.\n", section_name);
    printf("%s", KNRM);
  }
  assert_no = INT_MIN;
  total_failed += failed;
}

void __assert(const char* expr_str, int a, const char* file, int line){
  if (!a){
    if (assert_no < 0){
      printf("%s", KRED);
      printf("\n\nWARNING!! assert() called before begin_section()!!\n");
    }
    printf("%s", KYEL);
    printf("\nAssert %d failed on section '%s':\n", assert_no, section_name);
    printf("Expected\t(%s) != 0, found %d\n", expr_str, a);
    printf("Source code:\tassert(%s)\n", expr_str);
    printf("Source:\t\t%s, line %d\n",file, line);
    printf("%s", KNRM);
    failed++;
  }
  assert_no++;
}

void __assert_eq(const char* expr_str_a, const char* expr_str_b, int a, int b, const char* file, int line){
  if (a != b){
    if (assert_no < 0){
      printf("%s", KRED);
      printf("\n\nWARNING!! assert() called before begin_section()!!\n");
    }
    printf("%s", KYEL);
    printf("\nAssert %d failed on section '%s':\n", assert_no, section_name);
    printf("Expected:\t%d == %d\n", a, b);
    printf("Source code:\t%s === %s\n", expr_str_a, expr_str_b);
    printf("Source:\t\t%s, line %d\n",file, line);
    printf("%s", KNRM);
    failed++;
  } else {
    printf(".");
  }
  assert_no++;
}

void __assert_str_eq(const char* expr_str_a, const char* expr_str_b, const char* a, const char* b, const char* file, int line){
  if (strcmp(a, b)){
    if (assert_no < 0){
      printf("%s", KRED);
      printf("\n\nWARNING!! assert() called before begin_section()!!\n");
    }
    printf("%s", KYEL);
    printf("\nAssert %d failed on section '%s':\n", assert_no, section_name);
    printf("Expected:\t%s == %s\n", a, b);
    printf("Source code:\t%s === %s\n", expr_str_a, expr_str_b);
    printf("Source:\t\t%s, line %d\n",file, line);
    printf("%s", KNRM);
    failed++;
  } else {
    printf(".");
  }
  assert_no++;
}
