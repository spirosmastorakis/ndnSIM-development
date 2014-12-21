/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2014  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "ndn-net-device-face.hpp"
#include "ndn-l3-protocol.hpp"

#include "ns3/net-device.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/pointer.h"

#include "ns3/address.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/channel.h"

#include <ndn-cxx/name.hpp>

NS_LOG_COMPONENT_DEFINE ("ndn.NetDeviceFace");

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED (NetDeviceFace);

TypeId
NetDeviceFace::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ndn::NetDeviceFace")
    .SetParent<Face> ()
    .SetGroupName ("Ndn")
    ;
  return tid;
}

/**
 * By default, Ndn face are created in the "down" state.  Before
 * becoming useable, the user must invoke SetUp on the face
 */
NetDeviceFace::NetDeviceFace (Ptr<Node> node, const Ptr<NetDevice> &netDevice)
  : Face (node)
  , m_netDevice (netDevice)
{
  NS_LOG_FUNCTION (this << netDevice);

  SetMetric (1); // default metric

  NS_ASSERT_MSG (m_netDevice != 0, "NetDeviceFace needs to be assigned a valid NetDevice");
}

NetDeviceFace::~NetDeviceFace ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

NetDeviceFace& NetDeviceFace::operator= (const NetDeviceFace &)
{
  return *this;
}

void
NetDeviceFace::close ()
{
}

Ptr<NetDevice>
NetDeviceFace::GetNetDevice () const
{
  return m_netDevice;
}

void
NetDeviceFace::RegisterProtocolHandlers ()
{
  NS_LOG_FUNCTION (this);

  m_node->RegisterProtocolHandler (MakeCallback (&NetDeviceFace::ReceiveFromNetDevice, this),
                                   L3Protocol::ETHERNET_FRAME_TYPE, m_netDevice, true/*promiscuous mode*/);
}

void
NetDeviceFace:: UnRegisterProtocolHandlers ()
{
  m_node->UnregisterProtocolHandler (MakeCallback (&NetDeviceFace::ReceiveFromNetDevice, this));
}

bool
NetDeviceFace::Send (Ptr<Packet> packet)
{
  if (!Face::Send (packet))
    {
      return false;
    }
  
  NS_LOG_FUNCTION (this << packet);
  // to please the syntax highlighter
  std::string s = "for Ndn ; fragmentation not supported";
  NS_ASSERT_MSG (packet->GetSize () <= m_netDevice->GetMtu (),
                 "Packet size " << packet->GetSize () << " exceeds device MTU "
                 << m_netDevice->GetMtu ()
                 << s);

  bool ok = m_netDevice->Send (packet, m_netDevice->GetBroadcast (),
                               L3Protocol::ETHERNET_FRAME_TYPE);
  return ok;
}

// callback
void
NetDeviceFace::ReceiveFromNetDevice (Ptr<NetDevice> device,
                                     Ptr<const Packet> p,
                                     uint16_t protocol,
                                     const Address &from,
                                     const Address &to,
                                     NetDevice::PacketType packetType)
{
  NS_LOG_FUNCTION (device << p << protocol << from << to << packetType);
  Receive (p);
}


std::ostream&
NetDeviceFace::Print (std::ostream& os) const
{
#ifdef NS3_LOG_ENABLE
  os << "dev[" << GetNode ()->GetId () << "]=net(" << GetId ();

  if (DynamicCast<PointToPointNetDevice> (m_netDevice))
    {
      // extra debugging information which available ONLY for PointToPointNetDevice's
      os << ",";
      os << DynamicCast<PointToPointNetDevice> (m_netDevice)->GetChannel ()->GetDevice (0)->GetNode ()->GetId ();
      os << "-";
      os << DynamicCast<PointToPointNetDevice> (m_netDevice)->GetChannel ()->GetDevice (1)->GetNode ()->GetId ();
    }
  os << ")";
#else
  os << "dev=net(" << GetId () << ")";
#endif
  return os;
}

} // namespace ndnsim
} // namespace ns3

