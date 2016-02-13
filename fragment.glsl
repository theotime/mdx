uniform mat4 mv;
uniform mat4 mv_normal;

smooth in vec3 v;
smooth in vec3 n;

void main(void) {
    vec4 local_point = mv * vec4(v, 1.0);
    vec4 local_normal = normalize(mv_normal * vec4(n, 1.0));

    vec3 lpos = vec3(0.0, 0.0, 10.0);
    vec4 vcolor = vec4(1.0);
    vec3 ldir = normalize(lpos - local_point.xyz);
    float phong = max(dot(reflect(-ldir, local_normal.xyz), normalize(-local_point.xyz)), 0);
    gl_FragColor = vcolor * max(dot(local_normal.xyz, ldir), 0) + vcolor * pow(phong, 80.0);
}
