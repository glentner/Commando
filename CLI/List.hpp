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
#include <vector>
#include <list>
#include <typeinfo>

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

        // conversion to other containers
        template<typename ContainerType>
        operator ContainerType() const;

        // specializations for std::string, const char*, char*
        operator std::vector<std::string>() const;
        operator std::list<std::string>() const;
        operator std::vector<char*>() const;
        operator std::list<char*>() const;
        operator std::vector<const char*>() const;
        operator std::list<const char*>() const;

};



// show usage statement for argument
inline std::string List::GetHelp(const int& width)
{
        std::string spaces(width - name.length() - 3, ' ');

        std::stringstream message;
        SSPrint(message, " ", name, "...", spaces, description, " (required).\n");
        return message.str();
}


template<typename ContainerType>
inline List::operator ContainerType() const
{
        ContainerType return_value;

        for (std::size_t i = 0; i < value.size(); i++)
        {
                std::stringstream convert(value[i]);
                typename ContainerType::value_type interm_value;

                if (!(convert >> interm_value))
                throw Error("Cannot convert to type ", typeid(ContainerType::value_type).name(),
                        "from std::string for ", typeid(ContainerType).name(),
                        " for CLI::List argument.");

                return_value.push_back(interm_value);
        }

        return return_value;
}


// specializations added to fix multi-word arguments

inline List::operator std::vector<std::string>() const
{
        return value;
}



inline List::operator std::list<std::string>() const
{
        return std::list<std::string>(value.begin(), value.end());
}



inline List::operator std::vector<char*>() const
{
        std::vector<char*> return_value;
        for (const auto& item: value)
                return_value.push_back(const_cast<char*>(item.c_str()));

        return return_value;
}



inline List::operator std::list<char*>() const
{
        std::list<char*> return_value;
        for (const auto& item: value)
                return_value.push_back(const_cast<char*>(item.c_str()));

        return return_value;
}



inline List::operator std::vector<const char*>() const
{
        std::vector<const char*> return_value;
        for (const auto& item: value)
                return_value.push_back(const_cast<char*>(item.c_str()));

        return return_value;
}



inline List::operator std::list<const char*>() const
{
        std::list<const char*> return_value;
        for (const auto& item: value)
                return_value.push_back(const_cast<char*>(item.c_str()));

        return return_value;
}





} // namespace CLI

#endif
