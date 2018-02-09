#include <zmq.hpp>
#include <string>
#include <iostream>

// ZMQ
#include <zmq.h>

// O2
#include "Common/Exceptions.h"

// QualityControl
#include "QualityControl/QcInfoLogger.h"

using namespace std;
using namespace AliceO2::Common;
using namespace o2::quality_control::core;

int main (void) {
  int error;

  try {
    // Open a socket
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REQ);
    error = zmq_connect(socket, "tcp://localhost:5555");
    if (error) {
      string details;
      details += "Unable to connect zmq: ";
      details += zmq_strerror(zmq_errno());
      BOOST_THROW_EXCEPTION(FatalException() << errinfo_details(details));
    }

    // Send request
    string request = "ping";
    zmq_msg_t message;
    error = zmq_msg_init_data(&message, (void*)request.data(), request.size(), NULL, NULL);
    if (error) {
      string details;
      details += "Unable to init zmq message: ";
      details += zmq_strerror(zmq_errno());
      BOOST_THROW_EXCEPTION(FatalException() << errinfo_details(details));
    }

    QcInfoLogger::GetInstance() << "Sending request: " << request << infologger::endm;
    error = zmq_msg_send(&message, socket, 0);
    if (error) {
      string details;
      details += "Unable to send zmq message: ";
      details += zmq_strerror(zmq_errno());
      BOOST_THROW_EXCEPTION(FatalException() << errinfo_details(details));
    }

    // Answer
    error = zmq_msg_recv(&message, socket, 0);
    if (error) {
      string details;
      details += "Unable to connect zmq: ";
      details += zmq_strerror(zmq_errno());
      BOOST_THROW_EXCEPTION(FatalException() << errinfo_details(details));
    }
    string response((const char*)zmq_msg_data(&message), zmq_msg_size(&message));
    QcInfoLogger::GetInstance() << "Received answer: " << response << infologger::endm;

    // Close socekt
    zmq_close(socket);
    zmq_ctx_destroy(context);

  } catch (boost::exception & exc) {
     std::string diagnostic = boost::current_exception_diagnostic_information();
     QcInfoLogger::GetInstance() << "Unexpected exception, diagnostic information follows:\n" << diagnostic << infologger::endm;
     return 1;
     if (diagnostic == "No diagnostic information available.") {
       throw;
     }
   }

  return 0;
}
