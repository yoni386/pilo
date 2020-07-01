//
// Created by Yoni Shperling on 23/07/2017.
//

#include <cpr/cpr.h>
#include "DELL.h"
//#include "json.hpp"
#include "spdlog/spdlog.h"

#include "rapidjson/document.h"

using namespace rapidjson;

using std::string;

DELL::DELL(const string &name, const string &user, const string &pass) : ILO(name, user, pass)
{}

DELL::~DELL()
{

    auto r = cpr::Delete(cpr::Url{https + this->getName() + this->getLocation()},
                         cpr::Header{{XAuthToken, this->getToken()}},
                         cpr::VerifySsl{false}
    );

    spdlog::get("console")->debug("Host: {} Operation ~DELL state: {}", this->getName(), r.status_code);

    this->setLocation("");
    this->setToken("");
}

void DELL::getState()
{
    auto r = cpr::Get(cpr::Url{https + this->getName() + "/redfish/v1/Systems/System.Embedded.1"},
                      cpr::Header{{XAuthToken,  this->getToken()},
                                  {ContentType, app_json}},
                      cpr::VerifySsl{false}
    );

    if (r.status_code == 200) {
//        auto json = nlohmann::json::parse(r.text);
//        std::cout << "Host: " << this->getName() << " Operation code: " << r.status_code << " PowerState: "
//                  << json["PowerState"] << " Status: " << json["Status"]["Health"] << std::endl;
//        spdlog::get("console")->info("Host: {} Operation getState code: {} PowerState: {} Status: {}", this->getName(), r.status_code, json["PowerState"], json["Status"]["Health"]);

//        string power = json["PowerState"];
//        string health = json["Status"]["Health"];


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

void DELL::coldBoot()
{
    this->powerOff();
    std::this_thread::sleep_for(std::chrono::seconds(6));
    this->powerON();
}

