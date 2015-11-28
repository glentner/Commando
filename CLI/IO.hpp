/*
 *  Copyright (c) Geoffrey Lentner 2015. All Rights Reserved.
 *  GNU General Public License v3.0, see LICENSE file.
 *  CLI/IO.hpp
 *
 *
 *
 *   TODO: IO docs
 */


#ifndef __CLI__IO__
#define __CLI__IO__

#include <iostream>
#include <sstream>
#include <string>
#include <utility>


namespace CLI {



inline void OSPrint(std::ostream &stream){}

template<typename Arg0, typename... Args>
inline void OSPrint(std::ostream &stream, Arg0 &&arg0, Args &&... args)
{
    stream << arg0;
    OSPrint(stream, std::forward<Args>(args)...);
}




inline void SSPrint(std::stringstream &stream){}

template<typename Arg0, typename... Args>
inline void SSPrint(std::stringstream &stream, Arg0 &&arg0, Args &&... args)
{
    stream << arg0;
    SSPrint(stream, std::forward<Args>(args)...);
}





template<typename Arg0, typename... Args>
inline void Print(Arg0 &&arg0, Args &&... args)
{
    OSPrint(std::cout, std::forward<Arg0>(arg0), std::forward<Args>(args)...);
}




inline void Alert()
{
        std::cout << std::flush;
}

template<typename Arg0, typename... Args>
inline void Alert(Arg0 &&arg0, Args &&... args)
{
    std::cout << arg0;
    Alert(std::forward<Args>(args)...);
}


} // namespace CLI

#endif
