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

int main (void) {
  // Open a socket
  void *context = zmq_ctx_new();
  void *socket = zmq_socket(context, ZMQ_REQ);
  zmq_connect(socket, "tcp://localhost:5555");

  // Send request
  string request = "ping";
  zmq_msg_t message;
  zmq_msg_init(&message);
  zmq_msg_init_data(&message, (void*)request.data(), request.size(), NULL, NULL);
  std::cout << "Sending request: " << request << std::endl;
  int result = zmq_msg_send(&message, socket, 0);
  if (result != 0) {
    string details;
    details += "Unable to send zmq message: ";
    details += zmq_strerror(zmq_errno());
    BOOST_THROW_EXCEPTION(FatalException() << errinfo_details(details));
  }

  // Answer
  zmq_msg_recv(&message, socket, 0);
  string response((const char*)zmq_msg_data(&message), zmq_msg_size(&message));
  std::cout << "Received answer: " << response << std::endl;

  // Close socekt
  zmq_close(socket);
  zmq_ctx_destroy(context);
  return 0;
}
