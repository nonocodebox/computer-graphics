#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

uniform mat4 projTransform;
uniform mat4 viewTransform;
uniform float alpha;
out vec4 color;

void main()
{
    vec3 light_color1 = vec3(1.0, 0.9, 0.7); // First light color
    vec3 light_color2 = vec3(0.6, 0.6, 1.0); // Second light color
    vec3 ambient_color = vec3(1.0, 1.0, 1.0); // Ambient light color

    vec3 light_position1 = vec3(3.0, 2.0, 1.0); // First light position
    vec3 light_position2 = vec3(-3.0, 0.0, 1.0); // Second light position

    vec3 ka = vec3(0.1, 0.1, 0.1); // Ambient coefficient
    vec3 kd = vec3(0.3, 0.3, 0.3); // Diffuse coefficient
    vec3 ks = vec3(0.3, 0.3, 0.3); // Specular coefficient

	// Transform the vertex
	gl_Position = projTransform * position;

	vec3 viewPos = (viewTransform * position).xyz;
	vec3 tnormal = normalize((viewTransform * vec4(normal, 0)).xyz);

    vec3 light_vec1 = normalize(light_position1 - viewPos);
    vec3 light_vec2 = normalize(light_position2 - viewPos);

    float reflected_diffuse_1 = dot(tnormal, light_vec1);
    float reflected_diffuse_2 = dot(tnormal, light_vec2);

    vec3 reflected_ideal_1 = 2 * tnormal * reflected_diffuse_1 - light_vec1;
    vec3 reflected_ideal_2 = 2 * tnormal * reflected_diffuse_2 - light_vec2;

    reflected_diffuse_1 = max(0, reflected_diffuse_1);
    reflected_diffuse_2 = max(0, reflected_diffuse_2);

    vec3 viewer = normalize(vec3(0, 0, 0) - viewPos);

	vec3 diffuse_1 = light_color1 * reflected_diffuse_1;
	vec3 diffuse_2 = light_color2 * reflected_diffuse_2;

	vec3 specular_1 = light_color1 * pow(max(dot(reflected_ideal_1, viewer), 0), alpha);
	vec3 specular_2 = light_color2 * pow(max(dot(reflected_ideal_2, viewer), 0), alpha);

	vec3 ambient = ka * ambient_color;
	vec3 color_light1 = kd * diffuse_1 + ks * specular_1;
    vec3 color_light2 = kd * diffuse_2 + ks * specular_2;

    color.xyz = ambient + color_light1 + color_light2;
	color.w = 1;
}
