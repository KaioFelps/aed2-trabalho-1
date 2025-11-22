#pragma once

#include "app_context.hpp"
#include "indexer.hpp"
#include <string>
#include <vector>

namespace gateways::commands
{

class Command
{
public:
  virtual ~Command() = default;
  virtual void execute(AppContext &ctx) = 0;

  static std::vector<std::string> parse_arguments(const int argc,
                                                  const char *argv[]);
};

} // namespace gateways::commands

#include "commands/factory.hpp"
#include "commands/index_build.hpp"
#include "commands/index_search.hpp"
