#pragma once

#include <future>

template <typename T, typename F>
auto then(std::shared_future<T> fut, F&& f) {
    return std::async(std::launch::async, [fut, f = std::forward<F>(f)]() mutable {
        return f(fut.get());
    });
}
