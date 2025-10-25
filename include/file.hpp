#pragma once

#include <string>

namespace core
{

class File
{
private:
  std::string id;
  std::string path;

public:
  File(std::string id, std::string path);

  const std::string &get_id() const noexcept;
  const std::string &get_path() const noexcept;

  void set_path(std::string path) noexcept;

  bool operator<(const File &other) const noexcept;
  bool operator==(const File &other) const noexcept;
};

} // namespace core
