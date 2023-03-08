#include <napi.h>
#include "stomptalk/parser.hpp"
#include "stomptalk/hook_base.hpp"

using namespace Napi;

struct NativeHook final
	: stomptalk::hook_base
{

private:
	virtual void on_frame(stomptalk::parser_hook&, const char *frame_start) noexcept override 
	{	}
 
    virtual void on_method(stomptalk::parser_hook&, 
        std::uint64_t method_id, const char*, std::size_t) noexcept override
	{   }
 
    virtual void on_hdr_key(stomptalk::parser_hook&, 
        std::uint64_t hader_id, const char*, std::size_t) noexcept override
	{   }
 
    virtual void on_hdr_val(stomptalk::parser_hook&, const char*, std::size_t) noexcept override
	{   }
 
    virtual void on_body(stomptalk::parser_hook&, const void*, std::size_t) noexcept override
	{   }
 
    virtual void on_frame_end(stomptalk::parser_hook&, const char *frame_end) noexcept override
	{   }
};

class NativeStomptalk final
	: public ObjectWrap<NativeStomptalk>
{	
public:
    NativeStomptalk(const Napi::CallbackInfo& callbackInfo)
		: ObjectWrap(callbackInfo)
	{   }
   
	static Napi::Object Init(Napi::Env env, Napi::Object exports) {
  		Napi::Function func = DefineClass(env, "NativeStomptalk", {
			InstanceMethod("parse", &NativeStomptalk::parse)
		});

		Napi::FunctionReference* constructor = new Napi::FunctionReference();
		*constructor = Napi::Persistent(func);
  		env.SetInstanceData(constructor);

  		exports.Set("NativeStomptalk", func);
  		return exports;
	}

    ~NativeStomptalk()
	{   }

private:
	Napi::Value parse(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
		Napi::Function cb = info[0].As<Napi::Function>();
  		cb.Call(env.Global(), { NativeStomptalk::GetValue(info) });
		return Napi::Number::New(info.Env(), 0);
	}

	Napi::Value GetValue(const Napi::CallbackInfo& info) {
  		return Napi::Number::New(info.Env(), 42);
	}
};

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  return NativeStomptalk::Init(env, exports);
}

NODE_API_MODULE(addon, Init)