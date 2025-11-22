#include "commands/index_build.hpp"
#include <iostream>
#include <span>

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

std::unique_ptr<IndexBuildCommand>
IndexBuildCommand::from_args(std::span<const std::string> args)
{
  if (args[0] != "--path" && (args[0] != "-p"))
  {
    throw std::runtime_error(
        "Argumentos inválidos para o comando `indice construir`. Refira-se "
        "ao README.md para instruções.");
  }

  auto path = std::filesystem::path(args[1]);
  if (!std::filesystem::exists(path))
  {
    throw std::runtime_error("O caminho " + path.string() +
                             " não é um caminho válido.");
  }

  return std::make_unique<IndexBuildCommand>(path);
}

} // namespace gateways::commands
