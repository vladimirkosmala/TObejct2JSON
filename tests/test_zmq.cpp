#include <zmq.hpp>
#include <string>
#include <iostream>

using namespace std;

int main (void) {
  zmq::message_t message;
  zmq::context_t context(1);
  zmq::socket_t socket(context, ZMQ_REQ);

  socket.connect("tcp://localhost:5555");

  string request = "ping";
  message.rebuild(request.data(), request.size());
  std::cout << "Sending request: " << request << std::endl;
  socket.send(message);

  socket.recv(&message);
  string response((const char*)message.data(), message.size());
  std::cout << "Received answer: " << response << std::endl;

  return 0;
}
