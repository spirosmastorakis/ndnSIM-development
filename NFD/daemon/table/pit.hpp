/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2014,  Regents of the University of California,
 *                      Arizona Board of Regents,
 *                      Colorado State University,
 *                      University Pierre & Marie Curie, Sorbonne University,
 *                      Washington University in St. Louis,
 *                      Beijing Institute of Technology,
 *                      The University of Memphis
 *
 * This file is part of NFD (Named Data Networking Forwarding Daemon).
 * See AUTHORS.md for complete list of NFD authors and contributors.
 *
 * NFD is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * NFD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * NFD, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NFD_DAEMON_TABLE_PIT_HPP
#define NFD_DAEMON_TABLE_PIT_HPP

#include "name-tree.hpp"
#include "pit-entry.hpp"

namespace nfd {
namespace pit {

/** \class DataMatchResult
 *  \brief an unordered iterable of all PIT entries matching Data
 *
 *  This type shall support:
 *    iterator<shared_ptr<pit::Entry>> begin()
 *    iterator<shared_ptr<pit::Entry>> end()
 */
typedef std::vector<shared_ptr<pit::Entry>> DataMatchResult;

} // namespace pit

/** \brief represents the Interest Table
 */
class Pit : noncopyable
{
public:
  explicit
  Pit(NameTree& nameTree);

  ~Pit();

  /** \return number of entries
   */
  size_t
  size() const;

  /** \brief inserts a PIT entry for Interest
   *
   *  If an entry for exact same name and selectors exists, that entry is returned.
   *  \return the entry, and true for new entry, false for existing entry
   */
  std::pair<shared_ptr<pit::Entry>, bool>
  insert(const Interest& interest);

  /** \brief performs a Data match
   *  \return an iterable of all PIT entries matching data
   */
  pit::DataMatchResult
  findAllDataMatches(const Data& data) const;

  /**
   *  \brief erases a PIT Entry
   */
  void
  erase(shared_ptr<pit::Entry> pitEntry);

public: // enumeration
  class const_iterator;

  const_iterator
  begin() const;

  const_iterator
  end() const;

  class const_iterator : public std::iterator<std::forward_iterator_tag, const pit::Entry>
  {
  public:
    const_iterator();

    explicit
    const_iterator(const NameTree::const_iterator& it);

    ~const_iterator();

    const pit::Entry&
    operator*() const;

    shared_ptr<pit::Entry>
    operator->() const;

    const_iterator&
    operator++();

    const_iterator
    operator++(int);

    bool
    operator==(const const_iterator& other) const;

    bool
    operator!=(const const_iterator& other) const;

  private:
    NameTree::const_iterator m_nameTreeIterator;
    // use index, so it is not necessary to dereference m_nameTreeIterato, which could be invalid
    size_t m_nLastPitEntry;
  };

private:
  NameTree& m_nameTree;
  size_t m_nItems;
};

inline size_t
Pit::size() const
{
  return m_nItems;
}

inline Pit::const_iterator
Pit::end() const
{
  return const_iterator(m_nameTree.end());
}

inline
Pit::const_iterator::const_iterator()
  : m_nLastPitEntry(0)
{
}

inline
Pit::const_iterator::const_iterator(const NameTree::const_iterator& it)
  : m_nameTreeIterator(it)
  , m_nLastPitEntry(0)
{
}

inline
Pit::const_iterator::~const_iterator()
{
}

inline
Pit::const_iterator
Pit::const_iterator::operator++(int)
{
  Pit::const_iterator temp(*this);
  ++(*this);
  return temp;
}

inline Pit::const_iterator&
Pit::const_iterator::operator++()
{
  ++m_nLastPitEntry;
  if (m_nLastPitEntry < m_nameTreeIterator->getPitEntries().size()) {
    return *this;
  }

  ++m_nameTreeIterator;
  m_nLastPitEntry = 0;
  return *this;
}

inline const pit::Entry&
Pit::const_iterator::operator*() const
{
  return *(m_nameTreeIterator->getPitEntries()[m_nLastPitEntry]);
}

inline shared_ptr<pit::Entry>
Pit::const_iterator::operator->() const
{
  return m_nameTreeIterator->getPitEntries()[m_nLastPitEntry];
}

inline bool
Pit::const_iterator::operator==(const Pit::const_iterator& other) const
{
  return m_nameTreeIterator == other.m_nameTreeIterator &&
         m_nLastPitEntry == other.m_nLastPitEntry;
}

inline bool
Pit::const_iterator::operator!=(const Pit::const_iterator& other) const
{
  return !(*this == other);
}

} // namespace nfd

#endif // NFD_DAEMON_TABLE_PIT_HPP
