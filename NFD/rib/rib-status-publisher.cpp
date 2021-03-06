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

#include "ns3/ndnSIM/NFD/rib/rib-status-publisher.hpp"
#include "ns3/ndnSIM/NFD/rib/rib.hpp"
#include "ns3/ndnSIM/NFD/core/logger.hpp"

#include "ns3/ndnSIM/model/ndn-face.h"

#include <ndn-cxx/management/nfd-rib-entry.hpp>

using ns3::ndn::Face;

namespace nfd {
namespace rib {

NFD_LOG_INIT("RibStatusPublisher");

RibStatusPublisher::RibStatusPublisher(const Rib& rib,
                                       ndn::Face& face,
                                       const Name& prefix,
                                       ndn::KeyChain& keyChain)
  : SegmentPublisher<ndn::Face>(face, prefix, keyChain)
  , m_rib(rib)
{
}

RibStatusPublisher::~RibStatusPublisher()
{
}

size_t
RibStatusPublisher::generate(ndn::EncodingBuffer& outBuffer)
{
  size_t totalLength = 0;

  for (Rib::const_iterator ribIt = m_rib.begin(); ribIt != m_rib.end(); ++ribIt)
    {
      RibEntry& entry = *ribIt->second;

      const Name& prefix = entry.getName();
      size_t ribEntryLength = 0;

      ndn::nfd::RibEntry tlvEntry;
      const RibEntry::FaceList& faces = entry.getFaces();

      for (RibEntry::FaceList::const_iterator faceIt = faces.begin();
           faceIt != faces.end(); ++faceIt)
        {
          const FaceEntry& face = *faceIt;

          ndn::nfd::Route route;
          route
            .setFaceId(face.faceId)
            .setOrigin(face.origin)
            .setCost(face.cost)
            .setFlags(face.flags);
          if (face.expires < time::steady_clock::TimePoint::max()) {
            route.setExpirationPeriod(time::duration_cast<time::milliseconds>
                                      (face.expires - time::steady_clock::now()));
          }
          tlvEntry.addRoute(route);
        }

      tlvEntry.setName(prefix);
      ribEntryLength += tlvEntry.wireEncode(outBuffer);

      NFD_LOG_DEBUG("generate: rib entry length = " << ribEntryLength);

      totalLength += ribEntryLength;
    }
  NFD_LOG_DEBUG("generate: Total length = " << totalLength);
  return totalLength;
}


} // namespace rib
} // namespace nfd
