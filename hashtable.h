
#pragma once
#include <stddef.h>
#include <stdbool.h>

typedef size_t (*hashtable_hash_function)(void*, void*);
typedef bool (*hashtable_compare_function)(void*, void*, void*);

typedef struct hashtable_class {
  hashtable_hash_function hashfunction;
  hashtable_compare_function comparefunction;
  void *hashfunctionarg;
  void *comparefunctionarg;
} hashtable_class;

typedef struct hashtable_entry {
  void *key;
  void *value;
  bool reserved;
} hashtable_entry;

typedef struct hashtable {
  hashtable_class clss;
  hashtable_entry *entries;
  size_t length;
} hashtable;

typedef struct hashtable_iterator {
  size_t index;
} hashtable_iterator;

#define MAKE_LOCAL_HASHTABLE(varname, length, clss)\
hashtable_entry __ ## varname ## _sequence[(length)] = {};\
hashtable __ ## varname = { { clss->hashfunction, clss->comparefunction }, __ ## varname ## _sequence, (length) };\
hashtable *varname = &__ ## varname;

extern const hashtable_class __DEFAULT_HASHTABLE_CLASS;
extern const hashtable_class *DEFAULT_HASHTABLE_CLASS;

extern void hashtable_class_init (hashtable_hash_function, void*, hashtable_compare_function, void*, hashtable_class*);
extern void hashtable_entry_init (void*, void*, bool, hashtable_entry*);
extern void hashtable_init (hashtable_entry*, size_t, const hashtable_class*, hashtable*);
extern hashtable *make_hashtable (size_t, const hashtable_class*);
extern void free_hashtable (hashtable*);
extern void hashtable_clear (hashtable*);
extern int hashtable_set (void*, void*, hashtable*);
extern int hashtable_unset (void*, hashtable*);
extern int hashtable_get (void*, hashtable*, void**);
extern size_t hashtable_count (hashtable*);
extern int hashtable_stretch (hashtable_entry*, size_t, hashtable*);
extern hashtable_iterator hashtable_iterate (hashtable*);
extern bool hashtable_iterator_has_next (hashtable_iterator*, hashtable*);
extern int hashtable_iterator_next (hashtable_iterator*, hashtable*, hashtable_entry*);
extern int hashtable_iterator_next_key (hashtable_iterator*, hashtable*, void**);
extern int hashtable_iterator_next_value (hashtable_iterator*, hashtable*, void**);
