
#include "StringUtils.h"

namespace StringUtils {
	int stoi(const std::string& strInt) {
	    std::istringstream ss(strInt);
	    int i;
	    ss >> i;
	    return i;
	}
}