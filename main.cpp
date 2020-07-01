#include <iostream>
#include <vector>
#include <array>
#include <cpr/cpr.h>
//#include "json.hpp"
#include "ILO.h"
#include "HP.h"
#include "DELL.h"

#include <unordered_map>

#include "spdlog/spdlog.h"
#include <args/args.hxx>
//#include "rapidjson/document.h"
#include <ArduinoJson/ArduinoJson.h>

//#include <typeinfo>
using namespace std;

//using json = nlohmann::json;
//using namespace rapidjson;

enum class ILO_TYPE {
    ilo_hp, ilo_dell, unknown, error
};

ILO_TYPE getILOType(const string& name)
{

    auto r = cpr::Get(cpr::Url{https + name + "/redfish/v1/"},
                          cpr::Header{{"Content-Type", "application/json"}},
                          cpr::VerifySsl{false}
    );

    if (!r.error && r.status_code == 200) {

        StaticJsonBuffer<5500> jsonBuffer;
        JsonObject& jsonBufferObj = jsonBuffer.parseObject(r.text);

        if (!jsonBufferObj.success()) {
            spdlog::get("console")->warn("parseObject error Host: {} GET_ILO_TYPE http problem: {}", name, r.status_code);
            return ILO_TYPE::error;
        }

        if (jsonBufferObj["Oem"]["Hp"].success()) {
            spdlog::get("console")->debug("Host: {} is: hp fn GET_ILO_TYPE: {}", name, r.status_code);
            return ILO_TYPE::ilo_hp;
        }

        if (jsonBufferObj["AccountService"]["@odata.id"] == "/redfish/v1/Managers/iDRAC.Embedded.1/AccountService") {
            spdlog::get("console")->debug("Host: {} is: hp fn GET_ILO_TYPE: {}", name, r.status_code);
            return ILO_TYPE::ilo_dell;
        }
        else {
            spdlog::get("console")->debug("Host: {} is: unknown fn GET_ILO_TYPE: {}", name, r.status_code);
            return ILO_TYPE::unknown;
        }
    }
    else {
        spdlog::get("console")->warn("Host: {} GET_ILO_TYPE http problem: {} status is not 200", name, r.status_code);
        return ILO_TYPE::error;
    }
}


//template<typename T>
//void getFuture(T);
////template<typename T>
////int getFuture(auto);
//template<typename T>
//void getFutureVoid(vector<future<void>>& VF)

void getFutures(vector<future<void>>& VF)
{
    try {
        for (auto& F : VF)
            F.get();
    }
    catch (const exception& e) {
        cout << "getFuture exception: " << e.what() << endl;
    }
}

template<typename T>
const auto getFutures(T& VF)
{
    vector<shared_ptr<ILO>> hosts;
    hosts.reserve(VF.size()); // TODO size of vector

    try {
            for (auto& F : VF)
                try {
                        hosts.emplace_back(F.get());
            }
            catch (const exception& e) {
                cout << "host_future.get() exception: " << e.what() << endl;
            }
    }
    catch (const exception& e) {
        cout << "all_hosts_futures exception: " << e.what() << endl;
    }

    return hosts;
}

//shared_ptr<ILO> newILO(const string& name, const string& user, const string& pass)
unique_ptr<ILO> newILO(const string& name, const string& user, const string& pass)
{
    auto ilo_type = getILOType(name);
    switch(ilo_type) {
        case ILO_TYPE::ilo_hp: return make_unique<HP>(name, user, pass);
        case ILO_TYPE::ilo_dell: return make_unique<DELL>(name, user, pass);
        case ILO_TYPE::unknown: throw std::runtime_error("host is unknown host: " + name);
        case ILO_TYPE::error: throw std::runtime_error("error on host: " + name);
    }
}


//vector<future<ILO_TYPE>> Get_ILO_TYPE_Futures(const vector<string>& hosts_names)
//{
//    vector<future<ILO_TYPE>> ilo_type_futures;
//    for (const auto& host_name : hosts_names) {
//        try
//        {
//            ilo_type_futures.emplace_back(async(launch::async, &getILOType, host_name));
//        }
//        catch (const exception& e) // TODO check if required
//        {
//            cout << "get_hosts_futures exception: " << e.what() << endl;
//        }
//
//    };
//    return ilo_type_futures;
//};


//vector<future<shared_ptr<ILO>>> get_hosts_futures(const vector<string>& hosts_names)
vector<future<unique_ptr<ILO>>> get_hosts_futures(const vector<string>& hosts_names)
{
//    vector<future<shared_ptr<ILO>>> hosts_futures;
    vector<future<unique_ptr<ILO>>> hosts_futures;
    for (const auto& host_name : hosts_names)
        try {
            hosts_futures.emplace_back(async(launch::async, &newILO, host_name, "root", "password"));
        }
        catch (const exception& e) {
            cout << "get_hosts_futures exception: " << e.what() << endl;
        }

    return hosts_futures;
}

void all()
{
    // vector of all host_names strings
    const vector<string> hosts_names{
            "mac-dev-008-ilo.fqdn.local",
            "mac-dev-009-ilo.fqdn.local",
            "mac-dev-010-ilo.fqdn.local",
            "mac-dev-011-ilo.fqdn.local",
            "mac-dev-012-ilo.fqdn.local",
            "mac-dev-013-ilo.fqdn.local,
            "mac-dev-014-ilo.fqdn.local",
            "mac-dev-015-ilo.fqdn.local",
    };



    // vector of all shared_ptr ILO future
//    vector<shared_ptr<ILO>> hosts;

    auto hosts_futures = get_hosts_futures(hosts_names);
    // vector of all future void for VF for wait / block async
    vector<future<void>> VF;
    VF.reserve(hosts_futures.size()); // TODO size of vector

//    hosts.reserve(hosts_futures.size()); // TODO size of vector
    auto hosts = getFutures(hosts_futures);
//    try {
//        for (auto& host_future: hosts_futures)
//            try {
//                hosts.emplace_back(host_future.get());
//            }
//            catch (const exception& e) {
//                cout << "host_future.get() exception: " << e.what() << endl;
//            }
//    }
//    catch (const exception& e) {
//        cout << "all_hosts_futures exception: " << e.what() << endl;
//    }

    try {
        for (const auto& host : hosts)
            VF.emplace_back(async(launch::async, &ILO::getState, host));
//           vl.emplace_back(async(launch::async, &ILO::coldBoot, host));
    }
    catch (const exception& e) {
        cout << "all_hosts exception: " << e.what() << endl;
    }
//
//
//    try {
//        for (auto &element : VF)
//            element.get();
//    }
//    catch (const exception &e) {
//        std::cout << "try catch vl with exception" << std::endl;
//        cout << "vl exception: " << e.what() << endl;
//    }


//    getFutureVoid(VF);
    getFutures(VF);

}


void Status(const std::string &progname, std::vector<std::string>::const_iterator beginargs, std::vector<std::string>::const_iterator endargs);
void Cold(const std::string &progname, std::vector<std::string>::const_iterator beginargs, std::vector<std::string>::const_iterator endargs);

using commandtype = std::function<void(const std::string &, std::vector<std::string>::const_iterator, std::vector<std::string>::const_iterator)>;


int main(int argc, char **argv)
{


    std::unordered_map<std::string, commandtype> map{
            {"status", Status},
            {"cold", Cold}};


    const std::vector<std::string> args(argv + 1, argv + argc);
    args::ArgumentParser parser("This is pilo parallel remote ilo ", "Valid commands are status and cold");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    parser.Prog(argv[0]);
    parser.ProglinePostfix("{command options}");
    args::Flag version(parser, "version", "Show the version of this program", {"version"});
    args::Flag verbose(parser, "verbose", "be verbose", {'v', "verbose"});
    args::Flag debug(parser, "debug", "Set debug level", {'d', "debug"});

//    args::ValueFlag<std::string> htmlpath(parser, "html-path", "Specify the html path", {"html-path"});



    args::MapPositional<std::string, commandtype> command(parser, "command", "Command to execute [status or cold]", map);
    command.KickOut(true);
    try
    {
        auto next = parser.ParseArgs(args);

        if (verbose) {
            auto console = spdlog::stdout_color_mt("console");
            spdlog::set_pattern("[%l] [%d/%m/%C %H:%M:%S] [Process %P thread %t] %v");
        }
        else {
            auto console = spdlog::stdout_color_mt("console");
            spdlog::set_pattern("[%l] [%d/%m/%C %H:%M:%S] %v");
        }

        if (debug)
            spdlog::set_level(spdlog::level::debug); //Set global log level to info

//        spdlog::get("console")->error("LINE: {} Operation FILE: {}",__LINE__, __FUNCTION__);


//        std::cout << std::boolalpha;
//        std::cout << "Before command options:" << std::endl;
//        std::cout << "Version called: " << bool{version} << std::endl;
//        std::cout << "html-path called: " << bool{htmlpath} << ", value: " << args::get(htmlpath) << std::endl;
        if (command)
        {
            args::get(command)(argv[0], next, std::end(args));
        }
        else
        {
            std::cout << parser;
        }
    }
    catch (const args::Help&)
    {
        std::cout << parser;
        return 0;
    }
    catch (const args::Error& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }


    try
    {
//        all();

    }
    catch (const exception& e)
    {
        cout << e.what() << endl;
    }

    catch (...)
    {
        std::cout << "catch exiting with exception" << std::endl;
    }

    return 0;


}

void Status(const std::string &progname, std::vector<std::string>::const_iterator beginargs, std::vector<std::string>::const_iterator endargs)
{
//    std::cout << "In Status" << std::endl;
    args::ArgumentParser parser("");
    parser.Prog(progname + " status");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
//    args::ValueFlag<std::string> prefix(parser, "FQDN", "The prefix flag", {'p', "prefix"}, ".fqdn.local");
    args::ValueFlag<std::string> prefix(parser, "FQDN", "Set prefix FQDN [-ilo.fqdn.local]", {'p',}, "-ilo.fqdn.local");

    args::PositionalList<std::string> hosts_names(parser, "hosts", "array hosts");

    try
    {
        parser.ParseArgs(beginargs, endargs);
        if (hosts_names) {
            vector<string> hosts_address = args::get(hosts_names);

            std::string pr = args::get(prefix);

//            std::cout << "hosts: " << std::endl;

            for (auto& host : hosts_address)
//            spdlog::get("console")->debug("Host: {} Total of {}", host, hosts_address.size());
//                std::cout << " - " << host << std::endl;
//                spdlog::get("console")->debug("Host: {} is: hp fn GET_ILO_TYPE: {}", host, r.status_code);
//                spdlog::get("console").debug

                if (host.find('.') == std::string::npos)
                    host += pr;

            auto hosts_futures = get_hosts_futures(hosts_address);
            // vector of all future void for VF for wait / block async
            vector<future<void>> VF;
            VF.reserve(hosts_futures.size()); // TODO size of vector

//    hosts.reserve(hosts_futures.size()); // TODO size of vector
            auto hosts = getFutures(hosts_futures);
            try {
                for (const auto& host : hosts)
                    VF.emplace_back(async(launch::async, &ILO::getState, host));
            }
            catch (const exception& e) {
                cout << "all_hosts exception: " << e.what() << endl;
            }


            getFutures(VF);

        }
    }
    catch (const args::Help&)
    {
        std::cout << parser;
        return;
    }
    catch (const args::ParseError& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return;
    }
}

void Cold(const std::string &progname, std::vector<std::string>::const_iterator beginargs, std::vector<std::string>::const_iterator endargs)
{
    args::ArgumentParser parser("");
    parser.Prog(progname + " cold");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    args::Flag dryrun(parser, "dryrun", "dry run", {'n', "dry-run"});
    args::Flag verbose(parser, "verbose", "be verbose", {'v', "verbose"});
    args::ValueFlag<std::string> prefix(parser, "FQDN", "Set prefix FQDN [-ilo.fqdn.local]", {'p',}, "-ilo.fqdn.local");
//    args::PositionalList<std::string> hosts(parser, "hosts", "array hosts");
    args::PositionalList<std::string> hosts_names(parser, "hosts", "array hosts");


    try {
        parser.ParseArgs(beginargs, endargs);

        if (hosts_names) {
            auto hosts_names1 = args::get(hosts_names);

            std::string pr = args::get(prefix);

            std::cout << "hosts: " << std::endl;
            for (auto& host : hosts_names1) {
                std::cout << " - " << host << std::endl;
                if (!(host.find('.') != std::string::npos)) {
                    std::cout << "found!" << '\n';
                    host += pr;
                }
            }

            for (auto& host : hosts_names1) {
                std::cout << " -- " << host  << std::endl;
            }

            auto hosts_futures = get_hosts_futures(hosts_names1);
            vector<future<void>> VF;
            VF.reserve(hosts_futures.size()); // TODO size of vector

            auto hosts = getFutures(hosts_futures);
            try {
                for (const auto& host : hosts)
                    VF.emplace_back(async(launch::async, &ILO::coldBoot, host));
            }
            catch (const exception& e) {
                cout << "all_hosts exception: " << e.what() << endl;
            }

            getFutures(VF);
        }
    }
    catch (const args::Help&) {
        std::cout << parser;
        return;
    }
    catch (const args::ParseError& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return;
    }
}
