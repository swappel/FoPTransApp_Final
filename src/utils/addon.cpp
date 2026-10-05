#include <napi.h>
#include <string>

std::string TestTranslation(const std::string& input)
{
    return "Some sthi was called here: " + input;
}

Napi::Value TranslateWrapper(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsString())
    {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string input = info[0].As<Napi::String>().Utf8Value();
    std::string result = TestTranslation(input);

    return Napi::String::New(env, result);
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "translateAsset"),
                Napi::Function::New(env, TranslateWrapper));
    return exports;
}

NODE_API_MODULE(FoPTransApp_Final, Init);