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

#ifndef NDN_UDP_FACE_H
#define NDN_UDP_FACE_H

#include "ns3/ndnSIM/model/ndn-face.hpp"

#include "ns3/socket.h"
#include "ns3/ptr.h"

#include <map>

namespace ns3 {
namespace ndn {

/**
 * \ingroup ndn-face
 * \brief Implementation of UDP/IP NDN face
 *
 * \see ndn::AppFace, ndn::NetDeviceFace, ndn::Ipv4Face, ndn::TcpFace
 */
class UdpFace : public Face {
public:
  static TypeId
  GetTypeId();

  /**
   * \brief Constructor
   *
   * @param node Node associated with the face
   */
  UdpFace(Ptr<Node> node, Ptr<Socket> socket, Ipv4Address address);
  virtual ~UdpFace();

  virtual void
  close();

  Ipv4Address
  GetAddress() const;

  virtual bool
  ReceiveFromUdp(Ptr<const Packet> p);

  ////////////////////////////////////////////////////////////////////
  // methods overloaded from ndn::Face
  virtual std::ostream&
  Print(std::ostream& os) const;

protected:
  // also from ndn::Face
  virtual bool
  Send(Ptr<Packet> p);

private:
  UdpFace(const UdpFace&); ///< \brief Disabled copy constructor
  UdpFace&
  operator=(const UdpFace&); ///< \brief Disabled copy operator

private:
  Ptr<Socket> m_socket;
  Ipv4Address m_address;
};

} // namespace ndn
} // namespace ns3

#endif // NDN_UDP_FACE_H
