/**
 * @file    FileHelper.hpp
 * @author  Tyrone Davison
 * @date    September 2012
 */

#pragma once
#ifndef __TYGA_FILEHELPER__
#define __TYGA_FILEHELPER__

#include <string>

namespace tyga
{
    /**
     * Construct a new string object with the contents of a text file.
     * @param   A valid, full path to a text file to read.
     * @return  The new new string object
     */
    std::string
    stringFromFile(std::string filepath);

} // end namespace tyga

#endif
