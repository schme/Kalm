#pragma once

#include "ResourceId.h"

#include <string>

namespace ks {

class SceneBank {
	
    void init(const std::string& projectRoot) {
        this->projectRoot = projectRoot;
    }
    std::string projectRoot = "";
};

}
