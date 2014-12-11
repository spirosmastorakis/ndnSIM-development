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
 *
 */

#include "ndn-face.h"

#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/assert.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "ns3/boolean.h"
#include "ns3/simulator.h"
#include "ns3/random-variable.h"
#include "ns3/pointer.h"
#include "ns3/ptr.h"
#include "ns3/ndn-interest.h"
#include "ns3/ndn-data.h"

#include "ns3/ndn-fw-hop-count-tag.h"

#include <boost/ref.hpp>

NS_LOG_COMPONENT_DEFINE ("ndn.Face");

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED (Face);

TypeId
Face::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ndn::Face")
    .SetParent<Object> ()
    .SetGroupName ("Ndn")
    .AddAttribute ("Id", "Face id (unique integer for the Ndn stack on this node)",
                   TypeId::ATTR_GET, // allow only getting it.
                   UintegerValue (0),
                   MakeUintegerAccessor (&Face::m_id),
                   MakeUintegerChecker<uint32_t> ())
    ;
  return tid;
}

void
Face::close ()
{
}

/**
 * By default, Ndn face are created in the "down" state
 *  with no IP addresses.  Before becoming useable, the user must
 * invoke SetUp on them once an Ndn address and mask have been set.
 */
Face::Face (Ptr<Node> node)
  : m_idNfd(nfd::INVALID_FACEID)
  , m_remoteUri("ns3://face")
  , m_localUri("ns3://face")
  , m_node (node)
  , m_ifup (false)
  , m_id ((uint32_t)-1)
  , m_metric (0)
  , m_flags (0)
    //, ::nfd::Face (m_remoteUri, m_localUri, false)
{
  NS_LOG_FUNCTION (this << node);

  NS_ASSERT_MSG (node != 0, "node cannot be NULL. Check the code");
}
/*
Face::Face (nfd::FaceUri remoteUri, nfd::FaceUri localUri, bool isLocal)
  : m_node (0) // TO DO: Do we need to know the node number?
  , m_upstreamInterestHandler (MakeNullCallback< void, Ptr<Face>, ::ndn::shared_ptr< ::ndn::Interest> > ())
  , m_upstreamDataHandler (MakeNullCallback< void, Ptr<Face>, ::ndn::shared_ptr< ::ndn::Data> > ())
  , m_ifup (false)
  , m_id ((uint32_t)-1)
  , m_metric (0)
  , m_flags (0)
  , m_idNfd (INVALID_FACEID)
  , m_remoteUri(0)
  , m_localUri(0)
  , m_isLocal(isLocal)
{
  onReceiveInterest += [this](const ::ndn::Interest&) { ++m_counters.getNInInterests(); };
  onReceiveData     += [this](const ::ndn::Data&) {     ++m_counters.getNInDatas(); };
  onSendInterest    += [this](const ::ndn::Interest&) { ++m_counters.getNOutInterests(); };
  onSendData        += [this](const ::ndn::Data&) {     ++m_counters.getNOutDatas(); };
}

Face::Face (const Face &)
{
  onReceiveInterest += [this](const ::ndn::Interest&) { ++m_counters.getNInInterests(); };
  onReceiveData     += [this](const ::ndn::Data&) {     ++m_counters.getNInDatas(); };
  onSendInterest    += [this](const ::ndn::Interest&) { ++m_counters.getNOutInterests(); };
  onSendData        += [this](const ::ndn::Data&) {     ++m_counters.getNOutDatas(); };
}

*/
Face::~Face ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

Face& Face::operator= (const Face &)
{
  return *this;
}

Ptr<Node>
Face::GetNode () const
{
  return m_node;
}

void
Face::sendInterest (const ::ndn::Interest& interest)
{
  NS_LOG_FUNCTION (this << boost::cref (*this) << interest.getName ());

  // if (!IsUp ())
  //   {
  //     return false;
  //   }
  // I assume that this should work...

  const Interest i = static_cast<const Interest&>(interest);
  Ptr<Packet> packet = Create <Packet> ();

  FwHopCountTag hopCount;
  i.getPacket ()->RemovePacketTag (hopCount);
  packet->AddPacketTag (hopCount);
  Block block = interest.wireEncode ();
  Convert::ToPacket (make_shared <Block> (block), packet);
  Send (packet);
}

void
Face::sendData (const ::ndn::Data& data)
{
  NS_LOG_FUNCTION (this << data);

  // if (!IsUp ())
  //   {
  //     return false;
  //   }
  // I assume that this should work..

  const Data& d = static_cast<const Data&>(data);

  Ptr<Packet> packet = Create <Packet> ();

  FwHopCountTag hopCount;
  bool tagExists = d.getPacket ()->PeekPacketTag (hopCount);
  if (tagExists)
    packet->AddPacketTag (hopCount);
  Block block = data.wireEncode ();
  Convert::ToPacket (make_shared <Block> (block), packet);
  Send (packet);
}

bool
Face::Send (Ptr<Packet> packet)
{
  FwHopCountTag hopCount;
  bool tagExists = packet->RemovePacketTag (hopCount);
  if (tagExists)
    {
      hopCount.Increment ();
      packet->AddPacketTag (hopCount);
    }

  return true;
}

bool
Face::Receive (Ptr<const Packet> p)
{
  NS_LOG_FUNCTION (this << p << p->GetSize ());

  if (!IsUp ())
    {
      // no tracing here. If we were off while receiving, we shouldn't even know that something was there
      return false;
    }
  Ptr<Packet> packet = p->Copy (); // give upper layers a rw copy of the packet
  try
    {
      //Let's see..
      uint32_t hopTag = 0;
      Block block = Convert::FromPacket (packet, hopTag);
      decodeAndDispatchInput(block, hopTag);
    }
  catch (::ndn::UnknownHeaderException)
    {
      NS_FATAL_ERROR ("Unknown NDN header. Should not happen");
      return false;
    }

  return false;
}

void
Face::SetMetric (uint16_t metric)
{
  NS_LOG_FUNCTION (metric);
  m_metric = metric;
}

uint16_t
Face::GetMetric (void) const
{
  return m_metric;
}

void
Face::SetFlags (uint32_t flags)
{
  m_flags = flags;
}

bool
Face::operator== (const Face &face) const
{
  NS_ASSERT_MSG (m_node->GetId () == face.m_node->GetId (),
                 "Faces of different nodes should not be compared to each other: " << *this << " == " << face);

  return (m_id == face.m_id);
}

bool
Face::operator< (const Face &face) const
{
  NS_ASSERT_MSG (m_node->GetId () == face.m_node->GetId (),
                 "Faces of different nodes should not be compared to each other: " << *this << " == " << face);

  return (m_id < face.m_id);
}

std::ostream&
Face::Print (std::ostream &os) const
{
  os << "id=" << GetId ();
  return os;
}

std::ostream&
operator<< (std::ostream& os, const Face &face)
{
  face.Print (os);
  return os;
}

} // namespace ndn
} // namespace ns3
