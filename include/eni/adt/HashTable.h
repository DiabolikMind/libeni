/*==- adt/HashTable.h ------------------------------------------------------==

  This file is part of the Lity project distributed under GPL-3.0.
  See LICENSE for details.

==------------------------------------------------------------------------==*/
#ifndef ENI_ADT_HASH_TABLE
#define ENI_ADT_HASH_TABLE

#include <adt/HashBase.h>
#include <adt/HashIterator.h>
#include <support/MallocAllocator.h>

namespace eni {

template <class KeyType, class ValueType, typename HasherType,
          template<class, class> class EntryType = internal::Entry,
          template<class> class AllocType = MallocAllocator> class HashTable;

/** \class HashTable
 *  \brief HashTable is a hash table which follows boost::unordered_map, but it
 *  is open addressing and can linear probing.
 *
 *  HashTable is a linear probing hash table.
 */
template <class KeyType, class ValueType, typename HasherType,
          template<class, class> class EntryType,
          template<class> class AllocType>
class HashTable : public internal::TableImpl<KeyType,
                                             ValueType,
                                             HasherType,
                                             EntryType>
{
protected:
  typedef internal::TableImpl<KeyType, ValueType, HasherType, EntryType> super;

  typedef AllocType<typename super::entry_type> allocator_type;

public:
  typedef size_t size_type;
  typedef typename super::key_type    key_type;
  typedef typename super::value_type  value_type;
  typedef typename super::hasher_type hasher_type;
  typedef typename super::entry_type  entry_type;
  typedef typename super::bucket_type bucket_type;

  typedef HashIterator<ChainIterBase, const super> const_chain_iterator;
  typedef HashIterator<ChainIterBase, super>       chain_iterator;

  typedef HashIterator<EntryIterBase, const super> const_entry_iterator;
  typedef HashIterator<EntryIterBase, super>       entry_iterator;

  typedef entry_iterator       iterator;
  typedef const_entry_iterator const_iterator;

public:
  /**
   * constructor
   * @param pSize    initial number of the buckets
   */
  explicit HashTable(size_type pSize=3);

  HashTable(const HashTable& pCopy);

  HashTable(HashTable&& pOther);

  virtual ~HashTable();

  HashTable& operator=(const HashTable& pCopy);

  HashTable& operator=(HashTable&& pOther);

  void clear();

  /// insert - insert a new element to the container. The element is
  /// constructed in-place, i.e. no copy or move operations are performed.
  /// If the element already exists, return the element, and set pExist true.
  entry_type* insert(const key_type& pKey, bool& pExist);

  /// remove one element with the key.
  /// A hash table may contain multiple element with the same key.
  /// This function remove one element only.
  /// @retval false There are no element with the key.
  bool erase(const key_type& pKey);

  /// find - finds an element with key pKey
  /// If the element does not exist, return end()
  iterator find(const key_type& pKey);

  /// find - finds an element with key pKey, constant version
  /// If the element does not exist, return end()
  const_iterator find(const key_type& pKey) const;

  size_type count(const key_type& pKey) const;

  float load_factor() const;

  /// rehash - if the load factor is larger than 75%, or the empty buckets is
  /// less than 12.5%, the rehash the hash table
  void rehash();

  /// rehash - immediately re-new the hash table to the size pCount, and
  /// rehash all elements.
  void rehash(size_type pCount);

  iterator begin();
  iterator end();

  const_iterator begin() const;
  const_iterator end() const;

  chain_iterator begin(const key_type& pKey);
  chain_iterator end(const key_type& pKey);

  const_chain_iterator begin(const key_type& pKey) const;
  const_chain_iterator end(const key_type& pKey) const;

  using super::empty;

  using super::numOfBuckets;

  using super::numOfEntries;

  using super::numOfTombstones;

  using super::EmptyBucket;

  using super::Tombstone;

protected:
  allocator_type m_EntryAlloc;
};

#include "bits/HashTable.tcc"

} // namespace of eni

#endif
