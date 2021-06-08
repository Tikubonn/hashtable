
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "hashtable.h"

#define TEST(form)\
if (form){\
printf("success: %s at %s:%d\n", #form, __FILE__, __LINE__);\
}\
else {\
printf("error: %s at %s:%d\n", #form, __FILE__, __LINE__); abort();\
}

static void test_hashtable_set (){
  // set and get.
  {
    MAKE_LOCAL_HASHTABLE(htable, 128, DEFAULT_HASHTABLE_CLASS);
    TEST(hashtable_count(htable) == 0);
    TEST(hashtable_set((void*)11, (void*)1, htable) == 0);
    TEST(hashtable_set((void*)22, (void*)2, htable) == 0);
    TEST(hashtable_set((void*)33, (void*)3, htable) == 0);
    TEST(hashtable_count(htable) == 3);
    void *value;
    TEST(hashtable_get((void*)1, htable, &value) == 0);
    TEST(value == (void*)11);
    TEST(hashtable_get((void*)2, htable, &value) == 0);
    TEST(value == (void*)22);
    TEST(hashtable_get((void*)3, htable, &value) == 0);
    TEST(value == (void*)33);
    TEST(hashtable_get((void*)4, htable, &value) != 0); //get unset entry (always error!)
    hashtable_clear(htable);
    TEST(hashtable_count(htable) == 0);
  }
  // set and unset 
  {
    MAKE_LOCAL_HASHTABLE(htable, 128, DEFAULT_HASHTABLE_CLASS);
    TEST(hashtable_count(htable) == 0);
    TEST(hashtable_set((void*)11, (void*)1, htable) == 0);
    TEST(hashtable_set((void*)22, (void*)2, htable) == 0);
    TEST(hashtable_set((void*)33, (void*)3, htable) == 0);
    TEST(hashtable_count(htable) == 3);
    void *value;
    TEST(hashtable_get((void*)1, htable, &value) == 0);
    TEST(value == (void*)11);
    TEST(hashtable_get((void*)2, htable, &value) == 0);
    TEST(value == (void*)22);
    TEST(hashtable_get((void*)3, htable, &value) == 0);
    TEST(value == (void*)33);
    TEST(hashtable_unset((void*)1, htable) == 0);
    TEST(hashtable_unset((void*)2, htable) == 0);
    TEST(hashtable_count(htable) == 1);
    TEST(hashtable_get((void*)1, htable, &value) != 0); //get unset entry (always error!)
    TEST(hashtable_get((void*)2, htable, &value) != 0); //get unset entry (always error!)
    TEST(hashtable_get((void*)3, htable, &value) == 0);
    TEST(value == (void*)33);
  }
}

static void test_hashtable_iterate (){
  // set and iterate.
  {
    MAKE_LOCAL_HASHTABLE(htable, 128, DEFAULT_HASHTABLE_CLASS);
    TEST(hashtable_count(htable) == 0);
    TEST(hashtable_set((void*)11, (void*)1, htable) == 0);
    TEST(hashtable_set((void*)22, (void*)2, htable) == 0);
    TEST(hashtable_set((void*)33, (void*)3, htable) == 0);
    TEST(hashtable_count(htable) == 3);
    // iterate entries.
    {
      struct hashtable_iterator iterator = hashtable_iterate(htable);
      struct hashtable_entry entry;
      TEST(hashtable_iterator_has_next(&iterator, htable) == true);
      TEST(hashtable_iterator_next(&iterator, htable, &entry) == 0);
      TEST(entry.key == (void*)1 || entry.key == (void*)2 || entry.key == (void*)3);
      TEST(entry.value == (void*)11 || entry.value == (void*)22 || entry.value == (void*)33);
      TEST(hashtable_iterator_has_next(&iterator, htable) == true);
      TEST(hashtable_iterator_next(&iterator, htable, &entry) == 0);
      TEST(entry.key == (void*)1 || entry.key == (void*)2 || entry.key == (void*)3);
      TEST(entry.value == (void*)11 || entry.value == (void*)22 || entry.value == (void*)33);
      TEST(hashtable_iterator_has_next(&iterator, htable) == true);
      TEST(hashtable_iterator_next(&iterator, htable, &entry) == 0);
      TEST(entry.key == (void*)1 || entry.key == (void*)2 || entry.key == (void*)3);
      TEST(entry.value == (void*)11 || entry.value == (void*)22 || entry.value == (void*)33);
      TEST(hashtable_iterator_has_next(&iterator, htable) == false);
      TEST(hashtable_iterator_next(&iterator, htable, &entry) != 0);
    }
    // iterate keys.
    {
      struct hashtable_iterator iterator = hashtable_iterate(htable);
      void *key;
      TEST(hashtable_iterator_has_next(&iterator, htable) == true);
      TEST(hashtable_iterator_next_key(&iterator, htable, &key) == 0);
      TEST(key == (void*)1 || key == (void*)2 || key == (void*)3);
      TEST(hashtable_iterator_has_next(&iterator, htable) == true);
      TEST(hashtable_iterator_next_key(&iterator, htable, &key) == 0);
      TEST(key == (void*)1 || key == (void*)2 || key == (void*)3);
      TEST(hashtable_iterator_has_next(&iterator, htable) == true);
      TEST(hashtable_iterator_next_key(&iterator, htable, &key) == 0);
      TEST(key == (void*)1 || key == (void*)2 || key == (void*)3);
      TEST(hashtable_iterator_has_next(&iterator, htable) == false);
      TEST(hashtable_iterator_next_key(&iterator, htable, &key) != 0);
    }
    // iterate values.
    {
      struct hashtable_iterator iterator = hashtable_iterate(htable);
      void *value;
      TEST(hashtable_iterator_has_next(&iterator, htable) == true);
      TEST(hashtable_iterator_next_value(&iterator, htable, &value) == 0);
      TEST(value == (void*)11 || value == (void*)22 || value == (void*)33);
      TEST(hashtable_iterator_has_next(&iterator, htable) == true);
      TEST(hashtable_iterator_next_value(&iterator, htable, &value) == 0);
      TEST(value == (void*)11 || value == (void*)22 || value == (void*)33);
      TEST(hashtable_iterator_has_next(&iterator, htable) == true);
      TEST(hashtable_iterator_next_value(&iterator, htable, &value) == 0);
      TEST(value == (void*)11 || value == (void*)22 || value == (void*)33);
      TEST(hashtable_iterator_has_next(&iterator, htable) == false);
      TEST(hashtable_iterator_next_value(&iterator, htable, &value) != 0);
    }
  }
}

static void test_hashtable_stretch (){
  // stretch hashtable.
  {
    MAKE_LOCAL_HASHTABLE(htable, 3, DEFAULT_HASHTABLE_CLASS);
    TEST(hashtable_set((void*)11, (void*)1, htable) == 0);
    TEST(hashtable_set((void*)22, (void*)2, htable) == 0);
    TEST(hashtable_set((void*)33, (void*)3, htable) == 0);
    TEST(hashtable_set((void*)44, (void*)4, htable) != 0); //set to full hashtable (always error!)
    struct hashtable_entry newentries[128] = {};
    TEST(hashtable_stretch(newentries, 128, htable) == 0);
    TEST(hashtable_set((void*)44, (void*)4, htable) == 0);
    TEST(hashtable_set((void*)55, (void*)5, htable) == 0);
    TEST(hashtable_set((void*)66, (void*)6, htable) == 0);
    void *value;
    TEST(hashtable_get((void*)1, htable, &value) == 0);
    TEST(value == (void*)11);
    TEST(hashtable_get((void*)2, htable, &value) == 0);
    TEST(value == (void*)22);
    TEST(hashtable_get((void*)3, htable, &value) == 0);
    TEST(value == (void*)33);
    TEST(hashtable_get((void*)4, htable, &value) == 0);
    TEST(value == (void*)44);
    TEST(hashtable_get((void*)5, htable, &value) == 0);
    TEST(value == (void*)55);
    TEST(hashtable_get((void*)6, htable, &value) == 0);
    TEST(value == (void*)66);
    struct hashtable_entry newentries2[3] = {};
    TEST(hashtable_stretch(newentries2, 3, htable) != 0); //stretch to smaller size (always error!)
  }
}

static void test_make_hashtable (){
  {
    hashtable *htable = make_hashtable(128, DEFAULT_HASHTABLE_CLASS);
    TEST(htable != NULL);
    TEST(hashtable_count(htable) == 0);
    TEST(hashtable_set((void*)11, (void*)1, htable) == 0);
    TEST(hashtable_set((void*)22, (void*)2, htable) == 0);
    TEST(hashtable_set((void*)33, (void*)3, htable) == 0);
    TEST(hashtable_count(htable) == 3);
    void *value;
    TEST(hashtable_get((void*)1, htable, &value) == 0);
    TEST(value == (void*)11);
    TEST(hashtable_get((void*)2, htable, &value) == 0);
    TEST(value == (void*)22);
    TEST(hashtable_get((void*)3, htable, &value) == 0);
    TEST(value == (void*)33);
    TEST(hashtable_get((void*)4, htable, &value) != 0); //get unset entry (always error!)
    hashtable_clear(htable);
    TEST(hashtable_count(htable) == 0);
    free_hashtable(htable);
  }
}

int main (){
  test_hashtable_set();
  test_hashtable_iterate();
  test_hashtable_stretch();
  test_make_hashtable();
  return 0;
}
