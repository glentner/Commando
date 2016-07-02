/*
 *  Copyright (c) Geoffrey Lentner 2015. All Rights Reserved.
 *  GNU General Public License v3.0, see LICENSE file.
 *  CLI/SingleMode.hpp
 *
 *   A SingleMode application represents a single executable.
 *   TODO: SingleMode docs
 *
 */


#ifndef __CLI__SingleMode__
#define __CLI__SingleMode__

#include <iostream>
#include <utility>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#include "Argument.hpp"
#include "Terminator.hpp"
#include "Flag.hpp"
#include "Default.hpp"
#include "Required.hpp"
#include "Switch.hpp"
#include "List.hpp"

#include "IO.hpp"
#include "Utilities.hpp"
#include "Exceptions.hpp"
#include "ArgManager.hpp"


namespace CLI {



// short-hand notation
typedef std::vector<std::string> StringVector;

template<typename ArgType>
using vector = std::vector<ArgType>;



class SingleMode {
public:

        SingleMode();
        SingleMode(const int argc, const char **argv, const std::string& description,
                const std::string& contact_information = "");

        virtual ~SingleMode();

        virtual void FromMultiMode(const std::string& parent,
                StringVector& argv, const std::string& description = "");

        // user must wrap their code w/ the `main` function
        virtual int main() = 0;

        // runs RunTimeConfigure() followed by the user's main()
        virtual int Exe(Catch set_catch = true, const int& modifier = 0);

protected:

        StringVector argv;
        ArgManager   ArgV;

        // the name of the executable when called
        std::string name, description, contact_information;

        // the only default parameter
        Flag help {"help", "0", "show this message", "h"};

        // used to sort out the command-line arguments
        vector<Terminator*> AllTerminators;
        vector<Flag*>       AllFlags;
        vector<Switch*>     AllSwitches;
        vector<Required*>   AllRequired;
        vector<Default*>    AllDefaults;
        vector<List*>       AllLists;

        // track what is provided on the command line
        std::map<int, Switch*> GivenSwitches;
        std::map<int, std::string> Remainder;


        template<typename Arg0, typename... Args>
        void Register(Arg0&&, Args&&...);
        virtual void Register(Terminator*);
        virtual void Register(Required*);
        virtual void Register(Default*);
        virtual void Register(Switch*);
        virtual void Register(Flag*);
        virtual void Register(List*);
        virtual void Register();



        virtual void RuntimeConfigure(const int& modifier = 0);
        virtual void Interpret(const int&, const std::string&);
        virtual void InterpretShortForm(const int&, const std::string&);
        virtual void InterpretLongForm(const int&, const std::string&);


        virtual void RunLogicChecks();


        virtual int SetFlag(const std::string&);
        virtual int SetSwitch(const int&, const std::string&);


        virtual std::string GetUsage(const int& modifier = 0);
        virtual std::string GetHelp(const int& modifier = 0);
};



// overloaded functions for tracking member arguments
// ...
template<typename Arg0, typename... Args>
inline void SingleMode::Register(Arg0&& arg0, Args&&... args)
{
        Register(std::forward<Arg0>(arg0));
        Register(std::forward<Args>(args)...);
}

inline void SingleMode::Register(Required *arg)
{
        AllRequired.push_back(arg);
        ArgV.insert(arg);
}

inline void SingleMode::Register(Default *arg)
{
        AllDefaults.push_back(arg);
        ArgV.insert(arg);
}

inline void SingleMode::Register(Switch *arg)
{
        AllSwitches.push_back(arg);
        ArgV.insert(arg);
}

inline void SingleMode::Register(Terminator *arg)
{
        AllTerminators.push_back(arg);
        AllFlags.push_back(arg);
        ArgV.insert(arg);
}

inline void SingleMode::Register(Flag *arg)
{
        AllFlags.push_back(arg);
        ArgV.insert(arg);
}

inline void SingleMode::Register(List *arg)
{
        AllLists.push_back(arg);
        ArgV.insert(arg);
}

inline void SingleMode::Register()
{
}


} // namespace CLI

#endif
