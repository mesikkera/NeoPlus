//
//  Helper.cpp
//  NeoPlus
//
//  Created by mesikkera on 26/11/2018.
//  Copyright © 2018 mesikkera. All rights reserved.
//

#include "Helper.h"

#include <vector>
#include <boost/algorithm/string.hpp>

namespace neoplus {
	bool CompareVersion(const std::string &version1, const std::string &version2) {

		std::vector<std::string> ver1;
		boost::split(ver1, version1, boost::algorithm::is_any_of("."));
		
		std::vector<std::string> ver2;
		boost::split(ver2, version2, boost::algorithm::is_any_of("."));

		// 01. version1 > version2	
		// 02. version1 == version2
		// 03. version1 < version2
		// return type은 bool이면 안된다.

		// version1 > version2 ==> return 1
		// version1 == version2 ==> return 0
		// version1 < version2 ==>return -1


		// temp return 
		return true;
	}
}

