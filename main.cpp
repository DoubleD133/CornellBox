#include <iostream>
#include <time.h>
#include <stdio.h>
#include "SDL.h" 
#include "SDL_image.h"
#include "float.h"
#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "sphere.h"
#include "hittable.h"
#include "hittable_list.h"
#include "instance.h"
#include "mesh.h"
#include "raster.h"
#include "camera.h"
#include "color.h"
#include "texture.h"

#include <cstdio>
#include <vector>
#include "spline.h"


using namespace std;

int init(int width, int height) {
	/* // Initialize SDL2. */
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	/* Create the window where we will draw. */
	window = SDL_CreateWindow("Texture Mesh", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	/* We must call SDL_CreateRenderer in order for draw calls to affect this window. */
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		SDL_DestroyWindow(window);
		cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	return 0;
}


void close() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* argv[])
{

	// World
	hittable_list world;
	list_light worldlight;


	material* red = new lambertian(color(0.75, 0.05, 0.05));
	red->texture = NULL;
	material* white = new lambertian(color(0.93, 0.93, 0.93));
	white->texture = NULL;
	material* green = new lambertian(color(0.12, 0.75, 0.15));
	green->texture = NULL;
	material* light = new area_light();
	light->texture = new constant_texture(color(15, 15, 15));
	

	quadrilatero* rectangle_model1 = new quadrilatero(point3(343.0, 548.8, 227.0), point3(343.0, 548.8, 332.0), point3(213.0, 548.8, 332.0), point3(213.0, 548.8, 227.0));
	auto rectangle_ptr1 = make_shared<instance>(rectangle_model1, light);
	world.add(rectangle_ptr1);

	//mesh* rectangle_model = new mesh("models/Square.obj", "models/");

	//auto rectangle_ptr = make_shared<instance>(rectangle_model, light);
	//rectangle_ptr->translate(0.5f, 0.5f, 0.0f);
	//// ad ora il rettangolo ha lati unitari, è nel piano xy e 0 come vertice in basso a siunistra
	//rectangle_ptr->rotate_x(90.0f);
	//// ad ora il rettangolo ha lati unitari, è nel piano xz e 0 come vertice in basso a siunistra
	//rectangle_ptr->scale(343.0 - 213.0, 1.0, 332.0 - 227.0);
	//// ad ora il rettangolo non ha lati unitari, è nel piano xz e 0 come vertice in basso a siunistra
	//rectangle_ptr->translate(213.0f, 548.8f, 227.0f);
	//// ad ora il rettangolo non ha lati unitari, è in un piano parallelo ad xz e non ha 0 come vertice in basso a siunistra
	//rectangle_ptr->InOmbrabile = true;
	//rectangle_ptr->FaOmbra = true;
	//
	//world.add(rectangle_ptr);

	// Back wall
	quadrilatero* rectangle_model2 = new quadrilatero(point3(549.6, 0.0, 559.2), point3(0.0, 0.0, 559.2), point3(0.0, 548.8, 559.2), point3(556.0, 548.8, 559.2));
	auto rectangle_ptr2 = make_shared<instance>(rectangle_model2, white);
	world.add(rectangle_ptr2);

	//auto rectangle_ptr2 = make_shared<instance>(rectangle_model, white);
	//rectangle_ptr2->translate(0.5f, 0.5f, 0.0f);
	//// ad ora il rettangolo ha lati unitari, è nel piano xy e 0 come vertice in basso a siunistra
	//rectangle_ptr2->scale(549.6 - 0.0, 548.8 - 0.0, 1.0);
	//// ad ora il rettangolo non ha lati unitari, è nel piano xy e 0 come vertice in basso a siunistra
	//rectangle_ptr2->translate(0.0f, 0.0f, 559.2f);
	//// ad ora il rettangolo non ha lati unitari, è in un piano parallelo ad xy e non ha 0 come vertice in basso a siunistra
	//rectangle_ptr2->InOmbrabile = true;
	//rectangle_ptr2->FaOmbra = true;

	world.add(rectangle_ptr2);

	// Right wall
	quadrilatero* rectangle_model3 = new quadrilatero(point3(0.0, 0.0, 559.2), point3(0.0, 0.0, 0.0), point3(0.0, 548.8, 0.0), point3(0.0, 548.8, 559.2));
	auto rectangle_ptr3 = make_shared<instance>(rectangle_model3, green);
	world.add(rectangle_ptr3);

	//auto rectangle_ptr3 = make_shared<instance>(rectangle_model, green);
	//rectangle_ptr3->translate(0.5f, 0.5f, 0.0f);
	//// ad ora il rettangolo ha lati unitari, è nel piano xy e 0 come vertice in basso a siunistra
	//rectangle_ptr->rotate_y(90.0f);
	//// ad ora il rettangolo ha lati unitari, è nel piano yz e 0 come vertice in basso a siunistra
	//rectangle_ptr3->scale(1.0, 548.8 - 0.0, 559.2 - 0.0);
	//// ad ora il rettangolo non ha lati unitari, è nel piano yz e 0 come vertice in basso a siunistra
	//rectangle_ptr3->InOmbrabile = true;
	//rectangle_ptr3->FaOmbra = true;

	//world.add(rectangle_ptr3);

	// Left wall
	quadrilatero* rectangle_model4 = new quadrilatero(point3(552.8, 0.0, 0.0), point3(549.6, 0.0, 559.2), point3(556.0, 548.8, 559.2), point3(556.0, 548.8, 0.0));
	auto rectangle_ptr4 = make_shared<instance>(rectangle_model4, red);
	world.add(rectangle_ptr4);

	// Ceiling
	quadrilatero* rectangle_model5 = new quadrilatero(point3(556.0, 548.8, 0.0), point3(556.0, 548.8, 559.2), point3(0.0, 548.8, 559.2), point3(0.0, 548.8, 0.0));
	auto rectangle_ptr5 = make_shared<instance>(rectangle_model5, white);
	world.add(rectangle_ptr5);
	
	// Floor
	quadrilatero* rectangle_model6 = new quadrilatero(point3(552.8, 0.0, 0.0), point3(0.0, 0.0, 0.0), point3(0.0, 0.0, 559.2), point3(549.6, 0.0, 559.2));
	auto rectangle_ptr6 = make_shared<instance>(rectangle_model6, white);
	world.add(rectangle_ptr6);

	sphere* sphere_model = new sphere();
	auto sphere_ptr = make_shared<instance>(sphere_model, new dielectric(1.5f));
	sphere_ptr->scale(100.0f, 100.0f, 100.0f);
	sphere_ptr->translate(150, 150, 250);
	world.add(sphere_ptr);

	auto sphere_ptr2 = make_shared<instance>(sphere_model, new specchio(color(0.7f, 0.6f, 0.5f)));
	sphere_ptr2->scale(100.0f, 100.0f, 100.0f);
	sphere_ptr2->translate(400, 100, 200);
	world.add(sphere_ptr2);

	camera cam;
	cam.lookfrom = point3(278.0, 278.0, -800.0);
	cam.lookat = point3(278.0, 278.0, 0.0);
	cam.vup = point3(0.0, 1.0, 0.0);

	cam.aspect_ratio = 16.0f / 9.0f;
	cam.image_width = 1280; // 1280;
	cam.samples_per_pixel = 20000;
	cam.vfov = 38;

	cam.initialize();

	if (init(cam.image_width, cam.image_height) == 1) {
		cout << "App Error! " << std::endl;
		return 1;
	}

	cout << "Image Resolution: " << cam.image_width << "x" << cam.image_height << "\n255\n";

	time_t start, end;
	time(&start);

	cam.parallel_render(world, worldlight);
	//cam.render(world, *worldlight);

	time(&end);
	double dif = difftime(end, start);
	cout << "\n" << "Rendering time: " << dif << "\n";

	SDL_Event event;
	bool quit = false;

	/* Poll for events */
	while (SDL_PollEvent(&event) || (!quit)) {

		switch (event.type) {

		case SDL_QUIT:
			quit = true;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				quit = true;
				break;
				// cases for other keypresses

			case SDLK_s:
				saveScreenshotBMP("screenshot.bmp");
				cout << "Screenshot saved!" << endl;
				break;
			}
		}
	}

	close();
	return 0;
}
