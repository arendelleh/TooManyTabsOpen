#version 120
uniform float daylight;
varying vec3 surfaceNormal;
varying vec3 eyePosition;
varying vec4 surfaceColor;
varying vec3 emission;
void main() {
    vec3 n = normalize(surfaceNormal);
    vec3 view = normalize(-eyePosition);
    vec3 light = normalize(vec3(-0.45, 0.85, 0.55));
    float diffuse = max(dot(n, light), 0.0);
    float rim = pow(1.0 - max(dot(n, view), 0.0), 3.0);
    vec3 ambient = mix(vec3(0.22, 0.28, 0.43), vec3(0.55, 0.63, 0.66), daylight);
    vec3 sun = mix(vec3(0.30, 0.38, 0.62), vec3(0.90, 0.83, 0.74), daylight);
    vec3 color = surfaceColor.rgb * (ambient + sun * diffuse);
    color += rim * mix(vec3(0.07,0.12,0.21),vec3(0.16,0.13,0.065),daylight);
    color += emission;
    float mist = smoothstep(24.0, 57.0, length(eyePosition));
    vec3 haze = mix(vec3(0.105,0.14,0.23),vec3(0.68,0.77,0.73),daylight);
    gl_FragColor = vec4(mix(color, haze, mist * 0.72), surfaceColor.a);
}
