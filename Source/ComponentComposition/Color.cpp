#include "ComponentComposition/Color.h"

namespace PlatinumEngine
{
	//Constructors

	Color::Color()
	{
		Set(0,0,0,0);
	}
	//RBG color values between 0-1
	Color::Color(Maths::Vec3 color)
	{
		Set(color.x, color.y, color.z, 1.f);
	}
	//RBGA color values between 0-1
	Color::Color(Maths::Vec4 color)
	{
		Set(color.x, color.y, color.z, color.w);
	}
	//RBG color values between 0-255
	Color::Color(int red, int green, int blue)
	{
		Set(red, green, blue, 255);
	}
	//RBGA color values between 0-255
	Color::Color(int red, int green, int blue, int alpha)
	{
		Set(red,green,blue,alpha);
	}
	//RBG color values between 0-1
	Color::Color(float red, float green, float blue)
	{
		Set(red,green,blue,1.f);
	}
	//RBGA color values between 0-1
	Color::Color(float red, float green, float blue, float alpha)
	{
		Set(red, blue, green, alpha);
	}
	//Packed 32bit integer RGBA color value (R will be the least significant 8 bits followed by G and B while A will be the most significant 8 bits)
	Color::Color(uint32_t color)
	{
		int red = (color&0x0ff);
		int green = (color>>8)&0x0ff;
		int blue = (color>>16)&0x0ff;
		int alpha = (color>>24)&0x0ff;
		Set(red, green, blue, alpha);
	}

	Color::~Color()
	{}

	//Operator overloading

	void Color::operator=(Color other)
	{
		r=other.r;
		g=other.g;
		b=other.b;
		a=other.a;
	}
	bool Color::operator ==(Color& other)
	{
		return (r==other.r && g==other.g && b==other.b && a==other.a);
	}
	bool Color::operator !=(Color& other)
	{
		return (r!=other.r || g!=other.g || b!=other.b || a!=other.a);
		//return !(*this == other);
	}
	Color Color::operator *(Color& other)
	{
		float red = r * other.r;
		float green = g * other.g;
		float blue = b * other.b;
		float alpha = a * other.a;
		return Color(red, green, blue, alpha);
	}
	Color Color::operator *(float value)
	{
		return Color(r*value, g*value, b*value, a*value);
	}
	void Color::operator *=(Color& other)
	{
		r = r * other.r;
		g = g * other.g;
		b = b * other.b;
		a = a * other.a;
	}
	void Color::operator *=(float value)
	{
		r=r*value;
		g=g*value;
		b=b*value;
		a=a*value;
	}

	//Conversions

	//Returns floating RGBA values (between 0 and 1)
	Maths::Vec4 Color::ToVec4()
	{
		return Maths::Vec4(r,g,b,a);
	}
	//Returns floating RGB values (between 0 and 1)
	Maths::Vec3 Color::ToVec3()
	{
		return Maths::Vec3(r,g,b);
	}
	//Returns a packed 32bit integer RGBA value
	uint32_t Color::ToPacked()
	{
		int red = (int)(r * 255.f);
		int green = (int)(g * 255.f);
		int blue = (int)(b * 255.f);
		int alpha = (int)(a * 255.f);
		return ((alpha&0x0ff) << 24) | ((blue&0x0ff) << 16) | ((green&0x0ff) << 8) | (red&0x0ff);
	}

	//Static methods

	//Linear interpolation between 2 colours
	Color Color::Lerp(Color a, Color b, float t)
	{
		Color nc;
		nc.r = Maths::Common::Lerp(a.r,b.r,t);
		nc.g = Maths::Common::Lerp(a.g,b.g,t);
		nc.b = Maths::Common::Lerp(a.b,b.b,t);
		nc.a = Maths::Common::Lerp(a.a,b.a,t);
		return nc;
	}

	//Private methods

	//Sets the colors (rgba values are already public but this method can be public too if needed)
	void Color::Set(int red, int green, int blue, int alpha)
	{
		r=red/255.f;
		g=green/255.f;
		b=blue/255.f;
		a=alpha/255.f;
	}
	void Color::Set(float red, float green, float blue, float alpha)
	{
		r=red;
		g=green;
		b=blue;
		a=alpha;
	}

	//Output operator overloading

	std::ostream& operator <<(std::ostream& os, Color& color)
	{
		os <<color.r<<", "<<color.g<<", "<<color.b<<", "<<color.a;
		return os;
	}
}