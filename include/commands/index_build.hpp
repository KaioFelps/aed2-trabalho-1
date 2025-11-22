#pragma once

#include "commands.hpp"

namespace gateways::commands
{

class IndexBuildCommand : public Command
{
private:
  std::filesystem::path lookup_path;

public:
  IndexBuildCommand(std::filesystem::path lookup_path);
  void execute(AppContext &ctx) override final;
};

} // namespace gateways::commands
