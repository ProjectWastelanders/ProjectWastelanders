#version 410 core
#ifdef VERTEX_PROGRAM
	layout (location = 0) in vec3 aPos; 
	layout (location = 1) in vec3 normals;
	layout (location = 2) in vec2 textCoords;
	
	uniform mat4 view;
	uniform mat4 projection;
	uniform mat4 model;
	uniform mat4 model_rot;
	
	uniform sampler2D normal_texture;
	
	out vec2 TextureCoords;
	out vec3 Normal;
	out vec3 FragPos;
	
	void main()
	{
		vec4 aPos4 = vec4(aPos, 1.0);
		
		//OUT
		FragPos = vec3(model * aPos4);
		TextureCoords = textCoords;

		//Normal = normalize(texture(normal_texture, TextureCoords).rgb * 2.0 - 1.0);
		//Normal = normalize (vec3(model * vec4(normals, 0.0)));
		Normal = normalize(mat3(transpose(inverse(model))) * normals);
		
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
	};
	
	struct PointLight
	{
		Light Base;
		
		float Constant;
		float Linear;
		float Exp;
		
		vec3 Position;
		
		float Distance;
	};
	
	struct SpotLight
	{
		Light Base;
		
		float Constant;
		float Linear;
		float Exp;
		float Cutoff;
		
		vec3 Position;
		vec3 Direction;
		float Distance;
	};
	
	uniform DirectionalLight Light_Directional;
	uniform PointLight[MAX_POINT] Light_Point;
	uniform SpotLight[MAX_SPOT] Light_Spot; 
	
	uniform int Actual_Point;
	uniform int Actual_Spot;
	
	uniform sampler2D albedo_texture;
	//uniform sampler2D normal_texture;
	//uniform sampler2D metallic_texture;
	
	uniform vec3 ViewPoint;
	//uniform vec3 LightColor;
	//uniform vec3 specularLightColor;

	//uniform float shininess = 32.0f;
	//uniform float specularIntensity = 1.0f;
	
	int steps = 2;
	float scaleFactor = 1.0f/steps;
	
	in vec2 TextureCoords;
	in vec3 FragPos;
	in vec3 Normal;
	
	out vec4 FragColor;
	
	vec4 CalculateLight(Light light, vec3 direction, vec3 normal)
	{
		//Ambient
		vec4 Ambient = vec4(light.Color, 1.0f) * light.AmbientIntensity; // * AmbientColor;
		
		//Diffuse
		float diff = max(dot(normal, direction), 0.0);
		//float diff = dot(normal, -direction);
		
		//diff = floor(diff * steps) * scaleFactor;
		diff = ceil(diff * steps) * scaleFactor;
			
		vec4 Diffuse = vec4(light.Color, 1.0f) * light.DiffuseIntensity * diff;
		//vec4 Diffuse = awawasasdvec4(light.Color, 1.0f) * light.DiffuseIntensity * (diff + 0.1);
		Diffuse.xyz = clamp(Diffuse.xyz, 0.00, 1.0);
		if (Diffuse.xyz == vec3(0.0f))
		{
			Diffuse.xyz = vec3(0.1f);
		}
		Diffuse.w = 1.0f;
	
		
		return (Ambient * (Diffuse));
	}
	
	//Calculate Lights
	vec4 CalculateDirectionalLight(vec3 normal)
	{
		vec3 dir = normalize(Light_Directional.Direction);
		return CalculateLight(Light_Directional.Base, dir, normal);
	}
	
	vec4 CalculatePointLight(PointLight light, vec3 normal)
	{
		vec3 lightDir = normalize(light.Position - FragPos);
		float dist = length(light.Position - FragPos);
		//lightPos = normalize(lightPos);
		
		vec4 color = vec4(0.1f);
		
		if (light.Distance > dist)
		{
			color = CalculateLight(light.Base, lightDir, normal);
		}
		
		float attenuation = light.Constant + (light.Linear * dist) * (light.Exp * dist) * (dist * dist);
		//float attenuation = 1.0 / (light.Constant + light.Linear * dist + light.Exp * (dist * dist));
		
		return (color / attenuation);
	}
	
	vec4 CalculateSpotLight(SpotLight light, vec3 normal)
	{
		vec3 lightToPixel = normalize(FragPos - light.Position) * -1.0f;
		vec3 lightDir = normalize(light.Position - FragPos);
		float theta = dot(lightDir, normalize(-light.Direction));
		
		if (theta > light.Cutoff)
		{
			//vec3 lightDir = normalize(light.Position - FragPos);
			float dist = length(light.Position - FragPos);
			
			vec4 color = vec4(0.0f);
		
			if (light.Distance > dist)
			{
				color = CalculateLight(light.Base, lightDir, normal);
			}
			
			//float epsilon = light.Cutoff - 
			
			float attenuation = light.Constant + (light.Linear * dist) * (light.Exp * dist) * (dist * dist);
			float spotLightIntensity = (1.0 - (1.0 - theta) / (1.0 - light.Cutoff));
			
			vec4 result = (color / attenuation) * spotLightIntensity;
			result.w = 1.0f;
			return result;
		}
		
		//return vec4(light.Base.AmbientIntensity);
	}
	
	uniform vec3 ColourTest;
	
	void main()
	{
		//vec3 norm = normalize(texture(normal_texture, TextureCoords).rgb * 2.0 - 1.0);
		//norm = norm + Normal;
		
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
		
		
		vec3 texDiffCol = texture2D(albedo_texture, TextureCoords).rgb;
		if (length(texDiffCol) != 0.0)
		{
			FragColor = texture(albedo_texture, TextureCoords) * result * vec4(ColourTest, 1.0f);
		}
		else
		{
		//FragColor = vec4(normalize(Light_Point[0].Position - FragPos) , 1.0);
		//FragColor = vec4(FragPos, 1.0f);
		FragColor = result * vec4(ColourTest, 1.0f);
		}
		
	}
	
#endif

















