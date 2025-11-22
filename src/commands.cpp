#include "commands.hpp"
#include "commands/factory.hpp"
#include <span>
#include <vector>

namespace gateways::commands
{

std::vector<std::string> Command::parse_arguments(const int argc,
                                                  const char *argv[])
{
  auto arguments = std::vector<std::string>();

  for (int i = 0; i < argc; i++)
  {
    auto argument_chars = argv[i];
    std::string argument;
    while (*argument_chars != 0)
    {
      argument.push_back(*argument_chars);
      argument_chars++;
    }
    arguments.push_back(argument);
  }

  if (arguments.size() <= 1)
  {
    throw std::runtime_error("Você não inseriu nenhum comando! Refira-se ao "
                             "README.md para instruções sobre o programa.");
  }

  arguments.erase(arguments.begin());
  return arguments;
}

void CommandsFactory::required_arguments(
    const std::span<const std::string> &args, const size_t count)
{
  if (args.size() >= count) return;
  throw std::runtime_error(
      "Quantidade insuficiente de argumentos para este comando. Refira-se "
      "ao README.md para instruções.");
}

std::unique_ptr<Command>
CommandsFactory::get_command(const std::vector<std::string> &args)
{

  if (args[0] == "indice")
  {
    if (args.size() < 2)
    {
      throw std::runtime_error("Subcomando não especificado. Refira-se ao "
                               "README.md para instruções.");
    }

    const auto subcommand = args[1];
    auto args_subvec = std::span(std::next(args.begin(), 2), args.end());
    if (subcommand == "construir")
    {
      return IndexBuildCommand::from_args(args_subvec);
    }
    if (subcommand == "buscar")
    {
      return IndexSearchCommand::from_args(args_subvec);
    }
  }

  std::string concatenated_command;
  for (auto &piece : args)
  {
    concatenated_command += piece + " ";
  }

  throw std::runtime_error("Você digitou um comando que não existe (" +
                           concatenated_command +
                           ") .Refira - se ao REAMDE.md para instruções sobre "
                           "como e quais comandos utilizar.");
}

}; // namespace gateways::commands
