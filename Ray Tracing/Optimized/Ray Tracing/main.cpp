#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <cfloat>
#include <chrono>
#include <pthread.h>
#include <sys/sysinfo.h>

#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"

using namespace std;
const int THREAD_NUM = get_nprocs() - 1;

#pragma region Structs

struct pixel
{
	pixel() {}
	pixel(int x, int y) : x(x), y(y) {}
	int x, y;
	vec3 color;
};
struct thread_arg
{
	thread_arg() {}
	thread_arg(int w, int h, int y1, int y2, int nr, int nb, camera *c, hitable *world) : width(w), heigth(h), startY(y1), endY(y2), numRays(nr), bounces(nb), cam(c), world(world) {}

	void setFramebuffer(pixel *fb)
	{
		framebuffer = fb;
	}

	int width, heigth, startY, endY, numRays, bounces;
	camera *cam;
	hitable *world;
	pixel *framebuffer;
};
#pragma endregion

vec3 color(const ray &r, hitable *world, int max_bounces, int depth = 0)
{
	hit_record rec;
	if (world->hit(r, 0.0001f, FLT_MAX, rec))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < max_bounces && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * color(scattered, world, max_bounces, depth + 1);
		}
		else
		{
			return vec3(0, 0, 0);
		}
	}
	else
	{
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f * (unit_direction.y() + 1.0f);
		return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
	}
}

void *ThreadLoop(void *arguments)
{
	struct thread_arg *arg = (struct thread_arg *)arguments;
	if (arg->endY >= arg->heigth)
	{
		cout << "end is bigger then heigth : end = " << arg->endY << " heigth = " << arg->heigth << endl;
		arg->endY = arg->heigth - 1;
	}
	cout 
		<< "thread loop " << pthread_self
		<< "\n thread start = " << arg->startY
		<< "\n thread end = " << arg->endY << endl; 
	
	for (int y = arg->endY; y >= arg->startY; y--	)
	{
		for (int x = 0; x < arg->width; x++)
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < arg->numRays; s++)
			{
				float u = (float(x) + randomFloat()) / float(arg->width);
				float v = (float(y) + randomFloat()) / float(arg->heigth);
				ray r = arg->cam->get_ray(u, v);
				col += color(r, arg->world, arg->bounces);
			}
			col /= float(arg->numRays);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			arg->framebuffer[x + y * arg->width].color.set(ir, ig, ib);
		}
	}

	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
#pragma region Const arguments
	//============================ const arguments ============================
	int nx = 500;
	int ny = 250;
	int ns = 10;
	int spheres = 10;
	int canvasSize = nx * ny;

	pixel *framebuffer = new pixel[canvasSize];

	vec3 lookfrom(0, 5, 3);
	vec3 lookat(0, 0, -1);
	float dist_to_focus = (lookfrom - lookat).length();

	int bounces = 5;
	float vfov = 90.0f;
	float aperature = 0.0f;

	int spread = 10;
	float radius_max = 1.0f;
	float radius_min = 0.2f;

	vec3 vup(0, 1, 0);
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << aperature;

	camera cam(lookfrom, lookat, vup, vfov, float(nx) / float(ny), aperature, dist_to_focus);

//=========================================================================
#pragma endregion
#pragma region read Inputs
	//============================ input arguments ============================
	for (int i = 0; i < argc; ++i)
	{
		std::string arg = argv[i];

		cout << "Arguments = " << argc << endl;
		if (arg.substr(arg.length() - 4) == ".exe" || arg.substr(arg.length() - 2) == ".o")
		{
			continue;
		}
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
#pragma endregion
#pragma region File saving(setup)
	//============================== file saving ==============================
	ofstream outputfile;
	outputfile.open("Output/-spheres " + std::to_string(spheres - 2) + " -rpp " + std::to_string(ns) + " -bounces " + std::to_string(bounces) + " -vfov " + std::to_string(vfov) + " -ap " + stream.str() + ".ppm");
	outputfile << "P3\n"
			   << nx << " " << ny << "\n255\n";

	if (!outputfile.is_open())
		std::cerr << "*** error: could not open output file\n";

//=========================================================================
#pragma endregion
#pragma region Sphere setup
	//============================= create sphere =============================
	float R = (float)cos(M_PI / 4);
	hitable **list = new hitable *[spheres];
	material **matList = new material *[spheres];

	// first two will always have a set position and be locked down.
	matList[0] = new metal(vec3(0.8f, 0.8f, 0.8f), 0.0f);
	matList[1] = new lambertian(vec3(0.8f, 0.8f, 0.0f));
	matList[2] = new metal(vec3(0.8f, 0.6f, 0.2f), 0.5f);
	matList[3] = new dielectric(2.0f);

	list[0] = new sphere(vec3(0, 0, -1), 0.5, matList[0]);			// center sphere
	list[1] = new sphere(vec3(0, -10000.5, -1), 10000, matList[1]); // world
	list[2] = new sphere(vec3(1, 0, -1), 0.5, matList[2]);			//right of center
	list[3] = new sphere(vec3(-1, 0, -1), 0.5, matList[3]);			// left of center
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
	hitable *world = new hitable_list(list, spheres);
//=========================================================================
#pragma endregion
#pragma region Print config
	//============================= print configs =============================
	std::cout
		<< "number of threads = " << THREAD_NUM << " \n"
		<< "=========================================================================\n"
		<< "size:		" << nx << "x" << ny << "\n"
		<< "ray/pixel:	" << ns << "\n"
		<< "bounces:	" << bounces << "\n"
		<< "vfov:		" << vfov << "\n"
		<< "spheres:	" << spheres << "\n"
		<< "aperature:	" << aperature << "\n"
		<< "=========================================================================\n";
//=========================================================================
#pragma endregion
#pragma region Thread Setup
	//=============================== threading ===============================

	pthread_t t[THREAD_NUM];
	thread_arg *args[THREAD_NUM];
	void *status;

//=========================================================================
#pragma endregion
#pragma region Main Loop
	//=============================== main loop ===============================

	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

	int ySpread = ceil((float)ny / THREAD_NUM);

	int rc;

	for (int i = 0; i < THREAD_NUM; i++)
	{
		if (i == 0)
		{
			
			args[i] = new thread_arg(nx, ny, i * ySpread, (i + 1) * ySpread, ns, bounces, &cam, world);
		}
		else
		{
			args[i] = new thread_arg(nx, ny, i * ySpread + 1, (i + 1) * ySpread, ns, bounces, &cam, world);
		}
		args[i]->setFramebuffer(framebuffer);
		rc = pthread_create(&t[i], NULL, ThreadLoop, (void *)args[i]);
		if (rc)
		{
			std::cout << "ERROR: unable to create thread " << rc << endl;
			exit(-1);
		}
	}

	for (int i = 0; i < THREAD_NUM; i++)
	{
		pthread_join(t[i], &status);
		cout << "thread join loop : status : " << status << endl;
	}
	

	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

	

//=========================================================================
#pragma endregion
#pragma region Save image
	for (int i = 0; i < canvasSize; i++)
	{
		outputfile << framebuffer[i].color.r() << " " << framebuffer[i].color.g() << " " << framebuffer[i].color.b() << " \n";
	}
#pragma endregion
#pragma region Print results
	//============================= print results =============================
	std::cout << "=========================================================================" << std::endl;
	std::cout << "It took " << time_span.count() << " seconds." << std::endl;
	long rays = ns * nx * ny;
	std::cout << "total rays: " << rays << std::endl;
	std::cout << "rays/s: " << (double)rays / time_span.count() << std::endl;
	std::cout << "=========================================================================" << std::endl;
//=========================================================================
#pragma endregion
#pragma region Delete loop
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
#pragma endregion
}
