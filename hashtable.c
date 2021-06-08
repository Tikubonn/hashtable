
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hashtable.h"

static size_t default_hash_function (void *value, void *arg){
  return (size_t)value;
}

static bool default_compare_function (void *value1, void *value2, void *arg){
  return value1 == value2;
}

const hashtable_class __DEFAULT_HASHTABLE_CLASS = {
  default_hash_function,
  default_compare_function,
  NULL,
  NULL,
};

const hashtable_class *DEFAULT_HASHTABLE_CLASS = &__DEFAULT_HASHTABLE_CLASS;

void hashtable_class_init (hashtable_hash_function hashfunction, void *hashfunctionarg, hashtable_compare_function comparefunction, void *comparefunctionarg, hashtable_class *clss){
  clss->hashfunction = hashfunction;
  clss->comparefunction = comparefunction;
  clss->hashfunctionarg = hashfunctionarg;
  clss->comparefunctionarg = comparefunctionarg;
}

void hashtable_entry_init (void *key, void *value, bool reserved, hashtable_entry *entry){
  entry->key = key;
  entry->value = value;
  entry->reserved = reserved;
}

static void clear_hashtable_entries (hashtable_entry *entries, size_t length){
  for (size_t index = 0; index < length; index++){
    entries[index].reserved = false;
  }
}

void hashtable_init (hashtable_entry *entries, size_t length, const hashtable_class *clss, hashtable *htable){
  clear_hashtable_entries(entries, length);
  htable->clss = *clss;
  htable->entries = entries;
  htable->length = length;
}

hashtable *make_hashtable (size_t length, const hashtable_class *clss){
  hashtable *htable = malloc(sizeof(hashtable));
  if (htable == NULL){
    return NULL;
  }
  hashtable_entry *htableentries = malloc(sizeof(hashtable_entry) * length);
  if (htableentries == NULL){
    free(htable);
    return NULL;
  }
  hashtable_init(htableentries, length, clss, htable);
  return htable;
}

void free_hashtable (hashtable *htable){
  free(htable->entries);
  free(htable);
}

void hashtable_clear (hashtable *htable){
  for (size_t index = 0; index < htable->length; index++){
    htable->entries[index].reserved = false;
  }
}

int hashtable_set (void *value, void *key, hashtable *htable){
  if (0 < htable->length){
    size_t hash = htable->clss.hashfunction(key, htable->clss.hashfunctionarg);
    for (size_t index = 0; index < htable->length; index++){
      size_t trueindex = (index + hash) % htable->length;
      if (htable->entries[trueindex].reserved){
        if (htable->clss.comparefunction(htable->entries[trueindex].key, key, htable->clss.comparefunctionarg)){
          htable->entries[trueindex].value = value;
          return 0;
        }
      }
      else {
        htable->entries[trueindex].key = key;
        htable->entries[trueindex].value = value;
        htable->entries[trueindex].reserved = true;
        return 0;
      }
    }
    return 1;
  }
  else{
    return 1;
  }
}

int hashtable_unset (void *key, hashtable *htable){
  if (0 < htable->length){
    size_t hash = htable->clss.hashfunction(key, htable->clss.hashfunctionarg);
    for (size_t index = 0; index < htable->length; index++){
      size_t trueindex = (index + hash) % htable->length;
      if (htable->entries[trueindex].reserved){
        if (htable->clss.comparefunction(htable->entries[trueindex].key, key, htable->clss.comparefunctionarg)){
          htable->entries[trueindex].reserved = false;
          return 0;
        }
      }
      else {
        return 1;
      }
    }
    return 1;
  }
  else{
    return 1;
  }
}

int hashtable_get (void *key, hashtable *htable, void **valuep){
  if (0 < htable->length){
    size_t hash = htable->clss.hashfunction(key, htable->clss.hashfunctionarg);
    for (size_t index = 0; index < htable->length; index++){
      size_t trueindex = (index + hash) % htable->length;
      if (htable->entries[trueindex].reserved){
        if (htable->clss.comparefunction(htable->entries[trueindex].key, key, htable->clss.comparefunctionarg)){
          *valuep = htable->entries[trueindex].value;
          return 0;
        }
      }
      else {
        return 1;
      }
    }
    return 1;
  }
  else {
    return 1;
  }
}

size_t hashtable_count (hashtable *htable){
  size_t count = 0;
  for (size_t index = 0; index < htable->length; index++){
    if (htable->entries[index].reserved){
      count += 1;
    }
  }
  return count;
}

int hashtable_stretch (hashtable_entry *newentries, size_t newlength, hashtable *htable){
  hashtable newhtable;
  hashtable_init(newentries, newlength, &(htable->clss), &newhtable);
  for (size_t index = 0; index < htable->length; index++){
    if (htable->entries[index].reserved){
      if (hashtable_set(htable->entries[index].value, htable->entries[index].key, &newhtable) != 0){
        return 1;
      }
    }
  }
  *htable = newhtable;
  return 0;
}

hashtable_iterator hashtable_iterate (hashtable *htable){
  hashtable_iterator newiterator = { 0 };
  return newiterator;
}

bool hashtable_iterator_has_next (hashtable_iterator *iterator, hashtable *htable){
  for (size_t index = iterator->index; index < htable->length; index++){
    if (htable->entries[index].reserved){
      return true;
    }
  }
  return false;
}

int hashtable_iterator_next (hashtable_iterator *iterator, hashtable *htable, hashtable_entry *entryp){
  for (size_t index = iterator->index; index < htable->length; index++){
    if (htable->entries[index].reserved){
      iterator->index = index +1;
      *entryp = htable->entries[index];
      return 0;
    }
  }
  return 1;
}

int hashtable_iterator_next_key (hashtable_iterator *iterator, hashtable *htable, void **keyp){
  hashtable_entry entry;
  if (hashtable_iterator_next(iterator, htable, &entry)){
    return 1;
  }
  *keyp = entry.key;
  return 0;
}

int hashtable_iterator_next_value (hashtable_iterator *iterator, hashtable *htable, void **valuep){
  hashtable_entry entry;
  if (hashtable_iterator_next(iterator, htable, &entry)){
    return 1;
  }
  *valuep = entry.value;
  return 0;
}
