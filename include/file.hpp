#pragma once

#include <functional>
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
  bool operator<(const File &rhe) const noexcept;

  struct RefComparer
  {
    bool operator()(const std::reference_wrapper<const File> &lhs,
                    const std::reference_wrapper<const File> &rhs) const;
  };
};

} // namespace core
