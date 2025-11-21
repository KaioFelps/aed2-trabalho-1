#include "file.hpp"

namespace core
{

File::File(std::string path) : path(path) {}

const std::string &File::get_path() const noexcept { return this->path; }

bool File::operator==(const File &other) const noexcept
{
  return other.path == this->path;
}

bool File::operator<(const File &rhe) const noexcept
{
  return this->path < rhe.path;
}

bool File::RefComparer::operator()(
    const std::reference_wrapper<const File> &lhs,
    const std::reference_wrapper<const File> &rhs) const
{
  return lhs.get() < rhs.get();
}

} // namespace core
