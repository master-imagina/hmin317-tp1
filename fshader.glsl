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
uniform vec4 light_position1;
uniform vec4 light_position2;
uniform vec3 eye_pos;


varying lowp vec4 v_positon;
varying vec2 v_texcoord;
varying vec3 v_normal;
//! [0]
void main()
{

    vec3 object_color = vec3(0.1+v_positon.z/6,0.1,0.1+(3-v_positon.z)/6);
    vec3 ambient_color = vec3(0.1,0.1,0.1);
    vec3 light_color = vec3(0.,0.3,0.6);
    vec3 light_color2 = vec3(0.9,0.3,0.0);

    // Set fragment color from texture
    //
    vec3 norm = normalize(v_normal);
    vec3 light_direction1 = normalize(v_positon - light_position1  );
    /*ambiant*/
    float ambient_strength =0.1;
    vec3 ambient = ambient_color * material.ambient * ambient_strength;

    /*Diffuse*/
    float diff = max(dot(norm,light_direction1 ), 0.0)/2;
    vec3 diffuse = diff * light_color * material.diffuse ;

    /*Diffuse*/
    vec3 light_direction2 = normalize(v_positon - light_position2  );
    float diff2 = max(dot(norm,light_direction2 ), 0.0)/2;
    vec3 diffuse2 = diff2 * light_color2 * material.diffuse ;

    /*Specular*/
    float specular_strength = 0.2;

    vec3 eye_direction = normalize(eye_pos - v_positon);
    vec3 reflect_direction = reflect(-light_direction1, norm);
    float spec = pow(max((dot(eye_direction, reflect_direction)), 0.0), material.shininess)/2;
    vec3 specular = spec * light_color * material.specular;


    vec3 reflect_direction2 = reflect(-light_direction2, norm);
    float spec1 = pow(max((dot(eye_direction, reflect_direction2)), 0.0), material.shininess)/3;
    vec3 specular2 = spec1 * light_color2 * material.specular;


    vec3 result = ( ambient + diffuse + specular + diffuse2+specular2);

    //result = object_color; // Disable lighting

    gl_FragColor = vec4(result, 1.0);
}


//! [0]
