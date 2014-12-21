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

#include "callback-based-app.hpp"

#include <ns3/log.h>

NS_LOG_COMPONENT_DEFINE("CallbackBasedApp");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(CallbackBasedApp);

TypeId
CallbackBasedApp::GetTypeId(void)
{
  static TypeId tid =
    TypeId("ns3::CallbackBasedApp")
      .SetGroupName("Ndn")
      .SetParent<Application>()
      .AddConstructor<CallbackBasedApp>()

      .AddAttribute("OnStart", "OnStart callback", CallbackValue(),
                    MakeCallbackAccessor(&CallbackBasedApp::m_onStart), MakeCallbackChecker())

      .AddAttribute("OnStop", "OnStop callback", CallbackValue(),
                    MakeCallbackAccessor(&CallbackBasedApp::m_onStop), MakeCallbackChecker());
  return tid;
}

CallbackBasedApp::CallbackBasedApp()
{
}

CallbackBasedApp::~CallbackBasedApp()
{
}

void
CallbackBasedApp::SetOnStartCallback(Callback<void, Ptr<Application>> onStart)
{
  m_onStart = onStart;
}

void
CallbackBasedApp::SetOnStopCallback(Callback<void, Ptr<Application>> onStop)
{
  m_onStop = onStop;
}

void
CallbackBasedApp::StartApplication()
{
  NS_LOG_FUNCTION(this);
  if (!m_onStart.IsNull())
    m_onStart(this);
}

void
CallbackBasedApp::StopApplication()
{
  NS_LOG_FUNCTION(this);
  if (!m_onStop.IsNull())
    m_onStop(this);
}
}
