/*----------------------------------------------------------------------------*\
|Opengl extension hook x64 and x86												|
\*----------------------------------------------------------------------------*/

#include <windows.h>
#include <fstream>

#define GLEW_STATIC
#include "glew/glew.h"
#if defined _M_X64
#pragma comment(lib, "glew/x64/glew32s.lib")
#elif defined _M_IX86
#pragma comment(lib, "glew/x86/glew32s.lib")
#endif

#pragma comment(lib, "OpenGL32.lib")

#include "detours\detours.h"
#if defined _M_X64
#pragma comment(lib, "detours/x64/detours.lib") 
#elif defined _M_IX86
#pragma comment(lib, "detours/x86/detours.lib")
#endif

//#include "minhook\minhook.h"
//#if defined _M_X64
//#pragma comment(lib, "minhook/x64/libMinHook.x64.lib") 
//#elif defined _M_IX86
//#pragma comment(lib, "minhook/x86/libMinHook.x86.lib")
//#endif

#pragma warning( disable : 4244 )

//========================================================================================================================//

using namespace std;

void Log(const char* fmt, ...)
{
	if (!fmt)	return;

	char		text[4096];
	va_list		ap;
	va_start(ap, fmt);
	vsprintf_s(text, fmt, ap);
	va_end(ap);

	ofstream logfile("log.txt", ios::app);
	if (logfile.is_open() && text)	logfile << text << endl;
	logfile.close();
}

//========================================================================================================================//

bool waitedOnce = false;  // Flag to track if we have already waited once
bool callonce = true;
GLint mlocation; //models location
GLint mv0;
GLsizei mcount;
int countnum = -1;

//========================================================================================================================//

void lognospam(int duration, const char* name)
{
	if (!waitedOnce) 
	{
		int n;

		for (n = 0; n < duration; n++)
		{
			if (GetTickCount() % 100)
				Log(name);
		}
		waitedOnce = true;
	}
}

void logLocation() //manually with keys P, O and END
{
	if (countnum == mlocation)
	if (GetAsyncKeyState(VK_END) & 1) //press END to log to log.txt
	Log("mlocation == %d && mcount == %d", mlocation, mcount); //log selected values

	//erase selected textures
	if (countnum == mlocation)
	{
		return;
	}

	//hold down P key until a texture is erased, press END to log values of those textures
	if (GetAsyncKeyState('O') & 1) //-
	countnum--;
	if (GetAsyncKeyState('P') & 1) //+
	countnum++;
	if (GetAsyncKeyState(VK_MENU) && GetAsyncKeyState('9') & 1) //reset, set to -1
	countnum = -1;
}

//========================================================================================================================//

typedef void (WINAPI* glDrawElements_t)(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices);
glDrawElements_t real_glDrawElements;

void WINAPI hglDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices)
{
	lognospam(50, "glDrawElements hooked");

	if (mlocation == 0 && mcount == 53)//example
	{
		glDisable(GL_DEPTH_TEST);

		real_glDrawElements(mode, count, type, indices);

		glEnable(GL_DEPTH_TEST);
	}

	logLocation();

	real_glDrawElements(mode, count, type, indices);
}

typedef void (WINAPI* glDrawElementsBaseVertex_t)(GLenum mode, GLsizei count, GLenum type, void* indices, GLint basevertex);
glDrawElementsBaseVertex_t real_glDrawElementsBaseVertex;

void WINAPI hglDrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, void* indices, GLint basevertex) //called in doom 2016
{
	lognospam(50, "glDrawElementsBaseVertex hooked");

	if (mlocation == 129 && mv0 == 10)//example
	{
		glDisable(GL_DEPTH_TEST);

		real_glDrawElementsBaseVertex(mode, count, type, indices, basevertex);
		
		glEnable(GL_DEPTH_TEST);
	}

	logLocation();

	real_glDrawElementsBaseVertex(mode, count, type, indices, basevertex);
}

typedef void (WINAPI* glUniform1f_t)(GLint location, GLfloat v0);
glUniform1f_t real_glUniform1f;
void WINAPI hglUniform1f(GLint location, GLfloat v0)
{
	Log("1 location == %d && v0 == %f", location, v0);

	mlocation = location;
	mv0 = v0;

	real_glUniform1f(location, v0);
}

typedef void (WINAPI* glUniform2f_t)(GLint location, GLfloat v0, GLfloat v1);
glUniform2f_t real_glUniform2f;
void WINAPI hglUniform2f(GLint location,GLfloat v0,GLfloat v1)
{
	Log("2 location == %d && v0 == %d", location, v0);

	mlocation = location;
	mv0 = v0;

	real_glUniform2f(location, v0, v1);
}

typedef void (WINAPI* glUniform3f_t)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
glUniform3f_t real_glUniform3f;
void WINAPI hglUniform3f(GLint location,GLfloat v0,GLfloat v1,GLfloat v2)
{
	Log("3 location == %d && v0 == %d", location, v0);

	mlocation = location;
	mv0 = v0;

	real_glUniform3f(location, v0, v1, v2);
}

typedef void (WINAPI* glUniform4f_t)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
glUniform4f_t real_glUniform4f;
void WINAPI hglUniform4f(GLint location,GLfloat v0,GLfloat v1,GLfloat v2,GLfloat v3)
{
	Log("4 location == %d && v0 == %d", location, v0);

	mlocation = location;
	mv0 = v0;

	real_glUniform4f(location, v0, v1, v2, v3);
}

typedef void (WINAPI* glUniform1i_t)(GLint location, GLint v0);
glUniform1i_t real_glUniform1i;
void WINAPI hglUniform1i(GLint location,GLint v0)
{
	Log("5 location == %d && v0 == %d", location, v0);

	mlocation = location;
	mv0 = v0;

	real_glUniform1i(location, v0);
}

typedef void (WINAPI* glUniform2i_t)(GLint location, GLint v0, GLint v1);
glUniform2i_t real_glUniform2i;
void WINAPI hglUniform2i(GLint location,GLint v0,GLint v1)
{
	Log("6 location == %d && v0 == %d", location, v0);

	mlocation = location;
	mv0 = v0;

	real_glUniform2i(location, v0, v1);
}

typedef void (WINAPI* glUniform3i_t)(GLint location, GLint v0, GLint v1, GLint v2);
glUniform3i_t real_glUniform3i;
void WINAPI hglUniform3i(GLint location,GLint v0,GLint v1,GLint v2)
{
	Log("7 location == %d && v0 == %d", location, v0);

	mlocation = location;
	mv0 = v0;

	real_glUniform3i(location, v0, v1, v2);
}

typedef void (WINAPI* glUniform4i_t)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
glUniform4i_t real_glUniform4i;
void WINAPI hglUniform4i(GLint location,GLint v0,GLint v1,GLint v2,GLint v3)
{
	Log("8 location == %d && v0 == %d", location, v0);

	mlocation = location;
	mv0 = v0;

	real_glUniform4i(location, v0, v1, v2, v3);
}

typedef void (WINAPI* glUniform1ui_t)(GLint location, GLuint v0);
glUniform1ui_t real_glUniform1ui;
void WINAPI hglUniform1ui(GLint location,GLuint v0)
{
	Log("9 location == %d && v0 == %d", location, v0);

	mlocation = location;
	mv0 = v0;

	real_glUniform1ui(location, v0);
}

typedef void (WINAPI* glUniform2ui_t)(GLint location, GLuint v0, GLuint v1);
glUniform2ui_t real_glUniform2ui;
void WINAPI hglUniform2ui(GLint location,GLuint v0,GLuint v1)
{
	Log("10 location == %d && v0 == %d", location, v0);

	mlocation = location;
	mv0 = v0;

	real_glUniform2ui(location, v0, v1);
}

typedef void (WINAPI* glUniform3ui_t)(GLint location, GLuint v0, GLuint v1, GLuint v2);
glUniform3ui_t real_glUniform3ui;
void WINAPI hglUniform3ui(GLint location,GLuint v0,GLuint v1,GLuint v2)
{
	Log("11 location == %d && v0 == %d", location, v0);

	mlocation = location;
	mv0 = v0;

	real_glUniform3ui(location, v0, v1, v2);
}

typedef void (WINAPI* glUniform4ui_t)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
glUniform4ui_t real_glUniform4ui;
void WINAPI hglUniform4ui(GLint location,GLuint v0,GLuint v1,GLuint v2,GLuint v3)
{
	Log("12 location == %d && v0 == %d", location, v0);

	mlocation = location;
	mv0 = v0;

	real_glUniform4ui(location, v0, v1, v2, v3);
}

typedef void (WINAPI* glUniform1fv_t)(GLint location, GLsizei count, const GLfloat* value);
glUniform1fv_t real_glUniform1fv;
void WINAPI hglUniform1fv(GLint location,GLsizei count,const GLfloat* value)
{
	Log("13 location == %d && count == %d", location, count);

	mlocation = location;
	mcount = count;

	real_glUniform1fv(location, count, value);
}

typedef void (WINAPI* glUniform2fv_t)(GLint location, GLsizei count, const GLfloat* value);
glUniform2fv_t real_glUniform2fv;
void WINAPI hglUniform2fv(GLint location,GLsizei count,const GLfloat* value)
{
	Log("14 location == %d && count == %d", location, count);

	mlocation = location;
	mcount = count;

	real_glUniform2fv(location, count, value);
}

typedef void (WINAPI* glUniform3fv_t)(GLint location, GLsizei count, const GLfloat* value);
glUniform3fv_t real_glUniform3fv;
void WINAPI hglUniform3fv(GLint location,GLsizei count,const GLfloat* value)
{
	Log("15 location == %d && count == %d", location, count);

	mlocation = location;
	mcount = count;

	real_glUniform3fv(location, count, value);
}

typedef void (WINAPI* glUniform4fv_t)(GLint location, GLsizei count, const GLfloat* value); 
glUniform4fv_t real_glUniform4fv;
void WINAPI hglUniform4fv(GLint location,GLsizei count,const GLfloat* value) //doom 2016
{
	Log("16 location == %d && count == %d", location, count); 

	mlocation = location;
	mcount = count;

	real_glUniform4fv(location, count, value);
}

typedef void (WINAPI* glUniform1iv_t)(GLint location, GLsizei count, const GLint* value);
glUniform1iv_t real_glUniform1iv;
void WINAPI hglUniform1iv(GLint location,GLsizei count,const GLint* value)
{
	Log("17 location == %d && count == %d", location, count);
	
	mlocation = location;
	mcount = count;

	real_glUniform1iv(location, count, value);
}

typedef void (WINAPI* glUniform2iv_t)(GLint location, GLsizei count, const GLint* value);
glUniform2iv_t real_glUniform2iv;
void WINAPI hglUniform2iv(GLint location,GLsizei count,const GLint* value)
{
	Log("18 location == %d && count == %d", location, count);
	
	mlocation = location;
	mcount = count;

	real_glUniform2iv(location, count, value);
}

typedef void (WINAPI* glUniform3iv_t)(GLint location, GLsizei count, const GLint* value);
glUniform3iv_t real_glUniform3iv;
void WINAPI hglUniform3iv(GLint location,GLsizei count,const GLint* value)
{
	Log("19 location == %d && count == %d", location, count);
	
	mlocation = location;
	mcount = count;

	real_glUniform3iv(location, count, value);
}

typedef void (WINAPI* glUniform4iv_t)(GLint location, GLsizei count, const GLint* value);
glUniform4iv_t real_glUniform4iv;
void WINAPI hglUniform4iv(GLint location,GLsizei count,const GLint* value)
{
	Log("20 location == %d && count == %d", location, count);
	
	mlocation = location;
	mcount = count;

	real_glUniform4iv(location, count, value);
}

typedef void (WINAPI* glUniform1uiv_t)(GLint location, GLsizei count, const GLuint* value);
glUniform1uiv_t real_glUniform1uiv;
void WINAPI hglUniform1uiv(GLint location,GLsizei count,const GLuint* value)
{
	Log("21 location == %d && count == %d", location, count);
	
	mlocation = location;
	mcount = count;

	real_glUniform1uiv(location, count, value);
}

typedef void (WINAPI* glUniform2uiv_t)(GLint location, GLsizei count, const GLuint* value);
glUniform2uiv_t real_glUniform2uiv;
void WINAPI hglUniform2uiv(GLint location,GLsizei count,const GLuint* value)
{
	Log("22 location == %d && count == %d", location, count);
	
	mlocation = location;
	mcount = count;

	real_glUniform2uiv(location, count, value);
}

typedef void (WINAPI* glUniform3uiv_t)(GLint location, GLsizei count, const GLuint* value);
glUniform3uiv_t real_glUniform3uiv;
void WINAPI hglUniform3uiv(GLint location,GLsizei count,const GLuint* value)
{
	Log("23 location == %d && count == %d", location, count);
	
	mlocation = location;
	mcount = count;

	real_glUniform3uiv(location, count, value);
}

typedef void (WINAPI* glUniform4uiv_t)(GLint location, GLsizei count, const GLuint* value);
glUniform4uiv_t real_glUniform4uiv;
void WINAPI hglUniform4uiv(GLint location,GLsizei count,const GLuint* value)
{
	Log("24 location == %d && count == %d", location, count);
	
	mlocation = location;
	mcount = count;

	real_glUniform4uiv(location, count, value);
}

typedef GLint(WINAPI* glGetUniformLocation_t)(GLuint program, const GLchar* name);
glGetUniformLocation_t real_glGetUniformLocation;

GLint WINAPI hglGetUniformLocation(GLuint program, const GLchar* name)
{
	//Log("name == %s", name);

	return real_glGetUniformLocation(program, name);
}

typedef void (WINAPI* glActiveTexture_t)(GLenum texture);
glActiveTexture_t real_glActiveTexture;

void WINAPI hglActiveTexture(GLenum texture)
{
	real_glActiveTexture(texture);
}

//========================================================================================================================//

static void HookExtensions(void)
{
	// to get functions we need valid GL context, we create dummy window for dummy GL contex
	HWND dummy = CreateWindowExW(
		0, L"STATIC", L"DummyWindow", WS_OVERLAPPED,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, NULL, NULL);
	//Assert(dummy && "Failed to create dummy window");

	HDC dc = GetDC(dummy);
	//Assert(dc && "Failed to get device context for dummy window");

	PIXELFORMATDESCRIPTOR desc =
	{
		desc.nSize = sizeof(desc),
		desc.nVersion = 1,
		desc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		desc.iPixelType = PFD_TYPE_RGBA,
		desc.cColorBits = 24,
	};

	int format = ChoosePixelFormat(dc, &desc);
	if (!format)
	{
		Log("Cannot choose OpenGL pixel format for dummy window!");
	}

	int ok = DescribePixelFormat(dc, format, sizeof(desc), &desc);
	//Assert(ok && "Failed to describe OpenGL pixel format");

	// reason to create dummy window is that SetPixelFormat can be called only once for the window
	if (!SetPixelFormat(dc, format, &desc))
	{
		Log("Cannot set OpenGL pixel format for dummy window!");
	}

	HGLRC rc = wglCreateContext(dc);
	//Assert(rc && "Failed to create OpenGL context for dummy window");

	ok = wglMakeCurrent(dc, rc);
	//Assert(ok && "Failed to make current OpenGL context for dummy window");

	real_glDrawElements = (glDrawElements_t)wglGetProcAddress("glDrawElements");
	real_glDrawElementsBaseVertex = (glDrawElementsBaseVertex_t)wglGetProcAddress("glDrawElementsBaseVertex");
	//glDrawElementsIndirect
	//glDrawElementsInstanced
	//glDrawElementsInstancedBaseInstance
	//glDrawElementsInstancedBaseVertex
	//glDrawElementsInstancedBaseVertexBaseInstance
	//glDrawRangeElements
	//glDrawRangeElementsBaseVertex

	real_glUniform1f = (glUniform1f_t)wglGetProcAddress("glUniform1f");
	real_glUniform2f = (glUniform2f_t)wglGetProcAddress("glUniform2f");
	real_glUniform3f = (glUniform3f_t)wglGetProcAddress("glUniform3f");
	real_glUniform4f = (glUniform4f_t)wglGetProcAddress("glUniform4f");
	real_glUniform1i = (glUniform1i_t)wglGetProcAddress("glUniform1i"); 
	real_glUniform2i = (glUniform2i_t)wglGetProcAddress("glUniform2i");
	real_glUniform3i = (glUniform3i_t)wglGetProcAddress("glUniform3i");
	real_glUniform4i = (glUniform4i_t)wglGetProcAddress("glUniform4i");
	real_glUniform1ui = (glUniform1ui_t)wglGetProcAddress("glUniform1ui");
	real_glUniform2ui = (glUniform2ui_t)wglGetProcAddress("glUniform2ui");
	real_glUniform3ui = (glUniform3ui_t)wglGetProcAddress("glUniform3ui");
	real_glUniform4ui = (glUniform4ui_t)wglGetProcAddress("glUniform4ui");
	real_glUniform1fv = (glUniform1fv_t)wglGetProcAddress("glUniform11fv");
	real_glUniform2fv = (glUniform2fv_t)wglGetProcAddress("glUniform2fv");
	real_glUniform3fv = (glUniform3fv_t)wglGetProcAddress("glUniform3fv");
	real_glUniform4fv = (glUniform4fv_t)wglGetProcAddress("glUniform4fv");
	real_glUniform1iv = (glUniform1iv_t)wglGetProcAddress("glUniform1iv");
	real_glUniform2iv = (glUniform2iv_t)wglGetProcAddress("glUniform2iv");
	real_glUniform3iv = (glUniform3iv_t)wglGetProcAddress("glUniform3iv");
	real_glUniform4iv = (glUniform4iv_t)wglGetProcAddress("glUniform4iv");
	real_glUniform1uiv = (glUniform1uiv_t)wglGetProcAddress("glUniform1uiv");
	real_glUniform2uiv = (glUniform2uiv_t)wglGetProcAddress("glUniform2uiv");
	real_glUniform3uiv = (glUniform3uiv_t)wglGetProcAddress("glUniform3uiv");
	real_glUniform4uiv = (glUniform4uiv_t)wglGetProcAddress("glUniform4uiv");

	real_glGetUniformLocation = (glGetUniformLocation_t)wglGetProcAddress("glGetUniformLocation");
	real_glActiveTexture = (glActiveTexture_t)wglGetProcAddress("glActiveTexture");

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_glDrawElements, hglDrawElements);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_glDrawElementsBaseVertex, hglDrawElementsBaseVertex);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_glUniform1f, hglUniform1f);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_glUniform2f, hglUniform2f);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_glUniform3f, hglUniform3f);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_glUniform4f, hglUniform4f);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_glUniform1i, hglUniform1i);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_glUniform2i, hglUniform2i);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_glUniform3i, hglUniform3i);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_glUniform4i, hglUniform4i);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_glUniform1ui, hglUniform1ui);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_glUniform2ui, hglUniform2ui);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_glUniform3ui, hglUniform3ui);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_glUniform4ui, hglUniform4ui);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_glUniform1fv, hglUniform1fv);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_glUniform2fv, hglUniform2fv);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_glUniform3fv, hglUniform3fv);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_glUniform4fv, hglUniform4fv);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_glUniform1iv, hglUniform1iv);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_glUniform2iv, hglUniform2iv);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_glUniform3iv, hglUniform3iv);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_glUniform4iv, hglUniform4iv);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_glUniform1uiv, hglUniform1uiv);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_glUniform2uiv, hglUniform2uiv);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_glUniform3uiv, hglUniform3uiv);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_glUniform4uiv, hglUniform4uiv);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_glGetUniformLocation, hglGetUniformLocation);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_glActiveTexture, hglActiveTexture);
	DetourTransactionCommit();


	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(rc);
	ReleaseDC(dummy, dc);
	DestroyWindow(dummy);
}

//========================================================================================================================//

DWORD WINAPI nStart(LPVOID lpParameter)
{
	while (GetModuleHandleA("opengl32.dll") == 0)
	{
		Sleep(4000);
	}

	if (callonce)
	{
		callonce = false;

		HookExtensions();
	}

	return 0;
}

//========================================================================================================================//

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpvReserved)
{
	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			//Log("DLL_PROCESS_ATTACH");
			DisableThreadLibraryCalls(hModule);

			CreateThread(NULL, 0, nStart, hModule, 0, NULL);
			break;
		}

		case DLL_PROCESS_DETACH:
		{

		}
	}
	return TRUE;
}