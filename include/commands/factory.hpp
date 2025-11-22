#pragma once

#include "commands.hpp"
#include <memory>
#include <span>

namespace gateways::commands
{

class CommandsFactory
{
public:
  static void required_arguments(const std::span<const std::string> &args,
                                 const size_t quantity);

  static std::unique_ptr<Command>
  get_command(const std::vector<std::string> &args);
};

} // namespace gateways::commands
