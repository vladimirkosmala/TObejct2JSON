///
/// \file   main.cpp
/// \author Vladimir Kosmala
///

// O2
#include "Common/Exceptions.h"

// TObject2Json
#include "TObject2Json/TObject2Json.h"

// QC
#include "QualityControl/QcInfoLogger.h"

using o2::quality_control::TObject2Json;
using namespace o2::quality_control::core;

// Run main() only if running outside of ROOT interpreter
// https://root.cern.ch/cint
# ifndef __CINT__
int main(int argc, char *argv[])
{
  std::string mySqlIp;
  std::string mySqlDataBase;
  std::string mySqlUser;
  std::string mySqlPassword;
  std::string zmqEndpoint;

  // Arguments parsing
  if (argc != 6) {
    QcInfoLogger::GetInstance() << "Info: no configuration passed, default used. "
                                << "Usage: cmd [ip dbName user pass zmqURI]" << infologger::endm;
    mySqlIp = "127.0.0.1";
    mySqlDataBase = "quality_control";
    mySqlUser = "root";
    mySqlPassword = "";
    zmqEndpoint = "tcp://127.0.0.1:5555";
  } else {
    mySqlIp = argv[1];
    mySqlDataBase = argv[2];
    mySqlUser = argv[3];
    mySqlPassword = argv[4];
    zmqEndpoint = argv[5];
  }

  try {
    TObject2Json server;
    server.connectMySQLClient(mySqlIp, mySqlDataBase, mySqlUser, mySqlPassword);
    server.startZmqServer(zmqEndpoint);
  } catch (boost::exception & exc) {
    std::string diagnostic = boost::current_exception_diagnostic_information();
    std::cerr << "Unexpected exception, diagnostic information follows:\n" << diagnostic << std::endl;
    if (diagnostic == "No diagnostic information available.") {
      throw;
    }
  }

  return 0;
}
# endif
