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

#ifndef NDN_IP_FACE_STACK_H
#define NDN_IP_FACE_STACK_H

#include "ns3/application.h"
#include "ns3/socket.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ptr.h"

#include <ndn-cxx/name.hpp>

#include <map>

namespace ns3 {

class Packet;

namespace ndn {

class Face;
class TcpFace;
class UdpFace;

/**
 * @ingroup ndn
 * @brief Application that provides functionality of creating IP-based faces on NDN nodes
 *
 * The class implements virtual calls onInterest, onNack, and onData
 */
class IpFaceStack: public Object
{
public:
  static TypeId GetTypeId ();

  /**
   * @brief Default constructor
   */
  IpFaceStack ();
  virtual ~IpFaceStack ();

  /**
   * @brief Lookup TcpFace for a given address
   */
  Ptr<TcpFace>
  GetTcpFaceByAddress (const Ipv4Address &addr);

  /**
   * @brief Destroy TcpFace, e.g., after TCP connection got dropped
   */
  void
  DestroyTcpFace (Ptr<TcpFace> face);

  /**
   * @brief Lookup UdpFace for a given address
   */
  Ptr<UdpFace>
  GetUdpFaceByAddress (const Ipv4Address &addr);

  /**
   * @brief Method allowing creation and lookup of faces
   *
   * All created UDP faces are stored internally in the map, and if the same face is created, it will simply be looked up
   */
  Ptr<TcpFace>
  CreateOrGetTcpFace (Ipv4Address address,
                      Callback< void, Ptr<Face> > onCreate = NULL_CREATE_CALLBACK);

  /**
   * @brief Method allowing creation and lookup of faces
   *
   * All created TCP faces are stored internally in the map, and if the same face is created, it will simply be looked up
   */
  Ptr<UdpFace>
  CreateOrGetUdpFace (Ipv4Address address);

protected:
  void
  NotifyNewAggregate ();

private:
  void
  StartServer ();

  bool
  OnTcpConnectionRequest (Ptr< Socket > sock, const Address &addr);

  void
  OnTcpConnectionAccept (Ptr< Socket > sock, const Address &addr);

  void
  OnTcpConnectionClosed (Ptr< Socket > sock);

  void
  OnUdpPacket (Ptr< Socket > sock);

public:
  const static Callback< void, Ptr<Face> > NULL_CREATE_CALLBACK;

protected:
  Ptr<Node> m_node;

  bool m_enableTcp;
  bool m_enableUdp;

  Ptr<Socket> m_tcpServer;
  Ptr<Socket> m_udpServer;

  typedef std::map< Ipv4Address, Ptr<TcpFace> > TcpFaceMap;
  typedef std::map< Ipv4Address, Ptr<UdpFace> > UdpFaceMap;
  TcpFaceMap m_tcpFaceMap;
  UdpFaceMap m_udpFaceMap;
};

} // namespace ndn
} // namespace ns3

#endif // NDN_IP_FACE_STACK_H
