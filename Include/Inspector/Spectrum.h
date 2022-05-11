//
// Created by Jinyuan on 25/04/2022.
//

#pragma once

#include "Maths/Vectors.h"
#include <cmath>
#include <ostream>

namespace PlatinumEngine
{
	struct Spectrum {

		Spectrum() {
			r = 0.0f;
			g = 0.0f;
			b = 0.0f;
		}
		explicit Spectrum(float _r, float _g, float _b) {
			r = _r;
			g = _g;
			b = _b;
		}
		explicit Spectrum(float f) {
			r = g = b = f;
		}

		Spectrum(const Spectrum&) = default;
		Spectrum& operator=(const Spectrum&) = default;
		~Spectrum() = default;

		Spectrum operator+=(Spectrum v) {
			r += v.r;
			g += v.g;
			b += v.b;
			return *this;
		}
		Spectrum operator*=(Spectrum v) {
			r *= v.r;
			g *= v.g;
			b *= v.b;
			return *this;
		}
		Spectrum operator*=(float s) {
			r *= s;
			g *= s;
			b *= s;
			return *this;
		}

		static Spectrum direction(Maths::Vec3 v) {
			v = v.unit();
			Spectrum s(std::abs(v.x), std::abs(v.y), std::abs(v.z));
			s.to_linear();
			return s;
		}

		static float to_linear(float f) {
			if(f > 0.04045f) {
				return std::pow((f + 0.055f) / 1.055f, 2.4f);
			} else {
				return f / 12.92f;
			}
		}
		static float to_srgb(float f) {
			if(f > 0.0031308f) {
				return 1.055f * (std::pow(f, (1.0f / 2.4f))) - 0.055f;
			} else {
				return f * 12.92f;
			}
		}

		Spectrum to_srgb() const {
			Spectrum ret;
			ret.r = to_srgb(r);
			ret.g = to_srgb(g);
			ret.b = to_srgb(b);
			return ret;
		}
		Spectrum to_linear() const {
			Spectrum ret;
			ret.r = to_linear(r);
			ret.g = to_linear(g);
			ret.b = to_linear(b);
			return ret;
		}

		Spectrum operator+(Spectrum v) const {
			return Spectrum(r + v.r, g + v.g, b + v.b);
		}
		Spectrum operator-(Spectrum v) const {
			return Spectrum(r - v.r, g - v.g, b - v.b);
		}
		Spectrum operator*(Spectrum v) const {
			return Spectrum(r * v.r, g * v.g, b * v.b);
		}

		Spectrum operator+(float s) const {
			return Spectrum(r + s, g + s, b + s);
		}
		Spectrum operator*(float s) const {
			return Spectrum(r * s, g * s, b * s);
		}
		Spectrum operator/(float s) const {
			return Spectrum(r / s, g / s, b / s);
		}

		bool operator==(Spectrum v) const {
			return r == v.r && g == v.g && b == v.b;
		}
		bool operator!=(Spectrum v) const {
			return r != v.r || g != v.g || b != v.b;
		}

		float luma() const {
			return 0.2126f * r + 0.7152f * g + 0.0722f * b;
		}

		bool valid() const {
			return std::isfinite(r) && std::isfinite(g) && std::isfinite(b);
		}

		Maths::Vec3 to_vec() const {
			return Maths::Vec3(r, g, b);
		}

		union {
			struct {
				float r;
				float g;
				float b;
			};
			float data[3] = {};
		};
	};

	inline Spectrum operator+(float s, Spectrum v) {
		return Spectrum(v.r + s, v.g + s, v.b + s);
	}
	inline Spectrum operator*(float s, Spectrum v) {
		return Spectrum(v.r * s, v.g * s, v.b * s);
	}

	inline std::ostream& operator<<(std::ostream& out, Spectrum v) {
		out << "Spectrum{" << v.r << "," << v.g << "," << v.b << "}";
		return out;
	}
}


