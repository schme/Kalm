#pragma once

#include <string>

namespace ks
{

/// Returns true if a selection was made, and the filepath is placed in outFile
bool showFileBrowser(const std::string &rootPath, std::string &outFile, bool *opt = nullptr);

}
