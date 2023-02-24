//
// Created by Yoni Shperling on 23/07/2017.
//


#include "ILO.h"
#include <cpr/cpr.h>
#include "spdlog/spdlog.h"

using std::string;

ILO::ILO(const string& name, const string& user, const string& pass) : name(name), user(user), pass(pass)
{
    const string userPass = string{R"({"UserName":")" + this->getUser() + R"(")" + ',' + R"("Password":")" + this->getPass() + "\"}"};

    auto r = cpr::Post(cpr::Url{https + this->name + SESSION},
//                       cpr::Body{R"({\"UserName\":root, "Password":"change_me"})"},
//                       cpr::Body{my_json.dump()},
                       cpr::Body{userPass},
                       cpr::Header{{"Content-Type", "application/json"}},
                       cpr::VerifySsl{false}
    );

//    std::cout << r.status_code << std::endl;

    this->setLocation(r.header.operator[]("Location"));
    this->setToken(r.header.operator[]("X-Auth-Token"));
}


const string &ILO::getName() const {
    return name;
}

void ILO::setName(const string &name) {
    ILO::name = name;
}

const string &ILO::getToken() const {
    return token;
}

const string &ILO::getUser() const {
    return user;
}

void ILO::setUser(const string &user) {
    ILO::user = user;
}

const string &ILO::getPass() const {
    return pass;
}

void ILO::setPass(const string &pass) {
    ILO::pass = pass;
}

void ILO::setToken(const string &token) {
    ILO::token = token;
}

const string &ILO::getLocation() const {
    return location;
}

void ILO::setLocation(const string &location) {
    ILO::location = location;
}

void ILO::powerON()
{
    auto r = cpr::Post(cpr::Url{https + this->getName() + ACTION_POWER},
                       cpr::Header{{XAuthToken,  this->getToken()},
                                   {ContentType, app_json}},
                       cpr::Body{R"({"ResetType":"On"})"},
                       cpr::VerifySsl{false}
    );

    spdlog::get("console")->debug("Host: {} Operation powerON state: {}", this->getName(), r.status_code);
}

void ILO::powerOff()
{
    auto r = cpr::Post(cpr::Url{https + this->getName() + ACTION_POWER},
                       cpr::Header{{XAuthToken,  this->getToken()},
                                   {ContentType, app_json}},
                       cpr::Body{R"({"ResetType":"ForceOff"})"},
//                       cpr::Body{R"({"ResetType":"PushPowerButton"})"},
                       cpr::VerifySsl{false}
    );

    spdlog::get("console")->debug("Host: {} Operation powerOff state: {}", this->getName(), r.status_code);
}

void ILO::warmBoot()
{
    auto r = cpr::Post(cpr::Url{https + this->getName() + ACTION_POWER},
                       cpr::Header{{XAuthToken,  this->getToken()},
                                   {ContentType, app_json}},
                       cpr::Body{R"({"ResetType":"GracefulRestart"})"},
                       cpr::VerifySsl{false}
    );

    spdlog::get("console")->debug("Host: {} Operation warmBoot state: {}", this->getName(), r.status_code);
}



