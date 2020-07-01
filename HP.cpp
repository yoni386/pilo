//
// Created by Yoni Shperling on 23/07/2017.
//

#include <iostream>
#include <cpr/cpr.h>
#include "HP.h"
//#include "json.hpp"
#include "spdlog/spdlog.h"
#include "rapidjson/document.h"

using std::string;

using namespace rapidjson;


HP::HP(const string& name, const string& user, const string& pass) : ILO(name, user, pass)
{}

HP::~HP() {
    auto r = cpr::Delete(cpr::Url{this->getLocation()},
                         cpr::Header{{XAuthToken, this->getToken()}},
                         cpr::VerifySsl{false}
    );

//    std::cout << "Host: " << this->getName() << " Operation destr state: " << r.status_code << std::endl;
    spdlog::get("console")->debug("Host: {} Operation ~HP state: {}", this->getName(), r.status_code);

    this->setLocation("");
    this->setToken("");
}

void HP::coldBoot() {

    auto r = cpr::Post(cpr::Url{https + this->getName() + ACTION_COLD_BOOT},
                       cpr::Header{{XAuthToken,  this->getToken()},
                                   {ContentType, app_json}},
                       cpr::Body{R"({"ResetType":"ColdBoot"})"},
                       cpr::VerifySsl{false}
    );

//    std::cout << "Host: " << this->getName() << " Operation cold_boot state: " << r.status_code << std::endl;
    spdlog::get("console")->debug("Host: {} Operation cold_boot state: {}", this->getName(), r.status_code);

}

void HP::getState() {
//    using nlohmann::json;
    auto r = cpr::Get(cpr::Url{https + this->getName() + "/redfish/v1/Systems/1/"},
                      cpr::Header{{XAuthToken,  this->getToken()},
                                  {ContentType, app_json}},
                      cpr::VerifySsl{false}
    );

//    auto json1 = json::parse(r.text);
//    std::cout << json.dump(4) << std::endl;
//    std::cout << "PowerState " << json["PowerState"] << std::endl;
//    std::cout << json << std::endl;
//    std::cout << json["PowerState"] << std::endl;
//
//    std::cout << "Host: " << this->getName() << " Operation code: " << r.status_code << " PowerState: "
//              << json1["PowerState"] << " Status: " << json1["Status"]["Health"] << std::endl;

//    spdlog::get("console")->info("Host: {} Operation getState code: {} PowerState: {} Status: {}", this->getName(), r.status_code, json1["PowerState"], json1["Status"]["Health"]);

    if (r.status_code == 200) {
        Document json;

//        auto json = nlohmann::json::parse(r.text);

        json.Parse(r.text.c_str()).HasParseError();

//        std::cout << "Host: " << this->getName() << " Operation code: " << r.status_code << " PowerState: "
//                  << json["PowerState"] << " Status: " << json["Status"]["Health"] << std::endl;
//        spdlog::get("console")->info("Host: {} Operation getState code: {} PowerState: {} Status: {}", this->getName(), r.status_code, json["PowerState"], json["Status"]["Health"]);

        string power = json["PowerState"].GetString();
        string health = json["Status"]["Health"].GetString();

        spdlog::get("console")->info("Host: {} Operation getState code: {} PowerState: {} Status: {}", this->getName(), r.status_code, power, health);

    } else {
//        std::cout << "Host: " << this->getName() << " Operation code: " << r.status_code << std::endl;
        spdlog::get("console")->warn("Host: {} Operation getState state: {}", this->getName(), r.status_code);
    };
}




