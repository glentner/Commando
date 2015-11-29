/*
 *  Copyright (c) Geoffrey Lentner 2015. All Rights Reserved.
 *  GNU General Public License v3.0, see LICENSE file.
 *  CLI/ArgManager.hpp
 *
 *  The ArgManager wraps a std::map, `data`, of std::string keys to Argument*
 *  values. It is made available to the users' derived application classes
 *  as `ArgV`.
 *
 */


#ifndef __CLI__ArgMap__
#define __CLI__ArgMap__

#include <iostream>
#include <string>
#include <map>

#include "Argument.hpp"

namespace CLI {


// short-hand notation
typedef std::map<std::string, Argument*> ArgMap;



class ArgManager {
public:

        ArgMap data;

        ArgManager(){}
        ~ArgManager(){}

        void insert(Argument*);

        template<typename ReturnType, typename ArgType = Argument>
        inline ReturnType get(const std::string&);

	std::size_t size() const;
        ArgMap::iterator begin();
        ArgMap::iterator end();
        ArgMap::const_iterator begin() const;
        ArgMap::const_iterator end() const;
};




inline void ArgManager::insert(Argument *arg)
{
        data[arg -> name] = arg;
}



template<typename ReturnType, typename ArgType>
inline ReturnType ArgManager::get(const std::string& key)
{
        try {
		ReturnType value = *static_cast<ArgType*>(data.at(key));
                return value;

        } catch (const std::out_of_range& error) {

                throw Error("`", key, "` does not name an available parameter!\n",
                        " --> ", error.what());
        }

}



inline std::size_t ArgManager::size() const
{
	return data.size();
}


// Iterators for the argument map

inline typename ArgMap::iterator ArgManager::begin()
{
        return data.begin();
}

inline typename ArgMap::iterator ArgManager::end()
{
        return data.end();
}

inline typename ArgMap::const_iterator ArgManager::begin() const
{
        return data.begin();
}

inline typename ArgMap::const_iterator ArgManager::end() const
{
        return data.end();
}



} // namespace CLI

#endif
