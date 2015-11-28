/*
 *  Copyright (c) Geoffrey Lentner 2015. All Rights Reserved.
 *  GNU General Public License v3.0, see LICENSE file.
 *  CLI//Exceptions.hpp
 *
 *   Exceptions provided by CLI framework.
 *   TODO: Exceptions docs
 *
 */

#ifndef __CLI__Exceptions__
#define __CLI__Exceptions__

#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include "IO.hpp"

namespace CLI {

class Exception {
public:

        template<typename... Args>
	explicit Exception(Args &&... args)
        {
                Init(std::forward<Args>(args)...);
        }

        template<typename... Args>
        void Init(Args &&... args)
	{
		SSPrint(__stream, std::forward<Args>(args)...);
	}

        virtual const char* what() const throw(){ return __stream.str().c_str(); }

protected:

        std::stringstream __stream;
};

class Error : public Exception {
public:

	template<typename... Args>
	Error(Args &&... args):
        Exception("Error: ", std::forward<Args>(args)...) {}
};

class Usage: public Exception {
public:
        template<typename... Args>
	Usage(Args &&... args):
        Exception(std::forward<Args>(args)...) {}
};



} // namespace RootTree

#endif
