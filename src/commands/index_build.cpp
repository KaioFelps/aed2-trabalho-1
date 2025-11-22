#include "commands/index_build.hpp"
#include <iostream>

namespace gateways::commands
{

IndexBuildCommand::IndexBuildCommand(std::filesystem::path lookup_path)
    : lookup_path(lookup_path)
{
}

void IndexBuildCommand::execute(AppContext &ctx)
{
  ctx.indexer.build(this->lookup_path);
  ctx.indexer.flush();
  std::cout << "Índice construído com sucesso. Você pode começar a realizar "
               "buscas!\n";
}

} // namespace gateways::commands
