#pragma once

#include <vector>
#include <string>

class Filesystem
{
public:
    static std::vector<uint8_t> LoadFile(const std::wstring& path);
};


