
/*
 * File:   Debug.h
 * Author: Frantisek Brablik
 *
 * Created on 5. května 2019, 23:29
 */

#pragma once

#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

template<typename Thing>
void dbg(const Thing& thing) {
    std::cout << thing;
}

template<typename Thing>
void dbgln(const Thing& thing) {
    std::cout << thing << "\n";
}

template<typename Thing, typename ... Args>
void dbgln(const Thing& thing, Args ... args) {
    dbg(thing);
    dbg(" ");
    dbgln(args...);
}

#endif /* DEBUG_H */

