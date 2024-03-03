# Overview

[![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/pribault/ReCpp/cmake-multi-platform.yml?branch=develop)
](https://github.com/pribault/ReCpp/actions)
[![Maintenance](https://img.shields.io/badge/Maintained%3F-yes-green.svg)](https://github.com/pribault/ReCpp/graphs/commit-activity)
[![GitHub License](https://img.shields.io/github/license/pribault/ReCpp)](https://github.com/pribault/ReCpp/blob/develop/LICENSE)
![](https://img.shields.io/badge/C%2B%2B-17-blue)

Welcome to **ReCpp**, a sophisticated C++ library for reactive programming that builds on the [Reactive Streams standard](https://www.reactive-streams.org/) and leverages the [ReactiveX](https://reactivex.io/) model.

This library, powered by the [RsCpp](https://github.com/pribault/RsCpp) implementation of the Reactive Streams standard, empowers developers to create clean, concise, and efficient asynchronous and event-driven code with ease.

## Features

- **Reactive Streams Standard:** ReCpp is built on top of the RsCpp implementation of the Reactive Streams standard, providing a consistent and interoperable API for handling asynchronous data streams.

- **Functional Reactive Programming (FRP):** Embrace the paradigm of FRP to model complex asynchronous workflows with a clear and functional approach.

- **Comprehensive Operators:** Enjoy a rich set of operators inspired by the ReactiveX model, enabling effortless manipulation and transformation of data streams.

- **Backpressure Handling:** Efficiently manage backpressure to ensure your application remains responsive and scalable, even in high-load scenarios.

- **Extensibility:** Easily extend the library with your own custom operators or integrate it with existing codebases.

## Why use ReCpp over other asynchronous technologies ?

### Callback hell

ReCpp avoid the callback hell, when multiple callbacks are nested into each other. ReCpp allows you to chain your callbacks in a linear way and handle errors more easily. For example :

```cpp
// Callback hell
asyncFunction1(
    []()
    {
        asyncFunction2(
            []()
            {
                doSomething();
            },
            [](const auto &error)
            {
                std::cerr << error.what() << std::endl;
            });
    },
    [](const auto &error)
    {
        std::cerr << error.what() << std::endl;
    });

// ReCpp chaining
rxAsyncFunction1()
    .andThen(rxAsyncFunction2())
    .subscribe(
        []()
        {
            doSomething();
        },
        [](const auto &error)
        {
            std::cerr << error.what() << std::endl;
        });
```

### Coroutines

Even if c++20 coroutines offers a really great standard solution for asynchronous programming, ReCpp has some advantages over them:
- natively forward errors which leads to a higher resiliency
- faster to write
- easier to read
- bring powerful operators to create highly performant asynchronous code that would be hard to create with coroutines
- backpressure handling

## Supported platforms

- Windows
- MacOS
- Ubuntu

# Getting started

## Installation

You want to integrate ReCpp to your project ? Check our [installation guide](https://github.com/pribault/ReCpp/blob/develop/INSTALL.md)

## Documentation

Explore the [Documentation](https://pribault.github.io/ReCpp/index.html) to learn more about ReCpp.

## Interoperability

ReCpp can easily interoperate with other asynchronous programming systems. Here are some examples:

### Callbacks

```cpp
// The async function that uses callbacks
void asyncFunction(const std::function<void(int)> &onSuccess, const std::function<void(const std::exception_ptr &)> onError);

// The ReCpp equivalent
recpp::Single<int> rxAsyncFunctionWrapper()
{
    return recpp::Single<int>::create(
        [](auto &subscriber) // Lambda called on subscription
        {
            asyncFunction(
                [subscriber](int result) // Lambda called on asyncFunction success
                {
                    subscriber.onNext(result); // Emit the value and complete the Single
                },
                [](const auto &error) // Lambda called on asyncFunction error
                {
                    subscriber.onError(error); // Emit the error
                });
        });
}
```

### Coroutines

```cpp
// The async function that uses coroutines
task<std::expected<int, std::exception_ptr>> asyncFunction();

// The ReCpp equivalent
recpp::Single<int> rxAsyncFunctionWrapper()
{
    return recpp::Single<int>::create(
        [](auto &subscriber) // Lambda called on subscription
        {
            const auto result = asyncFunction().get();
            if (result.has_value())
                subscriber.onNext(result.value());
            else
                subscriber.onError(result.error());
        });
}
```

### RxCpp

```cpp
// The rxcpp function
rxcpp::observable<int> rxcppAsyncFunction();

// The ReCpp equivalent
recpp::Observable<int> recppAsyncFunction()
{
    return recpp::Observable<int>::create(
        [](auto &subscriber) // Lambda called on subscription
        {
            rxcppAsyncFunction()
                .subscribe(
                    [subscriber](auto value)
                    {
                        subscriber.onNext(value);
                    },
                    [subscriber](auto error)
                    {
                        subscriber.onError(error);
                    },
                    [subscriber]()
                    {
                        subscriber.onComplete();
                    }
                );
        });
}
```

# License

ReCpp is licensed under the MIT License. See the [LICENSE](https://github.com/pribault/ReCpp/blob/develop/LICENSE) file for details.

# Contributing

We welcome contributions from the community! Whether you want to report a bug, request a feature, or submit a pull request, please follow our [Contribution Guidelines](https://github.com/pribault/ReCpp/blob/develop/CODE_OF_CONDUCT.md).
