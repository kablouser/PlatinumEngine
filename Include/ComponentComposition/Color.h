#pragma once

#include "Maths/Vectors.h"
#include "ostream"
#include "math.h"
#include "Maths/Common.h"

namespace PlatinumEngine
{
	class Color
	{
	public:
		Color();
		Color(Maths::Vec3 color);
		Color(Maths::Vec4 color);
		Color(int red, int green, int blue, int alpha);
		Color(int red, int green, int blue);
		Color(float red, float green, float blue, float alpha);
		Color(float red, float green, float blue);
		Color(uint32_t color);
		~Color();

		void operator =(Color other);
		bool operator ==(Color& other);
		bool operator !=(Color& other);
		Color operator *(Color& other);
		Color operator *(float value);
		void operator *=(Color& other);
		void operator *=(float value);


		Maths::Vec3 ToVec3();
		Maths::Vec4 ToVec4();
		uint32_t ToPacked();

		static Color Lerp(Color a, Color b, float t);

	public:
		float r, g, b, a;
		float& x = r, y = g, z = b, w = a;

	private:
		void Set(int red, int green, int blue, int alpha);
		void Set(float red, float green, float blue, float alpha);

	};

	std::ostream& operator <<(std::ostream& os, Color& color);
}