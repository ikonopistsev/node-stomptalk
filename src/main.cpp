#include <napi.h>
#include "stomptalk/parser.hpp"
#include "stomptalk/hook_base.hpp"

using namespace Napi;

class NativeStomptalk final
	: public ObjectWrap<NativeStomptalk>
	, public stomptalk::hook_base
{
	virtual void on_frame(stomptalk::parser_hook&, const char *frame_start) noexcept override 
	{	}
 
    virtual void on_method(stomptalk::parser_hook&, 
        std::uint64_t method_id, const char*, std::size_t) noexcept override
	{   }
 
    virtual void on_hdr_key(stomptalk::parser_hook&, 
        std::uint64_t hader_id, const char*, std::size_t) noexcept override
	{

	}
 
    virtual void on_hdr_val(stomptalk::parser_hook&, const char*, std::size_t) noexcept override
	{

	}
 
    virtual void on_body(stomptalk::parser_hook&, const void*, std::size_t) noexcept override
	{

	}
 
    virtual void on_frame_end(stomptalk::parser_hook&, const char *frame_end) noexcept override
	{

	}

	double value_;
	Napi::Function onFrame_{};
	// Napi::Function onMethod_{};
	// Napi::Function onHeaderKey_{};
	// Napi::Function onHeaderVal_{};
	// Napi::Function onFrameEnd_{};

		
public:
    NativeStomptalk(const Napi::CallbackInfo& callbackInfo)
		: ObjectWrap(callbackInfo)
		, value_(callbackInfo[0].As<Napi::Number>().DoubleValue())
		, onFrame_(callbackInfo[1].As<Napi::Function>())
		// , onMethod_(callbackInfo[2].As<Napi::Function>())
		// , onHeaderKey_(callbackInfo[3].As<Napi::Function>())
		// , onHeaderVal_(callbackInfo[4].As<Napi::Function>())
		// , onFrameEnd_(callbackInfo[5].As<Napi::Function>())
	{   

	}
   
	static Napi::Object Init(Napi::Env env, Napi::Object exports) {
  		Napi::Function func = DefineClass(env, "NativeStomptalk", {
			InstanceMethod("plusOne", &NativeStomptalk::PlusOne),
            InstanceMethod("value", &NativeStomptalk::GetValue),
			InstanceMethod("onFrame", &NativeStomptalk::onFrame),
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
	Napi::Value PlusOne(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
		Napi::Function cb = info[0].As<Napi::Function>();
		this->value_ = this->value_ + 1;
  		cb.Call(env.Global(), {NativeStomptalk::GetValue(info)});
		return NativeStomptalk::GetValue(info);
	}

	Napi::Value GetValue(const Napi::CallbackInfo& info) {
 	 double num = this->value_;

  		return Napi::Number::New(info.Env(), num);
	}

	Napi::Value onFrame(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
  		onFrame_.Call(env.Global(), {NativeStomptalk::GetValue(info)});
		return NativeStomptalk::GetValue(info);
	}
};

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  return NativeStomptalk::Init(env, exports);
}

NODE_API_MODULE(addon, Init)