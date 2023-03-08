#include <napi.h>
#include "stomptalk/parser.hpp"
#include "stomptalk/hook_base.hpp"

using namespace Napi;

namespace {

class NativeHook final
	: public stomptalk::hook_base
{
	Napi::Env& env_;
	Napi::Function& onFrameStart_;
	Napi::Function& onMethod_;
	Napi::Function& onHeaderKey_;
	Napi::Function& onHeaderVal_;
	Napi::Function& onBody_;
	Napi::Function& onFrameEnd_;

public:
	NativeHook(Napi::Env& env,
		Napi::Function& onFrameStart, 
		Napi::Function& onMethod, 
		Napi::Function& onHeaderKey,
		Napi::Function& onHeaderVal, 
		Napi::Function& onBody,
		Napi::Function& onFrameEnd)
		: env_(env)
		, onFrameStart_(onFrameStart)
		, onMethod_(onMethod)
		, onHeaderKey_(onHeaderKey)
		, onHeaderVal_(onHeaderVal)
		, onBody_(onBody)
		, onFrameEnd_(onFrameEnd)
	{   }

private:
	void on_frame(stomptalk::parser_hook&, const char*) noexcept override 
	{	
		onFrameStart_.Call(env_.Global(), {});
	}
 
    void on_method(stomptalk::parser_hook&, 
        std::uint64_t method_id, const char* at, std::size_t size) noexcept override
	{   
		onMethod_.Call(env_.Global(), {
			Napi::String::New(env_, at, size)
		});
	}
 
    void on_hdr_key(stomptalk::parser_hook&, 
        std::uint64_t hader_id, const char* at, std::size_t size) noexcept override
	{   
		onHeaderKey_.Call(env_.Global(), {
			Napi::String::New(env_, at, size)
		});		
	}
 
    void on_hdr_val(stomptalk::parser_hook&, const char* at, std::size_t size) noexcept override
	{   
		onHeaderVal_.Call(env_.Global(), {
			Napi::String::New(env_, at, size)
		});
	}
 
    void on_body(stomptalk::parser_hook&, const void* at, std::size_t size) noexcept override
	{   
		auto ch = const_cast<char*>(static_cast<const char*>(at));
		onBody_.Call(env_.Global(), {
			Napi::Buffer<char>::New(env_, ch, size)
		});
	}
 
    void on_frame_end(stomptalk::parser_hook&, const char*) noexcept override
	{   
		onFrameEnd_.Call(env_.Global(), {});
	}
};

}

class NativeStomptalk final
	: public ObjectWrap<NativeStomptalk>
{
	stomptalk::parser parser_{};

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

private:
	Napi::Value parse(const Napi::CallbackInfo& info) {
		auto env = info.Env();
		auto inputBuffer = info[0].As<Napi::Buffer<char>>();
		auto onError = info[1].As<Napi::Function>();
		auto onFrameStart = info[2].As<Napi::Function>();
		auto onMethod = info[3].As<Napi::Function>();
		auto onHeaderKey = info[4].As<Napi::Function>();
		auto onHeaderVal = info[5].As<Napi::Function>();
		auto onBody = info[6].As<Napi::Function>();
		auto onFrameEnd = info[7].As<Napi::Function>();
		NativeHook user{env, onFrameStart, 
			onMethod, onHeaderKey, onHeaderVal, 
			onBody, onFrameEnd};
		stomptalk::parser_hook hook{user};
		auto length = inputBuffer.Length();
		auto size = parser_.run(hook, inputBuffer.Data(), length);
		if (size != length) {
			onError.Call(env.Global(), {
				Napi::Number::New(env, hook.error())
			});
		}

		return Napi::Number::New(env, size);
	}
};

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  return NativeStomptalk::Init(env, exports);
}

NODE_API_MODULE(addon, Init)