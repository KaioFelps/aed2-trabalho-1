#include "commands/index_search.hpp"
#include <iostream>

namespace gateways::commands
{

IndexSearchCommand::IndexSearchCommand(std::vector<std::string> terms)
    : terms(terms)
{
}

void IndexSearchCommand::execute(AppContext &ctx)
{
  if (this->terms.empty())
  {
    std::cout << "Você não digitou nenhum termo de busca.\n";
    return;
  }

  ctx.indexer.try_load();

  auto files = ctx.indexer.get_files_with_words(this->terms);
  if (this->terms.size() == 1)
  {
    std::cout << "Aqui estão os arquivos onde a palavra " << this->terms[0]
              << " aparece:\n";
  }
  else
  {
    std::cout << "Aqui estão os arquivos onde todas as palavras ";
    for (const auto &word : this->terms)
    {
      std::cout << word;
    }
    std::cout << " aparecem:\n";
  }

  for (const auto &file : files)
  {
    std::cout << "| " << file.get().get_path() << "\n";
  }
}

std::unique_ptr<IndexSearchCommand>
IndexSearchCommand::from_args(std::span<const std::string> args)
{
  CommandsFactory::required_arguments(args, 1);
  return std::make_unique<IndexSearchCommand>(
      std::vector(args.begin(), args.end()));
}

} // namespace gateways::commands
