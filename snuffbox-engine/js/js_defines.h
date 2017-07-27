#pragma once

#define JS_FUNCTION_REG(x) { js_ ## x ## _name_, JS ## x }
#define JS_FUNCTION_REG_END { nullptr }

#ifdef SNUFF_JAVASCRIPT

#include "js_object.h"
#include "js_wrapper.h"

#define JS_BODY(...) __VA_ARGS__

#define JS_FUNCTION_DECL(x) \
static void JS ## x (const v8::FunctionCallbackInfo<v8::Value>& args); \
static const char* js_ ## x ## _name_

#define JS_FUNCTION_IMPL(type, x, body) \
void type::JS ## x (const v8::FunctionCallbackInfo<v8::Value>& args) body \
const char* type::js_ ## x ## _name_ = #x

#define JS_OBJECT public JSObject
#define JS_OBJECT_MULTI public JSObject,

#define JS_REGISTER_DECL_SINGLE static void RegisterJS(const v8::Local<v8::Object>& obj)
#define JS_REGISTER_IMPL_SINGLE(type, body) void type::RegisterJS(const v8::Local<v8::Object>& obj) body

#define JS_REGISTER_DECL_TMPL static void RegisterJS(const v8::Local<v8::FunctionTemplate>& func, const v8::Local<v8::ObjectTemplate>& obj)
#define JS_REGISTER_IMPL_TMPL(type, body) void type::RegisterJS(const v8::Local<v8::FunctionTemplate>& func, const v8::Local<v8::ObjectTemplate>& obj) body

#define JS_NAME(x) static const char* js_name(){ return #x; }
#define JS_NAME_SINGLE(x) JS_NAME(x); JS_REGISTER_DECL_SINGLE
#define JS_NAME_TMPL(x) JS_NAME(x); JS_REGISTER_DECL_TMPL; x ## (const v8::FunctionCallbackInfo<v8::Value>& args)

#define JS_CONSTRUCTOR(type, body) type:: ## type (const v8::FunctionCallbackInfo<v8::Value>& args) body

#else

#define JS_FUNCTION_DECL(x)
#define JS_FUNCTION_IMPL(type, x, body)

#define JS_OBJECT
#define JS_OBJECT_MULTI

#define JS_NAME_SINGLE(x)
#define JS_NAME_TMPL(x)

#define JS_REGISTER_IMPL_SINGLE(type, body)
#define JS_REGISTER_IMPL_TMPL(type, body)

#define JS_CONSTRUCTOR(type, body)

#endif
