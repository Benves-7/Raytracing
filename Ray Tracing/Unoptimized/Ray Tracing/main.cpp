// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <cfloat>
#include <chrono>
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"

vec3 color(const ray& r, hitable *world, int max_bounces, int depth = 0) {
	hit_record rec;
	if (world->hit(r, 0.0001f, FLT_MAX, rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < max_bounces && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, max_bounces, depth + 1);
		}
		else {
			return vec3(0, 0, 0);
		}
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f * (unit_direction.y() + 1.0f);
		return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
	}
 }

int main(int argc, char** argv)
{
	//============================ const arguments ============================
	int nx = 200;
	int ny = 100;
	int ns = 10;
	int bounces = 5;
	int spheres = 10;
	float vfov = 90.0f;
	float aperature = 0.0f;

	int spread = 10;
	float radius_max = 1.0f;
	float radius_min = 0.2f;

	vec3 lookfrom(0, 5, 3);
	vec3 lookat(0, 0, -1);
	vec3 vup(0, 1, 0);
	float dist_to_focus = (lookfrom - lookat).length();
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << aperature;
	//=========================================================================

	//============================ input arguments ============================
	for (int i = 0; i < argc; ++i)
	{
		std::string arg = argv[i];
		if (arg.substr(arg.length() - 4) == ".exe" || arg.substr(arg.length() - 2) == ".o") { continue; }
		else if (arg == "size" || arg == "-size")
		{
			nx = std::atoi(argv[i + 1]);
			ny = std::atoi(argv[i + 2]);
			i += 2;
			continue;
		}
		else if (arg == "rays" || arg == "-rays")
		{
			ns = std::atoi(argv[i + 1]);
			i++;
			continue;
		}
		else if (arg == "bounces" || arg == "-bounces")
		{
			bounces = std::atoi(argv[i + 1]);
			i++;
			continue;
		}
		else if (arg == "vfov" || arg == "-vfov" || arg == "fov" || arg == "-fov")
		{
			vfov = (float)std::atoi(argv[i + 1]);
			i++;
			continue;
		}
		else if (arg == "aperature" || arg == "-aperature" || arg == "ap" || arg == "-ap")
		{
			aperature = (float)std::atoi(argv[i + 1]);
			i++;
			continue;
		}
		else if (arg == "spheres" || arg == "-spheres")
		{
			spheres = std::atoi(argv[i + 1]) + 2;
			i++;
			continue;
		}
		std::cout << "You have entered a unrecognised command." << std::endl;
	}

	//=========================================================================

	//============================== file saving ==============================
	std::ofstream outputfile;
	outputfile.open("Output/" + std::to_string(spheres - 2) + " - " + std::to_string(ns) + " - " + std::to_string(bounces) + ".ppm");
	outputfile << "P3\n" << nx << " " << ny << "\n255\n";

	if (!outputfile.is_open())
		std::cerr << "*** error: could not open output file\n";

	//=========================================================================

	//============================= create sphere =============================
	float R = (float)cos(M_PI / 4);
	hitable** list = new hitable*[spheres];
	material** matList = new material*[spheres];

	// first two will always have a set position and be locked down.
	matList[0] = new metal(vec3(0.8f, 0.8f, 0.8f), 0.0f);
	matList[1] = new lambertian(vec3(0.8f, 0.8f, 0.0f));
	matList[2] = new metal(vec3(0.8f, 0.6f, 0.2f), 0.5f);
	matList[3] = new dielectric(2.0f);

	list[0] = new sphere(vec3(0, 0, -1), 0.5, matList[0]); // center sphere
	list[1] = new sphere(vec3(0, -10000.5, -1), 10000, matList[1]); // world
	list[2] = new sphere(vec3(1, 0, -1), 0.5, matList[2]); //right of center
	list[3] = new sphere(vec3(-1, 0, -1), 0.5, matList[3]); // left of center
	float size;
	for (int i = 4; i < spheres; i++)
	{
		float ran = randomFloat();
		size = ran * radius_max + radius_min;
		float type = randomFloat();
		// lambertian
		if (type <= 0.5)
		{
			matList[i] = new lambertian(vec3(randomFloat(), randomFloat(), randomFloat()));
			list[i] = new sphere(vec3(randomFloatboth() * spread, size - 0.5f, randomFloatboth() * spread), size, matList[i]);
		}
		// metal
		else if (type > 0.5 && type <= 0.9)
		{
			matList[i] = new metal(vec3(randomFloat(), randomFloat(), randomFloat()), randomFloat());
			list[i] = new sphere(vec3(randomFloatboth() * spread, size - 0.5f, randomFloatboth() * spread), size, matList[i]);
		}
		// dielectric
		else if (type > 0.9)
		{
			matList[i] = new dielectric(randomFloat());
			list[i] = new sphere(vec3(randomFloatboth() * spread, size - 0.5f, randomFloatboth() * spread), size, matList[i]);
		}
	}
	hitable* world = new hitable_list(list, spheres);
	//=========================================================================

	//============================= print configs =============================
	std::cout
		<< "=========================================================================\n"
		<< "size:		" << nx << "x" << ny << "\n"
		<< "ray/pixel:	" << ns << "\n"
		<< "bounces:	" << bounces << "\n"
		<< "vfov:		" << vfov << "\n"
		<< "spheres:	" << spheres << "\n"
		<< "aperature:	" << aperature << "\n"
		<< "=========================================================================\n";
	//=========================================================================

	//=============================== main loop ===============================

	camera cam(lookfrom, lookat, vup, vfov, float(nx) / float(ny), aperature, dist_to_focus);

	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) {
				float u = (float(i) + randomFloat()) / float(nx);
				float v = (float(j) + randomFloat()) / float(ny);
				ray r = cam.get_ray(u, v);
				col += color(r, world, bounces);
			} // per ray
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			outputfile << ir << " " << ig << " " << ib << "\n";
		}// per pixel
	}// per row
	//=========================================================================

	//============================= print results =============================
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

	std::cout << "=========================================================================" << std::endl;
	std::cout << "It took " << time_span.count() << " seconds." << std::endl;
	long rays = ns * nx * ny;
	std::cout << "total rays: " << rays << std::endl;
	std::cout << "rays/s: " << (double)rays / time_span.count() << std::endl;
	std::cout << "=========================================================================" << std::endl;
	//=========================================================================

	//============================== Delete loop ==============================
	for (int i = 0; i < spheres; i++)
	{
		delete list[i];
		delete matList[i];
	}
	delete[] list;
	delete[] matList;
	delete world;

	//=========================================================================
}