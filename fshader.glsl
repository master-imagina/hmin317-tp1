#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
uniform sampler2D grass;
uniform sampler2D sand;
uniform sampler2D rock;
uniform sampler2D cliff;


varying vec2 v_texcoord;
varying vec3 normal;
varying vec3 lightDir;
varying float height;

//! [0]
void main()
{
    // Set fragment color from texture
    float diff = max(dot(normal, lightDir), 0.0);
    vec4 albedo = vec4(1.0);
    if(height<0.5){
        albedo = texture2D(sand,v_texcoord*40.0);
    }else if(height<2.0){
        albedo = mix(texture2D(sand,v_texcoord*40.0),texture2D(grass,v_texcoord*40.0),(height - 0.5)/(1.5));
    }else if(height<8.0){
        albedo = texture2D(grass,v_texcoord*40.0);
    }else if(height<9.0){
        albedo = mix(texture2D(grass,v_texcoord*40.0),texture2D(rock,v_texcoord*40.0),(height - 8.0));
    }else{
        albedo = texture2D(rock,v_texcoord*40.0);
    }


    if(normal.y<0.8)
        albedo = mix(texture2D(cliff,v_texcoord*40.0),albedo,normal.y-0.3);

    gl_FragColor = albedo * diff;
}
//! [0]

