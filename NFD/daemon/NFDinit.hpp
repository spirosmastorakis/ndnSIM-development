#ifndef NFD_INIT_HPP
#define NFD_INIT_HPP

//#include "version.hpp"
#include "ns3/ndnSIM/NFD/core/logger.hpp"
#include "ns3/ndnSIM/NFD/core/global-io.hpp"
#include "ns3/ndnSIM/NFD/core/privilege-helper.hpp"
#include "ns3/ndnSIM/NFD/daemon/fw/forwarder.hpp"
#include "ns3/ndnSIM/NFD/daemon/face/null-face.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/internal-face.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/fib-manager.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/face-manager.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/strategy-choice-manager.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/status-server.hpp"
#include "ns3/ndnSIM/NFD/core/config-file.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/general-config-section.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/tables-config-section.hpp"

using ns3::ndn::Face;

namespace nfd {

struct ProgramOptions
{
  bool showUsage;
  bool showVersion;
  bool showModules;
  std::string config;
};

class Nfd : noncopyable
{
public:
  explicit
  Nfd(const std::string& configFile);

  ~Nfd();

  void
  initialize();

  void
  initializeLogging();

  class IgnoreRibAndLogSections
  {
  public:
    void
    operator()(const std::string& filename,
             const std::string& sectionName,
             const ConfigSection& section,
             bool isDryRun);
  };

  void
  initializeManagement();

  static void
  printUsage(std::ostream& os, const std::string& programName);

  static void
  printModules(std::ostream& os);

  static bool
  parseCommandLine(int argc, char** argv, ProgramOptions& options);

  void
  terminate(const boost::system::error_code& error,
          int signalNo,
          boost::asio::signal_set& signalSet);
  void
  reload(const boost::system::error_code& error,
       int signalNo,
       boost::asio::signal_set& signalSet);

private:
  std::string m_configFile;

  shared_ptr<Forwarder> m_forwarder;

  shared_ptr<InternalFace>          m_internalFace;
  shared_ptr<FibManager>            m_fibManager;
  shared_ptr<FaceManager>           m_faceManager;
  shared_ptr<StrategyChoiceManager> m_strategyChoiceManager;
  shared_ptr<StatusServer>          m_statusServer;

  shared_ptr<std::ofstream>         m_logFile;
  std::basic_streambuf<char>*       m_originalStreamBuf;
  ndn::KeyChain                     m_keyChain;
};

} // namespace nfd

#endif // NFD_INIT_HPP
