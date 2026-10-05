#include <napi.h>
#include <string>
#include <thread>

#include "utils/Backend.h"

static Backend g_backend;

Napi::Value LoadFilesWrapper(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 2 || !info[0].IsString() || !info[1].IsString())
    {
        Napi::TypeError::New(env, "Expected two string arguments (locpackPath, locpackbinPath)").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string locpackPath = info[0].As<Napi::String>().Utf8Value();
    std::string locpackbinPath = info[1].As<Napi::String>().Utf8Value();

    g_backend.loadFiles(locpackPath, locpackbinPath);

    return Napi::Boolean::New(env, true);
}

Napi::Value GetTotalLinesWrapper(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    return Napi::Number::New(env, g_backend.countLines());
}

Napi::Value VerifyFilesWrapper(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsFunction())
    {
        Napi::TypeError::New(env, "Expected progress callback function!").ThrowAsJavaScriptException();
        return env.Null();
    }

    Napi::Function jsCallback = info[0].As<Napi::Function>();
    auto tsfn = Napi::ThreadSafeFunction::New(env, jsCallback, "VerifyProgress", 0, 1);

    std::thread([](Napi::ThreadSafeFunction tsfn) {

        auto errors = g_backend.verify([tsfn](int currentLine) {
            tsfn.BlockingCall([currentLine](Napi::Env env, Napi::Function jsCb) {
                jsCb.Call({ Napi::Number::New(env, currentLine) });
            });
        });

        tsfn.Release();
    }, tsfn).detach();

    return Napi::Boolean::New(env, true);

}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "loadFiles"), Napi::Function::New(env, LoadFilesWrapper));
    exports.Set(Napi::String::New(env, "getTotalLines"), Napi::Function::New(env, GetTotalLinesWrapper));
    exports.Set(Napi::String::New(env, "verifyFiles"), Napi::Function::New(env, VerifyFilesWrapper));

    return exports;
}

NODE_API_MODULE(FoPTransApp_Final, Init);