#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#ifdef PRINT_TIMINGS
#include <time.h>
#endif
#include <microcuts.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define LINE  49

int assert_no = INT_MIN;
int failed = 0;
char * section_name = NULL;
int total_failed = 0;
int total_asserts = 0;
int section_asserts = 0;
int (*cleanup_func)(void) = NULL;
void (*target_func)(void) = NULL;
int operations = 0;
#ifdef PRINT_TIMINGS
clock_t start = 0;
clock_t section_start = 0;
#endif

void start_tests(void){
  setvbuf(stdout, NULL, _IONBF, 0);
  assert_no = INT_MIN;
  failed = 0;
  section_name = NULL;
  total_failed = 0;
}

void printf_line(void){
  for (int i = 0; i < LINE; i++) printf("=");
  printf("\n");
}

void end_tests(void){
#ifdef PRINT_TIMINGS
  clock_t end = clock();
  double duration = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
#endif
#ifdef HIDE_SECTION
  printf("\n");
#endif
  if (total_failed == 0){
    printf("%s", KGRN);
    printf_line();
#ifdef PRINT_TIMINGS
    printf("> %d tests passed in %.2f ms (%.2f mop/s)\n", total_asserts, duration, operations / (duration * 1000));
#else
    printf("> %d tests passed\n", total_asserts);
#endif
    printf_line();
    printf("%s", KNRM);
  } else {
    printf("%s", KYEL);
    printf_line();
    printf("> Check messages above. %d assertion%s failed\n", total_failed,
           (total_failed == 1? "" : "s"));
    printf_line();
    printf("%s", KNRM);
  }
}

void begin_section(const char* name){
  section_name = malloc(sizeof(char) * strlen(name) + 1);
  strcpy(section_name, name);
  assert_no = 1;
  failed = 0;
  section_asserts = 0;
#ifdef PRINT_TIMINGS
    section_start = clock();
#endif
}

void set_cleanup(int (*func)(void)){
  cleanup_func = func;
}

void set_target(void (*func)(void)){
  target_func = func;
}

int run_target(void){
  start_tests();
#ifdef BENCHMARK
  printf("%s", KCYN);
  printf_line();
  printf("> Benchmarking %d iterations...", BENCHMARK);
  printf("%s", KNRM);
#ifdef PRINT_TIMINGS
  start = clock();
#endif
  for (int i = 0; i < BENCHMARK; i++) {
    if (target_func) target_func();
  }
#else
#ifdef PRINT_TIMINGS
  start = clock();
#endif
  if (target_func) target_func();
#endif
  end_tests();
  return total_failed;
}

void end_section(void){
#ifdef PRINT_TIMINGS
#ifndef HIDE_SECTION
  printf("%s", KGRN);
  clock_t end = clock();
  double time_spent = (double)(end - section_start) * 1000 / CLOCKS_PER_SEC;
  printf("\n%d tests passed @ '%s' in %.2f ms\n", section_asserts, section_name, time_spent);
  printf("%s", KNRM);
#endif
#else
#ifndef HIDE_SECTION
  printf("%s", KGRN);
  printf("\n%d tests passed @ '%s'\n", section_asserts, section_name);
  printf("%s", KNRM);
#endif
#endif
  assert_no = INT_MIN;
  total_failed += failed;
  total_asserts += section_asserts;
  free(section_name);
  section_name = NULL;
}

void mc_assert(const char* expr_str, int a, const char* file, int line){
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
  } else {
#ifndef BENCHMARK
    printf(".");
#endif
  }
  assert_no++;
  section_asserts++;
  if (cleanup_func) operations += cleanup_func();
}

void mc_assert_eq(const char* expr_str_a, const char* expr_str_b, int a, int b,
                 const char* file, int line){
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

#ifdef STOPFAIL
    if (cleanup_func) cleanup_func();
    exit(1);
#endif

    failed++;
  } else {
#ifndef BENCHMARK
    printf(".");
#endif
  }
  assert_no++;
  section_asserts++;
  if (cleanup_func) operations += cleanup_func();
}

void mc_assert_str_eq(const char* expr_str_a, const char* expr_str_b,
                     const char* a, const char* b, const char* file, int line){
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
#ifndef BENCHMARK
    printf(".");
#endif
  }
  assert_no++;
  section_asserts++;
  if (cleanup_func) operations += cleanup_func();
}
