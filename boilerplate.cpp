// ==========================================================================
// Barebones OpenGL Core Profile Boilerplate
//    using the GLFW windowing system (http://www.glfw.org)
//
// Loosely based on
//  - Chris Wellons' example (https://github.com/skeeto/opengl-demo) and
//  - Camilla Berglund's example (http://www.glfw.org/docs/latest/quick.html)
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <algorithm>
#include <bits/stdc++.h>
#include "ImageBuffer.h"

#include <Magick++.h>

#include <glm/glm.hpp>

// specify that we want the OpenGL core profile before including GLFW headers
#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#define EPS 0.0001

using namespace std;
using namespace glm;

int width=1024,height=width;
ImageBuffer ib;


// --------------------------------------------------------------------------
// OpenGL utility and support function prototypes

void QueryGLVersion();
bool CheckGLErrors();


// --------------------------------------------------------------------------
// Rendering function that draws our scene to the frame buffer

void debugvec3(const vec3& v){
cout<<v.x<<"\t"<<v.y<<"\t"<<v.z<<endl;
}

// ==========================================================================
// draw triangle with PAA

vec2 _p0,_p1,_p2;

void init_triangle(){
	_p0=vec2(824.13,873.75);
	_p1=vec2(120.57,435.7);
	_p2=vec2(920,742.05);
}

void draw_triangle_pixel(int i, int j){
	vec2 p0=_p0-vec2(i,j);
	vec2 p1=_p1-vec2(i,j);
	vec2 p2=_p2-vec2(i,j);
	vector<vec2> output(3),input;
	output[0]=p0;
	output[1]=p1;
	output[2]=p2;
	
	// left edge
{
	input=output;
	output.clear();
	vec2 s=input.back();
	for(vec2 e : input){
		if(e.x>=0){
			if(s.x<0){
				output.push_back(vec2(0, (s+(-s.x/(e-s).x)*(e-s)).y ));
			}
			output.push_back(e);
		}else if(s.x>=0){
			output.push_back(vec2(0, (s+(-s.x/(e-s).x)*(e-s)).y ));
		}
		s=e;
	}
}

	//right edge
{
	input=output;
	output.clear();
	vec2 s=input.back();
	for(vec2 e : input){
		if(e.x<=1){
			if(s.x>1){
				output.push_back(vec2(1, (e+((-e.x+1)/(s-e).x)*(s-e)).y ));
			}
			output.push_back(e);
		}else if(s.x<=1){
			output.push_back(vec2(1, (e+((-e.x+1)/(s-e).x)*(s-e)).y ));
		}
		s=e;
	}
}

	for(int k=0;k<output.size();k++){
		float t=output[k].x;
		output[k].x=output[k].y;
		output[k].y=t;
	}
	
		// left edge
{
	input=output;
	output.clear();
	vec2 s=input.back();
	for(vec2 e : input){
		if(e.x>=0){
			if(s.x<0){
				output.push_back(vec2(0, (s+(-s.x/(e-s).x)*(e-s)).y ));
			}
			output.push_back(e);
		}else if(s.x>=0){
			output.push_back(vec2(0, (s+(-s.x/(e-s).x)*(e-s)).y ));
		}
		s=e;
	}
}

	//right edge
{
	input=output;
	output.clear();
	vec2 s=input.back();
	for(vec2 e : input){
		if(e.x<=1){
			if(s.x>1){
				output.push_back(vec2(1, (e+((-e.x+1)/(s-e).x)*(s-e)).y ));
			}
			output.push_back(e);
		}else if(s.x<=1){
			output.push_back(vec2(1, (e+((-e.x+1)/(s-e).x)*(s-e)).y ));
		}
		s=e;
	}
}

	for(int k=0;k<output.size();k++){
		float t=output[k].x;
		output[k].x=output[k].y;
		output[k].y=t;
	}

	for(vec2 e:output){
//		cout<<e.x<<'\t'<<e.y<<endl;
	}
	
	float a=0;
	if(!output.empty()){
		for(int k=0;k<output.size()-1;k++){
			a+=output[k].x*output[k+1].y;
			a-=output[k].y*output[k+1].x;
		}
		a+=output.back().x*output.front().y;
		a-=output.back().y*output.front().x;
	}
	a*=0.5f;
	if(a<0)a*=-1;
//	cout<<"pixel "<<i<<'\t'<<j<<" set "<<a<<endl;
	ib.SetPixel(i,j,vec3(a,a,a));
}

void draw_triangle(){
	for(int i=0;i<width;i++){
		for(int j=0;j<height;j++){
			draw_triangle_pixel(i,j);
		}
	}
}

// ==========================================================================

void RenderScene()
{
	init_triangle();
	draw_triangle();
//	draw_triangle_pixel(0,0);

    ib.Render();
	ib.SaveToFile("test.png");
	
    CheckGLErrors();
}

// --------------------------------------------------------------------------
// GLFW callback functions

// reports GLFW errors
void ErrorCallback(int error, const char* description)
{
    cout << "GLFW ERROR " << error << ":" << endl;
    cout << description << endl;
}



// ==========================================================================
// PROGRAM ENTRY POINT

int main(int argc, char *argv[])
{   
	Magick::InitializeMagick(NULL);
    // initialize the GLFW windowing system
    if (!glfwInit()) {
        cout << "ERROR: GLFW failed to initilize, TERMINATING" << endl;
        return -1;
    }
    glfwSetErrorCallback(ErrorCallback);

    // attempt to create a window with an OpenGL 4.1 core profile context
    GLFWwindow *window = 0;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(width, height, "CPSC 453 OpenGL Boilerplate", 0, 0);
    if (!window) {
        cout << "Program failed to create GLFW window, TERMINATING" << endl;
        glfwTerminate();
        return -1;
    }

    // set keyboard callback function and make our context current (active)

    glfwMakeContextCurrent(window);
	
	
	// image buffer
	
	cout<<(ib.Initialize()?"initialized":"failed to initialize")<<'\n';
	
	
    // run an event-triggered main loop
    while (!glfwWindowShouldClose(window))
    {
        // call function to draw our scene
        RenderScene();

        // scene is rendered to the back buffer, so swap to front for display
        glfwSwapBuffers(window);

        // sleep until next event before drawing again
        glfwWaitEvents();
        cout<<"image saved!\npress enter to exit"<<endl;
        string asdf;
		getline(cin,asdf);
        break;
    }

    // clean up allocated resources before exit

    glfwDestroyWindow(window);
    glfwTerminate();

    cout << "Goodbye!" << endl;
    return 0;
}

// ==========================================================================
// SUPPORT FUNCTION DEFINITIONS

// --------------------------------------------------------------------------
// OpenGL utility functions

void QueryGLVersion()
{
    // query opengl version and renderer information
    string version  = reinterpret_cast<const char *>(glGetString(GL_VERSION));
    string glslver  = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
    string renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));

    cout << "OpenGL [ " << version << " ] "
         << "with GLSL [ " << glslver << " ] "
         << "on renderer [ " << renderer << " ]" << endl;
}

bool CheckGLErrors()
{
    bool error = false;
    for (GLenum flag = glGetError(); flag != GL_NO_ERROR; flag = glGetError())
    {
        cout << "OpenGL ERROR:  ";
        switch (flag) {
        case GL_INVALID_ENUM:
            cout << "GL_INVALID_ENUM" << endl; break;
        case GL_INVALID_VALUE:
            cout << "GL_INVALID_VALUE" << endl; break;
        case GL_INVALID_OPERATION:
            cout << "GL_INVALID_OPERATION" << endl; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << endl; break;
        case GL_OUT_OF_MEMORY:
            cout << "GL_OUT_OF_MEMORY" << endl; break;
        default:
            cout << "[unknown error code]" << endl;
        }
        error = true;
    }
    return error;
}


// ==========================================================================
