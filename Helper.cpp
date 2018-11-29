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

	// version1이 최신일 경우 +1 리턴.
	// version2가 최신일 경우 -1 리턴.
	// version1과 version2가 동일할 경우 0 리턴.
	int CompareVersion(const std::string &version1, const std::string &version2) {

		std::vector<std::string> ver1;
		boost::split(ver1, version1, boost::algorithm::is_any_of("."));
		
		std::vector<std::string> ver2;
		boost::split(ver2, version2, boost::algorithm::is_any_of("."));

		for(unsigned i = 0; i < ver1.size(); ++i) {
			if(ver2.size() <= i) {
				return +1;
			}
			int num1 = atoi(ver1[i].c_str());
			int num2 = atoi(ver2[i].c_str());
			if( num1 > num2) {
				return +1;
			} else if (num1 < num2) { 
				return -1;
			}
		}
		return 0;
	}
}

