#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <boost/filesystem.hpp>
#include <iostream>

namespace cimarron {
namespace helper {
void createDir(const std::string path) {
  boost::filesystem::path dir(path);
  if (boost::filesystem::create_directory(dir)) {
    std::cerr << "Directory Created: " << path << std::endl;
  }
};
} // namespace helper
} // namespace cimarron

#endif
