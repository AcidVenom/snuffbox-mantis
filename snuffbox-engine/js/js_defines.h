#pragma once

#define JS_FUNCTION_REG(x) { js_ ## x ## _name_, JS ## x ## }
#define JS_FUNCTION_REG_END { nullptr, nullptr }

#ifdef SNUFF_JAVASCRIPT

#define JS_FUNCTION_DECL(x)\
static void JS ## x ## (const v8::FunctionCallbackInfo<v8::Value>& info);\
static const char* js_ ## x ## _name_;

#define JS_FUNCTION_IMPL(type, x, body)\
void type::JS ## x ## (const v8::FunctionCallbackInfo<v8::Value>& info) ## body ##\
const char* type::js_ ## x ## _name_ = #x

#else
#define JS_FUNCTION_DECL(x)
#define JS_FUNCTION_IMPL(type, x, body)
#endif