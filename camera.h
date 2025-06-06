#ifndef CAMERA_H
#define CAMERA_H
#include <windows.h>
#include <ppl.h>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <vector>
#include "color.h"
#include "common.h"
#include "light.h"
#include "raster.h"

using namespace std;
using namespace concurrency;

class camera {
public:
	float aspect_ratio;       // Ratio of image width over height
	int   image_width;        // Rendered image width in pixel count
	int   image_height;       // Rendered image height
	int   samples_per_pixel;  // Count of random samples for each pixel

	float  vfov = 90.0f;                             // Vertical view angle (field of view)
	point3 lookfrom = point3(0.0f, 0.0f, -1.0f);    // Point camera is looking from
	point3 lookat = point3(0.0f, 0.0f, 0.0f);       // Point camera is looking at
	vec3   vup = vec3(0.0f, 1.0f, 0.0f);            // Camera-relative "up" direction
	point3 camera_center;  // Camera center

	camera() {}

	camera(float _aspect_ratio, int _image_width) {
		aspect_ratio = _aspect_ratio;
		image_width = _image_width;

		// Calculate the image height, and ensure that it's at least 1.
		image_height = static_cast<int>(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;
	}

	void initialize() {
		// Calculate the image height, and ensure that it's at least 1.
		image_height = static_cast<int>(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;

		camera_center = lookfrom;

		// Determine viewport dimensions.
		float focal_length = (lookfrom - lookat).length();
		float theta = degrees_to_radians(vfov);
		float h = tan(theta / 2.0f);
		float viewport_height = 2.0f * h * focal_length;
		float viewport_width = viewport_height * (static_cast<float>(image_width) / image_height);

		// Calculate the u,v,w unit basis vectors for the camera coordinate frame.
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		// Calculate the vectors across the horizontal and down the vertical viewport edges.
		vec3 viewport_u = viewport_width * u;    // Vector across viewport horizontal edge
		vec3 viewport_v = viewport_height * -v;  // Vector down viewport vertical edge

		// Calculate the horizontal and vertical delta vectors from pixel to pixel.
		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		// Calculate the location of the upper left pixel.
		point3 viewport_upper_left = camera_center - (focal_length * w) - viewport_u / 2 - viewport_v / 2;
		pixel00_loc = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);
	}


	void render(hittable_list& world, light& worldlight) {
		for (int j = 0; j < image_height; ++j) {
			std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
			for (int i = 0; i < image_width; ++i) {
				color pixel_color(0.0f, 0.0f, 0.0f);

				for (int sample = 0; sample < samples_per_pixel; ++sample) {
					ray r = get_ray(i, j);
					pixel_color += ray_color(r, world, worldlight);
				}

				pixel_color /= samples_per_pixel;
				setColor(pixel_color[0], pixel_color[1], pixel_color[2]);
				setPixel(i, j);
			}
		}
		SDL_RenderPresent(renderer);
	}

	void parallel_render(hittable_list& world, light& worldlight, const char* filename = NULL, int jmin_salv = 0, int jmax_salv = 0, bool salvataggio = false) {
		// se presente un salvataggio precedente inserire:
		// filename : nome della foto con il precedente salvataggio
		// jmin_salv : indice della riga da cui partire
		// jmax_salv : indice della riga a cui arrivare e fermarsi
		// salvataggio : true
		// se NON presente un salvataggio precedente inserire:
		// filename : NULL
		// jmin_salv : 0
		// jmin_salv : image_height - 1
		// salvataggio : false

		vector<bool> righe_viste(image_height);
		vector<color> matrix(image_width * image_height);

		if (salvataggio && jmin_salv !=0) {
			SDL_Surface* surface;
			surface = loadTexture(filename, image_width, image_height);
			for (int j = 0; j < image_height; j++) {
				for (int i = 0; i < image_width; i++) {

					int h = i;
					int k = j;

					Uint32 value = getpixel(surface, h, k);

					float red = float((value >> 16) & 0xff) / 255.0f;
					float green = float((value >> 8) & 0xff) / 255.0f;
					float blue = float(value & 0xff) / 255.0f;

					matrix[j * image_width + i] = color(red, green, blue);
				}
			}
		}
		else {
			for (int j = 0; j < image_height; j++) {
				for (int i = 0; i < image_width; i++) {
					matrix[j * image_width + i] = color(0.0, 0.0, 0.0);
				}
			}
		}
		if( ! salvataggio ){
			jmin_salv = 0;
			jmax_salv = image_height - 1;
		}

		parallel_for(int(jmin_salv), jmax_salv + 1, [&](int j) {
			/*if (j == image_height / 5) {
				int j_max = 0;
				while (righe_viste[j_max]) {
					j_max++;
				}
				j_max--;
				cout << "j = " << j_max << ", primo quinto fatto\n";
				for (int k = 0; k < image_height; k++) {
					for (int i = 0; i < image_width; i++) {
						color pixel_color = matrix[k * image_width + i];
						setColor(pixel_color[0], pixel_color[1], pixel_color[2]);
						setPixel(i, k);
					}
				}
				SDL_RenderPresent(renderer);
				saveScreenshotBMP("screenshot_primo_quinto" + to_string(j_max) + ".bmp");
			}
			if (j == 2 * image_height / 5) {
				int j_max = 0;
				while (righe_viste[j_max]) {
					j_max++;
				}
				j_max--;
				cout << "j = " << j_max << ", secondo quinto fatto\n";
				for (int k = 0; k < image_height; k++) {
					for (int i = 0; i < image_width; i++) {
						color pixel_color = matrix[k * image_width + i];
						setColor(pixel_color[0], pixel_color[1], pixel_color[2]);
						setPixel(i, k);
					}
				}
				SDL_RenderPresent(renderer);
				saveScreenshotBMP("screenshot_secondo_quinto" + to_string(j_max) + ".bmp");
			}
			if (j == 3 * image_height / 5) {
				int j_max = 0;
				while (righe_viste[j_max]) {
					j_max++;
				}
				j_max--;
				cout << "j = " << j_max << ", terzo quinto fatto\n";
				for (int k = 0; k < image_height; k++) {
					for (int i = 0; i < image_width; i++) {
						color pixel_color = matrix[k * image_width + i];
						setColor(pixel_color[0], pixel_color[1], pixel_color[2]);
						setPixel(i, k);
					}
				}
				SDL_RenderPresent(renderer);
				saveScreenshotBMP("screenshot_terzo_quinto" + to_string(j_max) + ".bmp");
			}
			if (j == 4 * image_height / 5) {
				int j_max = 0;
				while (righe_viste[j_max]) {
					j_max++;
				}
				j_max--;
				cout << "j = " << j_max << ", quarto quinto fatto\n";
				for (int k = 0; k < image_height; k++) {
					for (int i = 0; i < image_width; i++) {
						color pixel_color = matrix[k * image_width + i];
						setColor(pixel_color[0], pixel_color[1], pixel_color[2]);
						setPixel(i, k);
					}
				}
				SDL_RenderPresent(renderer);
				saveScreenshotBMP("screenshot_quarto_quinto" + to_string(j_max) + ".bmp");
			}*/

			for (int i = 0; i < image_width; ++i) {


				color pixel_color(0.0f, 0.0f, 0.0f);

				for (int sample = 0; sample < samples_per_pixel; ++sample) {
					ray r = get_ray(i, j);
					pixel_color += ray_color(r, world, worldlight);
				}

				pixel_color /= samples_per_pixel;
				matrix[j * image_width + i] = pixel_color;
			}
			righe_viste[j] = true;
			});

		for (int j = 0; j < image_height; j++) {
			for (int i = 0; i < image_width; i++) {
				color pixel_color = matrix[j * image_width + i];
				setColor(pixel_color[0], pixel_color[1], pixel_color[2]);
				setPixel(i, j);
			}
		}
		SDL_RenderPresent(renderer);
	}

	void parallel_renderReUseSemple(hittable_list& world, light& worldlight, const char* filename, int sOld, int sNew) {
		// se presente un salvataggio di Semple precedente inserire:
		// filename : nome della foto con il precedente salvataggio
		// sOld : numero di Semple per pixel nel precedente salvataggio
		// sNew : numero di Semple per pixel nell'attuale rendering (cam.samples_per_pixel)

		vector<color> matrix(image_width * image_height);

		SDL_Surface* surface;
		surface = loadTexture(filename, image_width, image_height);

		parallel_for(0, image_height, [&](int j) {
			for (int i = 0; i < image_width; ++i) {

				Uint32 value = getpixel(surface, i, j);

				float red = float((value >> 16) & 0xff) / 255.0f;
				float green = float((value >> 8) & 0xff) / 255.0f;
				float blue = float(value & 0xff) / 255.0f;

				color pixel_color = color(red, green, blue) * sOld; // somma delle scansioni fatte nel precedente rendering

				for (int sample = sOld + 1; sample < sNew; ++sample) {
					ray r = get_ray(i, j);
					pixel_color += ray_color(r, world, worldlight);
				}

				pixel_color /= sNew;
				matrix[j * image_width + i] = pixel_color;
			}
			});

		for (int j = 0; j < image_height; j++) {
			for (int i = 0; i < image_width; i++) {
				color pixel_color = matrix[j * image_width + i];
				setColor(pixel_color[0], pixel_color[1], pixel_color[2]);
				setPixel(i, j);
			}
		}
		SDL_RenderPresent(renderer);
	}


private:
	point3 pixel00_loc;    // Location of pixel 0, 0
	vec3   pixel_delta_u;  // Offset to pixel to the right
	vec3   pixel_delta_v;  // Offset to pixel below

	vec3   u, v, w;        // Camera frame basis vectors

	ray get_ray(int i, int j) const {
		// Get a randomly sampled camera ray for the pixel at location i,j.

		vec3 pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
		vec3 pixel_sample = pixel_center + pixel_sample_square();

		point3 ray_origin = camera_center;
		vec3 ray_direction = pixel_sample - ray_origin;

		return ray(ray_origin, ray_direction);
	}

	vec3 pixel_sample_square() const {
		// Returns a random point in the square surrounding a pixel at the origin.
		float px = -0.5 + random_float();
		float py = -0.5 + random_float();
		return (px * pixel_delta_u) + (py * pixel_delta_v);
	}

	color ray_color(const ray& r, hittable_list& world, light& worldlight) {
		hit_record rec;

		if (world.hit(r, interval(0, infinity), rec)) {
			return rec.m->shading(r, worldlight, rec, camera_center, world, 0);
		}

		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f * (unit_direction.y() + 1.0f);
		//return lerp(color(1.0f, 1.0f, 1.0f), color(0.5f, 0.7f, 1.0f), t);
		return (1.0f - t) * color(1.0f, 1.0f, 1.0f) + t * color(0.5f, 0.7f, 1.0f);
	}
};

#endif




