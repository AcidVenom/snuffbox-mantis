#include "input.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		Input::Input() :
			last_type_(InputType::kKeyboard),
			controllers_{ 0, 1, 2, 3 }
		{

		}

		//-----------------------------------------------------------------------------------------------
		KeyQueue::Event Input::CreateKeyEvent(int key, int action)
		{
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

			return evt;
		}

		//-----------------------------------------------------------------------------------------------
		Input* Input::Self()
		{
			return static_cast<Input*>(&Services::Get<InputService>());
		}

		//-----------------------------------------------------------------------------------------------
		void Input::Update()
		{
			unsigned int count = keyboard_.Flush();
			last_type_ = count > 0 ? InputType::kKeyboard : last_type_;
			last_type_ = mouse_.Update() == true ? InputType::kKeyboard : last_type_;

			for (int i = 0; i < 4; ++i)
			{
				controllers_[i].Poll();
			}
		}

		//-----------------------------------------------------------------------------------------------
		bool Input::HandleKeyQueue(const KeyQueue& queue, KeyCodes::KeyCode key, KeyQueue::KeyState state) const
		{
			return queue.HasKeyState(key, state);
		}

		//-----------------------------------------------------------------------------------------------
		bool Input::HandleController(int id, KeyCodes::KeyCode key, KeyQueue::KeyState state) const
		{
			if ((id < 0 || id >= 4) || controllers_[id].connected() == false)
			{
				return false;
			}

			return HandleKeyQueue(controllers_[id], key, state);
		}

		//-----------------------------------------------------------------------------------------------
		bool Input::KeyboardPressed(KeyCodes::KeyCode key) const
		{
			return HandleKeyQueue(keyboard_, key, KeyQueue::KeyState::kPressed);
		}

		//-----------------------------------------------------------------------------------------------
		bool Input::KeyboardDown(KeyCodes::KeyCode key) const
		{
			return HandleKeyQueue(keyboard_, key, KeyQueue::KeyState::kDown);
		}

		//-----------------------------------------------------------------------------------------------
		bool Input::KeyboardReleased(KeyCodes::KeyCode key) const
		{
			return HandleKeyQueue(keyboard_, key, KeyQueue::KeyState::kReleased);
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode Input::LastKeyboardPressed() const
		{
			return keyboard_.Last(KeyQueue::KeyState::kPressed);
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode Input::LastKeyboardDown() const
		{
			return keyboard_.Last(KeyQueue::KeyState::kDown);
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode Input::LastKeyboardReleased() const
		{
			return keyboard_.Last(KeyQueue::KeyState::kReleased);
		}

		//-----------------------------------------------------------------------------------------------
		void Input::MousePosition(int* px, int* py) const
		{
			mouse_.MousePosition(px, py);
		}

		//-----------------------------------------------------------------------------------------------
		void Input::MouseMovement(int* mx, int* my) const
		{
			mouse_.MouseMovement(mx, my);
		}

		//-----------------------------------------------------------------------------------------------
		void Input::MouseWheelDelta(int* sx, int* sy) const
		{
			mouse_.MouseWheelDelta(sx, sy);
		}

		//-----------------------------------------------------------------------------------------------
		bool Input::MousePressed(KeyCodes::KeyCode key) const
		{
			return HandleKeyQueue(mouse_, key, KeyQueue::KeyState::kPressed);
		}

		//-----------------------------------------------------------------------------------------------
		bool Input::MouseDown(KeyCodes::KeyCode key) const
		{
			return HandleKeyQueue(mouse_, key, KeyQueue::KeyState::kDown);
		}

		//-----------------------------------------------------------------------------------------------
		bool Input::MouseReleased(KeyCodes::KeyCode key) const
		{
			return HandleKeyQueue(mouse_, key, KeyQueue::KeyState::kReleased);
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode Input::LastMousePressed() const
		{
			return mouse_.Last(KeyQueue::KeyState::kPressed);
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode Input::LastMouseDown() const
		{
			return mouse_.Last(KeyQueue::KeyState::kDown);
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode Input::LastMouseReleased() const
		{
			return mouse_.Last(KeyQueue::KeyState::kReleased);
		}

		//-----------------------------------------------------------------------------------------------
		bool Input::IsControllerConnected(int id) const
		{
			if (id < 0 || id >= 4)
			{
				return false;
			}

			return controllers_[id].connected();
		}

		//-----------------------------------------------------------------------------------------------
		unsigned int Input::ConnectedControllers() const
		{
			int count = 0;
			for (int i = 0; i < 4; ++i)
			{
				count += controllers_[i].connected() == true ? 1 : 0;
			}
			
			return count;
		}

		//-----------------------------------------------------------------------------------------------
		void Input::SetControllerDeadZone(int id, float dz)
		{
			if (id < 0 || id >= 4)
			{
				return;
			}

			controllers_[id].set_dead_zone(dz);
		}

		//-----------------------------------------------------------------------------------------------
		float Input::ControllerAxis(int id, ControllerButtons::Axes axis) const
		{
			if (id < 0 || id >= 4)
			{
				return 0.0f;
			}

			return controllers_[id].Axis(axis);
		}

		//-----------------------------------------------------------------------------------------------
		bool Input::ControllerPressed(int id, KeyCodes::KeyCode key) const
		{
			return HandleController(id, key, KeyQueue::KeyState::kPressed);
		}

		//-----------------------------------------------------------------------------------------------
		bool Input::ControllerDown(int id, KeyCodes::KeyCode key) const
		{
			return HandleController(id, key, KeyQueue::KeyState::kDown);
		}

		//-----------------------------------------------------------------------------------------------
		bool Input::ControllerReleased(int id, KeyCodes::KeyCode key) const
		{
			return HandleController(id, key, KeyQueue::KeyState::kReleased);
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode Input::LastControllerPressed(int id) const
		{
			return (id < 0 || id >= 4 || IsControllerConnected(id) == false) ? false : controllers_[id].Last(KeyQueue::KeyState::kPressed);
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode Input::LastControllerDown(int id) const
		{
			return (id < 0 || id >= 4 || IsControllerConnected(id) == false) ? false : controllers_[id].Last(KeyQueue::KeyState::kDown);
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode Input::LastControllerReleased(int id) const
		{
			return (id < 0 || id >= 4 || IsControllerConnected(id) == false) ? false : controllers_[id].Last(KeyQueue::KeyState::kReleased);
		}

		//-----------------------------------------------------------------------------------------------
		void Input::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			Self()->keyboard_.PostEvent(CreateKeyEvent(key, action));
		}

		//-----------------------------------------------------------------------------------------------
		void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
		{
			Self()->mouse_.PostEvent(CreateKeyEvent(button, action));
		}

		//-----------------------------------------------------------------------------------------------
		void Input::MousePositionCallback(GLFWwindow* window, double xpos, double ypos)
		{
			Self()->mouse_.SetPosition(static_cast<int>(xpos + 0.5f), static_cast<int>(ypos + 0.5f));
		}

		//-----------------------------------------------------------------------------------------------
		void Input::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
		{
			Self()->mouse_.SetScrollDelta(static_cast<int>(xoffset + 0.5f), static_cast<int>(yoffset + 0.5f));
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
				JS_FUNCTION_REG(setControllerDeadZone),
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

			v8::Local<v8::Object> axes = JSWrapper::CreateObject();
			JSWrapper::SetObjectValue(axes, "LeftStickX", static_cast<unsigned int>(ControllerButtons::Axes::kLeftStickX));
			JSWrapper::SetObjectValue(axes, "LeftStickY", static_cast<unsigned int>(ControllerButtons::Axes::kLeftStickY));
			JSWrapper::SetObjectValue(axes, "RightStickX", static_cast<unsigned int>(ControllerButtons::Axes::kRightStickX));
			JSWrapper::SetObjectValue(axes, "RightStickY", static_cast<unsigned int>(ControllerButtons::Axes::kRightStickY));
			JSWrapper::SetObjectValue(axes, "LT", static_cast<unsigned int>(ControllerButtons::Axes::kLT));
			JSWrapper::SetObjectValue(axes, "RT", static_cast<unsigned int>(ControllerButtons::Axes::kRT));

			JSWrapper::SetObjectValue(input_type_enum, "Keyboard", static_cast<unsigned int>(InputType::kKeyboard));
			JSWrapper::SetObjectValue(input_type_enum, "Controller", static_cast<unsigned int>(InputType::kController));

			JSWrapper::RegisterGlobal("Keyboard", key_enum, true);
			JSWrapper::RegisterGlobal("Mouse", mb_enum, true);
			JSWrapper::RegisterGlobal("Controller", control_enum, true);
			JSWrapper::RegisterGlobal("Axes", axes, true);
			JSWrapper::RegisterGlobal("InputType", input_type_enum, true);

			JSFunctionRegister::Register(funcs, obj);
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, keyboardPressed, JS_BODY({
			
			JSWrapper wrapper(args);
			
			if (wrapper.Check("N", false) == true)
			{
                int key = wrapper.GetValue<int>(0, static_cast<int>(KeyCodesEnum::kNone));
				wrapper.ReturnValue<bool>(Services::Get<InputService>().KeyboardPressed(static_cast<KeyCodes::KeyCode>(key)));
			}
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, keyboardDown, JS_BODY({
		
			JSWrapper wrapper(args);

			if (wrapper.Check("N", false) == true)
			{
                int key = wrapper.GetValue<int>(0, static_cast<int>(KeyCodesEnum::kNone));
				wrapper.ReturnValue<bool>(Services::Get<InputService>().KeyboardDown(static_cast<KeyCodes::KeyCode>(key)));
			}
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, keyboardReleased, JS_BODY({
		
			JSWrapper wrapper(args);

			if (wrapper.Check("N", false) == true)
			{
                int key = wrapper.GetValue<int>(0, static_cast<int>(KeyCodesEnum::kNone));
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
			
			JSWrapper wrapper(args);
			int px, py;

			Services::Get<InputService>().MousePosition(&px, &py);

			v8::Local<v8::Object> to_return = JSWrapper::CreateObject();

			JSWrapper::SetObjectValue(to_return, "x", px);
			JSWrapper::SetObjectValue(to_return, "y", py);

			wrapper.ReturnValue<v8::Local<v8::Object>>(to_return);
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, mouseMovement, JS_BODY({
		
			JSWrapper wrapper(args);
			int mx, my;

			Services::Get<InputService>().MouseMovement(&mx, &my);

			v8::Local<v8::Object> to_return = JSWrapper::CreateObject();

			JSWrapper::SetObjectValue(to_return, "x", mx);
			JSWrapper::SetObjectValue(to_return, "y", my);

			wrapper.ReturnValue<v8::Local<v8::Object>>(to_return);
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, mouseWheelDelta, JS_BODY({
			
			JSWrapper wrapper(args);
			int sx, sy;

			Services::Get<InputService>().MouseWheelDelta(&sx, &sy);

			v8::Local<v8::Object> to_return = JSWrapper::CreateObject();

			JSWrapper::SetObjectValue(to_return, "x", sx);
			JSWrapper::SetObjectValue(to_return, "y", sy);

			wrapper.ReturnValue<v8::Local<v8::Object>>(to_return);
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, mousePressed, JS_BODY({
		
			JSWrapper wrapper(args);

			if (wrapper.Check("N", false) == true)
			{
				int key = wrapper.GetValue<int>(0, static_cast<int>(MouseButtonsEnum::kNone));
				wrapper.ReturnValue<bool>(Services::Get<InputService>().MousePressed(static_cast<KeyCodes::KeyCode>(key)));
			}
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, mouseDown, JS_BODY({
		
			JSWrapper wrapper(args);

			if (wrapper.Check("N", false) == true)
			{
				int key = wrapper.GetValue<int>(0, static_cast<int>(MouseButtonsEnum::kNone));
				wrapper.ReturnValue<bool>(Services::Get<InputService>().MouseDown(static_cast<KeyCodes::KeyCode>(key)));
			}
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, mouseReleased, JS_BODY({
			
			JSWrapper wrapper(args);

			if (wrapper.Check("N", false) == true)
			{
				int key = wrapper.GetValue<int>(0, static_cast<int>(MouseButtonsEnum::kNone));
				wrapper.ReturnValue<bool>(Services::Get<InputService>().MouseReleased(static_cast<KeyCodes::KeyCode>(key)));
			}
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, lastMousePressed, JS_BODY({
		
			JSWrapper wrapper(args);
			wrapper.ReturnValue<int>(static_cast<int>(Services::Get<InputService>().LastMousePressed()));
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, lastMouseDown, JS_BODY({
			
			JSWrapper wrapper(args);
			wrapper.ReturnValue<int>(static_cast<int>(Services::Get<InputService>().LastMouseDown()));
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, lastMouseReleased, JS_BODY({
			
			JSWrapper wrapper(args);
			wrapper.ReturnValue<int>(static_cast<int>(Services::Get<InputService>().LastMouseReleased()));
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, isControllerConnected, JS_BODY({
			
			JSWrapper wrapper(args);

			int id = wrapper.GetValue<int>(0, 0);
			wrapper.ReturnValue<bool>(Services::Get<InputService>().IsControllerConnected(id));
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, connectedControllers, JS_BODY({
			
			JSWrapper wrapper(args);
			wrapper.ReturnValue<int>(Services::Get<InputService>().ConnectedControllers());
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, setControllerDeadZone, JS_BODY({
			
			JSWrapper wrapper(args);

			if (wrapper.Check("NN", false) == true)
			{
				int id = wrapper.GetValue<int>(0, 0);
				float dz = wrapper.GetValue<float>(1, Controller::DEFAULT_DEAD_ZONE_);

				Services::Get<InputService>().SetControllerDeadZone(id, dz);
			}
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, controllerAxis, JS_BODY({
			
			JSWrapper wrapper(args);
			if (wrapper.Check("NN", false) == true)
			{
				int id = wrapper.GetValue<int>(0, 0);
				int axis = wrapper.GetValue<int>(1, static_cast<int>(ControllerButtons::Axes::kLeftStickX));

				wrapper.ReturnValue<float>(Services::Get<InputService>().ControllerAxis(id, static_cast<ControllerButtons::Axes>(axis)));
			}
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, controllerPressed, JS_BODY({
			
			JSWrapper wrapper(args);
			if (wrapper.Check("NN", false) == true)
			{
				int id = wrapper.GetValue<int>(0, 0);
				int key = wrapper.GetValue<int>(1, static_cast<int>(ControllerButtonsEnum::kNone));

				wrapper.ReturnValue<bool>(Services::Get<InputService>().ControllerPressed(id, static_cast<KeyCodes::KeyCode>(key)));
			}
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, controllerDown, JS_BODY({
			
			JSWrapper wrapper(args);
			if (wrapper.Check("NN", false) == true)
			{
				int id = wrapper.GetValue<int>(0, 0);
				int key = wrapper.GetValue<int>(1, static_cast<int>(ControllerButtonsEnum::kNone));

				wrapper.ReturnValue<bool>(Services::Get<InputService>().ControllerDown(id, static_cast<KeyCodes::KeyCode>(key)));
			}
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, controllerReleased, JS_BODY({
		
			JSWrapper wrapper(args);
			if (wrapper.Check("NN", false) == true)
			{
				int id = wrapper.GetValue<int>(0, 0);
				int key = wrapper.GetValue<int>(1, static_cast<int>(ControllerButtonsEnum::kNone));

				wrapper.ReturnValue<bool>(Services::Get<InputService>().ControllerReleased(id, static_cast<KeyCodes::KeyCode>(key)));
			}
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, lastControllerPressed, JS_BODY({
			
			JSWrapper wrapper(args);
			int id = wrapper.GetValue<int>(0, 0);
			wrapper.ReturnValue<int>(static_cast<int>(Services::Get<InputService>().LastControllerPressed(id)));
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, lastControllerDown, JS_BODY({

			JSWrapper wrapper(args);
			int id = wrapper.GetValue<int>(0, 0);
			wrapper.ReturnValue<int>(static_cast<int>(Services::Get<InputService>().LastControllerDown(id)));
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, lastControllerReleased, JS_BODY({

			JSWrapper wrapper(args);
			int id = wrapper.GetValue<int>(0, 0);
			wrapper.ReturnValue<int>(static_cast<int>(Services::Get<InputService>().LastControllerReleased(id)));
		}));
	}
}
