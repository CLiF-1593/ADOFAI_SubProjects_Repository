// Derived Functions
#include <cmath>
#define pi 3.141592653

double EaseInSine(double x) { return 1 / 2.0 * pi * sin(1 / 2.0 * pi * x); }
double EaseOutSine(double x) { return 1 / 2.0 * pi * cos(1 / 2.0 * pi * x); }
double EaseInOutSine(double x) { return 1 / 2.0 * pi * sin(pi * x); }

double EaseInQuad(double x) { return 2 * x; }
double EaseOutQuad(double x) { return -2 * x + 2; }
double EaseInOutQuad(double x) { return x < 0.5 ? 4 * x : -4 * x + 4; }

double EaseInCubic(double x) { return 3 * x * x; }
double EaseOutCubic(double x) { return 3 * x * x - 6 * x + 3; }
double EaseInOutCubic(double x) { return x < 0.5 ? 12 * x * x : 12 * x * x - 24 * x + 12; }

double EaseInQuart(double x) { return 4 * x * x * x; }
double EaseOutQuart(double x) { return -4 * x * x * x + 12 * x * x - 12 * x + 4; }
double EaseInOutQuart(double x) { return x < 0.5 ? 32 * x * x * x : -32 * x * x * x + 96 * x * x - 96 * x + 32; }

double EaseInQuint(double x) { return 5 * x * x * x * x; }
double EaseOutQuint(double x) { return 5 * x * x * x * x - 20 * x * x * x + 30 * x * x - 20 * x + 5; }
double EaseInOutQuint(double x) { return x < 0.5 ? 80 * x * x * x * x : 80 * x * x * x * x - 320 * x * x * x + 480 * x * x - 320 * x + 80; }

double EaseInExpo(double x) { return 10 * pow(2, 10 * x - 10) * log(2); }
double EaseOutExpo(double x) { return 10 * pow(2, -10 * x) * log(2); }
double EaseInOutExpo(double x) { return x < 0.5 ? 10 * pow(2, 20 * x - 10) * log(2) : 10 * pow(2, -20 * x + 10) * log (2); }

double EaseInCirc(double x) { return x / sqrt(-x * x + 1); }
double EaseOutCirc(double x) { return (-x + 1) / sqrt(-(x - 1) * (x - 1) + 1); }
double EaseInOutCirc(double x) { return x < 0.5 ? 2 * x / sqrt(-4 * x * x + 1) : (-2 * x + 2) / sqrt(-(-2 * x + 2) * (-2 * x + 2) + 1); }

double EaseInBack(double x) { return 8.10474 * x * x - 3.40316 * x; }
double EaseOutBack(double x) { return 8.10474 * x * x - 12.80632 * x + 4.70158; }
double EaseInOutBack(double x) { return x < 0.5 ? 43.138914 * x * x -10.379638 * x : 43.138914 * x * x - 75.89819 * x + 32.759276; }

double EaseInElastic(double x) { return 1.0 / 3.0 * (-20 * pow(2.0, 10.0 * x - 10.0) * pi * cos(2.0 / 3.0 * pi * (10.0 * x - 43.0 / 4.0)) - 30 * pow(2.0, 10.0 * x - 10.0) * log(2.0) * sin(2.0 / 3.0 * pi * (10.0 * x - 43.0 / 4.0))); }
double EaseOutElastic(double x) { return 1.0 / 3.0 * (20.0 * pow(2.0, -10.0 * x) * pi * cos(2.0 / 3.0 * pi * (10.0 * x - 3.0 / 4.0)) - 30.0 * pow(2.0, -10.0 * x) * log(2.0) * sin(2.0 / 3.0 * pi * (10.0 * x - 3.0 / 4.0))); }
double EaseInOutElastic(double x) {
	if (x < 0.5)
		return 1.0 / 9.0 * (-40.0 * pow(2.0, 20.0 * x - 10.0) * pi * cos(4.0 / 9.0 * pi * (20.0 * x - 89.0 / 8.0)) - 90.0 * pow(2.0, 20.0 * x - 10.0) * log(2.0) * sin(4.0 / 9.0 * pi * (20.0 * x - 89.0 / 8.0)));
	return 1.0 / 9.0 * (40.0 * pow(2.0, -20.0 * x + 10.0) * pi * cos(4.0 / 9.0 * pi * (20.0 * x - 89.0 / 8.0)) - 90.0 * pow(2.0, -20.0 * x + 10.0) * log(2.0) * sin(4.0 / 9.0 * pi * (20.0 * x - 89.0 / 8.0)));
}

double EaseInBounce(double x) {
	const double d1 = 2.75;
	if (1 - x < 1 / d1) {
		return -15.125 * x + 15.125;
	}
	else if (1 - x < 2 / d1) {
		return -15.125 * x + 6.875;
	}
	else if (1 - x < 2.5 / d1) {
		return -15.125 * x + 2.75;
	}
	else {
		return -15.125 * x + 0.6875;
	}
}
double EaseOutBounce(double x) { 
	const double d1 = 2.75;
	if (x < 1 / d1) {
		return 15.125 * x;
	}
	else if (x < 2 / d1) {
		return 15.125 * x - 8.25;
	}
	else if (x < 2.5 / d1) {
		return 15.125 * x - 12.375;
	}
	else {
		return 15.125 * x - 14.4375;
	}
}
double EaseInOutBounce(double x) {
	const double d1 = 2.75;
	if (x < 0.5) {
		if (1 - 2 * x < 1 / d1) {
			return -30.25 * x + 15.125;
		}
		else if (1 - 2 * x < 2 / d1) {
			return -30.25 * x + 6.875;
		}
		else if (1 - 2 * x < 2.5 / d1) {
			return -30.25 * x + 2.75;
		}
		else {
			return -30.25 * x + 0.6875;
		}
	}
	else {
		if (2 * x - 1 < 1 / d1) {
			return 30.25 * x - 15.125;
		}
		else if (2 * x - 1 < 2 / d1) {
			return 30.25 * x - 23.375;
		}
		else if (2 * x - 1 < 2.5 / d1) {
			return 30.25 * x - 27.5;
		}
		else {
			return 30.25 * x - 29.5625;
		}
	}
}