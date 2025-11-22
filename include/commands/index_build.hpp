#pragma once

#include "commands.hpp"
#include <span>

namespace gateways::commands
{

class IndexBuildCommand : public Command
{
private:
  std::filesystem::path lookup_path;

public:
  IndexBuildCommand(std::filesystem::path lookup_path);
  void execute(AppContext &ctx) override final;

  static std::unique_ptr<IndexBuildCommand>
  from_args(std::span<const std::string> args);
};

} // namespace gateways::commands
