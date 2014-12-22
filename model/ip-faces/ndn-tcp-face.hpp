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

#ifndef NDN_TCP_FACE_H
#define NDN_TCP_FACE_H

#include "ns3/ndnSIM/model/ndn-face.hpp"

#include "ns3/socket.h"
#include "ns3/ptr.h"
#include "ns3/callback.h"

#include <map>

namespace ns3 {
namespace ndn {

/**
 * \ingroup ndn-face
 * \brief Implementation of TCP/IP NDN face
 *
 * \see NdnAppFace, NdnNetDeviceFace, NdnIpv4Face, NdnUdpFace
 */
class TcpFace : public nfd::Face {
public:
  static TypeId
  GetTypeId();

  /**
   * \brief Constructor
   *
   * @param node Node associated with the face
   */
  TcpFace(Ptr<Node> node, Ptr<Socket> socket, Ipv4Address address);
  virtual ~TcpFace();

  void
  OnTcpConnectionClosed(Ptr<Socket> socket);

  virtual void
  close();

  Ipv4Address
  GetAddress() const;

  static Ptr<TcpFace>
  GetFaceByAddress(const Ipv4Address& addr);

  void
  SetCreateCallback(Callback<void, Ptr<Face>> callback);

  void
  OnConnect(Ptr<Socket> socket);

  ////////////////////////////////////////////////////////////////////
  // methods overloaded from ndn::Face
  // virtual void
  // RegisterProtocolHandlers(const InterestHandler& interestHandler, const DataHandler& dataHandler);

  // virtual void
  // UnRegisterProtocolHandlers();

  virtual std::ostream&
  Print(std::ostream& os) const;

protected:
  // also from ndn::Face
  virtual bool
  Send(Ptr<Packet> p);

private:
  TcpFace(const TcpFace&); ///< \brief Disabled copy constructor
  TcpFace&
  operator=(const TcpFace&); ///< \brief Disabled copy operator

  void
  ReceiveFromTcp(Ptr<Socket> clientSocket);

private:
  Ptr<Socket> m_socket;
  Ipv4Address m_address;
  uint32_t m_pendingPacketLength;
  Callback<void, Ptr<Face>> m_onCreateCallback;
};

} // namespace ndn
} // namespace ns3

#endif // NDN_TCP_FACE_H
