#include "ComponentComposition/Color.h"

namespace PlatinumEngine
{
	//Constructors

	Color::Color()
	{
		Set(0,0,0,0);
	}
	Color::Color(Maths::Vec3 color)
	{
		Set(color.x, color.y, color.z, 1.f);
	}
	Color::Color(Maths::Vec4 color)
	{
		Set(color.x, color.y, color.z, color.w);
	}
	Color::Color(int red, int green, int blue)
	{
		Set(red, green, blue, 255);
	}
	Color::Color(int red, int green, int blue, int alpha)
	{
		Set(red,green,blue,alpha);
	}
	Color::Color(float red, float green, float blue)
	{
		Set(red,green,blue,1.f);
	}
	Color::Color(float red, float green, float blue, float alpha)
	{
		Set(red, blue, green, alpha);
	}
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
		return (ToPacked() == other.ToPacked());
	}
	bool Color::operator !=(Color& other)
	{
		return (ToPacked() != other.ToPacked());
	}
	Color Color::operator *(Color& other)
	{
		int red = (r * other.r)/255;
		int green = (g * other.g)/255;
		int blue = (b * other.b)/255;
		int alpha = (a * other.a)/255;
		return Color(red, green, blue, alpha);
	}
	Color Color::operator *(float value)
	{
		return Color((int)(r*value), (int)(g*value), (int)(b*value), (int)(a*value));
	}
	void Color::operator *=(Color& other)
	{
		r = (r * other.r)/255;
		g = (g * other.g)/255;
		b = (b * other.b)/255;
		a = (a * other.a)/255;
		Clamp();
	}
	void Color::operator *=(float value)
	{
		r=(int)(r*value);
		g=(int)(g*value);
		b=(int)(b*value);
		a=(int)(a*value);
		Clamp();
	}

	//Conversions

	Maths::Vec4 Color::ToVec4()
	{
		return Maths::Vec4(r/255.f,g/255.f,b/255.f,a/255.f);
	}
	Maths::Vec3 Color::ToVec3()
	{
		return Maths::Vec3(r/255.f,g/255.f,b/255.f);
	}
	std::string Color::ToString()
	{
		return "{R:"+std::to_string(r)+" G:"+std::to_string(g)+" B:"+std::to_string(b)+" A:"+std::to_string(a)+"}";
	}
	uint32_t Color::ToPacked()
	{
		return ((a&0x0ff) << 24) | ((b&0x0ff) << 16) | ((g&0x0ff) << 8) | (r&0x0ff);
	}

	//Static methods

	Color Color::Lerp(Color a, Color b, float t)
	{
		Color nc;
		nc.r = Lerp(a.r,b.r,t);
		nc.g = Lerp(a.g,b.g,t);
		nc.b = Lerp(a.b,b.b,t);
		nc.a = Lerp(a.a,b.a,t);
		return nc;
	}

	//Private methods

	void Color::Set(int red, int green, int blue, int alpha)
	{
		r=red;
		g=green;
		b=blue;
		a=alpha;
		Clamp();
	}
	void Color::Set(float red, float green, float blue, float alpha)
	{
		red *= 255;
		green *= 255;
		blue *= 255;
		alpha *= 255;
		Set((int)red,(int)green,(int)blue,(int)alpha);
	}
	void Color::Clamp()
	{
		a = std::max(0, std::min(a, 255));
		r = std::max(0, std::min(r, 255));
		g = std::max(0, std::min(g, 255));
		b = std::max(0, std::min(b, 255));
	}

	//Output operator overloading

	std::ostream& operator <<(std::ostream& os, Color& color)
	{
		os <<color.r<<", "<<color.g<<", "<<color.b<<", "<<color.a;
		return os;
	}
}