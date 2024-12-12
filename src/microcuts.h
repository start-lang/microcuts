#ifndef ASSERT_H
#define ASSERT_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NDEBUG
#   define assert(A) mc_assert(#A, A, __FILE__, __LINE__)
#   define assert_eq(A, B) mc_assert_eq(#A, #B, A, B, __FILE__, __LINE__)
#   define assert_str_eq(A, B) mc_assert_str_eq(#A, #B, A, B, __FILE__, __LINE__)
#else
#   define assert(A) ;
#   define assert_eq(A, B) ;
#   define assert_str_eq(A, B) ;
#endif

#ifdef BENCHMARK
#define HIDE_SECTION
#endif

void start_tests(void);
void end_tests(void);
void begin_section(const char* name);
void end_section(void);
void set_cleanup(int (*func)(void));
void set_target(void (*func)(void));
int run_target(void);
void mc_assert(const char* expr_str, int a, const char* file, int line);
void mc_assert_eq(const char* expr_str_a, const char* expr_str_b, int a, int b,
                 const char* file, int line);
void mc_assert_str_eq(const char* expr_str_a, const char* expr_str_b,
                     const char* a, const char* b, const char* file, int line);

#ifdef __cplusplus
}
#endif

#endif
