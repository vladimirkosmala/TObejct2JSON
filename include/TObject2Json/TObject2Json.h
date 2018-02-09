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
/// \file   TObejct2Json.hpp
/// \author Vladimir Kosmala
///

#ifndef QUALITYCONTROL_TOBJECT2JSON_H
#define QUALITYCONTROL_TOBJECT2JSON_H

// QualityControl
#include "QualityControl/MySqlDatabase.h"

using o2::quality_control::repository::MySqlDatabase;

namespace o2 {
namespace quality_control {

constexpr int numberOfContextThread = 1;

class TObject2Json
{
  public:
    void connectMySQLClient(std::string host, std::string databse, std::string username, std::string password);
    std::string retrieveMonitoObjectJson(std::string agentName, std::string objectName);
    std::string handleRequest(std::string message);
    void startZmqServer(std::string endpoint);

  private:
    std::unique_ptr<MySqlDatabase> mSqlClient;
};

} // namespace o2
} // namespace quality_control

#endif // QUALITYCONTROL_TOBJECT2JSON_H
