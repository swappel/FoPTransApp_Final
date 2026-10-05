#include <napi.h>
#include <string>

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

    // TODO: Point to logic here

    return Napi::Boolean::New(env, true);
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "loadFiles"),
                Napi::Function::New(env, LoadFilesWrapper));

    return exports;
}

NODE_API_MODULE(FoPTransApp_Final, Init);