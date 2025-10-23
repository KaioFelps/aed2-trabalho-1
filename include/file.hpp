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

  std::string &get_id() const noexcept;
  std::string &get_path() const noexcept;

  void set_path(std::string path) noexcept;

  bool operator<(const File &other) const noexcept;
  bool operator==(const File &other) const noexcept;
};

} // namespace core
