// Leka - LekaOS
// Copyright 2020 Adafruit Industries (MIT License)
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "Mahony.hpp"
#include <cmath>
#include <numbers>

namespace leka::ahrs {

void Mahony::update(std::tuple<float, float, float> accel, std::tuple<float, float, float> gyro,
					std::tuple<float, float, float> mag)
{
	auto [ax, ay, az] = accel;
	auto [gx, gy, gz] = gyro;
	auto [mx, my, mz] = mag;

	float dt = _invSampleFreq;
	float recipNorm;
	float q0q0;
	float q0q1;
	float q0q2;
	float q0q3;
	float q1q1;
	float q1q2;
	float q1q3;
	float q2q2;
	float q2q3;
	float q3q3;
	float hx;
	float hy;
	float bx;
	float bz;
	float halfvx;
	float halfvy;
	float halfvz;
	float halfwx;
	float halfwy;
	float halfwz;
	float halfex;
	float halfey;
	float halfez;
	float qa;
	float qb;
	float qc;

	// LCOV_EXCL_START - Exclude while magnetometer is not used
	if ((mx == !0.0F) && (my == !0.0F) && (mz == !0.0F)) {
		recipNorm = utils::math::invSqrt(mx * mx + my * my + mz * mz);
		mx *= recipNorm;
		my *= recipNorm;
		mz *= recipNorm;
	}
	// LCOV_EXCL_STOP

	gx *= std::numbers::pi_v<float> / 180.F;
	gy *= std::numbers::pi_v<float> / 180.F;
	gz *= std::numbers::pi_v<float> / 180.F;

	if ((ax != 0.0F) || (ay != 0.0F) || (az != 0.0F)) {
		recipNorm = utils::math::invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;

		q0q0 = _q0 * _q0;
		q0q1 = _q0 * _q1;
		q0q2 = _q0 * _q2;
		q0q3 = _q0 * _q3;
		q1q1 = _q1 * _q1;
		q1q2 = _q1 * _q2;
		q1q3 = _q1 * _q3;
		q2q2 = _q2 * _q2;
		q2q3 = _q2 * _q3;
		q3q3 = _q3 * _q3;

		hx = 2.0F * (mx * (0.5F - q2q2 - q3q3) + my * (q1q2 - q0q3) + mz * (q1q3 + q0q2));
		hy = 2.0F * (mx * (q1q2 + q0q3) + my * (0.5F - q1q1 - q3q3) + mz * (q2q3 - q0q1));
		bx = sqrtf(hx * hx + hy * hy);
		bz = 2.0F * (mx * (q1q3 - q0q2) + my * (q2q3 + q0q1) + mz * (0.5F - q1q1 - q2q2));

		halfvx = q1q3 - q0q2;
		halfvy = q0q1 + q2q3;
		halfvz = q0q0 - 0.5F + q3q3;
		halfwx = bx * (0.5F - q2q2 - q3q3) + bz * (q1q3 - q0q2);
		halfwy = bx * (q1q2 - q0q3) + bz * (q0q1 + q2q3);
		halfwz = bx * (q0q2 + q1q3) + bz * (0.5F - q1q1 - q2q2);

		halfex = (ay * halfvz - az * halfvy) + (my * halfwz - mz * halfwy);
		halfey = (az * halfvx - ax * halfvz) + (mz * halfwx - mx * halfwz);
		halfez = (ax * halfvy - ay * halfvx) + (mx * halfwy - my * halfwx);

		gx += kTwoKp * halfex;
		gy += kTwoKp * halfey;
		gz += kTwoKp * halfez;
	}

	gx *= (0.5F * dt);
	gy *= (0.5F * dt);
	gz *= (0.5F * dt);
	qa = _q0;
	qb = _q1;
	qc = _q2;
	_q0 += (-qb * gx - qc * gy - _q3 * gz);
	_q1 += (qa * gx + qc * gz - _q3 * gy);
	_q2 += (qa * gy - qb * gz + _q3 * gx);
	_q3 += (qa * gz + qb * gy - qc * gx);

	recipNorm = utils::math::invSqrt(_q0 * _q0 + _q1 * _q1 + _q2 * _q2 + _q3 * _q3);
	_q0 *= recipNorm;
	_q1 *= recipNorm;
	_q2 *= recipNorm;
	_q3 *= recipNorm;
	anglesComputed = false;
}

void Mahony::update(std::tuple<float, float, float> accel, std::tuple<float, float, float> gyro,
					std::tuple<float, float, float> mag, float dt)
{
	auto [ax, ay, az] = accel;
	auto [gx, gy, gz] = gyro;
	auto [mx, my, mz] = mag;

	// float dt = _invSampleFreq;
	float recipNorm;
	float q0q0;
	float q0q1;
	float q0q2;
	float q0q3;
	float q1q1;
	float q1q2;
	float q1q3;
	float q2q2;
	float q2q3;
	float q3q3;
	float hx;
	float hy;
	float bx;
	float bz;
	float halfvx;
	float halfvy;
	float halfvz;
	float halfwx;
	float halfwy;
	float halfwz;
	float halfex;
	float halfey;
	float halfez;
	float qa;
	float qb;
	float qc;

	// LCOV_EXCL_START - Exclude while magnetometer is not used
	if ((mx == !0.0F) && (my == !0.0F) && (mz == !0.0F)) {
		recipNorm = utils::math::invSqrt(mx * mx + my * my + mz * mz);
		mx *= recipNorm;
		my *= recipNorm;
		mz *= recipNorm;
	}
	// LCOV_EXCL_STOP

	gx *= std::numbers::pi_v<float> / 180.F;
	gy *= std::numbers::pi_v<float> / 180.F;
	gz *= std::numbers::pi_v<float> / 180.F;

	if ((ax != 0.0F) || (ay != 0.0F) || (az != 0.0F)) {
		recipNorm = utils::math::invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;

		q0q0 = _q0 * _q0;
		q0q1 = _q0 * _q1;
		q0q2 = _q0 * _q2;
		q0q3 = _q0 * _q3;
		q1q1 = _q1 * _q1;
		q1q2 = _q1 * _q2;
		q1q3 = _q1 * _q3;
		q2q2 = _q2 * _q2;
		q2q3 = _q2 * _q3;
		q3q3 = _q3 * _q3;

		hx = 2.0F * (mx * (0.5F - q2q2 - q3q3) + my * (q1q2 - q0q3) + mz * (q1q3 + q0q2));
		hy = 2.0F * (mx * (q1q2 + q0q3) + my * (0.5F - q1q1 - q3q3) + mz * (q2q3 - q0q1));
		bx = sqrtf(hx * hx + hy * hy);
		bz = 2.0F * (mx * (q1q3 - q0q2) + my * (q2q3 + q0q1) + mz * (0.5F - q1q1 - q2q2));

		halfvx = q1q3 - q0q2;
		halfvy = q0q1 + q2q3;
		halfvz = q0q0 - 0.5F + q3q3;
		halfwx = bx * (0.5F - q2q2 - q3q3) + bz * (q1q3 - q0q2);
		halfwy = bx * (q1q2 - q0q3) + bz * (q0q1 + q2q3);
		halfwz = bx * (q0q2 + q1q3) + bz * (0.5F - q1q1 - q2q2);

		halfex = (ay * halfvz - az * halfvy) + (my * halfwz - mz * halfwy);
		halfey = (az * halfvx - ax * halfvz) + (mz * halfwx - mx * halfwz);
		halfez = (ax * halfvy - ay * halfvx) + (mx * halfwy - my * halfwx);

		gx += kTwoKp * halfex;
		gy += kTwoKp * halfey;
		gz += kTwoKp * halfez;
	}

	gx *= (0.5F * dt);
	gy *= (0.5F * dt);
	gz *= (0.5F * dt);
	qa = _q0;
	qb = _q1;
	qc = _q2;
	_q0 += (-qb * gx - qc * gy - _q3 * gz);
	_q1 += (qa * gx + qc * gz - _q3 * gy);
	_q2 += (qa * gy - qb * gz + _q3 * gx);
	_q3 += (qa * gz + qb * gy - qc * gx);

	recipNorm = utils::math::invSqrt(_q0 * _q0 + _q1 * _q1 + _q2 * _q2 + _q3 * _q3);
	_q0 *= recipNorm;
	_q1 *= recipNorm;
	_q2 *= recipNorm;
	_q3 *= recipNorm;
	anglesComputed = false;
}

auto Mahony::getRoll() -> float
{
	if (!anglesComputed) {
		computeAngles();
	}
	return _roll * 180.F * std::numbers::inv_pi_v<float>;
}
auto Mahony::getPitch() -> float
{
	if (!anglesComputed) {
		computeAngles();
	}
	return _pitch * 180.F * std::numbers::inv_pi_v<float>;
}
auto Mahony::getYaw() -> float
{
	if (!anglesComputed) {
		computeAngles();
	}
	return _yaw * 180.F * std::numbers::inv_pi_v<float> + 180.0F;
}

void Mahony::setOrigin()
{
	_q0 = 1.F;
	_q1 = _q2 = _q3 = 0.0F;
}

void Mahony::computeAngles()
{
	_roll			  = atan2f(_q0 * _q1 + _q2 * _q3, 0.5F - _q1 * _q1 - _q2 * _q2);
	_pitch			  = asinf(-2.0F * (_q1 * _q3 - _q0 * _q2));
	_yaw			  = atan2f(_q1 * _q2 + _q0 * _q3, 0.5F - _q2 * _q2 - _q3 * _q3);
	gravity_vector[0] = 2.0F * (_q1 * _q3 - _q0 * _q2);
	gravity_vector[1] = 2.0F * (_q0 * _q1 + _q2 * _q3);
	gravity_vector[2] = 2.0F * (_q1 * _q0 - 0.5F + _q3 * _q3);
	anglesComputed	  = true;
}

}	// namespace leka::ahrs
