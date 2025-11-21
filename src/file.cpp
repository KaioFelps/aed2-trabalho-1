#include "file.hpp"

namespace core
{

File::File(std::string path) : path(path) {}

const std::string &File::get_path() const noexcept { return this->path; }

bool File::operator==(const File &other) const noexcept
{
  return other.path == this->path;
}

} // namespace core
