/*
 *  Copyright (c) Geoffrey Lentner 2015. All Rights Reserved.
 *  GNU General Public License v3.0, see LICENSE file.
 *  CLI//Switch.hpp
 *
 *
 *   TODO: Switch docs
 */


#ifndef __CLI__Switch__
#define __CLI__Switch__

#include <string>
#include <sstream>
#include "Argument.hpp"
#include "IO.hpp"

namespace CLI {


class Switch : public Argument {
public:
        Switch(){}
        Switch(const std::string& name, const std::string& value,
                const std::string& description, const std::string& abbrv = "")
                : Argument(name, value, description, abbrv)
                {}

        virtual std::string GetHelp(const int& width);
};


// show usage statement for argument
inline std::string Switch::GetHelp(const int& width)
{
        std::string spaces(width - name.length() - 2, ' ');
        std::stringstream message;

        if ( ! abbrv.empty() ) {

                SSPrint(message, " -", abbrv, ",");
                spaces = spaces.substr(0, spaces.length() - 4);
        }


        SSPrint(message, " --", name, spaces, description, " (default: ", default_value, ").\n");

        return message.str();
}

} // namespace CLI

#endif
