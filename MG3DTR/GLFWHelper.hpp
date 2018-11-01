
namespace Test {

	class GLFWHelper {
	public:
		GLFWHelper() {}

		virtual void Update(){}

		virtual void ReshapeFunc(GLFWwindow *window, int width, int height){}

		virtual void KeyFunc(GLFWwindow* window, int k, int scan, int act, int mods){}

		virtual void CursorFunc(GLFWwindow* window, double x, double y){}

		virtual void MouseButtonFunc(GLFWwindow* window, int b, int act, int mods){}

		virtual void ScrollFunc(GLFWwindow* window, double x_off, double y_off){}

	};


	GLFWHelper *instance;
	GLFWwindow* window;

	void Update()
	{
		instance->Update();
	}

	void ReshapeFunc(GLFWwindow *window, int width, int height)
	{
		instance->ReshapeFunc(window, width, height);
	}

	void KeyFunc(GLFWwindow  *window, int k, int scan, int act, int mods)
	{
		instance->KeyFunc(window, k, scan, act, mods);
	}

	
	void CursorFunc(GLFWwindow *window, double x, double y)
	{
		instance->CursorFunc(window, x, y);
	}

	
	void MouseButtonFunc(GLFWwindow *window, int b, int act, int mods)
	{
		instance->MouseButtonFunc(window, b, act, mods);
	}

	void ScrollFunc(GLFWwindow *window, double x_off, double y_off) {
		instance->ScrollFunc(window, x_off, y_off);
	}

	void Init(GLFWHelper *inst, GLFWwindow* w)
	{
		instance = inst;
		window = w;
	}

}