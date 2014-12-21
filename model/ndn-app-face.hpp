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

#ifndef NDN_APP_FACE_H
#define NDN_APP_FACE_H

#include "ns3/ndnSIM/model/ndn-face.hpp"
#include "ns3/ndnSIM/model/ndn-common.hpp"

#include "ns3/traced-callback.h"
#include "ns3/attribute.h"
#include "ns3/attribute-helper.h"

namespace ns3 {

class Packet;

namespace ndn {

class App;

/**
 * \ingroup ndn-face
 * \brief Implementation of application Ndn face
 *
 * This class defines basic functionality of Ndn face. Face is core
 * component responsible for actual delivery of data packet to and
 * from Ndn stack
 *
 * \see AppFace, NdnNetDeviceFace, NdnIpv4Face, NdnUdpFace
 */
class AppFace : public Face {
public:
  static TypeId
  GetTypeId();

  /**
   * \brief Default constructor
   */
  AppFace(Ptr<App> app);
  virtual ~AppFace();

  virtual void
  close();

  ////////////////////////////////////////////////////////////////////
  // methods overloaded from Face
  virtual void
  sendInterest(const Interest& interest);

  virtual void
  sendData(const Data& data);

  virtual bool
  isLocal() const;

public:
  virtual std::ostream&
  Print(std::ostream& os) const;
  ////////////////////////////////////////////////////////////////////

private:
  AppFace();
  AppFace(const AppFace&); ///< \brief Disabled copy constructor
  AppFace&
  operator=(const AppFace&); ///< \brief Disabled copy operator

private:
  Ptr<App> m_app;
};

inline bool
AppFace::isLocal() const
{
  return true;
}

ATTRIBUTE_HELPER_HEADER(Name);

} // namespace ndn
} // namespace ns3

#endif // NDN_APP_FACE_H
