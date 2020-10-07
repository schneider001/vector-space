#include "TXlib.h"
#include <stdlib.h>
#include <string>
#include <cmath>


struct plot {
	double x, y;
};


class CoordSys {
	plot coords0_, coords1_;
	double scaleX_, scaleY_;
	const char* signature_;
	double* to_pixels(plot coords);
public:
	void draw_point(plot coords, COLORREF color_of_point);
	void draw_circle(plot coords, double radius, COLORREF color_of_circle);
	void draw_line(plot coords0, plot coords1, COLORREF color_of_line);
	void draw_window();
	void draw_grid();
	void draw_axis();
	void set_color_back();
	void write_signature();
	CoordSys(plot coords0, plot coords1, double scaleX, double scaleY, const char* signature) {
		coords0_ = coords0;
		coords1_ = coords1;
		signature_ = signature;
		scaleX_ = scaleX;
		scaleY_ = scaleY;
	}
};


class Vector {
public:
	plot coords_;
	double length();
	void turn_vector_counter_clock_wise(double degree);
	void turn_vector_clock_wise(double degree);
	void normalization();
	void draw_vector(plot start_coords, CoordSys* vector_space);
	Vector(plot coords) {
		coords_.x = coords.x;
		coords_.y = coords.y;
	}
};



Vector operator + (Vector a, Vector b);
Vector operator * (double koef_of_length, Vector a);
double operator * (Vector a, Vector b);



int main() {
	txCreateWindow(1200, 700);
	CoordSys vector_space({ 0, 0 }, { 1200, 700 }, 1, 1, "vector space");
	double pi = 3.14159265;
	vector_space.draw_window();
	vector_space.draw_grid();
	vector_space.draw_axis();
	vector_space.set_color_back();
	vector_space.write_signature();
	Vector a({ -300,-200 });
	Vector b({ -150,-100 });
	a.draw_vector({ 20,20 }, &vector_space);
	b.draw_vector({ 20,20 }, &vector_space);
	a.turn_vector_clock_wise(3 * pi / 2);
	b.turn_vector_clock_wise(3 * pi / 2);
	a.draw_vector({ 20,20 }, &vector_space);
	b.draw_vector({ 20,20 }, &vector_space);
	a.turn_vector_clock_wise(3.1 * pi / 2);
	b.turn_vector_clock_wise(3.1 * pi / 2);
	a.draw_vector({ 20,20 }, &vector_space);
	b.draw_vector({ 20,20 }, &vector_space);
	vector_space.draw_circle({ 20,20 }, a.length(), TX_LIGHTGRAY);
	vector_space.draw_circle({ 20,20 }, b.length(), TX_LIGHTGRAY);
}


double Vector::length() {
	return sqrt(coords_.x * coords_.x + coords_.y * coords_.y);
}


void Vector::turn_vector_counter_clock_wise(double degree) {
	plot new_coords = { 0, 0 };
	new_coords.x = coords_.x * cos(degree) - coords_.y * sin(degree);
	new_coords.y = coords_.x * sin(degree) + coords_.y * cos(degree);
	coords_ = new_coords;
}


void Vector::turn_vector_clock_wise(double degree) {
	plot new_coords = { 0, 0 };
	new_coords.x = coords_.x * cos(degree) + coords_.y * sin(degree);
	new_coords.y = -coords_.x * sin(degree) + coords_.y * cos(degree);
	coords_ = new_coords;
}


void Vector::normalization() {
	Vector temp_vector(coords_);
	double length = temp_vector.length();
	temp_vector = 20 / length * temp_vector;
	coords_.x = temp_vector.coords_.x;
	coords_.y = temp_vector.coords_.y;
}


void Vector::draw_vector(plot start_coords, CoordSys* vector_space) {
	plot end_coords = { coords_.x + start_coords.x, coords_.y + start_coords.y };
	(*vector_space).draw_line(start_coords, end_coords, RGB(0, 191, 255));
	(*vector_space).draw_point(start_coords, RGB(255, 0, 0));
	Vector vector_for_arrow(coords_);
	vector_for_arrow.normalization();
	double pi = 3.14159265;
	vector_for_arrow.turn_vector_clock_wise(2.5 * pi / 3);
	plot end_of_arrow1 = { vector_for_arrow.coords_.x + end_coords.x, vector_for_arrow.coords_.y + end_coords.y };
	(*vector_space).draw_line(end_coords, end_of_arrow1, TX_BLACK);
	vector_for_arrow.turn_vector_counter_clock_wise(5 * pi / 3);
	plot end_of_arrow2 = { vector_for_arrow.coords_.x + end_coords.x, vector_for_arrow.coords_.y + end_coords.y };
	(*vector_space).draw_line(end_coords, end_of_arrow2, TX_BLACK);
}


double* CoordSys::to_pixels(plot coords) {
	plot start_of_coord = { coords1_.x / 2, coords1_.y / 2 };
	double* rec_coord = new double[2];
	rec_coord[0] = coords.x * scaleX_ + start_of_coord.x;
	rec_coord[1] = start_of_coord.y - coords.y * scaleY_;
	return rec_coord;
}


void CoordSys::draw_point(plot coords, COLORREF color_of_point) {
	txSetColor(TX_LIGHTGREEN);
	txSetFillColor(color_of_point);
	int point_radius = 4;
	double* rec_coord = to_pixels(coords);
	if (rec_coord[1] >= coords0_.y &&
		rec_coord[1] <= coords1_.y &&
		rec_coord[0] <= coords1_.x &&
		rec_coord[0] >= coords0_.x) {
		txCircle(rec_coord[0], rec_coord[1], point_radius);
	}
	delete[] rec_coord;
}


void CoordSys::draw_circle(plot coords, double radius, COLORREF color_of_circle) {
	txSetColor(color_of_circle);
	txSetFillColor(TX_TRANSPARENT);
	double* rec_coord = to_pixels(coords);
	txCircle(rec_coord[0], rec_coord[1], radius);
	delete[] rec_coord;
}


void CoordSys::draw_line(plot coords0, plot coords1, COLORREF color_of_line) {
	txSetColor(color_of_line);
	double* rec_coord0 = to_pixels(coords0);
	double* rec_coord1 = to_pixels(coords1);
	if (rec_coord0[1] >= coords0_.y &&
		rec_coord0[1] <= coords1_.y &&
		rec_coord0[0] <= coords1_.x &&
		rec_coord0[0] >= coords0_.x &&
		rec_coord1[1] >= coords0_.y &&
		rec_coord1[1] <= coords1_.y &&
		rec_coord1[0] <= coords1_.x &&
		rec_coord1[0] >= coords0_.x) {
		txLine(rec_coord0[0], rec_coord0[1], rec_coord1[0], rec_coord1[1]);
	}
	delete[] rec_coord0;
	delete[] rec_coord1;
}


void CoordSys::draw_window() {
	txSetColor(TX_LIGHTGREEN);

	txRectangle(coords0_.x, coords0_.y, coords1_.x, coords1_.y);
}

void CoordSys::draw_grid() {
	for (int line = coords0_.x; line < coords1_.x; line += 10) {
		txLine(line, coords0_.y, line, coords1_.y);
	}
	for (int line = coords0_.y; line < coords1_.y; line += 10) {
		txLine(coords0_.x, line, coords1_.x, line);
	}
}


void CoordSys::draw_axis() {
	txSetColor(TX_BLACK);

	txLine(coords1_.x / 2, coords0_.y, coords1_.x / 2, coords1_.y);
	txLine(coords0_.x, coords1_.y / 2, coords1_.x, coords1_.y / 2);

	txLine(coords1_.x / 2 , coords0_.y, coords1_.x / 2 + 5, coords0_.y + 5);
	txLine(coords1_.x / 2, coords0_.y, coords1_.x / 2 - 5, coords0_.y + 5);

	txLine(coords1_.x - 5, coords1_.y / 2 + 5, coords1_.x, coords1_.y / 2);
	txLine(coords1_.x - 5, coords1_.y / 2 - 5, coords1_.x, coords1_.y / 2);
}


void CoordSys::set_color_back() {
	txSetFillColor(TX_WHITE);
}


void CoordSys::write_signature() {
	txDrawText(coords0_.x - 30, coords1_.y + 15, coords1_.x, coords1_.y + 30, signature_);
}


Vector operator + (Vector a, Vector b) {
	plot coords_of_new_vector = { a.coords_.x + b.coords_.x, a.coords_.y + b.coords_.y };
	return Vector(coords_of_new_vector);
}


Vector operator * (double koef_of_length, Vector a) {
	plot coords_of_new_vector = { koef_of_length * a.coords_.x, koef_of_length * a.coords_.y };
	return Vector(coords_of_new_vector);
}


double operator * (Vector a, Vector b) {
	return (a.coords_.x * b.coords_.x + a.coords_.y * b.coords_.y);
}


