/*
 *  Copyright (c) Geoffrey Lentner 2015. All Rights Reserved.
 *  GNU General Public License v3.0, see LICENSE file.
 *  CLI/Argument.hpp
 *
 *  An Argument is the base class to the derived types:
 *  Required, Default, Switch, Flag, and List.
 *
 *   TODO: Argument docs
 */


#ifndef __CLI__Argument__
#define __CLI__Argument__

#include <vector>
#include <string>
#include <typeinfo>

#include "Utilities.hpp"
#include "Exceptions.hpp"

namespace CLI {

class Argument {
public:

        // attributes of a generic `Argument`
        std::string name, default_value, value, description, abbrv;
        bool given {false};

        Argument(){}
        Argument(const std::string& name, const std::string& value,
                const std::string& description, const std::string& abbrv = "")
                : name(name)
		, default_value(value)
                , value(value)
                , description(description)
                , abbrv(abbrv)
                {}

        template<class ValueType>
        ValueType make() const;

        template<class ValueType>
        operator ValueType();

	operator const char*();
	operator std::string();


        virtual std::string GetHelp(const int& spacing) = 0;

protected:

        // initialize from derived class List and Required
        Argument(const std::string& name, const std::string& description)
                : name(name)
                , description(description)
                {}
};




template<class ValueType>
inline ValueType Argument::make() const
{
	ValueType return_value;
	std::stringstream convert(value);

	if (!(convert >> return_value))
	throw Error("Unable to convert std::string to ", typeid(ValueType).name(),
		" for argument `", name, "`.");

	return return_value;
}



template<class ValueType>
inline Argument::operator ValueType()
{
        return make<ValueType>();
}


// specializations allow return of multi-word `value`s.
inline Argument::operator std::string()
{
	return value;
}

inline Argument::operator const char*()
{
	return value.c_str();
}

} // namespace CLI

#endif
