#ifndef NDN_FIB_HELPER_H
#define NDN_FIB_HELPER_H

#include "ns3/node.h"
#include "ns3/object-vector.h"
#include "ns3/pointer.h"

#include "ns3/ndn-common.h"
#include "ns3/ndn-forwarder.h"
#include "ns3/ndn-face.h"
#include "ns3/ndn-stack-helper.h"

#include "ns3/ndnSIM/NFD/daemon/mgmt/fib-manager.hpp"
#include "ns3/ndnSIM/NFD/common.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/command-validator.hpp"

#include <ndn-cxx/util/command-interest-generator.hpp>

namespace ns3 {

namespace ndn {

using ::ndn::CommandInterestGenerator;
using ::nfd::ControlParameters;

class FibHelper
{
public:
  FibHelper ();

  ~FibHelper ();

  static void
  GenerateCommand (Interest& interest);

  static void
  AddNextHop (ControlParameters parameters, Ptr<Node> node);

  static void
  RemoveNextHop (ControlParameters parameters, Ptr<Node> node);

  /**
   * \brief Add forwarding entry to FIB
   *
   * \param nodeName Node name
   * \param prefix Routing prefix
   * \param faceId Face index
   * \param metric Routing metric
   */
  static void
  AddRoute (const std::string &nodeName, const std::string &prefix, uint32_t faceId, int32_t metric);

  /**
   * \brief Add forwarding entry to FIB
   *
   * \param nodeName Node
   * \param prefix Routing prefix
   * \param faceId Face index
   * \param metric Routing metric
   */
  static void
  AddRoute (Ptr<Node> node, const std::string &prefix, uint32_t faceId, int32_t metric);

  /**
   * \brief Add forwarding entry to FIB
   *
   * \param node   Node
   * \param prefix Routing prefix
   * \param face   Face
   * \param metric Routing metric
   */
  static void
  AddRoute (Ptr<Node> node, const std::string &prefix, Ptr<Face> face, int32_t metric);

  /**
   * @brief Add forwarding entry to FIB (work only with point-to-point links)
   *
   * \param node Node
   * \param prefix Routing prefix
   * \param otherNode The other node, to which interests (will be used to infer face id
   * \param metric Routing metric
   */
  static void
  AddRoute (Ptr<Node> node, const std::string &prefix, Ptr<Node> otherNode, int32_t metric);

  /**
   * @brief Add forwarding entry to FIB (work only with point-to-point links)
   *
   * \param nodeName Node name (refer to ns3::Names)
   * \param prefix Routing prefix
   * \param otherNode The other node name, to which interests (will be used to infer face id (refer to ns3::Names)
   * \param metric Routing metric
   */
  static void
  AddRoute (const std::string &nodeName, const std::string &prefix, const std::string &otherNodeName, int32_t metric);
};

} // namespace ndn

} // namespace ns3

#endif // NDN_FIB_HELPER_H
