/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011-2014 University of California, Los Angeles
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * See AUTHORS file for the list of authors.
 */

#ifndef NDN_COMMON_H
#define NDN_COMMON_H

#include "ns3/nstime.h"
#include "ns3/simulator.h"

#include "ns3/ndn-ns3.h"

#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/encoding/block.hpp>
#include <ndn-cxx/signature.hpp>
#include <ndn-cxx/signature-info.hpp>
#include <ndn-cxx/name.hpp>
#include <ndn-cxx/data.hpp>
#include <ndn-cxx/security/key-chain.hpp>

using std::shared_ptr;
using std::make_shared;

using ::ndn::Name;
using ::ndn::Interest;
using ::ndn::Data;
using ::ndn::KeyLocator;
using ::ndn::Signature;
using ::ndn::SignatureInfo;
using ::ndn::Convert;
using ::ndn::Block;
using ::ndn::KeyChain;

#define NDNSIM_MODE 1

#define NDN_NAMESPACE_BEGIN  namespace ns3 { namespace ndn {
#define NDN_NAMESPACE_END    } /*ndn*/ } /*ns3*/

/**
 * @brief NS-3 namespace
 */
namespace ns3 {

/**
 * @brief ndnSIM namespace
 */
namespace ndn {
}

}

NDN_NAMESPACE_BEGIN

typedef Time TimeInterval;

namespace time
{

inline Time
NowUnixTimestamp ()
{
  return Simulator::Now ();
}

}

NDN_NAMESPACE_END

#endif // NDN_COMMON_H
