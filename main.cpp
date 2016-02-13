/* 
 * File:   main.cpp
 * Author: theotime
 *
 * Created on 13 février 2016, 16:11
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream> 

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <math.h>
#include <vector>

#include "mdx.h"


/* GLOBAL VARIABLES */
GLuint program;
GLint attribute_coord3d;
GLint attribute_normal;
GLint uniform_mv;
GLint uniform_mv_normal;
GLint uniform_p;

GLuint vbo_mesh_vertices, vbo_mesh_normals;
GLuint ibo_mesh_elements;

int screen_width=800, screen_height=600;


bool filecontent(const std::string& filename, std::string& content) {
    std::ifstream fp;
    std::string line;

    fp.open(filename.c_str());

    if (!fp.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return false;
    }

    while (getline(fp, line)) {
        content += line;
    }

    fp.close();
    return true;
}

void print_log(GLuint object) {
    GLint log_length = 0;
    if (glIsShader(object)) {
        glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
    } else if (glIsProgram(object)) {
        glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
    } else {
        std::cerr << "printlog: Not a shader or a program" << std::endl;
        return;
    }

    char* log = (char*) malloc(log_length);

    if (glIsShader(object))
        glGetShaderInfoLog(object, log_length, NULL, log);
    else if (glIsProgram(object))
        glGetProgramInfoLog(object, log_length, NULL, log);

    std::cerr << log;
    free(log);
}

GLuint create_shader(std::string filename, GLenum type) {
    std::string source;
    
    if (!filecontent(filename, source)) {
        std::cerr << "Error opening " << filename << std::endl;
        return 0;
    }
    
    const GLchar * sources[] = {
#ifdef GL_ES_VERSION_2_0
        "#version 100\n" // OpenGL ES 2.0
#else
        "#version 130\n"
#endif
        ,
        source.c_str()
    };
    
    GLuint res = glCreateShader(type);
    glShaderSource(res, 2, sources, NULL);
    glCompileShader(res);
    GLint compile_ok = GL_FALSE;
    glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
    if (compile_ok == GL_FALSE) {
        std::cerr << filename << ":";
        print_log(res);
        glDeleteShader(res);
        return 0;
    }

    return res;
}

bool init_resources(Mdx& mdx) {

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<GLushort> elements;

    mdx.output_gl(0, vertices, normals, elements);

    glGenBuffers(1, &vbo_mesh_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_vertices);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &vbo_mesh_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_normals);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

    glGenBuffers(1, &ibo_mesh_elements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_mesh_elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(GLushort), &elements[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    GLint link_ok = GL_FALSE;
    GLuint vs, fs;
    if ((vs = create_shader("vertex.glsl", GL_VERTEX_SHADER)) == 0) {
        return false;
    }

    if ((fs = create_shader("fragment.glsl", GL_FRAGMENT_SHADER)) == 0) {
        return false;
    }

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        std::cerr << "glLinkProgram:";
        print_log(program);
        return false;
    }
    
    uniform_mv = glGetUniformLocation(program, "mv");
    if(uniform_mv == -1) {
        std::cerr << "Couldn't bind uniform_fade " << "mv" << std::endl;
        return false;
    }
   
    uniform_mv_normal = glGetUniformLocation(program,  "mv_normal");
    if(uniform_mv_normal == -1) {
        std::cerr << "Couldn't bind uniform_fade " <<  "mv_normal" << std::endl;
        return false;
    }
    
    uniform_p = glGetUniformLocation(program,  "p");
    if(uniform_p == -1) {
        std::cerr << "Couldn't bind uniform_fade " <<  "p" << std::endl;
        return false;
    }
    
    attribute_coord3d = glGetAttribLocation(program, "coord3d");
    if(attribute_coord3d == -1) {
        std::cerr << "Couldn't bind attribute " << "coord3d" << std::endl;
        return false;
    }
    
    attribute_normal = glGetAttribLocation(program, "normal");
    if(attribute_normal == -1) {
        std::cerr << "Couldn't bind attribute " << "normal" << std::endl;
        return false;
    }
    
    return true;
}

void render(SDL_Window* window) {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    glUseProgram(program);


    glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_vertices);
    glEnableVertexAttribArray(attribute_coord3d);
    glVertexAttribPointer(
            attribute_coord3d,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            0
            );
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_normals);
    glEnableVertexAttribArray(attribute_normal);
    glVertexAttribPointer(
            attribute_normal,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            0
            );
    

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_mesh_elements);
    int size;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size / sizeof (GLushort), GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(attribute_coord3d);
    glDisableVertexAttribArray(attribute_normal);
    
    SDL_GL_SwapWindow(window);
    
}

void free_resources() {
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo_mesh_vertices);
    glDeleteBuffers(1, &vbo_mesh_normals);
    glDeleteBuffers(1, &ibo_mesh_elements);
}

void logic() {

    float angle = SDL_GetTicks() / 50.0; 
    glm::vec3 axis_y(0, 1, 0);
    glm::vec3 axis_x(1, 0, 0);
    glm::mat4 anim = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis_y);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -50.0, -150.0))* anim * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), axis_x);
    
    glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
    
    glm::mat4 projection = glm::perspective(45.0f, 1.0f * screen_width/screen_height, 0.1f, 1000.0f);
    
    glm::mat4 mv = view * model;
    glm::mat4 mv_normal = glm::inverse(glm::transpose(mv));
    
    glUniformMatrix4fv(uniform_mv, 1, GL_FALSE, glm::value_ptr(mv));
    glUniformMatrix4fv(uniform_mv_normal, 1, GL_FALSE, glm::value_ptr(mv_normal));
    glUniformMatrix4fv(uniform_p, 1, GL_FALSE, glm::value_ptr(projection));
}

void onResize(int width, int height) {
    screen_width = width;
    screen_height = height;
    glViewport(0, 0, screen_width, screen_height);
}

void mainLoop(SDL_Window* window) {
    while (true) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT)
                return;

            if (ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                onResize(ev.window.data1, ev.window.data2);
        }

        logic();
        render(window);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Need filename" << std::endl;
        return false;
    }

    if (sizeof (float) != 4) {
        std::cerr << "Error non 32bits float" << std::endl;
        return false;
    }

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("OpenGL: MDX",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            screen_width, screen_height,
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    SDL_GL_CreateContext(window);
    
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 1);

    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK) {
        std::cerr << "Error: glewInit: " << glewGetErrorString(glew_status) << std::endl;
        return EXIT_FAILURE;
    }

    Mdx mdx;
    if(!mdx.load(argv[1])) {
        return EXIT_FAILURE;
    }
    
    if (!init_resources(mdx))
        return EXIT_FAILURE;

    mainLoop(window);

    free_resources();
    return EXIT_SUCCESS;
}

// to export to .obj
//main(){
//    Mdx mdx;
//    mdx.load(argv[1]);
//    mdx.output_obj(ALL_GEOSETS);
//}