#pragma once

#include <string>

namespace core
{

class File
{
private:
  std::string path;

public:
  File(std::string path);

  const std::string &get_path() const noexcept;

  bool operator==(const File &other) const noexcept;
};

} // namespace core
