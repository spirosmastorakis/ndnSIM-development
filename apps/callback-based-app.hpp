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

#ifndef NDN_CALLBACK_BASED_APP_H
#define NDN_CALLBACK_BASED_APP_H

#include "ns3/application.h"
#include "ns3/ptr.h"
#include "ns3/callback.h"

namespace ns3 {

/**
 * @ingroup ndn-apps
 * @brief A meta application that can be used to create custom apps within Python bindings
 */
class CallbackBasedApp : public Application {
public:
  static TypeId
  GetTypeId();

  /**
   * @brief Default constructor
   */
  CallbackBasedApp();

  /**
   * @brief Virtual destructor
   */
  virtual ~CallbackBasedApp();

  /**
   * @brief Define callback that will be fired when application need to start its work
   */
  void
  SetOnStartCallback(Callback<void, Ptr<Application>> onStart);

  /**
   * @brief Define callback that will be fired when application need to stop its work
   */
  void
  SetOnStopCallback(Callback<void, Ptr<Application>> onStart);

protected:
  // inherited from Application base class. Originally they were private
  virtual void
  StartApplication(); ///< @brief Called at time specified by Start

  virtual void
  StopApplication(); ///< @brief Called at time specified by Stop

private:
  Callback<void, Ptr<Application>> m_onStart;
  Callback<void, Ptr<Application>> m_onStop;
};

} // ns3

#endif // NDN_CALLBACK_BASED_APP_H
