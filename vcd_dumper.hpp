#pragma once

#include <string>
#include <set>
#include <map>
#include "simulator.hpp"

std::string ConvertNameToVcdId(const std::string& name);

class VcdDumper
{
public:
    struct Variable
    {
        std::string id;
    };

    using VarValueMap = std::map<std::string, int>;

    static void AddVariable(std::string name);
    static void TraceVariable(const std::string& name, int value);
    static void Dump(const std::string& filename);
private:
    static std::map<std::string, Variable> s_variables;
    static std::map<Time, VarValueMap>     s_values;

    static void DumpVariableIds(std::ostream& out);
    static void DumpVariableValues(std::ostream& out, const VarValueMap& values);
};
