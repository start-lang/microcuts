#include <stdio.h>
#include <string.h>
#include <microcuts.h>

int main(void){
  start_tests();
  begin_section("integers");
  assert_eq(1, 1);
  assert_eq(1, (1*2));
  assert_eq(1, 2);
  end_section();

  begin_section("strings");
  assert_str_eq("a", "a");
  assert_str_eq("a", "b");
  end_section();
  end_tests();

  start_tests();
  begin_section("integers ok");
  assert_eq(1, 1);
  assert_eq(1, 1);
  assert_eq(1, 1);
  end_section();

  begin_section("strings ok");
  assert_str_eq("a", "a");
  assert_str_eq("a", "a");
  end_section();
  end_tests();

  start_tests();
  begin_section("eq");
  assert(1 == 1);
  assert(1 == 1);
  assert(1 == 2);
  end_section();
  end_tests();

  return 0;
}
