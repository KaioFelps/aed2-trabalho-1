#include "app_context.hpp"
#include "commands.hpp"
#include "helpers.hpp"
#include "indexer.hpp"
#include "text_processor.hpp"
#include <iostream>

namespace fs = std::filesystem;

using gateways::commands::Command;
using gateways::commands::CommandsFactory;

const fs::path APP_DATA_DIR = fs::path("build/data/");
const fs::path STOPWORDS_FILE = fs::path("./stopwords.txt");

int main(const int argc, const char *argv[])
{
  // garante que o diretório dos dados binários exista
  fs::create_directories(APP_DATA_DIR);

  auto stopwords = core::helpers::parse_stopwords(STOPWORDS_FILE);
  auto processor = core::TextProcessor(stopwords);
  auto indexer = core::Indexer(APP_DATA_DIR, processor);
  auto args = Command::parse_arguments(argc, argv);

  auto ctx = AppContext{.indexer = indexer, .args = args};
  auto command = CommandsFactory::get_command(args);

  command->execute(ctx);

  return 0;
}
