#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif
uniform mat4 mvp_matrix;
uniform vec4 light_position;
uniform vec3 eye_pos;

attribute vec4 a_position;
attribute vec2 a_texcoord;
attribute vec3 normal;

varying vec2 v_texcoord;
varying lowp vec4 color_factor;
varying vec3 v_normal;

//! [0]
void main()
{
    //color_factor = max(dot(normal, light_position), 0.0);
    color_factor = a_position;


    v_normal = normal;
    // Calculate vertex position in screen space
    gl_Position = mvp_matrix * a_position;

    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    v_texcoord = a_texcoord;

}
//! [0]
