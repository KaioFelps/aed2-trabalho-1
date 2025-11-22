#pragma once

#include "commands.hpp"
#include <memory>

namespace gateways::commands
{

class CommandsFactory
{
public:
  static std::unique_ptr<Command>
  get_command(const std::vector<std::string> &args);
};

} // namespace gateways::commands
