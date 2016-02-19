#version 330

in vec4 gl_FragCoord;
in vec3 viewPos;
in vec3 tnormal;
in vec3 texturePosition;
uniform int state;
uniform float turbulenceCoeff;
uniform float textureScale;
uniform mat4 transform;
uniform mat4 viewTransform;
uniform float alpha = 200;
out vec4 outColor;
uniform sampler2D envMap;
uniform sampler2D brickTextureMap;
uniform sampler2D brickBumpMap;

float PI = 3.1415926535897;

// snoise and turb from here:
// http://glsl.heroku.com/e#812.1

vec3 mod289(vec3 x) {
	return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x) {
	return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x) {
	return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
	return 1.79284291400159 - 0.85373472095314 * r;
}

float snoise(vec3 v)
{
	const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
	const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

	// First corner
	vec3 i  = floor(v + dot(v, C.yyy) );
	vec3 x0 =   v - i + dot(i, C.xxx) ;

	// Other corners
	vec3 g = step(x0.yzx, x0.xyz);
	vec3 l = 1.0 - g;
	vec3 i1 = min( g.xyz, l.zxy );
	vec3 i2 = max( g.xyz, l.zxy );

	//   x0 = x0 - 0.0 + 0.0 * C.xxx;
	//   x1 = x0 - i1  + 1.0 * C.xxx;
	//   x2 = x0 - i2  + 2.0 * C.xxx;
	//   x3 = x0 - 1.0 + 3.0 * C.xxx;
	vec3 x1 = x0 - i1 + C.xxx;
	vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
	vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y

	// Permutations
	i = mod289(i);
	vec4 p = permute( permute( permute(
									   i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
							  + i.y + vec4(0.0, i1.y, i2.y, 1.0 ))
					 + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

	// Gradients: 7x7 points over a square, mapped onto an octahedron.
	// The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
	float n_ = 0.142857142857; // 1.0/7.0
	vec3  ns = n_ * D.wyz - D.xzx;

	vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)

	vec4 x_ = floor(j * ns.z);
	vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

	vec4 x = x_ *ns.x + ns.yyyy;
	vec4 y = y_ *ns.x + ns.yyyy;
	vec4 h = 1.0 - abs(x) - abs(y);

	vec4 b0 = vec4( x.xy, y.xy );
	vec4 b1 = vec4( x.zw, y.zw );

	//vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
	//vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
	vec4 s0 = floor(b0)*2.0 + 1.0;
	vec4 s1 = floor(b1)*2.0 + 1.0;
	vec4 sh = -step(h, vec4(0.0));

	vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
	vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

	vec3 p0 = vec3(a0.xy,h.x);
	vec3 p1 = vec3(a0.zw,h.y);
	vec3 p2 = vec3(a1.xy,h.z);
	vec3 p3 = vec3(a1.zw,h.w);

	//Normalise gradients
	vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
	p0 *= norm.x;
	p1 *= norm.y;
	p2 *= norm.z;
	p3 *= norm.w;

	// Mix final noise value
	vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
	m = m * m;
	return 42.0 * dot( m*m, vec4(dot(p0,x0), dot(p1,x1),
								 dot(p2,x2), dot(p3,x3) ) );
}

float turb(vec3 v)
{
	const float base_freq = 0.2; // SPIDER

	vec4 noisevec;
	noisevec.x = snoise(v * base_freq*1.0) * 8.0;
	noisevec.y = snoise(v * base_freq*2.0) * 4.0;
	noisevec.z = snoise(v * base_freq*4.0) * 2.0;
	noisevec.w = snoise(v * base_freq*8.0) * 1.0;
	// noisevec = (noisevec / 8.0 + 1.0) / 2.0;
	noisevec = noisevec / 8.0;
	// noisevec = noisevec * noisevec;

	float intensity = abs(noisevec[0] - 0.20) +
	abs(noisevec[1] - 0.10) +
	abs(noisevec[2] - 0.05) +
	abs(noisevec[3] - 0.025);
	return intensity;
}

vec2 sampleBumpMap(vec2 coords)
{
	vec2 size = textureSize(brickBumpMap, 0);
	float value = texture(brickBumpMap, coords).x;
	float dx = texture(brickBumpMap, coords + vec2(1 / size.x, 0)).x - value;
	float dy = texture(brickBumpMap, coords + vec2(0, 1 / size.y)).x - value;
	
	return vec2(dx, dy);
}

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
	vec3 nnormal = normalize(tnormal);

	if (state == 1) // marble
	{
		float s = sin(2*PI*(texturePosition.x + turbulenceCoeff * turb(textureScale * texturePosition)));
		kd = (0.5+0.45*vec3(s));
		ks = vec3(0.15);
	}
	else if (state == 2) // wood
	{
		float a = 0.3;//0.4;
		float t = turbulenceCoeff * turb(textureScale * texturePosition);
		float d = sqrt(pow(texturePosition.y, 2) + pow(texturePosition.z, 2)) + a * t;
		float color = abs(cos(2 * PI * (d - floor(d))));
		
        vec3 darkWoodColor = vec3(112.0/255.0, 55.0/255.0, 15.0/255.0);
        vec3 brightWoodColor = vec3(190.0/255.0, 120.0/255.0, 50.0/255.0);
		kd = mix(darkWoodColor, brightWoodColor, color);
		ks = vec3(0.002);
	}
	else if (state == 4) // Bump map
	{
		vec2 coords = vec2(0);
		vec3 derivative = vec3(0);

		if (texturePosition.x <= 0 || texturePosition.x >= 1)
		{
			coords.xy = texturePosition.yz;
		}
		else if (texturePosition.y <= 0 || texturePosition.y >= 1)
		{
			coords.xy = texturePosition.xz;
		}
		else if (texturePosition.z <= 0 || texturePosition.z >= 1)
		{
			coords.xy = texturePosition.xy;
		}

		derivative.xy = sampleBumpMap(coords);
		
		nnormal = normalize(nnormal - textureScale * derivative);
		vec4 color = texture(brickTextureMap, coords);

		kd = color.xyz;
		ks = vec3(0);
	}

    vec3 light_vec1 = normalize(light_position1 - viewPos);
    vec3 light_vec2 = normalize(light_position2 - viewPos);

    float reflected_diffuse_1 = dot(nnormal, light_vec1);
    float reflected_diffuse_2 = dot(nnormal, light_vec2);

    vec3 reflected_ideal_1 = 2 * nnormal * (reflected_diffuse_1) - light_vec1;
    vec3 reflected_ideal_2 = 2 * nnormal * (reflected_diffuse_2) - light_vec2;

    reflected_diffuse_1 = max(0, reflected_diffuse_1);
    reflected_diffuse_2 = max(0, reflected_diffuse_2);

    vec3 viewer = normalize(vec3(0, 0, 0) - viewPos); // OBJECT_DEPTH
	vec3 reflected_viewer = 2 * nnormal * dot(nnormal, viewer) - light_vec1;
    
	if (state == 3) // Environment map
	{
		float theta = atan(reflected_viewer.x, reflected_viewer.z);
		float phi = atan(reflected_viewer.y, sqrt(pow(reflected_viewer.x, 2) + pow(reflected_viewer.z, 2)));

		float u = 1 - (theta + PI) / (2 * PI);
		float v = 1 - (phi + PI / 2) / PI;

		vec4 color = texture(envMap, vec2(u, v));

		kd = color.xyz;
		ks = vec3(0);
	}
	
	vec3 diffuse_1 = light_color1 * reflected_diffuse_1;
	vec3 diffuse_2 = light_color2 * reflected_diffuse_2;

	vec3 specular_1 = light_color1 * pow(max(dot(reflected_ideal_1, viewer), 0), alpha);
	vec3 specular_2 = light_color2 * pow(max(dot(reflected_ideal_2, viewer), 0), alpha);

	vec3 ambient = ka * ambient_color;
	vec3 color_light1 = kd * diffuse_1 + ks * specular_1;
	vec3 color_light2 = kd * diffuse_2 + ks * specular_2;

	outColor.xyz = ambient + color_light1 + color_light2;
	outColor.w = 1;
}
