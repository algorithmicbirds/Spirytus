#include "SpirytusException.hpp"

SpirytusException::SpirytusException(const char *file, int line) noexcept
    : file(file), line(line) {}

const char *SpirytusException::what() const noexcept {
    std::ostringstream oss;
    oss << GetType() << std::endl << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char *SpirytusException::GetType() const noexcept { return "Spirytus Exception"; }

int SpirytusException::GetLine() const noexcept { return line; }

const std::string &SpirytusException::GetFile() const noexcept { return file; }

std::string SpirytusException::GetOriginString() const noexcept { std::ostringstream oss;
    oss << "[File]" << file << "[Line]" << line;
    return oss.str();
}
