/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2014  Regents of the University of California,
 *                     Arizona Board of Regents,
 *                     Colorado State University,
 *                     University Pierre & Marie Curie, Sorbonne University,
 *                     Washington University in St. Louis,
 *                     Beijing Institute of Technology
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
 **/

#include "ns3/pit-face-record.hpp"

namespace nfd {
namespace pit {

FaceRecord::FaceRecord(shared_ptr<ns3::ndn::Face> face)
  : m_face(face)
  , m_lastNonce(0)
  , m_lastRenewed(time::steady_clock::TimePoint::min())
  , m_expiry(time::steady_clock::TimePoint::min())
{
}

void
FaceRecord::update(const Interest& interest)
{
  m_lastNonce = interest.getNonce();
  m_lastRenewed = time::steady_clock::now();

  static const time::milliseconds DEFAULT_INTEREST_LIFETIME = time::milliseconds(4000);
  time::milliseconds lifetime = interest.getInterestLifetime();
  if (lifetime < time::milliseconds::zero()) {
    lifetime = DEFAULT_INTEREST_LIFETIME;
  }
  m_expiry = m_lastRenewed + time::milliseconds(lifetime);
}


} // namespace pit
} // namespace nfd
