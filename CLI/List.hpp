/*
 *  Copyright (c) Geoffrey Lentner 2015. All Rights Reserved.
 *  GNU General Public License v3.0, see LICENSE file.
 *  CLI//List.hpp
 *
 *
 *   TODO: List docs
 */


#ifndef __CLI__List__
#define __CLI__List__

#include <string>
#include <sstream>
#include "Argument.hpp"
#include "IO.hpp"

namespace CLI {


class List: public Argument {
public:
        // a variable length list
        std::vector<std::string> value;

        List(){}
        List(const std::string& name, const std::string& description)
                : Argument(name, description)
                {}

        virtual std::string GetHelp(const int& width);
};




// show usage statement for argument
inline std::string List::GetHelp(const int& width)
{
        std::string spaces(width - name.length() - 3, ' ');

        std::stringstream message;
        SSPrint(message, " ", name, "...", spaces, description, " (required).\n\n");
        return message.str();
}




} // namespace CLI

#endif
