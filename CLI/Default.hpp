/*
 *  Copyright (c) Geoffrey Lentner 2015. All Rights Reserved.
 *  GNU General Public License v3.0, see LICENSE file.
 *  CLI/Default.hpp
 *
 *
 *   TODO: Default docs
 */


#ifndef __CLI__Default__
#define __CLI__Default__

#include <string>
#include <sstream>
#include "Argument.hpp"
#include "IO.hpp"

namespace CLI {


class Default : public Argument {
public:
        Default(){}
        Default(const std::string& name, const std::string& value,
                const std::string& description, const std::string& abbrv = "")
                : Argument(name, value, description, abbrv)
                {}

        virtual std::string GetHelp(const int& width);
};


// show usage statement for argument
inline std::string Default::GetHelp(const int& width)
{
        std::string spaces(width - name.length(), ' ');
        std::stringstream message;

        SSPrint(message, " ", name, spaces, description, " (default: ", default_value, ").\n");
        return message.str();
}


} // namespace CLI

#endif
