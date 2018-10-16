#version 330 core

out vec4 ColorOut;

in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec3 LightPosition;
uniform vec3 AmbientLight;
uniform vec3 DifColor;
uniform vec3 SpecColor;
uniform float PhongExponent;
uniform vec3 CameraPosition;

void main()
{
    vec3 Diffuse = texture(texture1, TexCoord).rgb;

    vec3 ca = Diffuse * AmbientLight;
    float d = dot(normalize(Normal), normalize(LightPosition));
    vec3 cd, cs;
    if (d > 0) {
        cd = Diffuse * DifColor * d;
        cs = SpecColor * pow(dot(normalize(CameraPosition), reflect(normalize(LightPosition), normalize(Normal))), PhongExponent);
    }
    else {
        cd = vec3(0.0f, 0.0f, 0.0f);
        cs = vec3(0.0f, 0.0f, 0.0f);
    }

    ColorOut = vec4(clamp(ca+cd+cs, 0, 1), 1.0f);
}