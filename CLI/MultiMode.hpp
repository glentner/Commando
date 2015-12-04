/*
 *  Copyright (c) Geoffrey Lentner 2015. All Rights Reserved.
 *  GNU General Public License v3.0, see LICENSE file.
 *  CLI/MultiMode.hpp
 *
 *   A MultiMode application manages several SingleMode applications.
 *   TODO: MultiMode docs
 *
 */


#ifndef __CLI__MultiMode__
#define __CLI__MultiMode__

#include <iomanip>
#include <string>
#include <map>


#include "SingleMode.hpp"
#include "Exceptions.hpp"
#include "Utilities.hpp"



namespace CLI {


// short hand notation
typedef std::map<std::string, SingleMode*> CommandMap;

class MultiMode {
public:

        MultiMode();
        MultiMode(const int argc, const char **argv, const std::string& description,
                  const std::string& contact_information = "");
                  
        ~MultiMode();


        std::string name, description, contact_information;
        StringVector ArgV;

        std::string select;
        CommandMap Commands;


        virtual int  Exe();
        virtual void RuntimeConfigure();


        virtual std::string ShowUsage();
        virtual std::string ShowHelp();

        CLI::Terminator help {"help", "show this message", " ", "h"};

protected:

        vector<Terminator*> AllTerminators;

        template<typename Arg0, typename... Args>
        void Register(Arg0&&, Args&&...);
        void Register(Terminator*);
        void Register();


        void InterpretFlag(const std::string&);
        void InterpretShortForm(const std::string&);
        void InterpretLongForm(const std::string&);
};


// overloaded functions for tracking member arguments
// ...
template<typename Arg0, typename... Args>
inline void MultiMode::Register(Arg0&& arg0, Args&&... args)
{
        Register(std::forward<Arg0>(arg0));
        Register(std::forward<Args>(args)...);
}

inline void MultiMode::Register(Terminator *flag)
{
        AllTerminators.push_back(flag);
}

inline void MultiMode::Register()
{
}


} // namespace CLI

#endif
