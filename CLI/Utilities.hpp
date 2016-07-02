/*
 *  Copyright (c) Geoffrey Lentner 2015. All Rights Reserved.
 *  GNU General Public License v3.0, see LICENSE file.
 *  CLI/Utilities.hpp
 *
 *
 *
 *   TODO: Utilities docs
 */


#ifndef __CLI__Utilities__
#define __CLI__Utilities__

#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include "Exceptions.hpp"

namespace CLI {

// Simple template defined function dumps argument to stringstream before
// attempting to assign to the return type. Throws an CLI::Error upon failure.
template<typename ReturnType, typename InputType>
inline ReturnType ConvertTo(const InputType& input) {

	std::stringstream buffer;
	buffer << input;

	ReturnType output;
	if (!(buffer >> output)) throw Error("ConvertTo() failed.");

	return output;
}


// Strip characters from the left to keep only the `basename` of a file path
inline std::string BaseName(std::string filepath) {

	std::size_t pos = filepath.find_last_of("/");
	if (pos != std::string::npos)
		filepath.erase(0, pos + 1);

	return filepath;
}


} // namespace CLI

#endif
