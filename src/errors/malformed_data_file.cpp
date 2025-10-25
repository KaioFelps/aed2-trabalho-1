#include "errors/malformed_data_file.hpp"
#include <format>

namespace core::errors
{

MalformedDataFileException::MalformedDataFileException(
    std::filesystem::path file_path)
    : formatted_msg(
          std::format("O arquivo {} não é uma representação binária válida.",
                      file_path.string()))
{
}

MalformedDataFileException::MalformedDataFileException(
    std::filesystem::path file_path, std::string details)

    : formatted_msg(
          std::format("O arquivo {} não é uma representação binária válida. {}",
                      file_path.string(), details))
{
}

const char *MalformedDataFileException::what() const noexcept
{
  return this->formatted_msg.c_str();
}

} // namespace core::errors
