#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;


uniform sampler2D texture;
uniform float quantityVertices;


attribute vec4 a_position;
attribute vec2 a_texcoord;

varying vec2 v_texcoord;
varying vec3 normal;
varying vec3 lightDir;
varying float height;

//! [0]
void main()
{
    const vec3 omniLightPos = vec3(100,100,0);
    height = texture2D(texture, a_texcoord).r*10.0;
    // Calculate vertex position in screen space
    vec4 computePosition = vec4(a_position.x,height,a_position.z,1.0);
    vec4 worldPosition = computePosition;
    gl_Position = mvp_matrix * computePosition;

    //compute Normal


    // read neightbor heights using an arbitrary small offset
    ivec3 off = ivec3(-1, 1, 0);


    float left = texture2D(texture, a_texcoord-vec2(1.0/quantityVertices, 0.0)).r*10.0;
    float right = texture2D(texture, a_texcoord+vec2(1.0/quantityVertices, 0.0)).r*10.0;
    float up = texture2D(texture, a_texcoord+vec2(0.0, 1.0/quantityVertices)).r*10.0;
    float down = texture2D(texture, a_texcoord-vec2(0.0, 1.0/quantityVertices)).r*10.0;

    // deduce terrain normal
    normal.x = left - right;
    normal.y = 2.0;
    normal.z = down - up;
    normal = normalize(normal);

    lightDir = normalize(omniLightPos - worldPosition);

    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    v_texcoord = a_texcoord;
}
//! [0]
