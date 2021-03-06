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

#ifndef NFD_DAEMON_MGMT_FACE_STATUS_PUBLISHER_HPP
#define NFD_DAEMON_MGMT_FACE_STATUS_PUBLISHER_HPP

#include "ns3/ndnSIM/NFD/core/segment-publisher.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/app-face.hpp"
#include "ns3/ndnSIM/model/ndn-face.h"

namespace nfd {

class FaceTable;

class FaceStatusPublisher : public SegmentPublisher<AppFace>
{
public:
  FaceStatusPublisher(const FaceTable& faceTable,
                      AppFace& face,
                      const Name& prefix,
                      ndn::KeyChain& keyChain);

  virtual
  ~FaceStatusPublisher();

protected:

  virtual size_t
  generate(ndn::EncodingBuffer& outBuffer);

private:
  const FaceTable& m_faceTable;
};

} // namespace nfd

#endif // NFD_DAEMON_MGMT_FACE_STATUS_PUBLISHER_HPP
