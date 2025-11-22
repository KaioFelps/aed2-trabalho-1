#pragma once

#include "indexer.hpp"
#include <filesystem>

struct AppContext
{
  core::Indexer &indexer;
  const std::vector<std::string> &args;
};
