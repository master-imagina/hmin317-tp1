#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;

varying vec2 v_texcoord;
varying vec4 v_position;


void main()
{
    // Set fragment color from texture
    /*vec4 _red = vec4(0.8,0.2,0.2,1.0);
    vec4 _green = vec4(0.2,0.8,0.2,1.0);
    vec4 _blue = vec4(0.2,0.2,0.8,1.0);
    vec4 _gray = vec4(0.8,0.8,0.8,1.0);

    float p1 = gl_FragCoord.x +0.5;
    float p2 = gl_FragCoord.y +0.5;
    float res = p1+p2;


    if (mod(p1,3.0)== 0.0 || mod(p2,6.0) ==0.0) gl_FragColor = (texture2D(texture, v_texcoord)/2.0)*_gray;
    else if  (mod(p1,9.0)<= 2.0) gl_FragColor = texture2D(texture, v_texcoord)*_red;
    else if  (mod(p1,9.0)<= 6.0) gl_FragColor = texture2D(texture, v_texcoord)*_green;
    else gl_FragColor = texture2D(texture, v_texcoord)*_blue;
    */
    gl_FragColor = texture2D(texture, v_texcoord);
}
