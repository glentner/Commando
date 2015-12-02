/*
 *  Copyright (c) Geoffrey Lentner 2015. All Rights Reserved.
 *  GNU General Public License v3.0, see LICENSE file.
 *  lib/SingleMode.cpp
 *
 *   A SingleMode application has a single `exe`cutable function.
 *   TODO: SingleMode docs
 *
 */

#include <iostream>
#include <utility>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#include "CLI/SingleMode.hpp"
#include "CLI/Argument.hpp"
#include "CLI/Terminator.hpp"
#include "CLI/Flag.hpp"
#include "CLI/Default.hpp"
#include "CLI/Required.hpp"
#include "CLI/Switch.hpp"
#include "CLI/List.hpp"

#include "CLI/IO.hpp"
#include "CLI/Utilities.hpp"
#include "CLI/Exceptions.hpp"
#include "CLI/ArgManager.hpp"


namespace CLI {



// default constructor (currently does nothing)
SingleMode::SingleMode(){}


// base constructor
SingleMode::SingleMode(const int argc, const char **argv,
        const std::string& description)
{
        name = BaseName(argv[0]);
        this -> description = description;
        this -> argv = StringVector(argv + 1, argv + argc);

        Register(&help);
}



SingleMode::~SingleMode()
{
}


// construct from string vector instead
inline void SingleMode::FromMultiMode(const std::string& parent, StringVector& argv,
        const std::string& description)
{
        name = parent + " " + argv[0];

        if (not description.empty())
                this -> description = description;

        (this -> argv).assign(argv.begin() + 1, argv.end());
}





// the `Exe`cution function parses the arguments from `main`
// and runs the user's program.
inline int SingleMode::Exe(const int& modifier)
{
        try {
                RuntimeConfigure(modifier);
                return main();

        } catch (const Usage& message) {

                std::cout << message.what();
                return 0;

        } catch (const Error& error) {

                std::cerr << error.what() << std::endl;
                return 1;

        } catch (const std::exception& error) {

                std::cerr << error.what() << std::endl;
                return 2;

        } catch (...) {

                std::cerr << "An uncaught exception has occurred from "
                          << name << std::endl;

                return 3;
        }
}








void SingleMode::RuntimeConfigure(const int& modifier)
{

        // we have to reverse the order of the Required and Default
        // arguments because the variadic template Register() function
        // deposits them recursively (backwards).
        // FIXME: maybe this is wrong...
        // std::reverse(AllRequired.begin(), AllRequired.end());
        // std::reverse(AllDefaults.begin(), AllDefaults.end());

        // check for sensible definitions
        RunLogicChecks();

        // if no arguments are provided, throw Usage()
        if (argv.empty())
        {
                Alert(GetUsage(modifier));
                throw Usage();
        }

        // iterate through command line arguments and check for
        // flags+options (e.g., `-`, `--` prefixed).
        int idx = 0;
        for (const auto& arg : argv) {

                if (arg[0] == '-') {

                        if (arg.size() < 2)
                        throw Error("'-' is not a recognized flag or option!");

                        Interpret(idx, arg.substr(1, arg.length()));

                } else Remainder[idx] = arg;

                idx++;
        }


        // show help statement without considering other arguments
        if (help.given) {
                // FIXME: WHY YOU DO THIS???? AHHHHH!
                std::cout << GetHelp(modifier) << std::endl;
                throw Usage();
        }

        // look for Terminator flags
        for (auto& flag: AllTerminators) if (flag -> given)
        {
                std::cout << flag -> information << std::endl;
                throw Usage();
        }


        // walk the Switches and assign values (erase from Remainder)
        for (auto& opt: GivenSwitches)
        {
                if (Remainder.find(opt.first + 1) == Remainder.end())
                throw Error("--", opt.second -> name, " expected a free argument to ",
                        "follow but found `", argv[opt.first + 1], "` instead (not a "
                        "free argument).");

                // assign value and erase the argument
                opt.second -> value = Remainder[opt.first + 1];
                Remainder.erase(opt.first + 1);
        }


        // Required arguments take precident
        if (Remainder.size() < AllRequired.size())
        {
                std::stringstream warning;
                OSPrint(warning, "Insufficient arguments were provided. ");

                for (int i = AllRequired.size() - Remainder.size(); i < AllRequired.size(); i++)
                        OSPrint(warning, "`", AllRequired[i] -> name, "`, ");

                OSPrint(warning, "\033[2D went undefined.");

                throw Error(warning.str());
        }

        // move values off Remainder to std::vector (keys nolonger meaningful)
        vector<std::string> FinalRemainder;
        for (auto& arg: Remainder)
                FinalRemainder.push_back(arg.second);

        // assign values for AllRequired and pop off Remainder
        for (auto& opt: AllRequired)
        {
                opt -> value = FinalRemainder[0];
                FinalRemainder.erase(FinalRemainder.begin());
        }

        // check for valid status
        if (FinalRemainder.size() > AllDefaults.size() && AllLists.empty())
        throw Error("Too many arguments given! Only ", AllDefaults.size(),
                " default arguments available, but ", FinalRemainder.size(),
                " arguments given!");

        // assign values to the default arguments
        for (auto& opt: AllDefaults)
        {
                opt -> value = FinalRemainder[0];
                FinalRemainder.erase(FinalRemainder.begin());
        }

        // exit if appropriate
        if (FinalRemainder.empty() and AllLists.empty())
                return;

        // final status check
        if (not FinalRemainder.empty() and AllLists.empty())
        throw Error("There were ", FinalRemainder.size(), " too many arguments!");

        if (FinalRemainder.empty() and not AllLists.empty())
        throw Error("Expected at least one argument for `", AllLists[0] -> name, "`.");

        // fill the List argument with the remaining values
        List* only_list = AllLists[0];
        for (auto& arg: FinalRemainder)
                only_list -> value.push_back(arg);

}



// Decide whether this argument is a short form or long form flag/option
inline void SingleMode::Interpret(const int& idx, const std::string& option)
{
        if (option[0] == '-')
        {
                if (option.size() < 2)
                throw Error("'--' is not a recognized flag or option!");

                InterpretLongForm(idx, option.substr(1, option.size()));
        }

        else InterpretShortForm(idx, option.substr(0, option.size()));
}



// Handle a shortform Flag or Switch
void SingleMode::InterpretShortForm(const int& idx, const std::string& option)
{
        if (option.length() > 1) {

                // only permitted to stack for flag
                for (auto ch: option) if (not SetFlag(&ch))
                throw Error("-", ch, " does not name a flag!");

        } else if (SetFlag(option)) return;

        // we better find a switch then
        if (not SetSwitch(idx, option))
        throw Error("-", option, " does not name a flag or switch!");
}




int SingleMode::SetFlag(const std::string& ch)
{
        int found = false;
        for (auto& flag: AllFlags)
        {
                if (flag -> abbrv == ch)
                {
                        if (flag -> given)
                        throw Error("The `", flag -> name, "` flag was already given!");

                        found = true;
                        flag -> value = "1";
                        flag -> given = true;
                        break;
                }
        }

        return found;
}


int SingleMode::SetSwitch(const int& idx, const std::string& ch)
{
        int found = false;
        for (auto& sw: AllSwitches)
        {
                if (sw -> abbrv == ch)
                {
                        if (sw -> given)
                        throw Error("The `", sw -> name, "` switch was already given!");

                        found = true;
                        sw -> given = true;

                        GivenSwitches[idx] = sw;
                        break;
                }
        }

        return found;
}



// Handle a longform Flag or Switch
void SingleMode::InterpretLongForm(const int& idx, const std::string& option)
{
        // attempt to find a flag
        for (auto& flag: AllFlags)
        {
                if (flag -> name == option)
                {
                        if (flag -> given)
                        throw Error("The `", option, "` flag was already given!");

                        flag -> value = "1";
                        flag -> given = true;
                        return;
                }
        }

        // otherwise we better find a switch
        for (auto& sw: AllSwitches)
        {
                if (sw -> name == option)
                {
                        if (sw -> given)
                        throw Error("The `", option, "` switch was already given!");

                        sw -> given = true;
                        GivenSwitches[idx] = sw;
                        return;
                }
        }

        // oh no!
        throw Error("--", option, " does not name a flag or switch!");
}




// Construct the help message
std::string SingleMode::GetUsage(const int& modifier)
{
        std::stringstream message;

        if (!modifier)
                message << "\nusage: ";

        message << name;

        for (const auto& arg: AllRequired)
                message << " " << arg -> name;

        for (const auto& arg: AllDefaults)
                message << " [" << arg -> name << " " << arg -> value << "]";

        for (const auto& arg: AllLists)
                message << " " << arg -> name << "1 [" << arg -> name << "2 ...]";

        for (const auto& arg: AllSwitches)
                message << " [-" << arg -> abbrv << " | --" << arg -> name
                        << " " << arg -> value << "]";

        for (const auto& arg: AllFlags)
                message << " [-" << arg -> abbrv << " | --" << arg -> name << "]";


        message << "\n\n  " << description << "\n\n\n";

        return message.str();
}


std::string SingleMode::GetHelp(const int& modifier)
{

        int spacing = 0;
        for (const auto& opt: ArgV)
        if ( (opt.second -> name).length() > spacing )
                spacing = (opt.second -> name).length();

        spacing += 10;

        std::stringstream message;
        message << GetUsage(modifier);

        for (const auto& arg: AllRequired)
                message << arg -> GetHelp(spacing);

        for (const auto& arg: AllDefaults)
                message << arg -> GetHelp(spacing);

        for (const auto& arg: AllLists)
                message << arg -> GetHelp(spacing);

        for (const auto& arg: AllSwitches)
                message << arg -> GetHelp(spacing);

        for (const auto& arg: AllFlags)
                message << arg -> GetHelp(spacing);

        return message.str();
}



// Ensure that we have sensible definitions for our argument types
void SingleMode::RunLogicChecks()
{

        // it isn't clear how one would include more than one List
        // argument
        if (AllLists.size() > 1)
        {
                std::stringstream warning;
                OSPrint(warning, "Only one List argument is permitted (you've defined ");
                for (const auto& opt: AllLists)
                        OSPrint(warning, opt -> name, ", ");

                OSPrint(warning, "\033[2D)");
                throw Error(warning.str());
        }

	// check that we haven't defined more than one argument with the same
	// name or abbrv
	//std::vector<std::string> __names(ArgV.size()), __abbrv(ArgV.size());
	//for (const auto& arg: ArgV)
	//{
		//__names.push_back(arg.second -> name);
		//__abbrv.push_back(arg.second -> abbrv);
	//}

	//int arg_count = 0;
	//arg_count += AllRequired.size();
	//arg_count += AllDefaults.size();
	//arg_count += AllSwitches.size();
	//arg_count += AllFlags.size();
	//arg_count += AllTerminators.size();

	//if (arg_count != __names.size() or arg_count != __abbrv.size())
	//throw Error("Either a name or abbreviation was repeated in the registered arguments.");
}

} // namespace CLI
