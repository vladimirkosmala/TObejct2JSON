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
  int error, size;

  try {
    // Open a socket
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REQ);
    error = zmq_connect(socket, "tcp://127.0.0.1:5555");
    if (error) {
      string details;
      details += "Unable to connect zmq: ";
      details += zmq_strerror(zmq_errno());
      BOOST_THROW_EXCEPTION(FatalException() << errinfo_details(details));
    }

    // Send request
    string request = "ping";
    zmq_msg_t messageReq;
    error = zmq_msg_init_size(&messageReq, request.size());
    if (error) {
      string details;
      details += "Unable to init zmq message: ";
      details += zmq_strerror(zmq_errno());
      BOOST_THROW_EXCEPTION(FatalException() << errinfo_details(details));
    }
    memcpy(zmq_msg_data(&messageReq), request.data(), request.size());

    QcInfoLogger::GetInstance() << "Sending request: " << request << infologger::endm;
    size = zmq_msg_send(&messageReq, socket, 0);
    if (size == -1) {
      string details;
      details += "Unable to send zmq message: ";
      details += zmq_strerror(zmq_errno());
      BOOST_THROW_EXCEPTION(FatalException() << errinfo_details(details));
    }
    zmq_msg_close(&messageReq);

    // Answer
    zmq_msg_t messageRep;
    zmq_msg_init(&messageRep);
    size = zmq_msg_recv(&messageRep, socket, 0);
    if (size == -1) {
      string details;
      details += "Unable to receive zmq message: ";
      details += zmq_strerror(zmq_errno());
      BOOST_THROW_EXCEPTION(FatalException() << errinfo_details(details));
    }
    string response((const char*)zmq_msg_data(&messageRep), size);
    QcInfoLogger::GetInstance() << "Received answer: " << response << infologger::endm;
    zmq_msg_close(&messageRep);

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
