/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:ni -*- */
/*
 * Copyright (c) 2011 University of California, Los Angeles
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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 *         Ilya Moiseenko <iliamo@cs.ucla.edu>
 */

#include "ndn-l3-protocol.hpp"

#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/log.h"
#include "ns3/callback.h"
#include "ns3/uinteger.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/object-vector.h"
#include "ns3/pointer.h"
#include "ns3/simulator.h"
#include "ns3/random-variable.h"

#include "ndn-face.hpp"

#include "ndn-net-device-face.hpp"
#include "../helper/ndn-stack-helper.hpp"

#include <boost/foreach.hpp>

#include "ns3/ndnSIM/NFD/daemon/face/null-face.hpp"
#include "ns3/ndnSIM/NFD/core/config-file.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/general-config-section.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/tables-config-section.hpp"

NS_LOG_COMPONENT_DEFINE("ndn.L3Protocol");

namespace ns3 {
namespace ndn {

using nfd::NullFace;
using nfd::FaceUri;
using nfd::TablesConfigSection;

const uint16_t L3Protocol::ETHERNET_FRAME_TYPE = 0x7777;
const uint16_t L3Protocol::IP_STACK_PORT = 9695;

NS_OBJECT_ENSURE_REGISTERED(L3Protocol);

TypeId
L3Protocol::GetTypeId(void)
{
  static TypeId tid =
    TypeId("ns3::ndn::L3Protocol")
      .SetGroupName("ndn")
      .SetParent<Object>()
      .AddConstructor<L3Protocol>()

      .AddTraceSource("OutInterests", "OutInterests",
                      MakeTraceSourceAccessor(&L3Protocol::m_outInterests))
      .AddTraceSource("InInterests", "InInterests",
                      MakeTraceSourceAccessor(&L3Protocol::m_inInterests))

      ////////////////////////////////////////////////////////////////////

      .AddTraceSource("OutData", "OutData", MakeTraceSourceAccessor(&L3Protocol::m_outData))
      .AddTraceSource("InData", "InData", MakeTraceSourceAccessor(&L3Protocol::m_inData));
  return tid;
}

L3Protocol::L3Protocol()
{
  NS_LOG_FUNCTION(this);
}

L3Protocol::~L3Protocol()
{
  NS_LOG_FUNCTION(this);
}

void
L3Protocol::initialize()
{
  m_forwarder = make_shared<Forwarder>();

  initializeManagement();

  m_forwarder->getFaceTable().addReserved(make_shared<NullFace>(), nfd::FACEID_NULL);
  m_forwarder->getFaceTable().addReserved(make_shared<NullFace>(FaceUri("contentstore://")),
                                          nfd::FACEID_CONTENT_STORE);
}

void
L3Protocol::initializeManagement()
{
  m_internalFace = make_shared<InternalFace>();

  m_fibManager = make_shared<FibManager>(ref(m_forwarder->getFib()),
                                         bind(&Forwarder::getFace, m_forwarder.get(), _1),
                                         m_internalFace, std::ref(StackHelper::getKeyChain()));

  m_faceManager = make_shared<FaceManager>(ref(m_forwarder->getFaceTable()), m_internalFace,
                                           std::ref(StackHelper::getKeyChain()));

  m_strategyChoiceManager =
    make_shared<StrategyChoiceManager>(ref(m_forwarder->getStrategyChoice()), m_internalFace,
                                       std::ref(StackHelper::getKeyChain()));

  m_statusServer =
    make_shared<StatusServer>(m_internalFace, ref(*m_forwarder), std::ref(StackHelper::getKeyChain()));

  TablesConfigSection tablesConfig(m_forwarder->getCs(), m_forwarder->getPit(),
                                   m_forwarder->getFib(), m_forwarder->getStrategyChoice(),
                                   m_forwarder->getMeasurements());

  m_forwarder->getFaceTable().addReserved(m_internalFace, nfd::FACEID_INTERNAL_FACE);

  // add FIB entry for NFD Management Protocol
  shared_ptr<::nfd::fib::Entry> entry = m_forwarder->getFib().insert("/localhost/nfd").first;
  entry->addNextHop(m_internalFace, 0);
}

shared_ptr<FibManager>
L3Protocol::GetFibManager()
{
  return m_fibManager;
}

void
L3Protocol::SetFibManager(shared_ptr<FibManager> fibManager)
{
  m_fibManager = fibManager;
}

shared_ptr<Forwarder>
L3Protocol::GetForwarder()
{
  return m_forwarder;
}

void
L3Protocol::SetForwarder(shared_ptr<Forwarder> forwarder)
{
  m_forwarder = forwarder;
}

shared_ptr<StrategyChoiceManager>
L3Protocol::GetStrategyChoiceManager()
{
  return m_strategyChoiceManager;
}

void
L3Protocol::SetStrategyChoiceManager(shared_ptr<StrategyChoiceManager> strategyChoiceManager)
{
  m_strategyChoiceManager = strategyChoiceManager;
}

/*
 * This method is called by AddAgregate and completes the aggregation
 * by setting the node in the ndn stack
 */
void
L3Protocol::NotifyNewAggregate()
{
  // not really efficient, but this will work only once
  if (m_node == 0) {
    m_node = GetObject<Node>();
    if (m_node != 0) {
    }
  }

  Object::NotifyNewAggregate();
}

void
L3Protocol::DoDispose(void)
{
  NS_LOG_FUNCTION(this);

  m_node = 0;

  Object::DoDispose();
}

nfd::FaceId
L3Protocol::AddFace(shared_ptr<Face> face)
{
  NS_LOG_FUNCTION(this << face.get());

  m_forwarder->addFace(face);

  // Connect Signals to TraceSource
  face->onReceiveInterest +=
    [this, face](const Interest& interest) { this->m_inInterests(interest, *face); };

  face->onSendInterest +=
    [this, face](const Interest& interest) { this->m_outInterests(interest, *face); };

  face->onReceiveData += [this, face](const Data& data) { this->m_inData(data, *face); };

  face->onSendData += [this, face](const Data& data) { this->m_outData(data, *face); };

  return face->getId();
}

// void
// L3Protocol::RemoveFace(shared_ptr<Face> face)
// {
//   NS_LOG_FUNCTION(this << std::cref(*face));
// }

// shared_ptr<Face>
// L3Protocol::GetFace(uint32_t index) const
// {
//   return nullptr;
// }

shared_ptr<Face>
L3Protocol::GetFaceById(nfd::FaceId id) const
{
  return m_forwarder->getFaceTable().get(id);
}

shared_ptr<Face>
L3Protocol::GetFaceByNetDevice(Ptr<NetDevice> netDevice) const
{
  for (const auto& i : m_forwarder->getFaceTable()) {
    shared_ptr<NetDeviceFace> netDeviceFace = std::dynamic_pointer_cast<NetDeviceFace>(i);
    if (netDeviceFace == nullptr)
      continue;

    if (netDeviceFace->GetNetDevice() == netDevice)
      return i;
  }
  return nullptr;
}

// uint32_t
// L3Protocol::GetNFaces(void) const
// {
//   return 0;
// }

} // namespace ndn
} // namespace ns3
