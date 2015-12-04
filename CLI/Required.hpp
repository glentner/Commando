/*
 *  Copyright (c) Geoffrey Lentner 2015. All Rights Reserved.
 *  GNU General Public License v3.0, see LICENSE file.
 *  CLI/Required.hpp
 *
 *
 *   TODO: Required docs
 */


#ifndef __CLI__Required__
#define __CLI__Required__

#include <string>
#include <sstream>
#include "Argument.hpp"
#include "IO.hpp"

namespace CLI {


class Required : public Argument {
public:
        Required(){}
        Required(const std::string& name, const std::string& description)
                : Argument(name, description)
                {}

        virtual std::string GetHelp(const int& width);
};



// show usage statement for argument
inline std::string Required::GetHelp(const int& width)
{
        std::string spaces(width - name.length(), ' ');

        std::stringstream message;
        SSPrint(message, " ", name, spaces, description, " (required).\n");
        return message.str();
}



} // namespace CLI

#endif
