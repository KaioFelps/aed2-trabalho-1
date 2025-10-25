#include "file.hpp"

namespace core
{

File::File(std::string id, std::string path) : id(id), path(path) {}

const std::string &File::get_id() const noexcept { return this->id; }
const std::string &File::get_path() const noexcept { return this->path; }

void File::set_path(std::string path_) noexcept { this->path = path_; }

bool File::operator<(const File &other) const noexcept
{
  if (id != other.id) return id < other.id;
  return path < other.path;
}

bool File::operator==(const File &other) const noexcept
{
  return other.id == this->id && other.path == this->path;
}

} // namespace core
