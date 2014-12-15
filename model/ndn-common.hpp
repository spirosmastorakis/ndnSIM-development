/** -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 *
 * BSD license, See the doc/LICENSE file for more information
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef NDN_COMMON_H
#define NDN_COMMON_H

#include "ns3/nstime.h"
#include "ns3/simulator.h"

#include "ns3/ndn-ns3.hpp"

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

namespace ns3 {
namespace ndn {

using ::ndn::Name;
namespace name = ::ndn::name;

} // namespace ndn
} // namespace ns3


#endif // NDN_COMMON_H