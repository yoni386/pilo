//
// Created by Yoni Shperling on 23/07/2017.
//

#ifndef PILO_HP_H
#define PILO_HP_H

#include "ILO.h"

class HP : public ILO {
public:

    HP(const std::string& name, const std::string& user, const std::string& pass);

    HP(const HP &) = default;              // copy constructor
    HP &operator=(const HP &) = default;  // copy assignment
    HP(HP &&) = default;              // move constructor
    HP &operator=(HP &&) = default;

    ~HP() override;

    void getState() override;

    void coldBoot() override;

};


#endif //PILO_HP_H
