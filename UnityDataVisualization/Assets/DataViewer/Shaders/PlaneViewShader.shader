Shader "Custom/PlaneViewShader" {
	Properties {
		_Color ("Color", Color) = (1,1,1,1)

		_MainTex ("Albedo (RGB)", 2D) = "white" {}
		_Glossiness ("Smoothness", Range(0,1)) = 0.5

		_Normal ("Normal Vector (of Plane)", Vector) = (0,0,0,0)
		_PlanePosition("Position of Viewing Plane", Vector) = (0,0,0,0)
		_Distance ("Distance (Between Planes)", Float) = 0

		_Lock ("Plane Lock", Float) = 0
	}
	SubShader {
		Tags { "RenderType"="Transparent" "IGNOREPROJECTOR"="true" "Queue"="Transparent"}
		LOD 200

		CGPROGRAM
		// Physically based Standard lighting model, and enable shadows on all light types
		#pragma surface surf Standard fullforwardshadows alpha:fade

		// Use shader model 3.0 target, to get nicer looking lighting
		#pragma target 3.0

		sampler2D _MainTex;

		struct Input {
			float2 uv_MainTex;
			float3 worldPos;
		};

		half _Glossiness;
		fixed4 _Color;

		uniform float4 _Normal;
		uniform float4 _PlanePosition;
		uniform float _Distance;
		uniform float _Lock;

		void surf (Input IN, inout SurfaceOutputStandard o) {
			// Albedo comes from a texture tinted by color
			fixed4 c = tex2D (_MainTex, IN.uv_MainTex) * _Color;
			o.Albedo = c.rgb;
			// Smoothness come from slider variables
			//o.Smoothness = _Glossiness;

			float4 unitNorm = _Normal / length(_Normal);
	  		float3 relativePos = IN.worldPos - _PlanePosition;
	  		float projection = dot(relativePos, unitNorm);

			if (_Lock == 0) {
				if (projection < _Distance && projection > -1*_Distance) {
		  		c.a = 1;
				} else {
		  		c.a = 0.1;
		  	}
		  }

			o.Alpha = c.a;
			//o.Albedo.a = c.a;
		}
		ENDCG
	}
	FallBack "Transparency/Diffuse"
}
