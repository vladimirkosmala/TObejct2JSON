///
/// \file   TObejct2JSON.hpp
/// \author Vladimir Kosmala
///

#ifndef TOBJECT2JSON_HPP
#define TOBJECT2JSON_HPP

// QualityControl
#include "QualityControl/MySqlDatabase.h"

using o2::quality_control::repository::MySqlDatabase;

constexpr int numberOfContextThread = 1;

class TObject2JSON
{
  public:
    void connectMySQLClient(std::string host, std::string databse, std::string username, std::string password);
    std::string retrieveMonitoObjectJSON(std::string agentName, std::string objectName);
    std::string handleRequest(std::string message);
    void startZmqServer(std::string endpoint);

  private:
    std::unique_ptr<MySqlDatabase> sqlClient;
};

#endif // TOBJECT2JSON_HPP
