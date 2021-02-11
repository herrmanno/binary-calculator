#pragma once

/**
 * Generic Identity
 * @param t the argument
 * @returns `t` for all t's
 */
template<typename T>
T identity(T t) {
    return t;
}
