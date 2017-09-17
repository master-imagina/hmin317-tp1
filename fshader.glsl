#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;
uniform sampler2D texture;
uniform vec4 light_position;
uniform vec3 eye_pos;


varying lowp vec4 color_factor;
varying vec2 v_texcoord;
varying vec3 FragPos;
varying vec3 v_normal;
//! [0]
void main()
{

    vec3 object_color = vec3(0.1+color_factor.z/6,0.1,0.1+(3-color_factor.z)/6);
    vec3 ambient_color = vec3(0.0,0.0,1.0);
    vec3 light_color = vec3(1.0,1.0,1.0);

    // Set fragment color from texture
    //
    vec3 norm = normalize(v_normal);
    vec3 light_direction = normalize(color_factor - light_position  );
    /*ambiant*/
    float ambient_strength =0.3;
    vec3 ambient = ambient_color * material.ambient * ambient_strength;

    /*Diffuse*/
    float diff = max(dot(norm,light_direction ), 0.0);
    vec3 diffuse = diff * light_color * material.diffuse ;

    /*Specular*/
    float specular_strength = 0.2;

    vec3 eye_direction = normalize(eye_pos - color_factor);
    vec3 reflect_direction = reflect(-light_direction, norm);
    float spec = pow(max((dot(eye_direction, reflect_direction)), 0.0), material.shininess);
    vec3 specular = spec * light_color * material.specular;

    vec3 result = ( ambient + diffuse);

    //result = object_color; // Disable lighting

    gl_FragColor = vec4(result, 1.0);
}


//! [0]
