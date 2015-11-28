/*
 *  Copyright (c) Geoffrey Lentner 2015. All Rights Reserved.
 *  GNU General Public License v3.0, see LICENSE file.
 *  CLI//Terminator.hpp
 *
 *
 *   TODO: Terminator docs
 */


#ifndef __CLI__Terminator__
#define __CLI__Terminator__

#include <string>

#include "Flag.hpp"


namespace CLI {

class Terminator : public Flag {
public:

        std::string information;

        Terminator(){}
        Terminator(const std::string& name, const std::string& description,
                const std::string& information, const std::string& abbrv = "")
                : Flag(name, "0", description, abbrv)
                , information(information)
                {}

        // virtual std::string GetHelp(const int& width);
};

} // namespace CLI

#endif
