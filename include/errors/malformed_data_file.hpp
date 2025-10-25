#pragma once

#include <exception>
#include <filesystem>

namespace core::errors
{

class MalformedDataFileException : public std::exception
{
private:
  std::string formatted_msg;

public:
  MalformedDataFileException(std::filesystem::path file_path);
  MalformedDataFileException(std::filesystem::path file_path,
                             std::string details);

  const char *what() const noexcept final;
};

} // namespace core::errors
