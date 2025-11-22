#pragma once

#include "commands.hpp"
#include <memory>
#include <span>
#include <vector>

namespace gateways::commands
{

class IndexSearchCommand : public Command
{
private:
  std::vector<std::string> terms;

public:
  IndexSearchCommand(std::vector<std::string> terms);
  void execute(AppContext &ctx) override final;

  static std::unique_ptr<IndexSearchCommand>
  from_args(std::span<const std::string> args);
};

} // namespace gateways::commands
