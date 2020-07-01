//
// Created by Yoni Shperling on 23/07/2017.
//

#ifndef PILO_DELL_H
#define PILO_DELL_H


#include "ILO.h"

class DELL : public ILO {
public:

    DELL(const std::string &name, const std::string &user, const std::string &pass);

    DELL(const DELL &) = default;              // copy constructor
    DELL &operator=(const DELL &) = default;  // copy assignment
    DELL(DELL &&) = default;              // move constructor
    DELL &operator=(DELL &&) = default;

    ~DELL() override;

    void getState() override;

    void coldBoot() override;
};


#endif //PILO_DELL_H
