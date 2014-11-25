/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
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

#include "ndn-forwarder.h"

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

#include "ns3/ndn-face.h"
#include "ns3/ndn-fib-helper.h"
#include "ns3/ndn-net-device-face.h"

#include <getopt.h>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

using nfd::NullFace;
using nfd::FaceUri;
using nfd::TablesConfigSection;
using nfd::ControlParameters;

NS_LOG_COMPONENT_DEFINE ("ndn.L3Protocol");

namespace ns3 {
namespace ndn {

const uint16_t L3Protocol::ETHERNET_FRAME_TYPE = 0x7777;
const uint16_t L3Protocol::IP_STACK_PORT = 9695;

NS_OBJECT_ENSURE_REGISTERED (L3Protocol);

TypeId
L3Protocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::L3Protocol")
    .SetGroupName ("ndn")
    .SetParent<Object> ()
    .AddConstructor<L3Protocol> ()
    .AddAttribute ("FaceList", "List of faces associated with ndn stack",
                   ObjectVectorValue (),
                   MakeObjectVectorAccessor (&L3Protocol::m_faces),
                   MakeObjectVectorChecker<Face> ())

    .AddTraceSource("OutInterests",  "OutInterests",
                     MakeTraceSourceAccessor(&L3Protocol::m_outInterests))
    .AddTraceSource("InInterests",   "InInterests",
                    MakeTraceSourceAccessor(&L3Protocol::m_inInterests))

    ////////////////////////////////////////////////////////////////////

    .AddTraceSource ("OutData",  "OutData",
                      MakeTraceSourceAccessor(&L3Protocol::m_outData))
    .AddTraceSource ("InData",   "InData",
                      MakeTraceSourceAccessor(&L3Protocol::m_inData))
  ;
  return tid;
}

L3Protocol::L3Protocol ()
  : m_faceCounter (0)
{
  NS_LOG_FUNCTION (this);
}

L3Protocol::~L3Protocol ()
{
  NS_LOG_FUNCTION (this);
}

void
L3Protocol::initialize()
{
  m_forwarder = make_shared<Forwarder>();

  initializeManagement();

  m_forwarder->getFaceTable().addReserved(make_shared<NullFace>(), ns3::ndn::FACEID_NULL);
  m_forwarder->getFaceTable().addReserved(make_shared<NullFace>(FaceUri("contentstore://")), ns3::ndn::FACEID_CONTENT_STORE);

  nfd::PrivilegeHelper::drop();
}

void
L3Protocol::initializeManagement()
{
  m_internalFace = make_shared<InternalFace>();

  m_fibManager = make_shared<FibManager>(ref(m_forwarder->getFib()),
                                         bind(&Forwarder::getFace, m_forwarder.get(), _1),
                                         m_internalFace,
                                         ref(m_keyChain));

  m_faceManager = make_shared<FaceManager>(ref(m_forwarder->getFaceTable()),
                                           m_internalFace,
                                           ref(m_keyChain));

  m_strategyChoiceManager = make_shared<StrategyChoiceManager>(ref(m_forwarder->getStrategyChoice()),
                                                               m_internalFace,
                                                               ref(m_keyChain));

  m_statusServer = make_shared<StatusServer>(m_internalFace,
                                             ref(*m_forwarder),
                                             ref(m_keyChain));

  TablesConfigSection tablesConfig(m_forwarder->getCs(),
                                   m_forwarder->getPit(),
                                   m_forwarder->getFib(),
                                   m_forwarder->getStrategyChoice(),
                                   m_forwarder->getMeasurements());

  // Alex do we need this?
  m_forwarder->getFaceTable().addReserved(m_internalFace, ns3::ndn::FACEID_INTERNAL_FACE);

  tablesConfig.ensureTablesAreConfigured();

  // add FIB entry for NFD Management Protocol
  // shared_ptr<::nfd::fib::Entry> entry = m_forwarder->getFib().insert("/localhost/nfd").first;
  // entry->addNextHop(m_internalFace, 0);
  ControlParameters parameters;
  parameters.setName("/localhost/nfd");
  parameters.setFaceId(m_internalFace->getId ());
  parameters.setCost (0);

  FibHelper fibHelper;
  Ptr<Node> node = GetObject<Node> ();
  fibHelper.AddNextHop(parameters, node);
}

shared_ptr<FibManager>
L3Protocol::GetFibManager ()
{
  return m_fibManager;
}

void
L3Protocol::SetFibManager (shared_ptr<FibManager> fibManager)
{
  m_fibManager = fibManager;
}

shared_ptr<Forwarder>
L3Protocol::GetForwarder ()
{
  return m_forwarder;
}

void
L3Protocol::SetForwarder (shared_ptr<Forwarder> forwarder)
{
  m_forwarder = forwarder;
}

shared_ptr<StrategyChoiceManager>
L3Protocol::GetStrategyChoiceManager ()
{
  return m_strategyChoiceManager;
}

void
L3Protocol::SetStrategyChoiceManager (shared_ptr<StrategyChoiceManager> strategyChoiceManager)
{
  m_strategyChoiceManager = strategyChoiceManager;
}

/*
 * This method is called by AddAgregate and completes the aggregation
 * by setting the node in the ndn stack
 */
void
L3Protocol::NotifyNewAggregate ()
{
  // not really efficient, but this will work only once
  if (m_node == 0)
    {
      m_node = GetObject<Node> ();
      if (m_node != 0)
        {
          //NS_ASSERT_MSG (m_forwardingStrategy != 0,
          //            "Forwarding strategy should be aggregated before L3Protocol");
        }
    }/*
  if (m_forwardingStrategy == 0)
    {
      m_forwardingStrategy = GetObject<ForwardingStrategy> ();
    }
     */
  Object::NotifyNewAggregate ();
}

void
L3Protocol::DoDispose (void)
{
  NS_LOG_FUNCTION (this);

  for (FaceList::iterator i = m_faces.begin (); i != m_faces.end (); ++i)
  {
    *i = 0;
  }
  m_faces.clear ();
  m_node = 0;

  // Force delete on objects
  // m_forwardingStrategy = 0; // there is a reference to PIT stored in here

  Object::DoDispose ();
}

static void
faceNullDeleter(const Ptr<Face>& face)
{
}

uint32_t
L3Protocol::AddFace (const Ptr<Face>& face)
{
  NS_LOG_FUNCTION (this << &face);

  face->SetId (m_faceCounter); // sets a unique ID of the face. This ID serves only informational purposes

  shared_ptr<Face> sharedFace = shared_ptr<Face>(GetPointer(face),
                                                 bind(&faceNullDeleter, face));

  m_forwarder->addFace (sharedFace);

  face->RegisterProtocolHandlers ();

  m_faces.push_back (face);
  m_faceCounter++;

  face->onReceiveInterest += [this, face] (const Interest& interest) {
    this->m_inInterests(interest, *face);
  };

  face->onSendInterest += [this, face] (const Interest& interest) {
    this->m_outInterests(interest, *face);
  };

  face->onReceiveData += [this, face] (const Data& data) {
    this->m_inData(data, *face);
  };

  face->onSendData += [this, face] (const Data& data) {
    this->m_outData(data, *face);
  };
  //face->onSendInterest += bind(m_outInterests, _1, ref(*face));
  //face->onReceiveData += bind(m_inData, _1, ref(*face));
  //face->onSendData += bind(m_outData, _1, ref(*face));

  return face->GetId ();
}

void
L3Protocol::RemoveFace (Ptr<Face> face)
{
  NS_LOG_FUNCTION (this << std::cref (*face));

  face->UnRegisterProtocolHandlers ();

  // Just call the fail method. This should do the work for us and remove face from FIB and PIT
  face->fail ("Remove Face");

  FaceList::iterator face_it = find (m_faces.begin(), m_faces.end(), face);
  if (face_it == m_faces.end ())
    {
      return;
    }
  m_faces.erase (face_it);
}

Ptr<Face>
L3Protocol::GetFace (uint32_t index) const
{
  NS_ASSERT (0 <= index && index < m_faces.size ());
  return m_faces[index];
}

Ptr<Face>
L3Protocol::GetFaceById (uint32_t index) const
{
  BOOST_FOREACH (const Ptr<Face> &face, m_faces) // this function is not supposed to be called often, so linear search is fine
    {
      if (face->GetId () == index)
        return face;
    }
  return 0;
}

Ptr<Face>
L3Protocol::GetFaceByNetDevice (Ptr<NetDevice> netDevice) const
{
  BOOST_FOREACH (const Ptr<Face> &face, m_faces) // this function is not supposed to be called often, so linear search is fine
    {
      Ptr<NetDeviceFace> netDeviceFace = DynamicCast<NetDeviceFace> (face);
      if (netDeviceFace == 0) continue;

      if (netDeviceFace->GetNetDevice () == netDevice)
        return face;
    }
  return 0;
}

uint32_t
L3Protocol::GetNFaces (void) const
{
  return m_faces.size ();
}

} //namespace ndn
} //namespace ns3
