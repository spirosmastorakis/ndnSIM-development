#include "ndn-fib-helper.h"

namespace ns3 {

namespace ndn {

using ::ndn::CommandInterestGenerator;
using ::nfd::ControlParameters;
using ::nfd::FibManager;

FibHelper::FibHelper () :
  m_face(NULL),
  m_node (NULL)
{
}

FibHelper::FibHelper (ControlParameters parameters, Ptr<Face> face, Ptr<Node> node)
{
  m_parameters = parameters;
  m_face = face;
  m_node = node;
}

FibHelper::~FibHelper ()
{
}

void
FibHelper::SetFace (Ptr<Face> face)
{
  m_face = face;
}

Ptr<Face>
FibHelper::GetFace ()
{
  return m_face;
}

void
FibHelper::SetControlParameters (ControlParameters parameters)
{
  m_parameters = parameters;
}

ControlParameters
FibHelper::GetControlParameters ()
{
  return m_parameters;
}

void
FibHelper::SendCommand (Name commandName)
{
  Block encodedParameters(m_parameters.wireEncode());
  shared_ptr<Interest> command(make_shared<Interest>(commandName));
  //generateCommand(*command);
  m_face->onReceiveData += [this, command, encodedParameters] (const Data& response) {
  };
  Ptr<L3Protocol> L3protocol = m_node->GetObject<L3Protocol> ();
  shared_ptr<FibManager> fibManager = L3protocol->GetFibManager ();
  fibManager->onFibRequest(*command);
}

} // namespace ndn

} // namespace ns
