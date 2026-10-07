#include <napi.h>
#include <string>
#include <thread>
#include <mutex>

#include "utils/Backend.h"

static Backend g_backend;
static std::mutex g_backend_mutex;

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

    std::lock_guard<std::mutex> lock(g_backend_mutex);
    g_backend.loadFiles(locpackPath, locpackbinPath);

    return Napi::Boolean::New(env, true);
}

Napi::Value GetTotalLinesWrapper(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    std::lock_guard<std::mutex> lock(g_backend_mutex);
    return Napi::Number::New(env, g_backend.countLines());
}

Napi::Value VerifyFilesWrapper(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 2 || !info[0].IsFunction() || !info[1].IsFunction())
    {
        Napi::TypeError::New(env, "Expected progress callback and completion callback!").ThrowAsJavaScriptException();
        return env.Null();
    }

    Napi::Function progressCb = info[0].As<Napi::Function>();
    Napi::Function doneCb = info[1].As<Napi::Function>();

    auto tsfnProgress = Napi::ThreadSafeFunction::New(env, progressCb, "VerifyProgress", 0, 1);
    auto tsfnDone = Napi::ThreadSafeFunction::New(env, doneCb, "VerifyDone", 0, 1);

    std::thread([tsfnProgress, tsfnDone]() mutable {
        std::vector<int> errors;
        bool success = true;
        std::string errMsg;

        try {
            std::lock_guard<std::mutex> lock(g_backend_mutex);
            errors = g_backend.verify([tsfnProgress](int currentLine) {
                tsfnProgress.NonBlockingCall([currentLine](Napi::Env env, Napi::Function jsCb) {
                    if (jsCb.IsFunction()) {
                        jsCb.Call({ Napi::Number::New(env, currentLine) });
                    }
                });
            });
        } catch (const std::exception& e) {
            success = false;
            errMsg = e.what();
        } catch (...) {
            success = false;
            errMsg = "Unknown native error during verification";
        }

        tsfnDone.NonBlockingCall([success, errors, errMsg](Napi::Env env, Napi::Function jsCb) {
            if (jsCb.IsFunction()) {
                Napi::Object res = Napi::Object::New(env);
                res.Set("success", Napi::Boolean::New(env, success));

                Napi::Array errArr = Napi::Array::New(env, errors.size());
                for (size_t i = 0; i < errors.size(); i++) {
                    errArr.Set(i, Napi::Number::New(env, errors[i]));
                }
                res.Set("errors", errArr);

                if (!success) {
                    res.Set("error", Napi::String::New(env, errMsg));
                }

                jsCb.Call({ res });
            }
        });

        tsfnProgress.Release();
        tsfnDone.Release();
    }).detach();

    return Napi::Boolean::New(env, true);
}

Napi::Value GetLinesRangeWrapper(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 2 || !info[0].IsNumber() || !info[1].IsNumber()) {
        Napi::TypeError::New(env, "Expected start index and count integers").ThrowAsJavaScriptException();
        return env.Null();
    }

    int startIndex = info[0].As<Napi::Number>().Int32Value();
    int count = info[1].As<Napi::Number>().Int32Value();

    std::lock_guard<std::mutex> lock(g_backend_mutex);
    auto lines = g_backend.getLinesRange(startIndex, count);

    Napi::Array arr = Napi::Array::New(env, lines.size());
    for (size_t i = 0; i < lines.size(); ++i) {
        Napi::Object obj = Napi::Object::New(env);
        obj.Set("hash", lines[i].getHash());
        obj.Set("content", lines[i].getContent());

        Napi::Array fieldsArr = Napi::Array::New(env, lines[i].getFields().size());
        for (size_t j = 0; j < lines[i].getFields().size(); ++j) {
            fieldsArr.Set(j, Napi::Number::New(env, lines[i].getFields()[j]));
        }
        obj.Set("fields", fieldsArr);

        arr.Set(i, obj);
    }

    return arr;
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "loadFiles"), Napi::Function::New(env, LoadFilesWrapper));
    exports.Set(Napi::String::New(env, "getTotalLines"), Napi::Function::New(env, GetTotalLinesWrapper));
    exports.Set(Napi::String::New(env, "verifyFiles"), Napi::Function::New(env, VerifyFilesWrapper));
    exports.Set(Napi::String::New(env, "getLinesRange"), Napi::Function::New(env, GetLinesRangeWrapper));

    return exports;
}

NODE_API_MODULE(FoPTransApp_Final, Init);