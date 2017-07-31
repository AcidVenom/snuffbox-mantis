#include "input.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		Input::Input() :
			last_type_(InputType::kKeyboard)
		{

		}

		//-----------------------------------------------------------------------------------------------
		bool Input::KeyboardAny(Keyboard::KeyState state) const
		{
			for (KeyCodes::KeyCode i = 0; i < Keyboard::KEY_LENGTH_; ++i)
			{
				if (keyboard_.states_[i] == state || (state == Keyboard::KeyState::kDown && keyboard_.states_[i] == Keyboard::KeyState::kPressed))
				{
					return true;
				}
			}

			return false;
		}

		//-----------------------------------------------------------------------------------------------
		void Input::Update()
		{
			unsigned int count = keyboard_.Flush();
			last_type_ = count > 0 ? InputType::kKeyboard : last_type_;
		}

		//-----------------------------------------------------------------------------------------------
		bool Input::KeyboardPressed(KeyCodes::KeyCode key) const
		{
			if (key == KeyCodes::kAny)
			{
				return KeyboardAny(Keyboard::KeyState::kPressed);
			}

			Keyboard::KeyState state = keyboard_.states_[key];
			return state == Keyboard::KeyState::kPressed;
		}

		//-----------------------------------------------------------------------------------------------
		bool Input::KeyboardDown(KeyCodes::KeyCode key) const
		{
			if (key == KeyCodes::kAny)
			{
				return KeyboardAny(Keyboard::KeyState::kDown);
			}

			Keyboard::KeyState state = keyboard_.states_[key];
			return state == Keyboard::KeyState::kDown || state == Keyboard::KeyState::kPressed;
		}

		//-----------------------------------------------------------------------------------------------
		bool Input::KeyboardReleased(KeyCodes::KeyCode key) const
		{
			if (key == KeyCodes::kAny)
			{
				return KeyboardAny(Keyboard::KeyState::kReleased);
			}

			Keyboard::KeyState state = keyboard_.states_[key];
			return state == Keyboard::KeyState::kReleased;
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode Input::LastKeyboardPressed() const
		{
			return keyboard_.last_[static_cast<int>(Keyboard::KeyState::kPressed)];
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode Input::LastKeyboardDown() const
		{
			return keyboard_.last_[static_cast<int>(Keyboard::KeyState::kDown)];
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode Input::LastKeyboardReleased() const
		{
			return keyboard_.last_[static_cast<int>(Keyboard::KeyState::kReleased)];
		}

		//-----------------------------------------------------------------------------------------------
		void Input::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			Input* input = static_cast<Input*>(&Services::Get<InputService>());

			Keyboard::Event evt;
			switch (action)
			{
			case GLFW_PRESS:
				evt.type = Keyboard::Event::Type::kPressed;
				break;

			case GLFW_REPEAT:
				evt.type = Keyboard::Event::Type::kRepeat;
				break;

			case GLFW_RELEASE:
				evt.type = Keyboard::Event::Type::kReleased;
				break;
			}

			evt.key = static_cast<KeyCodes::KeyCode>(key);

			input->keyboard_.PostEvent(evt);
		}

		//-----------------------------------------------------------------------------------------------
		JS_REGISTER_IMPL_SINGLE(Input, JS_BODY(
		{
			JSFunctionRegister funcs[] = {
				JS_FUNCTION_REG(keyboardPressed),
				JS_FUNCTION_REG(keyboardDown),
				JS_FUNCTION_REG(keyboardReleased),
				JS_FUNCTION_REG(lastKeyboardPressed),
				JS_FUNCTION_REG(lastKeyboardDown),
				JS_FUNCTION_REG(lastKeyboardReleased),
				JS_FUNCTION_REG(mousePosition),
				JS_FUNCTION_REG(mouseMovement),
				JS_FUNCTION_REG(mouseWheelDelta),
				JS_FUNCTION_REG(mousePressed),
				JS_FUNCTION_REG(mouseDown),
				JS_FUNCTION_REG(mouseReleased),
				JS_FUNCTION_REG(lastMousePressed),
				JS_FUNCTION_REG(lastMouseDown),
				JS_FUNCTION_REG(lastMouseReleased),
				JS_FUNCTION_REG(isControllerConnected),
				JS_FUNCTION_REG(connectedControllers),
				JS_FUNCTION_REG(controllerAxis),
				JS_FUNCTION_REG(controllerPressed),
				JS_FUNCTION_REG(controllerDown),
				JS_FUNCTION_REG(controllerReleased),
				JS_FUNCTION_REG(lastControllerPressed),
				JS_FUNCTION_REG(lastControllerDown),
				JS_FUNCTION_REG(lastControllerReleased),
				JS_FUNCTION_REG_END
			};

			v8::Local<v8::Object> key_enum = JSWrapper::CreateObject();
			v8::Local<v8::Object> mb_enum = JSWrapper::CreateObject();
			v8::Local<v8::Object> control_enum = JSWrapper::CreateObject();
			v8::Local<v8::Object> input_type_enum = JSWrapper::CreateObject();

			KeyCodes::KeyCodeLoop([=](KeyCodes::KeyCode ch, const char* name) 
			{ 
				JSWrapper::SetObjectValue(key_enum, name, static_cast<unsigned int>(ch)); 
			});

			MouseButtons::MouseButtonsLoop([=](KeyCodes::KeyCode ch, const char* name)
			{ 
				JSWrapper::SetObjectValue(mb_enum, name, static_cast<unsigned int>(ch)); 
			});

			ControllerButtons::ControllerButtonsLoop([=](KeyCodes::KeyCode ch, const char* name) 
			{ 
				JSWrapper::SetObjectValue(control_enum, name, static_cast<unsigned int>(ch)); 
			});

			JSWrapper::SetObjectValue(control_enum, "LeftAxis", static_cast<unsigned int>(ControllerButtons::Axes::kLeftAxis));
			JSWrapper::SetObjectValue(control_enum, "RightAxis", static_cast<unsigned int>(ControllerButtons::Axes::kRightAxis));
			JSWrapper::SetObjectValue(control_enum, "LT", static_cast<unsigned int>(ControllerButtons::Axes::kLT));
			JSWrapper::SetObjectValue(control_enum, "RT", static_cast<unsigned int>(ControllerButtons::Axes::kRT));

			JSWrapper::SetObjectValue(input_type_enum, "Keyboard", static_cast<unsigned int>(InputType::kKeyboard));
			JSWrapper::SetObjectValue(input_type_enum, "Controller", static_cast<unsigned int>(InputType::kController));

			JSWrapper::RegisterGlobal("Keyboard", key_enum, true);
			JSWrapper::RegisterGlobal("Mouse", mb_enum, true);
			JSWrapper::RegisterGlobal("Controller", control_enum, true);
			JSWrapper::RegisterGlobal("InputType", input_type_enum, true);

			JSFunctionRegister::Register(funcs, obj);
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, keyboardPressed, JS_BODY({
			
			JSWrapper wrapper(args);
			
			if (wrapper.Check("N", false) == true)
			{
				int key = wrapper.GetValue<int>(0, static_cast<int>(KeyCodes::kNone));
				wrapper.ReturnValue<bool>(Services::Get<InputService>().KeyboardPressed(static_cast<KeyCodes::KeyCode>(key)));
			}
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, keyboardDown, JS_BODY({
		
			JSWrapper wrapper(args);

			if (wrapper.Check("N", false) == true)
			{
				int key = wrapper.GetValue<int>(0, static_cast<int>(KeyCodes::kNone));
				wrapper.ReturnValue<bool>(Services::Get<InputService>().KeyboardDown(static_cast<KeyCodes::KeyCode>(key)));
			}
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, keyboardReleased, JS_BODY({
		
			JSWrapper wrapper(args);

			if (wrapper.Check("N", false) == true)
			{
				int key = wrapper.GetValue<int>(0, static_cast<int>(KeyCodes::kNone));
				wrapper.ReturnValue<bool>(Services::Get<InputService>().KeyboardReleased(static_cast<KeyCodes::KeyCode>(key)));
			}
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, lastKeyboardPressed, JS_BODY({
			
			JSWrapper wrapper(args);
			wrapper.ReturnValue<int>(static_cast<int>(Services::Get<InputService>().LastKeyboardPressed()));
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, lastKeyboardDown, JS_BODY({
			
			JSWrapper wrapper(args);
			wrapper.ReturnValue<int>(static_cast<int>(Services::Get<InputService>().LastKeyboardDown()));
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, lastKeyboardReleased, JS_BODY({
			
			JSWrapper wrapper(args);
			wrapper.ReturnValue<int>(static_cast<int>(Services::Get<InputService>().LastKeyboardReleased()));
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, mousePosition, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, mouseMovement, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, mouseWheelDelta, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, mousePressed, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, mouseDown, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, mouseReleased, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, lastMousePressed, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, lastMouseDown, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, lastMouseReleased, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, isControllerConnected, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, connectedControllers, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, controllerAxis, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, controllerPressed, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, controllerDown, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, controllerReleased, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, lastControllerPressed, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, lastControllerDown, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, lastControllerReleased, JS_BODY({
		
		}));
	}
}