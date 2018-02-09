///
/// \file   main.cpp
/// \author Vladimir Kosmala
///

// O2
#include "Common/Exceptions.h"

// TObject2JSON
#include "TObject2JSON/TObject2JSON.h"

using o2::quality_control::TObject2JSON;

// Run main() only if running outside of ROOT interpreter
// https://root.cern.ch/cint
# ifndef __CINT__
int main()
{
  try {
    TObject2JSON server;
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
