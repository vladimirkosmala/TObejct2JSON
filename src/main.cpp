///
/// \file   main.cpp
/// \author Vladimir Kosmala
///

// O2
#include "Common/Exceptions.h"

// TObject2Json
#include "TObject2Json/TObject2Json.h"

using o2::quality_control::TObject2Json;

// Run main() only if running outside of ROOT interpreter
// https://root.cern.ch/cint
# ifndef __CINT__
int main()
{
  try {
    TObject2Json server;
    server.connectMySQLClient("127.0.0.1", "quality_control", "root", "");
    server.startZmqServer("tcp://127.0.0.1:5555");
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
