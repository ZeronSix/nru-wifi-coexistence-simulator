#include "vcd_dumper.hpp"
#include "simulator.hpp"
#include "config.hpp"
#include <fstream>
#include <bitset>

std::map<std::string, VcdDumper::Variable> VcdDumper::s_variables;
std::map<Time, VcdDumper::VarValueMap>     VcdDumper::s_values;

std::string ConvertNameToVcdId(const std::string &name) {
    std::string result(name.size(), ' ');
    for (size_t i = 0; i < name.size(); ++i) {
        char start = std::isdigit(static_cast<unsigned char>(name[i])) ? '0' : 'a';
        // The VCD id is composed of printable ASCII characters from ! to ~ (decimal 33 to 126).
        result[i] = static_cast<char>('!' + name[i] - start);
    }

    return result;
}

void VcdDumper::AddVariable(std::string name) {
    if (!cfg::g_writeVcdDump) return;
    ASSERT(s_variables.count(name) == 0);

    s_variables[name] = Variable{ConvertNameToVcdId(name)};
    s_values[0][name] = 0;
}

void VcdDumper::TraceVariable(const std::string &name, int value) {
    if (!cfg::g_writeVcdDump) return;
    if (s_variables.count(name) != 1) return;

    s_values[Simulator::Now()][name] = value;
}

void VcdDumper::Dump(const std::string &filename) {
    if (!cfg::g_writeVcdDump) return;
    std::ofstream stream;
    stream.open(filename, std::ios::out);

    stream << "$timescale 1us $end" << std::endl
           << "$scope module logic $end" << std::endl;
    DumpVariableIds(stream);

    stream << "$upscope $end" << std::endl
           << "$enddefinitions $end" << std::endl
           << "$dumpvars" << std::endl;
    DumpVariableValues(stream, s_values[0]);
    stream << "$end" << std::endl;

    for (const auto&[time, valueMap] : s_values) {
        stream << "#" << time << std::endl;
        DumpVariableValues(stream, valueMap);
    }
}

void VcdDumper::DumpVariableIds(std::ostream &out) {
    for (const auto&[varName, var] : s_variables) {
        out << "$var wire " << sizeof(int) * 8 << " " << var.id << " " << varName << " $end" << std::endl;
    }
}

void VcdDumper::DumpVariableValues(std::ostream &out, const VcdDumper::VarValueMap &values) {
    for (const auto&[varName, value] : values) {
        const auto &variable = s_variables[varName];
        std::bitset<sizeof(value) * 8> binary(value);
        out << 'b' << binary << ' ' << variable.id << std::endl;
    }
}
