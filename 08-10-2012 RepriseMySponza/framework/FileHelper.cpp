/**
 * @file    FileHelper.cpp
 * @author  Tyrone Davison
 * @date    September 2012
 */

#include "FileHelper.hpp"
#include <fstream>
#include <sstream>

namespace tyga
{

std::string
tyga::stringFromFile(std::string filepath)
{
    std::ifstream fp;
    fp.open(filepath, std::ifstream::in);
    if (fp.is_open() == false) {
        return "";
    }
    std::stringstream ss;
    ss << fp.rdbuf();
    return ss.str();
}

} // end namespace tyga
