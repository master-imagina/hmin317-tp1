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
uniform sampler2D cliffNormal;

varying vec2 v_texcoord;
varying vec3 normal;
varying vec3 lightDir;
varying float height;

//! [0]
void main()
{
    // Set fragment color from texture

    vec3 normalColor = -normalize(texture2D(cliffNormal,v_texcoord*40).rgb * 2.0 - 1.0);




    float diff = max(dot(normal, lightDir), 0.2);
    float diffnormalMap = max(abs(dot(normalColor, lightDir)), 0.5);
    vec4 albedo = vec4(1.0);

    if(height<0.5){
        albedo = texture2D(sand,v_texcoord*40.0);
        diffnormalMap *= 1.5;
    }else if(height<2.0){
        albedo = mix(texture2D(sand,v_texcoord*40.0),texture2D(grass,v_texcoord*40.0),(height - 0.5)/(1.5));
        diffnormalMap *= mix(1.5,1.0,(height - 0.5)/(1.5));
    }else if(height<8.0){
        albedo = texture2D(grass,v_texcoord*40.0);
    }else if(height<9.0){
        albedo = mix(texture2D(grass,v_texcoord*40.0),texture2D(rock,v_texcoord*40.0),(height - 8.0));
    }else{
        albedo = texture2D(rock,v_texcoord*40.0);
    }


    float cliffAmout = 1.0f - normal.y;
    float blendAmount;

    float cliffMin = 0.1;
    float cliffMax = 0.7;

   // Auto Cliff
   if(cliffAmout < cliffMin)
   {
       blendAmount = cliffAmout / cliffMin;
       albedo = mix(albedo, texture2D(cliff,v_texcoord*40.0), blendAmount);
   }

   if((cliffAmout < cliffMax) && (cliffAmout >=cliffMin))
   {
       blendAmount = (cliffAmout - cliffMin) * (1.0 / (cliffMax - cliffMin));
       albedo = mix(texture2D(cliff,v_texcoord*40.0), albedo, blendAmount);
   }

   if(cliffAmout >= cliffMax)
   {
       albedo = texture2D(cliff,v_texcoord*40.0);
   }

   float gamma = 1.2;
   vec3 color = albedo.rgb * diff *diffnormalMap + vec3(0.2, 0.55, 0.8)/6.0;
   color = pow(color, vec3(1.0/gamma));


    gl_FragColor = vec4(color,1.0);
}
//! [0]

