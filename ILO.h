//
// Created by Yoni Shperling on 23/07/2017.
//

#ifndef PILO_ILO_H
#define PILO_ILO_H

#include <string>

static const char *const https{"https://"};
static const char *const ACTION_POWER{"/redfish/v1/Systems/System.Embedded.1/Actions/ComputerSystem.Reset/"};
static const char *const ACTION_COLD_BOOT{"/redfish/v1/Systems/1/Actions/Oem/Hp/ComputerSystemExt.SystemReset/"};
static const char *const SESSION_OLD_DEL{"/redfish/v1/SessionService/Sessions/"};
static const char *const SESSION{"/redfish/v1/Sessions"};
static const char *const XAuthToken{"X-Auth-Token"};
static const char *const ContentType{"Content-Type"};
static const char *const app_json{"application/json;charset=UTF-8"};

class ILO {
private:
    std::string name;
    std::string user;
    std::string pass;
    std::string token;
    std::string location;

public:
    ILO(const std::string& name, const std::string& user, const std::string& pass);

    ILO(const ILO &) = default;              // copy constructor
    ILO &operator=(const ILO &) = default;  // copy assignment
    ILO(ILO &&) = default;              // move constructor
    ILO &operator=(ILO &&) = default;  // move assignment
    virtual ~ILO() = default;

    const std::string &getLocation() const;

    const std::string &getName() const;

    const std::string &getUser() const;

    const std::string &getPass() const;

    const std::string &getToken() const;

    void setToken(const std::string &token);

    void setLocation(const std::string &location);

    void setName(const std::string &name);

    void setUser(const std::string &user);

    void setPass(const std::string &pass);

    void powerON();

    void powerOff();

    virtual void getState() = 0;

    virtual void coldBoot() = 0;

    void warmBoot();


};


#endif //PILO_ILO_H
