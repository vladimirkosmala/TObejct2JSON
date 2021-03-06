// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

///
/// \file   TObejct2Json.cpp
/// \author Vladimir Kosmala
///

// TObject2Json
#include "TObject2Json/TObject2Json.h"

// QualityControl
#include "QualityControl/MonitorObject.h"
#include "QualityControl/MySqlDatabase.h"
#include "QualityControl/QcInfoLogger.h"

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

namespace o2 {
namespace quality_control {

void TObject2Json::connectMySQLClient(string host, string databse, string username, string password)
{
  mSqlClient = make_unique<MySqlDatabase>();
  try {
    mSqlClient->connect(host, databse, username, password);
  } catch(FatalException err) {
    string details;
    details += "Impossible to connect to mysql server: ";
    details += err.what();
    BOOST_THROW_EXCEPTION(FatalException() << errinfo_details(details));
  }
}

string TObject2Json::retrieveMonitorObjectJson(string agentName, string objectName)
{
  if (mSqlClient == nullptr) {
    BOOST_THROW_EXCEPTION(FatalException() << errinfo_details("Can't retrieve object without beeing connected"));
  }

  unique_ptr<MonitorObject> monitor(mSqlClient->retrieve(agentName, objectName));
  unique_ptr<TObject> obj(monitor->getObject());
  monitor->setIsOwner(false); // we will manage the internal TObject from now
  TString json = TBufferJSON::ConvertToJSON(obj.get());
  return json.Data();
}

string TObject2Json::handleRequest(string request)
{
  QcInfoLogger::GetInstance() << "Debug: Received request: " << request << infologger::endm;

  // Check empty messagae
  if (request.length() == 0) {
    QcInfoLogger::GetInstance() << "Warning: empty request received, ignoring" << infologger::endm;
    return "";
  }

  // Split request into command and arguments
  vector<string> parts;
  boost::split(parts, request, boost::is_any_of(" "));

  // Handle commands
  if (parts[0] == "getobject") {
    QcInfoLogger::GetInstance() << "Debug: getobject command to handle" << infologger::endm;
    if (parts.size() != 3) {
      QcInfoLogger::GetInstance() << "Warning: getobject command needs 3 argument" << infologger::endm;
    }
    string agentName = parts[1];
    string objectName = parts[2];
    try {
      return retrieveMonitorObjectJson(agentName, objectName);
    } catch (AliceO2::Common::DatabaseException &err) {
      if (*boost::get_error_info<errinfo_db_errno>(err) == 1146) {
        // In case the table does no exist means the agent does not exist (agent = table)
        return "404 agent not found";
      }

      return "500 unhandled error";
    }
  } else if (parts[0] == "ping") {
    QcInfoLogger::GetInstance() << "Debug: ping command to handle" << infologger::endm;
    return "pong";
  } else {
    QcInfoLogger::GetInstance() << "Warning: unknown command to handle" << infologger::endm;
    return "";
  }
}

void TObject2Json::startZmqServer(string endpoint)
{
  int error, size;
  void *context = zmq_ctx_new();
  void *socket = zmq_socket(context, ZMQ_REP);

  QcInfoLogger::GetInstance() << "Info: ZMQ server: Binding" << infologger::endm;
  error = zmq_bind(socket, endpoint.data());
  if (error) {
    string details;
    details += "Unable to bind zmq socket: ";
    details += zmq_strerror(zmq_errno());
    BOOST_THROW_EXCEPTION(FatalException() << errinfo_details(details));
  }

  QcInfoLogger::GetInstance() << "Info: ZMQ server: listening requests..." << infologger::endm;
  while(1) {
    // Wait for next request from client inside a zmq message
    zmq_msg_t messageReq;
    zmq_msg_init(&messageReq);
    size = zmq_msg_recv(&messageReq, socket, 0);
    if (size == -1) {
      string details;
      details += "Unable to receive zmq message: ";
      details += zmq_strerror(zmq_errno());
      BOOST_THROW_EXCEPTION(FatalException() << errinfo_details(details));
    }

    // Process message
    string request((const char*)zmq_msg_data(&messageReq), size);
    zmq_msg_close(&messageReq);
    string response = handleRequest(request);
    QcInfoLogger::GetInstance() << "Debug: ZMQ server: sending back " << response << infologger::endm;

    // Send back response inside a zmq message
    zmq_msg_t messageRep;
    zmq_msg_init_size(&messageRep, response.size());
    memcpy(zmq_msg_data(&messageRep), response.data(), response.size());
    size = zmq_msg_send(&messageRep, socket, 0);
    if (size == -1) {
      string details;
      details += "Unable to send zmq message: ";
      details += zmq_strerror(zmq_errno());
      BOOST_THROW_EXCEPTION(FatalException() << errinfo_details(details));
    }
    QcInfoLogger::GetInstance() << "Debug: ZMQ server: sent back " << size << " bytes" << infologger::endm;
    zmq_msg_close(&messageRep);
  }
}

} // namespace o2
} // namespace quality_control
