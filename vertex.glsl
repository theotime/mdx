
attribute vec3 coord3d;
attribute vec3 normal;

smooth out vec3 v;
smooth out vec3 n;

uniform mat4 mv;
uniform mat4 p;

void main(void) {
    gl_Position = p * mv * vec4(coord3d, 1.0);
    n = normal;
    v = coord3d;

}