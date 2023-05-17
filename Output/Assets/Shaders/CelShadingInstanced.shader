#version 410 core
#ifdef VERTEX_PROGRAM
	layout (location = 0) in vec3 aPos; 
	layout (location = 1) in mat4 model;
	layout (location = 5) in vec3 normals;
	layout (location = 6) in vec2 texCoords;
	
	uniform mat4 view;
	uniform mat4 projection;
	uniform mat4 lightSpaceMatrix;
	
	uniform sampler2D normal_texture;
	
	out vec2 TextureCoords;
	out vec3 Normal;
	out vec3 FragPos;
	out vec4 FragPosLightSpace;
	
	void main()
	{
		vec4 aPos4 = vec4(aPos, 1.0);
		
		//OUT
		FragPos = vec3(model * aPos4);
		TextureCoords = texCoords;
		Normal = normalize(mat3(transpose(inverse(model))) * normals);
		FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0f);
		
		gl_Position = projection * view * model * aPos4;
	}
#endif
#ifdef FRAGMENT_PROGRAM
	const uint MAX_POINT = 32;
	const uint MAX_SPOT = 32;

	struct Light
	{
		vec3 Color;
		float AmbientIntensity;
		float DiffuseIntensity;
	};
	
	struct DirectionalLight
	{
		Light Base;
		vec3 Direction;
		mat4 lightSpaceMatrix;
	};
	
	struct PointLight
	{
		Light Base;
		
		float Linear;
		float Exp;
		
		vec3 Position;
		
		float Distance;
	};
	
	struct SpotLight
	{
		Light Base;
		
		float Linear;
		float Exp;
		float Cutoff;
		
		vec3 Position;
		vec3 Direction;
		vec3 Euler;
		float Distance;
	};
	
	uniform DirectionalLight Light_Directional;
	uniform PointLight[MAX_POINT] Light_Point;
	uniform SpotLight[MAX_SPOT] Light_Spot; 
	
	uniform int Actual_Point;
	uniform int Actual_Spot;
	
	uniform sampler2D albedo_texture;
	uniform sampler2D shadowMap;
	
	uniform vec3 ViewPoint;
	
	int steps = 2;
	float scaleFactor = 1.0f/steps;
	
	in vec2 TextureCoords;
	in vec3 FragPos;
	in vec3 Normal;
	in vec4 FragPosLightSpace;
	
	out vec4 FragColor;
	
	//AUXILIAR METHODS
	mat4 rotate(mat4 matrix, float angle, vec3 axis) {
	    axis = normalize(axis);
	    float s = sin(angle);
	    float c = cos(angle);
	    float oc = 1.0 - c;
	    float x = axis.x;
	    float y = axis.y;
	    float z = axis.z;
	
	    mat4 rotation = mat4(oc * x * x + c, oc * x * y - z * s,  oc * x * z + y * s, 0.0,
	                         oc * x * y + z * s,  oc * y * y + c, oc * y * z - x * s, 0.0,
	                         oc * x * z - y * s,  oc * y * z + x * s,  oc * z * z + c, 0.0,
	                         0.0, 0.0, 0.0, 1.0);
	
		return matrix * rotation;
	}
	
	vec3 CalculateDirection(vec3 euler)
	{
		vec3 direction = vec3(0.0f, 0.0f, 1.0f);
		mat4 rotation;
		rotation[0] = vec4(1.0f, 0.0f, 0.0f, 0.0f);
		rotation[1] = vec4(0.0f, 1.0f, 0.0f, 0.0f);
		rotation[2] = vec4(0.0f, 0.0f, 1.0f, 0.0f);
		rotation[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		
		rotation = rotate(rotation, radians(euler.x), vec3(-1.0f, 0.0f, 0.0f));
		rotation = rotate(rotation, radians(euler.y), vec3(0.0f, 1.0f, 0.0f));
		rotation = rotate(rotation, radians(euler.z), vec3(0.0f, 0.0f, -1.0f));
		
		return vec3(rotation * vec4(direction, 0.0f));
	}
	
	//SHADOW METHODS

	
	//LIGHT METHODS
	vec4 CalculateLight(Light light, vec3 direction, vec3 normal)
	{
		//Ambient
		vec4 Ambient = vec4(light.Color, 1.0f) * light.AmbientIntensity;
		
		//Diffuse
		float diff = max(dot(normal, direction), 0.0);
		diff = ceil(diff * steps) * scaleFactor;
		
		vec4 Diffuse = vec4(light.Color, 1.0f) * light.DiffuseIntensity * diff;
		
		Diffuse.xyz = clamp(Diffuse.xyz, 0.15, 1.0);
	
		vec4 result = Ambient * Diffuse;
		//vec4 result = (Ambient * Diffuse);
		result.w = 1.0f;
		return result; 
	}
	
	//Calculate Lights
	vec4 CalculateDirectionalLight(vec3 normal)
	{
		vec3 dir = normalize(CalculateDirection(Light_Directional.Direction));
		
		return CalculateLight(Light_Directional.Base, dir, normal);
	}
	
	vec4 CalculatePointLight(PointLight light, vec3 normal)
	{
		vec3 lightDir = normalize(light.Position - FragPos); 
		float dist = length(light.Position - FragPos);
		
		vec4 color = vec4(0.0f);
		
		if (light.Distance > dist)
		{
			color = CalculateLight(light.Base, lightDir, normal);
		}
		
		float attenuation = 1 + (light.Linear * dist) * (light.Exp * dist *dist);
		
		return (color / attenuation);
	}
	
	vec4 CalculateSpotLight(SpotLight light, vec3 normal)
	{
		vec3 lightDir = normalize(light.Position - FragPos);
		
		float theta = dot(lightDir, normalize(CalculateDirection(light.Direction)));
		
		vec4 color = vec4(0.0f);
		
		if (theta > light.Cutoff)
		{
			float dist = length(lightDir);
		
			if (light.Distance > dist)
			{
				color = CalculateLight(light.Base, lightDir, normal);
			}
			
			float attenuation = 1 + (light.Linear * dist) * (light.Exp * dist *dist);
		
			vec4 result = (color / attenuation);
			result.w = 1.0f;
			return result;
		}
		
		return color;
	}
	
	uniform vec3 ColourTest;
	
	void main()
	{
		//Directional
		vec4 result = CalculateDirectionalLight(Normal);
		
		//Point
		for (int i = 0; i < Actual_Point; i++)
		{
			result += CalculatePointLight(Light_Point[i], Normal);
		}
		
		//Spot
		for (int i = 0; i < Actual_Spot; i++)
		{
			result += CalculateSpotLight(Light_Spot[i], Normal);
		}
		
		FragColor = texture(albedo_texture, TextureCoords) * result * vec4(ColourTest, 1.0f);
		
	}
#endif



























