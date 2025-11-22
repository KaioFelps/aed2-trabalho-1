#pragma once

#include <filesystem>
#include <string>
#include <unordered_set>

namespace core::helpers
{

std::unordered_set<std::string>
parse_stopwords(std::filesystem::path stopwords_file);

}
