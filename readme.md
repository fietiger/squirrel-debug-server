# [S]quirrel Debugger

![CMake Build](https://github.com/leweaver/squirrel-debug-server/actions/workflows/cmake.yml/badge.svg)

## Release Notes - v0.1
First versioned release, 'MVP'

[x] Websocket listener for server state changes
[x] HTTP Command interface
[x] Stack-Local variables
[x] Global Variables
[x] Simple Breakpoints
[x] Output redirection & capture
[x] Coding standards and formatting
[x] Better debug logging
[x] Run sample exe with arguments
[ ] Hover-evaluation
[ ] Copyright headers

## Not Supported
[ ] Multiple VM's (threads)
[ ] Improved output of variables in inspector
[ ] Conditional breakpoints
[ ] Modification of variable values (int and string only?)
[ ] Immediate window for execution
[ ] MacOS / Linux support
[ ] squirrel unicode builds

Setup:

1. download and install any toolchain dependencies (see following toolchains section)
1. restart your IDE if it was already open
1. open the top-level CMakeLists.txt in either CLion or Visual Studio
1. Set up the toolchain

# Building Sample from source using CMake
Currently, only support building on windows using CMake. This requires that you have Visual Studio 2019 installed.

To build:
open a "Developer Command Prompt for VS 2019" (via start menu) and change to the repository directory.

```
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --target "sample_app"
```

`sample_app.exe` will now exist in the `build/

# Embedding the debugger in your application
The repository contains a few CMake targets:

- *interfaces* Contains 2 pure-virtual interfaces that are implemented by the following 2 projects.
    - `MessageEventInterface`, allowing the Debugger to send messages to the debug user interface.
    - `MessageCommandInterface`, which handles debug commands from the debug user interface. 
- *embedded_server* 
    - uses OATPP to host an HTTP server. This acts as an endpoint for debug user interfaces to control the application. 
    - Provides an implementation of `MessageEventInterface`.
- *squirrel_debugger* 
    - Implementation of the Squirrel Debug API. Uses semaphores to lock the squirrel execution thread at breakpoints, and provides access to VM stack & variables.
    - Provides an implementation of `MessageCommandInterface`.
- *sample_app* 
    - Shows an example of how to embed the debugger in an application that runs a Quirrel script.

To use the debugger in your application, you will need to take a static library dependency on the `sdb::embedded_server` and `sdb::squirrel_debugger` targets. The easiest way to do this is with CMake FetchContent. In your `CMakeLists.txt`:

```Cmake
include(FetchContent)

FetchContent_Declare(
    sdb
    GIT_REPOSITORY https://github.com/leweaver/squirrel-debug-server.git
    # Optionally pin to a specific version
    # GIT_TAG v0.1.0
    GIT_TAG origin/main
)
FetchContent_MakeAvailable(sdb)

target_link_libraries(${PROJECT_NAME}
        sdb::embedded_server
        sdb::squirrel_debugger)
```

The provided `sample_app` source code shows fleshed out examples; but a detailed list of steps you need to take are:

## Create Instances
1. Initialize the global environment for the embedded server (`EmbeddedServer::InitEnvironment()`)
1. Create an `EmbeddedServer` instance (`embeddedServer_`), giving it a port number to listen to network requests on.
1. Create an `SquirrelDebugger` instance (`squirrelDebugger_`)
1. Set the `MessageCommandInterface` member of the EmbeddedServer to the SquirrelDebugger instance (`embeddedServer_->SetCommandInterface(squirrelDebugger)`)
1. Set the `MessageEventInterface` member of the SquirrelDebugger to the EmbeddedServer instance. (`squirrelDebugger->SetEventInterface(embeddedServer_->GetEventInterface())`)
1. Start the `EmbeddedServer` so it listens to network requests. (`embeddedServer_->Start()`)

## Create Squirrel VM
1. Create your squirrel VM
1. Add your Squirrel VM to the debugger: `squirrelDebugger_->AddVm`
1. call `sq_setprintfunc` with a function pointer that can redirect print calls to the `squirrelDebugger_->SquirrelPrintCallback` method.
1. call `sq_setnativedebughook` with a function pointer that can redirect VM debug calls to the `squirrelDebugger_->SquirrelNativeDebugHook` method.
1. call `sq_enabledebuginfo` with SQTrue to turn on native debugging
1. You can now execute and debug squirrel code.

## Teardown
1. Call `embeddedServer_->Stop(true)` to request shutdown and join the network thread to wait for completion
1. Call `EmbeddedServer::ShutdownEnvironment()` to cleanup global resources.
