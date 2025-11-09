#ifndef HEALTH_CHECK_CLIENT_H
#define HEALTH_CHECK_CLIENT_H

#include <string>

bool checkHealth();
std::string postJSON(const std::string& endpoint, const std::string& jsonPayload);

#endif
