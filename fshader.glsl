#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
uniform vec4 ambiant_color;
uniform vec4 light_position;

varying lowp vec4 color_factor;
varying vec2 v_texcoord;
varying vec3 FragPos;
varying vec3 v_normal;
//! [0]
void main()
{
    // Set fragment color from texture
    //texture2D(texture, v_texcoord);
    //gl_FragColor = vec4(color_factor.z/6,0.1,(4-color_factor.z)/6,1.0);
    vec3 normal = normalize(v_normal);
    vec3 light_direction = normalize(light_position - color_factor);
    float ambient_strength = 2.5;
    vec3 ambient = ambiant_color * vec3(1.0,1.0,1.0) * ambient_strength;

    float diff = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = diff * vec3(1.0,1.0,1.0);
    vec3 result = (ambient + diffuse) * vec3(color_factor.z/6,0.1,(4-color_factor.z)/6);

    //gl_FragColor = vec4(vec3(color_factor.z/6,0.1,(4-color_factor.z)/6), 1.0);


    gl_FragColor = vec4(result, 1.0);
}


//! [0]
