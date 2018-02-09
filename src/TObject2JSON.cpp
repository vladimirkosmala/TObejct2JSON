///
/// \file   TObejct2JSON.cpp
/// \author Vladimir Kosmala
///

// TObject2JSON
#include "TObject2JSON/TObject2JSON.hpp"

// QualityControl
#include "QualityControl/MonitorObject.h"
#include "QualityControl/MySqlDatabase.h"

// ZMQ
#include <zmq.h>

// Boost
#include <boost/algorithm/string.hpp>

// O2
#include "Common/Exceptions.h"

// ROOT
#include "TBufferJSON.h"

using namespace std;
using namespace o2::quality_control::core;
using namespace AliceO2::Common;

void TObject2JSON::connectMySQLClient(string host, string databse, string username, string password)
{
  sqlClient = make_unique<MySqlDatabase>();
  try {
    sqlClient->connect(host, databse, username, password);
  } catch(FatalException err) {
    string details;
    details += "Impossible to connect to mysql server: ";
    details += err.what();
    BOOST_THROW_EXCEPTION(FatalException() << errinfo_details(details));
  }
}

string TObject2JSON::retrieveMonitoObjectJSON(string agentName, string objectName)
{
  if (sqlClient == nullptr) {
    BOOST_THROW_EXCEPTION(FatalException() << errinfo_details("Can't retrieve object without beeing connected"));
  }

  unique_ptr<MonitorObject> monitor(sqlClient->retrieve(agentName, objectName));
  unique_ptr<TObject> obj(monitor->getObject());
  monitor->setIsOwner(false); // we will manage the internal TObject from now
  TString json = TBufferJSON::ConvertToJSON(obj.get());
  return json.Data();
}

string TObject2JSON::handleRequest(string request)
{
  cout << "Debug: Received request: " << request << endl;

  // Check empty messagae
  if (request.length() == 0) {
    cout << "Warning: empty request received, ignoring" << endl;
    return "";
  }

  // Split request into command and arguments
  vector<string> parts;
  boost::split(parts, request, boost::is_any_of(" "));

  // Handle commands
  if (parts[0] == "getobject") {
    cout << "Debug: getobject command to handle" << endl;
    if (parts.size() != 3) {
      cout << "Warning: getobject command needs 3 argument" << endl;
    }
    string agentName = parts[1];
    string objectName = parts[2];
    try {
      return retrieveMonitoObjectJSON(agentName, objectName);
    } catch (AliceO2::Common::DatabaseException &err) {
      if (*boost::get_error_info<errinfo_db_errno>(err) == 1146) {
        // In case the table does no exist means the agent does not exist (agent = table)
        return "404 agent not found";
      }

      return "500 unhandled error";
    }
  } else if (parts[0] == "ping") {
    cout << "Debug: ping command to handle" << endl;
    return "pong";
  } else {
    cout << "Warning: unknown command to handle" << endl;
    return "";
  }
}

void TObject2JSON::startZmqServer(string endpoint)
{
  int result;
  void *socket = zmq_ctx_new();
  void *responder = zmq_socket(socket, ZMQ_REP);

  cout << "Info: ZMQ server: Binding" << endl;
  result = zmq_bind(responder, endpoint.data());
  if (result != 0) {
    string details;
    details += "Unable to bind zmq socket: ";
    details += zmq_strerror(zmq_errno());
    BOOST_THROW_EXCEPTION(FatalException() << errinfo_details(details));
  }

  cout << "Info: ZMQ server: listening requests..." << endl;
  while(1) {
    zmq_msg_t message;

    // Wait for next request from client inside a zmq message
    zmq_msg_init(&message);
    zmq_msg_recv(&message, socket, 0);

    // Process message
    string request((const char*)zmq_msg_data(&message), zmq_msg_size(&message));
    string response = handleRequest(request);

    // Send back response inside a zmq message
    zmq_msg_init_data(&message, (void*)response.data(), response.size(), NULL, NULL);
    result = zmq_msg_send(&message, socket, 0);
    if (result != 0) {
      string details;
      details += "Unable to send zmq message: ";
      details += zmq_strerror(zmq_errno());
      BOOST_THROW_EXCEPTION(FatalException() << errinfo_details(details));
    }
  }
}
