#version 120
varying vec3 surfaceNormal;
varying vec3 eyePosition;
varying vec4 surfaceColor;
varying vec3 emission;
void main() {
    vec4 eye = gl_ModelViewMatrix * gl_Vertex;
    eyePosition = eye.xyz;
    surfaceNormal = normalize(gl_NormalMatrix * gl_Normal);
    surfaceColor = gl_Color;
    emission = gl_FrontMaterial.emission.rgb;
    gl_Position = gl_ProjectionMatrix * eye;
}
